#include "s_trans.h"
#include "v_trans.h"

#include <benchmark/benchmark.h>

static void BM_STransform(benchmark::State& state) {
    s_point<2> p = {1, 2};
    s_trans<2> t = {{-1, -1}, {1, 0}};
    s_point<2> q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t * p;
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_STransform);

static void BM_VTransformMul(benchmark::State& state) {
    v_point<2> p = {1, 2};
    v_trans<2> t = {{-1, -1}, {1, 0}};
    v_point<2> q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t * p;
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_VTransformMul);

BENCHMARK_MAIN();
