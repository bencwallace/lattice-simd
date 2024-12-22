#include <benchmark/benchmark.h>

#include "s_trans.h"
#include "v64_trans2d.h"
#include "v128_trans2d.h"

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

static void BM_V128Transform(benchmark::State& state) {
    v128_point2 p = {1, 2};
    v128_trans2 t = {{-1, 1}, {1, 0}};
    v128_point2 q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t * p;
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_V128Transform);


static void BM_V64Transform(benchmark::State& state) {
    v64_point2 p = {1, 2};
    v64_trans2 t = {{-1, 1}, true};
    v64_point2 q;
    benchmark::DoNotOptimize(t);
    for (auto _: state) {
        q = t * p;
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(BM_V64Transform);

BENCHMARK_MAIN();