#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <immintrin.h>
#include <limits.h>

#include "v64_point2d.h"

// https://stackoverflow.com/questions/776508/best-practices-for-circular-shift-rotate-operations-in-c
static inline uint64_t rol64(uint64_t n, uint8_t c)
{
  const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);
  c &= mask;
  return (n<<c) | (n>>( (-c)&mask ));
}

struct v64_trans2 {
    __m64 signs;
    uint8_t iperm;

    v64_trans2(std::array<int32_t, 2> signs, bool perm)
        : signs(_mm_setr_pi32(signs[0], signs[1])),
          iperm(perm ? 32 : 0)  // uses iperm == perm in dimension 2
        {}

    // T(p)[i] = S[i] * p[Pinv[i]]
    v64_point2 operator*(const v64_point2 &p) const {
        uint64_t temp = rol64(p.data, iperm);
        auto temp2 = _mm_sign_pi32(_mm_cvtsi64_m64(temp), signs);
        return _mm_cvtm64_si64(temp2);
    }
};
