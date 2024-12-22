#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <immintrin.h>
#include <limits.h>

#include "v64_point2.h"

// https://stackoverflow.com/questions/776508/best-practices-for-circular-shift-rotate-operations-in-c
uint64_t rotl (uint64_t x, unsigned int n)
{
  const unsigned int mask = (CHAR_BIT*sizeof(x)-1);  // e.g. 31

  n &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
  return (x<<n) | (x>>( (-n)&mask ));
}

uint64_t rot_const(uint64_t x)
{
  return rotl(x, 32);
}

struct v64_trans2 {
    __m64 signs;
    uint32_t perm;

    v64_trans2(std::array<int32_t, 2> signs, bool perm)
        : signs(_mm_setr_pi32(signs[0], signs[1])),
          perm(perm ? 32 : 0)
        {}

    v64_point2 operator*(const v64_point2 &p) const {
        uint64_t temp = rot_const(p.data);
        auto temp2 = _mm_sign_pi32(_mm_cvtsi64_m64(temp), signs);
        return _mm_cvtm64_si64(temp2);
    }
};
