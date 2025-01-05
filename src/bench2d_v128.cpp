#include <benchmark/benchmark.h>

#include "v128_trans2d.h"

template <bool OptimizeT>
static void BM_V128Transform(benchmark::State &state) {
  v128_point2d p = {1, 2};
  v128_trans2d t = {{-1, 1}, {1, 0}};
  v128_point2d q;
  if constexpr (!OptimizeT) {
    benchmark::DoNotOptimize(t);
  }
  for (auto _ : state) {
    q = t * p;
    benchmark::DoNotOptimize(q);
  }
}
BENCHMARK_TEMPLATE(BM_V128Transform, false);
BENCHMARK_TEMPLATE(BM_V128Transform, true);

template <bool OptimizeT> static void BM_V128Compose(benchmark::State &state) {
  v128_trans2d t1 = {{-1, -1}, {1, 0}};
  v128_trans2d t2 = {{-1, 1}, {0, 1}};
  v128_trans2d t;
  if constexpr (!OptimizeT) {
    benchmark::DoNotOptimize(t1);
    benchmark::DoNotOptimize(t2);
  }
  for (auto _ : state) {
    t = t1 * t2;
    benchmark::DoNotOptimize(t);
  }
}
BENCHMARK_TEMPLATE(BM_V128Compose, false);
BENCHMARK_TEMPLATE(BM_V128Compose, true);

template <bool OptimizeT>
static void BM_V128TransformBox(benchmark::State &state) {
  auto b = v128_box2d({v128_interval{1, 3}, v128_interval{3, 4}});
  v128_trans2d t = {{1, 1}, {1, 0}};
  v128_box2d c;
  if constexpr (!OptimizeT) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(t);
  }
  for (auto _ : state) {
    c = t * b;
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK_TEMPLATE(BM_V128TransformBox, false);
BENCHMARK_TEMPLATE(BM_V128TransformBox, true);

BENCHMARK_MAIN();
