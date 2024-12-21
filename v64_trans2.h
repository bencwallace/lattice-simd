#pragma once

#include <array>
#include <cstdint>
#include <immintrin.h>

#include "v64_point2.h"

struct v64_trans2 {
    __m64 signs;
    uint32_t perm;

    static inline uint64_t rol64_fixed(uint64_t value) {
        __asm__("rolq $32, %0" : "+r" (value));
        return value;
    }

    v64_trans2(std::array<int32_t, 2> signs, bool perm)
        : signs(_mm_setr_pi32(signs[0], signs[1])),
          perm(perm ? 32 : 0)
        {}

    v64_point2 operator*(const v64_point2 &p) const {
        uint64_t temp = rol64_fixed(p.data);
        auto temp2 = _mm_sign_pi32(_mm_cvtsi64_m64(temp), signs);
        return _mm_cvtm64_si64(temp2);
    }
};
