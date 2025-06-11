#include <gtest/gtest.h>
#include <random>

#include "s_trans.h"

TEST(Scalar, TransformPoint) {
  s_point<2> p = {1, 2};
  s_trans<2> t = {{1, -1}, {1, 0}};
  s_point<2> q = t * p;
  EXPECT_EQ(q, (s_point<2>{2, -1}));
}

TEST(Scalar, Union1) {
  auto box1 = s_box<2>({s_interval{1, 3}, s_interval{3, 5}});
  auto box2 = s_box<2>({s_interval{2, 4}, s_interval{2, 4}});
  s_box<2> box = box1 | box2;
  EXPECT_EQ(box[0], s_interval({1, 4}));
  EXPECT_EQ(box[1], s_interval({2, 5}));
}

TEST(Scalar, Union2) {
  auto box1 = s_box<2>({s_interval{-1, 5}, s_interval{-2, -1}});
  auto box2 = s_box<2>({s_interval{2, 4}, s_interval{-1, 0}});
  s_box<2> box = box1 | box2;
  EXPECT_EQ(box[0], s_interval({-1, 5}));
  EXPECT_EQ(box[1], s_interval({-2, 0}));
}

TEST(Scalar, Intersection1) {
  auto box1 = s_box<2>({s_interval{1, 3}, s_interval{3, 5}});
  auto box2 = s_box<2>({s_interval{2, 4}, s_interval{2, 4}});
  s_box<2> box = box1 & box2;
  EXPECT_EQ(box[0], s_interval({2, 3}));
  EXPECT_EQ(box[1], s_interval({3, 4}));
}

TEST(Scalar, Intersection2) {
  auto box1 = s_box<2>({s_interval{-1, 5}, s_interval{-2, -1}});
  auto box2 = s_box<2>({s_interval{2, 4}, s_interval{-1, 0}});
  s_box<2> box = box1 & box2;
  EXPECT_EQ(box[0], s_interval({2, 4}));
  EXPECT_EQ(box[1], s_interval({-1, -1}));
}

TEST(Scalar, BoxEmpty) {
  auto box1 = s_box<2>({s_interval{1, 3}, s_interval{3, 5}});
  EXPECT_FALSE(box1.empty());

  auto box2 = s_box<2>({s_interval{1, 1}, s_interval{2, 2}});
  EXPECT_FALSE(box2.empty());

  auto box3 = s_box<2>({s_interval{1, 1}, s_interval{2, 1}});
  EXPECT_TRUE(box3.empty());

  auto box4 = s_box<2>({s_interval{-1, 1}, s_interval{2, 1}});
  EXPECT_TRUE(box4.empty());
}

TEST(Scalar, TranslateBox) {
  auto b = s_box<2>({s_interval{1, 3}, s_interval{3, 4}});
  s_point<2> p = {1, 2};
  s_box<2> c = b + p;
  EXPECT_EQ(c[0], s_interval({2, 4}));
  EXPECT_EQ(c[1], s_interval({5, 6}));
}

TEST(Scalar, TransformBox) {
  auto b = s_box<2>({s_interval{1, 3}, s_interval{3, 4}});
  s_trans<2> t = {{1, 1}, {1, 0}};
  s_box<2> c = t * b;
  EXPECT_EQ(c[0], s_interval({3, 4}));
  EXPECT_EQ(c[1], s_interval({1, 3}));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
