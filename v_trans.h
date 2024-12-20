#include <array>
#include <cstdint>
#include <emmintrin.h>
#include <immintrin.h>

template <int N>
struct v_point;

template <>
struct v_point<2> {
    __m128i data;

    v_point() : data(_mm_setzero_si128()) {}
    v_point(__m128i data) : data(data) {}
    v_point(int32_t x, int32_t y) : data(_mm_setr_epi32(x, y, 0, 0)) {}

    bool operator==(const v_point<2> &other) const {
        return _mm_movemask_epi8(_mm_cmpeq_epi32(data, other.data)) == 0xFFFF;
    }
};

template <>
struct v_point<4> {
    __m128i data;

    v_point() : data(_mm_setzero_si128()) {}
    v_point(__m128i data) : data(data) {}
    v_point(int32_t x, int32_t y, int32_t z, int32_t w) : data(_mm_set_epi32(w, z, y, x)) {}

    bool operator==(const v_point<4> &other) const {
        return _mm_movemask_epi8(_mm_cmpeq_epi32(data, other.data)) == 0xFFFF;
    }
};

template <int N>
struct v_trans;

template <>
struct v_trans<4> {
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

    static __m128i perm_to_mm(std::array<uint32_t, 4> perm) {
        return _mm_setr_epi8(
            perm[0] * 4, perm[0] * 4 + 1, perm[0] * 4 + 2, perm[0] * 4 + 3,
            perm[1] * 4, perm[1] * 4 + 1, perm[1] * 4 + 2, perm[1] * 4 + 3,
            perm[2] * 4, perm[2] * 4 + 1, perm[2] * 4 + 2, perm[2] * 4 + 3,
            perm[3] * 4, perm[3] * 4 + 1, perm[3] * 4 + 2, perm[3] * 4 + 3);
    }

    v_trans(std::array<int32_t, 4> signs, std::array<uint32_t, 4> perm)
        : signs(_mm_set_epi32(signs[3], signs[2], signs[1], signs[0])),
          perm(perm_to_mm(perm)),
          iperm(perm_to_mm(invert_perm(perm)))
        {}

    v_point<4> permute(const v_point<4> &p) const {
        __m128i temp = _mm_shuffle_epi8(p.data, perm);
        return temp;
    }

    v_point<4> flip(const v_point<4> &p) const {
        __m128i temp = _mm_sign_epi32(p.data, signs);
        return temp;
    }

    v_point<4> operator*(const v_point<4> &p) const {
        __m128i temp = _mm_shuffle_epi8(p.data, iperm);
        temp = _mm_sign_epi32(temp, signs);
        return temp;
    }
};

template <>
struct v_trans<2> : v_trans<4> {
    v_trans(std::array<int32_t, 2> signs, std::array<uint32_t, 2> perm)
        : v_trans<4>(std::array<int32_t, 4>{signs[0], signs[1], 1, 1}, std::array<uint32_t, 4>{perm[0], perm[1], 2, 3}) {}

    v_point<2> operator*(const v_point<2> &p) const {
        __m128i temp = _mm_shuffle_epi8(p.data, iperm);
        temp = _mm_sign_epi32(temp, signs);
        return temp;
    }
};
