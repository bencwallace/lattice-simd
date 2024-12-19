#include <array>
#include <cstdint>
#include <immintrin.h>

template <int N>
struct v_point;

template <>
struct v_point<4> {
    __m128i data;

    v_point() : data(_mm_setzero_si128()) {}
    v_point(__m128i data) : data(data) {}
    v_point(int32_t x, int32_t y, int32_t z, int32_t w) : data(_mm_set_epi32(w, z, y, x)) {}
};

template <int N>
struct v_trans_mul;

template <>
struct v_trans_mul<4> {
    __m128i signs;
    __m128i perm;

    v_trans_mul(std::array<int32_t, 4> signs, std::array<uint32_t, 4> perm)
        : signs(_mm_set_epi32(signs[3], signs[2], signs[1], signs[0])), perm(_mm_set_epi32(perm[3], perm[2], perm[1], perm[0])) {}

    v_point<4> permute(const v_point<4> &p) const {
        __m128i temp = _mm_shuffle_epi8(p.data, perm);
        return temp;
    }

    v_point<4> flip(const v_point<4> &p) const {
        __m128i temp = _mm_mullo_epi32(p.data, signs);
        return temp;
    }

    v_point<4> operator()(const v_point<4> &p) const {
        __m128i temp = _mm_shuffle_epi8(p.data, perm);
        temp = _mm_mullo_epi32(temp, signs);
        return temp;
    }
};

template <int N>
struct v_trans_neg;

template <>
struct v_trans_neg<4> {
    __mmask8 mask;
    __m128i perm;

    static __mmask8 array_to_mask(std::array<uint8_t, 4> arr) {
        return (arr[0] << 0) | (arr[1] << 1) | (arr[2] << 2) | (arr[3] << 3);
    }

    v_trans_neg(std::array<uint8_t, 4> mask, std::array<uint32_t, 4> perm)
        : mask(array_to_mask(mask)), perm(_mm_set_epi32(perm[3], perm[2], perm[1], perm[0])) {}

    v_point<4> permute(const v_point<4> &p) const {
        __m128i temp = _mm_shuffle_epi8(p.data, perm);
        return temp;
    }

    v_point<4> flip(const v_point<4> &p) const {
        __m128i temp = _mm_mask_sub_epi32(p.data, mask, _mm_setzero_si128(), p.data);
        return temp;
    }

    v_point<4> operator()(const v_point<4> &p) const {
        __m128i temp = _mm_shuffle_epi8(p.data, perm);
        temp = _mm_mask_sub_epi32(temp, mask, _mm_setzero_si128(), temp);
        return temp;
    }
};
