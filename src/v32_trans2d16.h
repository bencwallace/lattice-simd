#include <cstddef>
#include <cstdint>

struct v32_point2d16 {
    uint32_t data;

    v32_point2d16(int16_t x, int16_t y) : data((uint32_t(y) << 16) | uint16_t(x)) {}

    int16_t operator[](size_t i) const {
        return (data >> (16 * i)) & 0xFFFF;
    }
};

// Idea:
//  xor eax, <mask>
//  add ax, <lower>
//  add eax, <upper>
// where mask = 0xFFFFFFFF or 0xFFFF0000 or 0x0000FFFF,
// lower = 0 or 1, and upper = 0 or 0x10000

struct v32_trans2d16 {
    v32_point2d16 operator*(const v32_point2d16 p) const {
        // TODO
        return v32_point2d16(p[1], p[0]);
    }
};
