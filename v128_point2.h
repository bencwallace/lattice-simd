#pragma once

#include <immintrin.h>

struct v128_point2 {
    __m128i data;

    v128_point2() : data(_mm_setzero_si128()) {}
    v128_point2(__m128i data) : data(data) {}
    v128_point2(int32_t x, int32_t y) : data(_mm_setr_epi32(x, y, 0, 0)) {}

    bool operator==(const v128_point2 &other) const {
        return _mm_movemask_epi8(_mm_cmpeq_epi32(data, other.data)) == 0xFFFF;
    }
};
