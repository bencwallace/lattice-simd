#include "s_trans.h"
#include "v_trans.h"

#include <benchmark/benchmark.h>

static void BM_SPermute(benchmark::State& state) {
    s_point<4> p = {1, 2, 3, 4};
    s_trans<4> t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    s_point<4> q;
    benchmark::DoNotOptimize(t);        // force read from memory -- prevent pre-computation
    for (auto _: state) {
        q = t.permute(p);
        benchmark::DoNotOptimize(q);    // force write to memory -- prevents dead code elimination
    }
}
BENCHMARK(BM_SPermute);

static void BM_VPermute(benchmark::State& state) {
    v_point<4> p = {1, 2, 3, 4};
    v_trans<4> t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    v_point<4> q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t.permute(p);
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_VPermute);

static void BM_SFlip(benchmark::State& state) {
    s_point<4> p = {1, 2, 3, 4};
    s_trans<4> t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    s_point<4> q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t.flip(p);
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_SFlip);

static void BM_VFlip(benchmark::State& state) {
    v_point<4> p = {1, 2, 3, 4};
    v_trans<4> t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    v_point<4> q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t.flip(p);
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_VFlip);

static void BM_STransform(benchmark::State& state) {
    s_point<4> p = {1, 2, 3, 4};
    s_trans<4> t = {{-1, -1, -1, -1}, {1, 2, 3, 0}};
    s_point<4> q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t * p;
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_STransform);

static void BM_VTransform(benchmark::State& state) {
    v_point<4> p = {1, 2, 3, 4};
    v_trans<4> t = {{-1, -1, -1, -1}, {1, 0, 3, 2}};
    v_point<4> q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t * p;
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_VTransform);

BENCHMARK_MAIN();
