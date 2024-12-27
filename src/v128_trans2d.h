#pragma once

#include <array>
#include <cstdint>
#include <immintrin.h>

#include "v128_point2d.h"

struct v128_trans2 {
  __m128i signs;
  __m128i iperm;

  v128_trans2(__m128i signs, __m128i iperm) : signs(signs), iperm(iperm) {}
  v128_trans2(std::array<int32_t, 2> signs, std::array<uint32_t, 2> perm)
      : signs(_mm_set_epi32(1, 1, signs[1], signs[0])),
        iperm(_mm_setr_epi32(perm[0], perm[1], 2,
                             3)) // uses iperm == perm in dimension 2
  {}

  std::pair<int32_t, uint32_t> operator[](size_t i) const {
    return {_mm_extract_epi32(signs, i), _mm_extract_epi32(iperm, i)};
  }

  v128_point2 operator*(const v128_point2 &p) const {
    __m128 temp = _mm_permutevar_ps(_mm_castsi128_ps(p.data), iperm);
    return _mm_sign_epi32(_mm_castps_si128(temp), signs);
  }

  v128_trans2 operator*(v128_trans2 &t) {
    __m128i new_iperm =
        _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(iperm), t.iperm));
    __m128i new_signs = _mm_sign_epi32(
        _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(t.signs), iperm)),
        signs);
    return {new_signs, new_iperm};
  }
};
