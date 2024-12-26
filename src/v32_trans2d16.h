#include <array>
#include <cstddef>
#include <cstdint>
#include <limits.h>

static inline uint64_t rol32(uint32_t n, uint8_t c) {
  const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
  c &= mask;
  return (n << c) | (n >> ((-c) & mask));
}

struct v32_point2d16 {
  uint32_t data;

  v32_point2d16(uint32_t data) : data(data) {}
  v32_point2d16(int16_t x, int16_t y)
      : data((uint32_t(y) << 16) | uint16_t(x)) {}
  v32_point2d16(std::array<int32_t, 2> coords)
      : v32_point2d16(coords[0], coords[1]) {}
  v32_point2d16() : v32_point2d16(0, 0) {}

  bool operator==(const v32_point2d16 &other) const {
    return data == other.data;
  }
  int16_t operator[](size_t i) const { return (data >> (16 * i)) & 0xFFFF; }
};

// Idea:
//  xor eax, <mask>
//  add ax, <lower>
//  add eax, <upper>
// where mask = 0xFFFFFFFF or 0xFFFF0000 or 0x0000FFFF,
// lower = 0 or 1, and upper = 0 or 0x10000

uint32_t cat(int16_t x, int16_t y) { return (int32_t(y) << 16) | uint16_t(x); }

int16_t get(uint32_t xy, size_t i) { return (xy >> (16 * i)) & 0xFFFF; }

// doesn't work if x is 0
static inline uint32_t flip_both(uint32_t xy) {
  return (xy ^ -1) + (1 << 16) + 1;
}

// doesn't work if x is 0
static inline uint32_t flip_low(uint32_t xy) { return (xy ^ uint16_t(-1)) + 1; }

static inline uint32_t flip_high(uint32_t xy) {
  return (xy ^ ((-1) << 16)) + (1 << 16);
}

struct v32_trans2d16 {
  std::array<int32_t, 2> signs;
  uint8_t iperm;

  v32_trans2d16(std::array<int32_t, 2> signs, bool perm)
      : signs(signs), iperm(perm ? 16 : 0) {}

  v32_point2d16 operator*(const v32_point2d16 p) const {
    uint32_t temp = rol32(p.data, iperm);
    if (signs[0] == -1 && signs[1] == -1) {
      return flip_both(temp);
    } else if (signs[0] == -1) {
      return flip_low(temp);
    } else if (signs[1] == -1) {
      return flip_high(temp);
    }
    return temp;
  }
};

// TODO: Representation is (y, x) but should be (x, y)
// TODO: Exceptions only occur because compiler isn't using high/low registers.
// Fix this, possibly with inline assembly
