#include <benchmark/benchmark.h>

#include "v128_trans2d.h"

static void BM_V128AddPoints(benchmark::State &state) {
  v128_point2d p = {1, 2};
  v128_point2d q = {3, 4};
  benchmark::DoNotOptimize(p);
  benchmark::DoNotOptimize(q);

  v128_point2d r;
  for (auto _ : state) {
    r = p + q;
    benchmark::DoNotOptimize(r);
  }
}
BENCHMARK(BM_V128AddPoints);

static void BM_V128Union(benchmark::State &state) {
  v128_box2d box1({v128_interval{1, 3}, v128_interval{3, 5}});
  v128_box2d box2({v128_interval{2, 4}, v128_interval{2, 4}});
  benchmark::DoNotOptimize(box1);
  benchmark::DoNotOptimize(box2);

  v128_box2d box;
  for (auto _ : state) {
    box = box1 | box2;
    benchmark::DoNotOptimize(box);
  }
}
BENCHMARK(BM_V128Union);

static void BM_V128Transform(benchmark::State &state) {
  v128_point2d p = {1, 2};
  v128_trans2d t = {{-1, 1}, {1, 0}};
  benchmark::DoNotOptimize(t);

  v128_point2d q;
  for (auto _ : state) {
    q = t * p;
    benchmark::DoNotOptimize(q);
  }
}
BENCHMARK(BM_V128Transform);

static void BM_V128Compose(benchmark::State &state) {
  v128_trans2d t1 = {{-1, -1}, {1, 0}};
  v128_trans2d t2 = {{-1, 1}, {0, 1}};
  benchmark::DoNotOptimize(t1);
  benchmark::DoNotOptimize(t2);

  v128_trans2d t;
  for (auto _ : state) {
    t = t1 * t2;
    benchmark::DoNotOptimize(t);
  }
}
BENCHMARK(BM_V128Compose);

static void BM_V128TranslateBox(benchmark::State &state) {
  auto b = v128_box2d({v128_interval{1, 3}, v128_interval{3, 4}});
  v128_point2d p = {1, 2};
  benchmark::DoNotOptimize(b);
  benchmark::DoNotOptimize(p);

  v128_box2d c;
  for (auto _ : state) {
    c = b + p;
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK(BM_V128TranslateBox);

static void BM_V128TransformBox(benchmark::State &state) {
  auto b = v128_box2d({v128_interval{1, 3}, v128_interval{3, 4}});
  v128_trans2d t = {{1, 1}, {1, 0}};
  benchmark::DoNotOptimize(b);
  benchmark::DoNotOptimize(t);

  v128_box2d c;
  for (auto _ : state) {
    c = t * b;
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK(BM_V128TransformBox);

BENCHMARK_MAIN();
