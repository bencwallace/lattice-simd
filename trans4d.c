#include <stdio.h>
#include <immintrin.h>
#include <xmmintrin.h>


typedef __m128i point4;

typedef struct {
    __m128i signs;
    __m128i perm;
} trans2x4;


int transform(point4 *q, trans2x4 *t, point4 *p) {
    __m256i temp = _mm256_castsi128_si256(_mm_mullo_epi32(*p, t->signs));
    temp = _mm256_permutevar8x32_epi32(temp, _mm256_castsi128_si256(t->perm));
    *q = _mm256_castsi256_si128(temp);
    return 0;
}

int main() {
    point4 p = _mm_setr_epi32(4, 3, 2, 1);

    trans2x4 t;
    t.signs = _mm_setr_epi32(-1, -1, -1, -1);
    t.perm = _mm_setr_epi32(0, 1, 3, 2);

    __m128i result;
    transform(&result, &t, &p);

    int x0 = _mm_extract_epi32(result, 0);
    int x1 = _mm_extract_epi32(result, 1);
    int x2 = _mm_extract_epi32(result, 2);
    int x3 = _mm_extract_epi32(result, 3);
    printf("(%d, %d, %d, %d)\n", x0, x1, x2, x3);

    return 0;
}
