#include <emmintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>


typedef __m128i point4;

typedef struct {
    __m128i signs;
    __m128i perm;
} trans4;


point4 transform(trans4 *t, point4 p) {
    __m128i temp = _mm_shuffle_epi8(p, t->perm);
    temp = _mm_mullo_epi32(temp, t->signs);
    return temp;
}

// point4 transform(trans4 *t, point4 p) {
//     __m128i temp = _mm_shuffle_epi8(p, t->perm);
//     return temp;
// }

// point4 transform(trans4 *t, point4 p) {
//     __m128i temp = _mm_mullo_epi32(p, t->signs);
//     return temp;
// }

int main() {
    point4 p = _mm_setr_epi32(1, 2, 3, 4);

    trans4 t;
    t.signs = _mm_setr_epi32(-1, -1, -1, -1);
    uint8_t perm[4] = {1, 2, 3, 0};
    t.perm = _mm_setr_epi8(
        perm[0] * 4, perm[0] * 4 + 1, perm[0] * 4 + 2, perm[0] * 4 + 3,
        perm[1] * 4, perm[1] * 4 + 1, perm[1] * 4 + 2, perm[1] * 4 + 3,
        perm[2] * 4, perm[2] * 4 + 1, perm[2] * 4 + 2, perm[2] * 4 + 3,
        perm[3] * 4, perm[3] * 4 + 1, perm[3] * 4 + 2, perm[3] * 4 + 3
    );

    size_t steps = 100000;
    size_t iters = steps * 100000 + 1;
    printf("iters: %zu\n", iters);
    for (size_t i = 0; i < iters; i++) {
        p = transform(&t, p);
    }

    int32_t x0 = _mm_extract_epi32(p, 0);
    int32_t x1 = _mm_extract_epi32(p, 1);
    int32_t x2 = _mm_extract_epi32(p, 2);
    int32_t x3 = _mm_extract_epi32(p, 3);
    printf("(%d, %d, %d, %d)\n", x0, x1, x2, x3);

    return 0;
}
