#include <benchmark/benchmark.h>

#include "v64_trans2d.h"

template <bool OptimizeT> static void BM_V64Transform(benchmark::State &state) {
  v64_point2d p = {1, 2};
  v64_trans2d t = {{-1, 1}, true};
  v64_point2d q;
  if constexpr (!OptimizeT) {
    benchmark::DoNotOptimize(t);
  }
  for (auto _ : state) {
    q = t * p;
    benchmark::DoNotOptimize(q);
  }
}
BENCHMARK_TEMPLATE(BM_V64Transform, false);
BENCHMARK_TEMPLATE(BM_V64Transform, true);

BENCHMARK_MAIN();
