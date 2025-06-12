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

TEST(Vector128, BoxEmpty) {
  v128_box2d box1({v128_interval{1, 3}, v128_interval{3, 5}});
  EXPECT_FALSE(box1.empty());

  v128_box2d box2({v128_interval{1, 1}, v128_interval{2, 2}});
  EXPECT_FALSE(box2.empty());

  v128_box2d box3({v128_interval{1, 1}, v128_interval{2, 1}});
  EXPECT_TRUE(box3.empty());

  v128_box2d box4({v128_interval{-1, 1}, v128_interval{2, 1}});
  EXPECT_TRUE(box4.empty());
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

TEST(Vector128, TransformIsIdentity) {
  v128_trans2d t1 = {{1, 1}, {0, 1}};
  EXPECT_TRUE(t1.is_identity());

  v128_trans2d t2 = {{-1, 1}, {0, 1}};
  EXPECT_FALSE(t2.is_identity());

  v128_trans2d t3 = {{1, 1}, {1, 0}};
  EXPECT_FALSE(t3.is_identity());

  v128_trans2d t4 = {{1, -1}, {0, 1}};
  EXPECT_FALSE(t4.is_identity());
}

TEST(Vector128, Inverse) {
  v128_trans2d id = {{1, 1}, {0, 1}};
  v128_point2d o = {0, 0};
  v128_point2d e1 = {1, 0};
  v128_point2d e2 = {0, 1};
  ASSERT_EQ(e1, id * e1);
  ASSERT_EQ(e2, id * e2);

  v128_trans2d t = {{-1, 1}, {1, 0}};
  v128_trans2d t_inv = t.inverse();
  auto f1 = t * e1;
  auto f2 = t * e2;
  ASSERT_EQ(f1, e2);
  ASSERT_EQ(f2, o - e1);
  EXPECT_EQ(e1, t_inv * f1);
  EXPECT_EQ(e2, t_inv * f2);

  auto ttinv = t * t_inv;
  auto tinvt = t_inv * t;
  EXPECT_EQ(ttinv, id);
  EXPECT_EQ(tinvt, id);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
