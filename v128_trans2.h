#pragma once

#include <array>
#include <cstdint>
#include <immintrin.h>

#include "v128_point2.h"

struct v128_trans2 {
    __m128i signs;
    __m128i perm;
    __m128i iperm;

    v128_trans2(std::array<int32_t, 2> signs, std::array<uint32_t, 2> perm)
        : signs(_mm_set_epi32(1, 1, signs[1], signs[0])),
          perm(_mm_setr_epi32(perm[0], perm[1], 2, 3)),
          iperm(_mm_setr_epi32(perm[0], perm[1], 2, 3))
        {}

    v128_point2 permute(const v128_point2 &p) const {
        auto temp1 = _mm_permutevar_ps(_mm_castsi128_ps(p.data), perm);
        auto temp = _mm_castps_si128(temp1);
        return temp;
    }

    v128_point2 flip(const v128_point2 &p) const {
        __m128i temp = _mm_sign_epi32(p.data, signs);
        return temp;
    }

    v128_point2 operator*(const v128_point2 &p) const {
        __m128 temp = _mm_permutevar_ps(_mm_castsi128_ps(p.data), iperm);
        return _mm_sign_epi32(_mm_castps_si128(temp), signs);
    }
};
