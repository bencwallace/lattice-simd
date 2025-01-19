#include <gtest/gtest.h>
#include <random>

#include "s_trans.h"
#include "v128_trans2d.h"
#include "v64_trans2d.h"

TEST(Consistency, AddPoints) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int32_t> dist_coords(-100, 100);

  s_point<2> s_p = {dist_coords(gen), dist_coords(gen)};
  s_point<2> s_q = {dist_coords(gen), dist_coords(gen)};
  v128_point2d v128_p = {s_p[0], s_p[1]};
  v128_point2d v128_q = {s_q[0], s_q[1]};

  s_point<2> s_r = s_p + s_q;
  v128_point2d v128_r = v128_p + v128_q;

  EXPECT_EQ(s_r[0], v128_r[0]);
  EXPECT_EQ(s_r[1], v128_r[1]);
}

TEST(Consistency, TransformPoint) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int32_t> dist_coords(-100, 100);
  std::uniform_int_distribution<int32_t> dist_signs(0, 1);

  for (size_t i = 0; i < 10; ++i) {
    std::array<int32_t, 2> coords = {dist_coords(gen), dist_coords(gen)};
    std::array<int32_t, 2> signs = {2 * dist_signs(gen) - 1,
                                    2 * dist_signs(gen) - 1};
    std::array<uint32_t, 2> perm = {0, 1};
    std::random_shuffle(perm.begin(), perm.end());

    s_point<2> s_p = coords;
    v64_point2d v64_p = coords;
    v128_point2d v128_p = coords;

    s_trans<2> s_t = {signs, perm};
    v64_trans2d v64_t = {signs, perm == std::array<uint32_t, 2>{1, 0}};
    v128_trans2d v128_t = {signs, perm};

    s_point<2> s_q = s_t * s_p;
    v64_point2d v64_q = v64_t * v64_p;
    v128_point2d v128_q = v128_t * v128_p;

    EXPECT_EQ(s_q[0], v64_q[0]);
    EXPECT_EQ(s_q[1], v64_q[1]);

    EXPECT_EQ(s_q[0], v128_q[0]);
    EXPECT_EQ(s_q[1], v128_q[1]);
  }
}

TEST(Consistency, Compose) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int32_t> dist_signs(0, 1);

  for (size_t i = 0; i < 10; ++i) {
    std::array<int32_t, 2> signs1 = {2 * dist_signs(gen) - 1,
                                     2 * dist_signs(gen) - 1};
    std::array<int32_t, 2> signs2 = {2 * dist_signs(gen) - 1,
                                     2 * dist_signs(gen) - 1};
    std::array<uint32_t, 2> perm1 = {0, 1};
    std::array<uint32_t, 2> perm2 = {0, 1};
    std::random_shuffle(perm1.begin(), perm1.end());
    std::random_shuffle(perm2.begin(), perm2.end());

    s_trans<2> s_t1 = {signs1, perm1};
    s_trans<2> s_t2 = {signs2, perm2};
    v128_trans2d v128_t1 = {signs1, perm1};
    v128_trans2d v128_t2 = {signs2, perm2};

    s_trans<2> s_t = s_t1 * s_t2;
    v128_trans2d v128_t = v128_t1 * v128_t2;

    auto temp1 = std::pair{s_t.signs[0], s_t.perm[0]};
    auto temp2 = v128_t[0];
    EXPECT_EQ(temp1, temp2);
  }
}

TEST(Consistency, TranslateBox) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int32_t> dist_coords(-100, 100);

  for (size_t i = 0; i < 10; ++i) {
    s_point<2> s_p = {dist_coords(gen), dist_coords(gen)};
    v128_point2d v128_p = {s_p[0], s_p[1]};

    std::array<int32_t, 4> coords = {dist_coords(gen), dist_coords(gen),
                                     dist_coords(gen), dist_coords(gen)};
    std::array<int32_t, 2> interval1 = {std::min(coords[0], coords[1]),
                                        std::max(coords[0], coords[1])};
    std::array<int32_t, 2> interval2 = {std::min(coords[2], coords[3]),
                                        std::max(coords[2], coords[3])};
    auto s_b = s_box<2>({s_interval{interval1[0], interval1[1]},
                         s_interval{interval2[0], interval2[1]}});
    auto v128_b = v128_box2d({v128_interval{interval1[0], interval1[1]},
                              v128_interval{interval2[0], interval2[1]}});

    s_box<2> s_c = s_b + s_p;
    v128_box2d v128_c = v128_b + v128_p;
    EXPECT_EQ(s_c[0], v128_c[0]);
    EXPECT_EQ(s_c[1], v128_c[1]);
  }
}

TEST(Consistency, TransformBox) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int32_t> dist_coords(-100, 100);
  std::uniform_int_distribution<int32_t> dist_signs(0, 1);

  for (size_t i = 0; i < 10; ++i) {
    std::array<int32_t, 2> signs = {2 * dist_signs(gen) - 1,
                                    2 * dist_signs(gen) - 1};
    std::array<uint32_t, 2> perm = {0, 1};
    std::random_shuffle(perm.begin(), perm.end());
    s_trans<2> s_t = {signs, perm};
    v128_trans2d v128_t = {signs, perm};

    std::array<int32_t, 4> coords = {dist_coords(gen), dist_coords(gen),
                                     dist_coords(gen), dist_coords(gen)};
    std::array<int32_t, 2> interval1 = {std::min(coords[0], coords[1]),
                                        std::max(coords[0], coords[1])};
    std::array<int32_t, 2> interval2 = {std::min(coords[2], coords[3]),
                                        std::max(coords[2], coords[3])};
    auto s_b = s_box<2>({s_interval{interval1[0], interval1[1]},
                         s_interval{interval2[0], interval2[1]}});
    auto v128_b = v128_box2d({v128_interval{interval1[0], interval1[1]},
                              v128_interval{interval2[0], interval2[1]}});

    auto s_c = s_t * s_b;
    auto v128_c = v128_t * v128_b;
    EXPECT_EQ(s_c[0], v128_c[0]);
    EXPECT_EQ(s_c[1], v128_c[1]);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
