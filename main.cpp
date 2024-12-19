#include "s_trans4.h"
#include "v_trans4.h"

#include <benchmark/benchmark.h>

static void BM_SPermute(benchmark::State& state) {
    s_point4 p = {1, 2, 3, 4};
    s_trans4 t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    s_point4 q;
    benchmark::DoNotOptimize(t);        // force read from memory -- prevent pre-computation
    for (auto _: state) {
        q = t.permute(p);
        benchmark::DoNotOptimize(q);    // force write to memory -- prevents dead code elimination
    }
}
BENCHMARK(BM_SPermute);

static void BM_VPermuteMul(benchmark::State& state) {
    v_point4 p = {1, 2, 3, 4};
    v_trans4_mul t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    v_point4 q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t.permute(p);
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_VPermuteMul);

static void BM_SFlip(benchmark::State& state) {
    s_point4 p = {1, 2, 3, 4};
    s_trans4 t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    s_point4 q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t.flip(p);
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_SFlip);

static void BM_VFlipMul(benchmark::State& state) {
    v_point4 p = {1, 2, 3, 4};
    v_trans4_mul t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    v_point4 q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t.flip(p);
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_VFlipMul);

static void BM_STransform(benchmark::State& state) {
    s_point4 p = {1, 2, 3, 4};
    s_trans4 t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    s_point4 q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t(p);
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_STransform);

static void BM_VTransformMul(benchmark::State& state) {
    v_point4 p = {1, 2, 3, 4};
    v_trans4_mul t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    v_point4 q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t(p);
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_VTransformMul);

// static void BM_VTransformNeg(benchmark::State& state) {
//     v_point4 p = {1, 2, 3, 4};
//     v_trans4_neg t = {{1, 1, 1, 1}, {1, 2, 3, 0}};
//     v_point4 q;
//     // benchmark::DoNotOptimize(t);      // not working
//     for (auto _: state) {
//         q = t(p);
//         benchmark::DoNotOptimize(q);
//     }
// }
// BENCHMARK(BM_VTransformNeg);

BENCHMARK_MAIN();
