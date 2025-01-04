#pragma once

#include <array>
#include <cstdint>
#include <immintrin.h>

#include "v128_point2d.h"

struct v128_interval {
  int32_t left;
  int32_t right;

  bool operator==(const v128_interval &other) const {
    return left == other.left && right == other.right;
  }
};

struct v128_box2 {
  __m128i data; // x_min, y_min, x_max, y_max

  v128_box2(__m128i data) : data(data) {}
  v128_box2(std::array<v128_interval, 2> intervals)
      : data(_mm_setr_epi32(intervals[0].left, intervals[1].left,
                            intervals[0].right, intervals[1].right)) {}

  v128_interval operator[](size_t i) const {
    return {_mm_extract_epi32(data, i), _mm_extract_epi32(data, i + 2)};
  }
};

struct v128_trans2 {
  __m128i signs;
  __m128i perm;

  v128_trans2(__m128i signs, __m128i perm) : signs(signs), perm(perm) {}
  v128_trans2(std::array<int32_t, 2> signs, std::array<uint32_t, 2> perm)
      : signs(_mm_setr_epi32(signs[0], signs[1], signs[0], signs[1])),
        perm(_mm_setr_epi32(perm[0], perm[1], 2 + perm[0], 2 + perm[1])) {}

  std::pair<int32_t, uint32_t> operator[](size_t i) const {
    return {_mm_extract_epi32(signs, i), _mm_extract_epi32(perm, i)};
  }

  v128_point2 operator*(const v128_point2 &p) const {
    __m128 temp = _mm_permutevar_ps(_mm_castsi128_ps(p.data), perm);
    return _mm_sign_epi32(_mm_castps_si128(temp), signs);
  }

  v128_trans2 operator*(v128_trans2 &t) {
    __m128i new_perm =
        _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(perm), t.perm));
    __m128i new_signs = _mm_sign_epi32(
        _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(t.signs), perm)),
        signs);
    return {new_signs, new_perm};
  }

  v128_box2 operator*(const v128_box2 &b) const {
    __m128i temp1 = _mm_sign_epi32(
        _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(b.data), perm)),
        signs);

    __m128i temp2 = _mm_shuffle_epi32(temp1, _MM_SHUFFLE(2, 3, 0, 1));
    __m128i minima = _mm_min_epi32(temp1, temp2);
    __m128i maxima = _mm_max_epi32(temp1, temp2);
    return _mm_blend_epi32(
        minima, maxima, 0b0101
        // 0b1010  // TODO: this mask works even though I feel like it shouldn't
    );
  }
};
