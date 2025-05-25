#include <benchmark/benchmark.h>

#include "s_trans.h"

static void BM_SAddPoints(benchmark::State &state) {
  s_point<2> p = {1, 2};
  s_point<2> q = {3, 4};
  benchmark::DoNotOptimize(p);
  benchmark::DoNotOptimize(q);

  s_point<2> r;
  for (auto _ : state) {
    r = p + q;
    benchmark::DoNotOptimize(r);
  }
}
BENCHMARK(BM_SAddPoints);

static void BM_V128Union(benchmark::State &state) {
  auto box1 = s_box<2>({s_interval{1, 3}, s_interval{3, 5}});
  auto box2 = s_box<2>({s_interval{2, 4}, s_interval{2, 4}});
  benchmark::DoNotOptimize(box1);
  benchmark::DoNotOptimize(box2);

  s_box<2> box;
  for (auto _ : state) {
    box = box1 | box2;
    benchmark::DoNotOptimize(box);
  }
}
BENCHMARK(BM_V128Union);

static void BM_STransform(benchmark::State &state) {
  s_point<2> p = {1, 2};
  s_trans<2> t = {{-1, -1}, {1, 0}};
  benchmark::DoNotOptimize(t);

  s_point<2> q;
  for (auto _ : state) {
    q = t * p;
    benchmark::DoNotOptimize(q);
  }
}
BENCHMARK(BM_STransform);

static void BM_SCompose(benchmark::State &state) {
  s_trans<2> t1 = {{-1, -1}, {1, 0}};
  s_trans<2> t2 = {{-1, 1}, {0, 1}};
  benchmark::DoNotOptimize(t1);
  benchmark::DoNotOptimize(t2);

  s_trans<2> t;
  for (auto _ : state) {
    t = t1 * t2;
    benchmark::DoNotOptimize(t);
  }
}
BENCHMARK(BM_SCompose);

static void BM_STranslateBox(benchmark::State &state) {
  auto b = s_box<2>({s_interval{1, 3}, s_interval{3, 4}});
  s_point<2> p = {1, 2};
  benchmark::DoNotOptimize(b);
  benchmark::DoNotOptimize(p);

  s_box<2> c;
  for (auto _ : state) {
    c = b + p;
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK(BM_STranslateBox);

static void BM_STransformBox(benchmark::State &state) {
  auto b = s_box<2>({s_interval{1, 3}, s_interval{3, 4}});
  s_trans<2> t = {{1, 1}, {1, 0}};
  benchmark::DoNotOptimize(b);
  benchmark::DoNotOptimize(t);

  s_box<2> c;
  for (auto _ : state) {
    c = t * b;
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK(BM_STransformBox);

BENCHMARK_MAIN();
