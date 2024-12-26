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

// // doesn't work if x is 0
// static inline uint32_t flip_both(uint32_t xy) {
//   return (xy ^ -1) + (1 << 16) + 1;
// }

// // doesn't work if x is 0
// static inline uint32_t flip_low(uint32_t xy) { return (xy ^ uint16_t(-1)) +
// 1; }

// static inline uint32_t flip_high(uint32_t xy) {
//   return (xy ^ ((-1) << 16)) + (1 << 16);
// }

// static inline uint32_t flip(uint32_t xy, uint32_t mask, uint32_t s) {
//   return (xy ^ mask) + s;
// }

static inline uint32_t flip(uint32_t xy, uint32_t mask, uint32_t s) {
  xy ^= mask;
  uint16_t lo = xy & 0x0000FFFF;
  uint16_t so = s & 0x0000FFFF;

  uint32_t hi = xy & 0xFFFF0000;
  uint32_t si = s & 0xFFFF0000;

  return (hi + si) | uint32_t(lo + so);
}

struct v32_trans2d16 {
  uint32_t mask;
  uint32_t s;
  uint8_t iperm;

  v32_trans2d16(std::array<int32_t, 2> signs, bool perm)
      : mask((signs[0] == -1 ? uint16_t(-1) : 0) +
             (signs[1] == -1 ? (-1) << 16 : 0)),
        s((signs[0] == -1 ? 1 : 0) + (signs[1] == -1 ? 1 << 16 : 0)),
        iperm(perm ? 16 : 0) {}

  v32_point2d16 operator*(const v32_point2d16 p) const {
    uint32_t temp = rol32(p.data, iperm);
    return flip(temp, mask, s);
  }
};

// TODO: Representation is (y, x) but should be (x, y)
// TODO: Exceptions only occur because compiler isn't using high/low registers.
// Fix this, possibly with inline assembly
