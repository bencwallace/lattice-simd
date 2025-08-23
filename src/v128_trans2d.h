#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <immintrin.h>
#include <random>
#include <span>
#include <stdexcept>

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

  v128_box2d(std::span<const v128_point2d> points) {
    std::array<int, 2> min;
    std::array<int, 2> max;
    min.fill(std::numeric_limits<int>::max());
    max.fill(std::numeric_limits<int>::min());
    for (const auto &p : points) {
      for (int i = 0; i < 2; ++i) { // TODO: vectorize (not urgent)
        min[i] = std::min(min[i], p[i]);
        max[i] = std::max(max[i], p[i]);
      }
    }
    // anchor at (1, 0, ..., 0)
    data = insert_epi32(data, min[0] - points[0][0] + 1, 0);
    data = insert_epi32(data, max[0] - points[0][0] + 1, 2);
    data = insert_epi32(data, min[1] - points[0][1], 1);
    data = insert_epi32(data, max[1] - points[0][1], 3);
  }

  v128_interval operator[](size_t i) const {
    return {int32_t(extract_epi32(data, i)),
            int32_t(extract_epi32(data, i + 2))};
  }

  v128_box2d operator+(const v128_point2d &p) const {
    __m128i offset = _mm_shuffle_epi32(p.data, _MM_SHUFFLE(1, 0, 1, 0));
    return _mm_add_epi32(data, offset);
  }

  // TODO: double-check
  v128_box2d operator-(const v128_point2d &b) {
    __m128i offset = _mm_shuffle_epi32(b.data, _MM_SHUFFLE(1, 0, 1, 0));
    return _mm_sub_epi32(data, offset);
  }

  v128_box2d operator|(const v128_box2d &b) const {
    // TODO: Look into using `_mm_maskz_{min,max}_epi32`
    // For example: `__m128i mins = _mm_min_epi32(0b0011, data, b.data)`
    // For now, benchmarks are crashing when I enable AVX512 in the build
    __m128i mins = _mm_min_epi32(data, b.data);
    __m128i maxs = _mm_max_epi32(data, b.data);
    return _mm_blend_epi32(mins, maxs, 0b1100);
  }

  v128_box2d operator&(const v128_box2d &b) const {
    // TODO: See comment under `operator|`
    __m128i mins = _mm_min_epi32(data, b.data);
    __m128i maxs = _mm_max_epi32(data, b.data);
    return _mm_blend_epi32(mins, maxs, 0b0011);
  }

  bool empty() const {
    __m128i swapped = _mm_shuffle_epi32(data, _MM_SHUFFLE(1, 0, 3, 2));
    __m128i cmp = _mm_cmpgt_epi32(data, swapped);
    auto result = _mm_cvtsi128_si64(cmp); // Only check the lower 64 bits
    return result != 0;
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

inline __m128i permutevar_epi32(__m128i data, __m128i perm) {
  return _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(data), perm));
}

// Note that many of the implementations below conflate the permutation and the
// inverse permutation since these are identical in 2 dimensions.
struct v128_trans2d {
  __m128i signs;
  __m128i perm;

  v128_trans2d() : signs(_mm_set1_epi32(1)), perm(_mm_setr_epi32(0, 1, 2, 3)) {}
  v128_trans2d(__m128i signs, __m128i perm) : signs(signs), perm(perm) {}

  // Since only two elements of each array are used, there are multiple possible
  // ways to fill the vectors. The choice below is especially convenient for the
  // implementation of operator*(const v128_box2 &b)
  v128_trans2d(std::array<int32_t, 2> signs, std::array<uint32_t, 2> perm)
      : signs(_mm_setr_epi32(signs[0], signs[1], signs[0], signs[1])),
        perm(_mm_setr_epi32(perm[0], perm[1], 2 + perm[0], 2 + perm[1])) {}

  bool operator==(const v128_trans2d &other) const {
    return _mm_movemask_epi8(_mm_cmpeq_epi32(signs, other.signs)) == 0xFFFF &&
           _mm_movemask_epi8(_mm_cmpeq_epi32(perm, other.perm)) == 0xFFFF;
  }

  static v128_trans2d from_points(const v128_point2d &p,
                                  const v128_point2d &q) {
    auto result = v128_trans2d();
    v128_point2d diff = q - p;
    int idx = -1;
    for (int i = 0; i < 2; ++i) {
      if (std::abs(diff[i]) == 1) {
        if (idx == -1) {
          idx = i;
        } else {
          throw std::invalid_argument("Points are not adjacent");
        }
      }
    }
    if (idx == -1) {
      throw std::invalid_argument("Points are not adjacent");
    }

    insert_epi32(result.perm, idx, 0);
    insert_epi32(result.perm, 0, idx);
    insert_epi32(result.signs, -diff[idx], 0);
    insert_epi32(result.signs, diff[idx], idx);

    return result;
  }

  template <typename Gen> static v128_trans2d rand(Gen &gen) {
    // TODO: Can this be vectorized?
    static std::bernoulli_distribution flip_;

    std::array<uint32_t, 2> perm = {0, 1};
    std::array<int32_t, 2> signs;
    for (int i = 0; i < 2; ++i) {
      signs[i] = 2 * flip_(gen) - 1;
    }
    std::shuffle(perm.begin(), perm.end(), gen);
    return v128_trans2d(signs, perm);
  }

  static v128_trans2d rand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return rand(gen);
  }

  std::pair<int32_t, uint32_t> operator[](size_t i) const {
    return {extract_epi32(signs, i), extract_epi32(perm, i)};
  }

  v128_point2d operator*(const v128_point2d &p) const {
    return _mm_sign_epi32(permutevar_epi32(p.data, perm), signs);
  }

  v128_trans2d operator*(const v128_trans2d &t) const {
    __m128i new_perm = permutevar_epi32(perm, t.perm);
    __m128i new_signs = _mm_sign_epi32(permutevar_epi32(t.signs, perm), signs);
    return {new_signs, new_perm};
  }

  v128_box2d operator*(const v128_box2d &b) const {
    __m128i pairs = _mm_sign_epi32(permutevar_epi32(b.data, perm), signs);
    return sort_bounds(pairs);
  }

  bool is_identity() const {
    return _mm_movemask_epi8(_mm_cmpeq_epi32(signs, _mm_set1_epi32(1))) ==
               0xFFFF &&
           _mm_movemask_epi8(
               _mm_cmpeq_epi32(perm, _mm_setr_epi32(0, 1, 2, 3))) == 0xFFFF;
  }

  v128_trans2d inverse() const {
    // In general, the inverse is given by signs S' and permutations P' such
    // that P' = P^-1 and S' = S P. The latter's components can be obtained by
    // viewing S as a vector and applying P to it (i.e. permuting it). Moreover,
    // in 2D, P^-1 is the same as P.
    return v128_trans2d(permutevar_epi32(signs, perm), perm);
  }
};
