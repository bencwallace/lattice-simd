#pragma once

#include <array>
#include <immintrin.h>

static inline uint32_t extract_epi32(__m128i data, size_t i) {
  switch (i) {
  case 0:
    return _mm_extract_epi32(data, 0);
  case 1:
    return _mm_extract_epi32(data, 1);
  case 2:
    return _mm_extract_epi32(data, 2);
  case 3:
    return _mm_extract_epi32(data, 3);
  default:
    return 0;
  }
}

static inline __m128i insert_epi32(__m128i data, int val, size_t i) {
  switch (i) {
  case 0:
    return _mm_insert_epi32(data, val, 0);
  case 1:
    return _mm_insert_epi32(data, val, 1);
  case 2:
    return _mm_insert_epi32(data, val, 2);
  case 3:
    return _mm_insert_epi32(data, val, 3);
  default:
    return data;
  }
}

struct v128_point2d {
  __m128i data;

  v128_point2d() : data(_mm_setzero_si128()) {}
  v128_point2d(__m128i data) : data(data) {}
  v128_point2d(int32_t x, int32_t y) : data(_mm_setr_epi32(x, y, 0, 0)) {}
  v128_point2d(std::array<int32_t, 2> coords)
      : v128_point2d(coords[0], coords[1]) {}

  bool operator==(const v128_point2d &other) const {
    return _mm_movemask_epi8(_mm_cmpeq_epi32(data, other.data)) == 0xFFFF;
  }

  int32_t operator[](size_t i) const { return extract_epi32(data, i); }

  v128_point2d operator+(const v128_point2d &other) const {
    return _mm_add_epi32(data, other.data);
  }

  v128_point2d operator-(const v128_point2d &other) const {
    return _mm_sub_epi32(data, other.data);
  }

  static v128_point2d unit() { return v128_point2d(1, 0); }
};

v128_point2d operator*(int k, const v128_point2d &p) {
  return _mm_mullo_epi32(_mm_set1_epi32(k), p.data);
}
