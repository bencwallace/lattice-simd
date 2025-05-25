#include <gtest/gtest.h>
#include <random>

#include "v128_trans2d.h"

TEST(Vector128, Equal) {
  v128_point2d p = {1, 2};
  v128_point2d q = {1, 2};
  EXPECT_EQ(p, q);
}

TEST(Vector128, Union1) {
  v128_box2d box1({v128_interval{1, 3}, v128_interval{3, 5}});
  v128_box2d box2({v128_interval{2, 4}, v128_interval{2, 4}});
  v128_box2d box = box1 | box2;
  EXPECT_EQ(box[0], v128_interval({1, 4}));
  EXPECT_EQ(box[1], v128_interval({2, 5}));
}

TEST(Vector128, Union2) {
  v128_box2d box1({v128_interval{-1, 5}, v128_interval{-2, -1}});
  v128_box2d box2({v128_interval{2, 4}, v128_interval{-1, 0}});
  v128_box2d box = box1 | box2;
  EXPECT_EQ(box[0], v128_interval({-1, 5}));
  EXPECT_EQ(box[1], v128_interval({-2, 0}));
}

TEST(Vector128, Intersection1) {
  v128_box2d box1({v128_interval{1, 3}, v128_interval{3, 5}});
  v128_box2d box2({v128_interval{2, 4}, v128_interval{2, 4}});
  v128_box2d box = box1 & box2;
  EXPECT_EQ(box[0], v128_interval({2, 3}));
  EXPECT_EQ(box[1], v128_interval({3, 4}));
}

TEST(Vector128, Intersection2) {
  v128_box2d box1({v128_interval{-1, 5}, v128_interval{-2, -1}});
  v128_box2d box2({v128_interval{2, 4}, v128_interval{-1, 0}});
  v128_box2d box = box1 & box2;
  EXPECT_EQ(box[0], v128_interval({2, 4}));
  EXPECT_EQ(box[1], v128_interval({-1, -1}));
}

TEST(Vector128, TransformPoint) {
  v128_point2d p = {1, 2};
  v128_trans2d t = {{1, -1}, {1, 0}};
  v128_point2d q = t * p;
  EXPECT_EQ(q, (v128_point2d{2, -1}));
}

TEST(Vector128, TranslateBox) {
  auto b = v128_box2d({v128_interval{1, 3}, v128_interval{3, 4}});
  v128_point2d p = {1, 2};
  v128_box2d c = b + p;
  EXPECT_EQ(c[0], v128_interval({2, 4}));
  EXPECT_EQ(c[1], v128_interval({5, 6}));
}

TEST(Vector128, TransformBox) {
  auto b = v128_box2d({v128_interval{1, 3}, v128_interval{3, 4}});
  v128_trans2d t = {{1, 1}, {1, 0}};
  v128_box2d c = t * b;
  EXPECT_EQ(c[0], v128_interval({3, 4}));
  EXPECT_EQ(c[1], v128_interval({1, 3}));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
