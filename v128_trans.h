#pragma once

#include <array>
#include <cstdint>
#include <immintrin.h>

#include "v128_point.h"

template <int N>
struct v128_trans;

template <>
struct v128_trans<4> {
    __m128i signs;
    __m128i perm;
    __m128i iperm;

    static std::array<uint32_t, 4> invert_perm(std::array<uint32_t, 4> perm) {
        std::array<uint32_t, 4> iperm;
        for (int i = 0; i < 4; i++) {
            iperm[perm[i]] = i;
        }
        return iperm;
    }

    static __m128i perm_to_mm_epi32(std::array<uint32_t, 4> perm) {
        return _mm_setr_epi32(perm[0], perm[1], perm[2], perm[3]);
        // return _mm_loadu_si128(reinterpret_cast<const __m128i_u *>(perm.data()));
    }

    v128_trans(std::array<int32_t, 4> signs, std::array<uint32_t, 4> perm)
        : signs(_mm_set_epi32(signs[3], signs[2], signs[1], signs[0])),
          perm(perm_to_mm_epi32(perm)),
          iperm(perm_to_mm_epi32(invert_perm(perm)))
        {}
};

template <>
struct v128_trans<2> : v128_trans<4> {
    v128_trans(std::array<int32_t, 2> signs, std::array<uint32_t, 2> perm)
        : v128_trans<4>(std::array<int32_t, 4>{signs[0], signs[1], 1, 1}, std::array<uint32_t, 4>{perm[0], perm[1], 2, 3}) {}

    v128_point<2> permute(const v128_point<2> &p) const {
        auto temp1 = _mm_permutevar_ps(_mm_castsi128_ps(p.data), perm);
        auto temp = _mm_castps_si128(temp1);
        return temp;
    }

    v128_point<2> flip(const v128_point<2> &p) const {
        __m128i temp = _mm_sign_epi32(p.data, signs);
        return temp;
    }

    v128_point<2> operator*(const v128_point<2> &p) const {
        __m128 temp = _mm_permutevar_ps(_mm_castsi128_ps(p.data), iperm);
        return _mm_sign_epi32(_mm_castps_si128(temp), signs);
    }
};
