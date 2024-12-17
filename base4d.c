#include <stdio.h>
#include <stdint.h>

typedef struct {
    int32_t signs[4];
    uint32_t perm[4];
} trans4;

void transform(int32_t *q, trans4 *t, int32_t *p) {
    for (size_t i = 0; i < 4; i++) {
        q[i] = p[t->perm[i]] * t->signs[i];
    }
}

// void transform(int32_t *q, trans4 *t, int32_t *p) {
//     for (size_t i = 0; i < 4; i++) {
//         q[i] = p[t->perm[i]];
//     }
// }

// void transform(int32_t *q, trans4 *t, int32_t *p) {
//     for (size_t i = 0; i < 4; i++) {
//         q[i] = p[i] * t->signs[i];
//     }
// }



int main() {
    int32_t p[] = {1, 2, 3, 4};
    trans4 t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};

    int32_t q[4];
    int32_t *pp = p;
    int32_t *qq = q;
    size_t steps = 100000;
    size_t iters = steps * 100000 + 1;
    for (size_t i = 0; i < iters; i++) {
        transform(qq, &t, pp);

        // swap
        int32_t *temp = pp;
        pp = qq;
        qq = temp;
    }
    int32_t *r = (iters % 2 == 0) ? p : q;

    printf("(%d, %d, %d, %d)\n", r[0], r[1], r[2], r[3]);

    return 0;
}

// _m128i
// 	_mm_shuffle_epi8
// 	_mm_permutexvar_epi16
// 	_mm_permutexvar_epi8
// 	_mm_permutexvar_ph
// 	_mm_permutex2var_... (3 args)

// _m128d
// 	_mm_permutevar_pd

// _m128
// 	_mm_permutevar_ps
