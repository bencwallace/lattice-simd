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
};
