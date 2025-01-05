#include <benchmark/benchmark.h>

#include "s_trans.h"
#include "v128_trans2d.h"
#include "v64_trans2d.h"

template <bool OptimizeT> static void BM_STransform(benchmark::State &state) {
  s_point<2> p = {1, 2};
  s_trans<2> t = {{-1, -1}, {1, 0}};
  s_point<2> q;
  if constexpr (!OptimizeT) {
    benchmark::DoNotOptimize(t);
  }
  for (auto _ : state) {
    q = t * p;
    benchmark::DoNotOptimize(q);
  }
}
BENCHMARK_TEMPLATE(BM_STransform, false);
BENCHMARK_TEMPLATE(BM_STransform, true);

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

template <bool OptimizeT> static void BM_SCompose(benchmark::State &state) {
  s_trans<2> t1 = {{-1, -1}, {1, 0}};
  s_trans<2> t2 = {{-1, 1}, {0, 1}};
  s_trans<2> t;
  if constexpr (!OptimizeT) {
    benchmark::DoNotOptimize(t1);
    benchmark::DoNotOptimize(t2);
  }
  for (auto _ : state) {
    t = t1 * t2;
    benchmark::DoNotOptimize(t);
  }
}
BENCHMARK_TEMPLATE(BM_SCompose, false);
BENCHMARK_TEMPLATE(BM_SCompose, true);

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
BENCHMARK_TEMPLATE(BM_SCompose, false);
BENCHMARK_TEMPLATE(BM_SCompose, true);

template <bool OptimizeT>
static void BM_STransformBox(benchmark::State &state) {
  auto b = s_box<2>({s_interval{1, 3}, s_interval{3, 4}});
  s_trans<2> t = {{1, 1}, {1, 0}};
  s_box<2> c;
  if constexpr (!OptimizeT) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(t);
  }
  for (auto _ : state) {
    c = t * b;
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK_TEMPLATE(BM_STransformBox, false);
BENCHMARK_TEMPLATE(BM_STransformBox, true);

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
