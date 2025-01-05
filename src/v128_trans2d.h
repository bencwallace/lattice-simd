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

struct v128_box2d {
  __m128i data; // x_min, y_min, x_max, y_max

  v128_box2d() : data(_mm_setzero_si128()) {}
  v128_box2d(__m128i data) : data(data) {}
  v128_box2d(std::array<v128_interval, 2> intervals)
      : data(_mm_setr_epi32(intervals[0].left, intervals[1].left,
                            intervals[0].right, intervals[1].right)) {}

  v128_interval operator[](size_t i) const {
    return {int32_t(extract_epi32(data, i)),
            int32_t(extract_epi32(data, i + 2))};
  }
};

// Given a vector (a, b, c, d) representing potentially unsorted intervals
// {a, c} and {b, d}, return [min(a, c), min(b, d), max(a, c), max(b, d)]
// representing intervals [min(a, c), max(a, c)] and [min(b, d), max(b, d)].
inline __m128i sort_bounds(__m128i pairs) {
  __m128i swapped = _mm_shuffle_epi32(pairs, _MM_SHUFFLE(1, 0, 3, 2));
  __m128i minima = _mm_min_epi32(pairs, swapped);
  __m128i maxima = _mm_max_epi32(pairs, swapped);
  return _mm_blend_epi32(minima, maxima, 0b1100);
}

struct v128_trans2d {
  __m128i signs;
  __m128i perm;

  v128_trans2d(__m128i signs, __m128i perm) : signs(signs), perm(perm) {}

  // Since only two elements of each array are used, there are multiple possible
  // ways to fill the vectors. The choice below is especially convenient for the
  // implementation of operator*(const v128_box2 &b)
  v128_trans2d(std::array<int32_t, 2> signs, std::array<uint32_t, 2> perm)
      : signs(_mm_setr_epi32(signs[0], signs[1], signs[0], signs[1])),
        perm(_mm_setr_epi32(perm[0], perm[1], 2 + perm[0], 2 + perm[1])) {}

  std::pair<int32_t, uint32_t> operator[](size_t i) const {
    return {extract_epi32(signs, i), extract_epi32(perm, i)};
  }

  v128_point2d operator*(const v128_point2d &p) const {
    __m128 temp = _mm_permutevar_ps(_mm_castsi128_ps(p.data), perm);
    return _mm_sign_epi32(_mm_castps_si128(temp), signs);
  }

  v128_trans2d operator*(const v128_trans2d &t) {
    __m128i new_perm =
        _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(perm), t.perm));
    __m128i new_signs = _mm_sign_epi32(
        _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(t.signs), perm)),
        signs);
    return {new_signs, new_perm};
  }

  v128_box2d operator*(const v128_box2d &b) const {
    __m128i pairs = _mm_sign_epi32(
        _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(b.data), perm)),
        signs);
    return sort_bounds(pairs);
  }
};
