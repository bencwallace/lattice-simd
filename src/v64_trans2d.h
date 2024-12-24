#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <immintrin.h>
#include <limits.h>

#include "v64_point2d.h"

// Would be nice to have a more portable version of this using ideas from here:
// https://stackoverflow.com/questions/776508/best-practices-for-circular-shift-rotate-operations-in-c
// However, attempts so far have been less performant than the inline assembly version.
// For some reason, this is even the case when calling __rolq(p.data, iperm) from <x86intrin.h>.
static inline uint64_t rol64_fixed(uint64_t value) {
    __asm__("rolq $32, %0" : "+r" (value));
    return value;
}

// same as above but takes the shift width as an argument
static inline uint64_t rol64(uint64_t value, uint8_t shift) {
    __asm__("rolq %1, %0" : "+r" (value) : "c" (shift));
    return value;
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
