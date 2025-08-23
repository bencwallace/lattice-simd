#include <gtest/gtest.h>
#include <random>

#include "s_trans.h"
#include "s_walk2d.cpp"

TEST(Scalar, TransformPoint) {
  s_point<2> p = {1, 2};
  s_trans<2> t = {{1, -1}, {1, 0}};
  s_point<2> q = t * p;
  EXPECT_EQ(q, (s_point<2>{2, -1}));
}

TEST(Vector128, Equal) {
  s_point<2> p = {1, 2};
  s_point<2> q = {1, 2};
  EXPECT_EQ(p, q);
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

TEST(Vector128, BoxFromSpan) {
  s_point<2> p1 = {1, 2};
  auto b1 = s_box<2>(std::vector{p1});
  EXPECT_EQ(b1[0].left, 1);
  EXPECT_EQ(b1[0].right, 1);
  EXPECT_EQ(b1[1].left, 0);
  EXPECT_EQ(b1[1].right, 0);

  s_point<2> p2 = {5, 5};
  auto b2 = s_box<2>(std::vector{p1, p2});
  EXPECT_EQ(b2[0].left, 1);
  EXPECT_EQ(b2[0].right, 5);
  EXPECT_EQ(b2[1].left, 0);
  EXPECT_EQ(b2[1].right, 3);

  auto b3 = s_box<2>(std::vector{p2, p1});
  EXPECT_EQ(b3[0].left, -3);
  EXPECT_EQ(b3[0].right, 1);
  EXPECT_EQ(b3[1].left, -3);
  EXPECT_EQ(b3[1].right, 0);

  s_point<2> p3 = {3, 4};
  auto b4 = s_box<2>(std::vector{p1, p2, p3});
  EXPECT_EQ(b4[0].left, 1);
  EXPECT_EQ(b4[0].right, 5);
  EXPECT_EQ(b4[1].left, 0);
  EXPECT_EQ(b4[1].right, 3);
}

TEST(Vector128, TransformPoint) {
  s_point<2> p = {1, 2};
  s_trans<2> t = {{1, -1}, {1, 0}};
  s_point<2> q = t * p;
  EXPECT_EQ(q, (s_point<2>{2, -1}));
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

TEST(Vector128, TransformIsIdentity) {
  s_trans<2> t1 = {{1, 1}, {0, 1}};
  EXPECT_TRUE(t1.is_identity());

  s_trans<2> t2 = {{-1, 1}, {0, 1}};
  EXPECT_FALSE(t2.is_identity());

  s_trans<2> t3 = {{1, 1}, {1, 0}};
  EXPECT_FALSE(t3.is_identity());

  s_trans<2> t4 = {{1, -1}, {0, 1}};
  EXPECT_FALSE(t4.is_identity());
}

TEST(Vector128, Inverse) {
  s_trans<2> id = {{1, 1}, {0, 1}};
  s_point<2> o = {0, 0};
  s_point<2> e1 = {1, 0};
  s_point<2> e2 = {0, 1};
  ASSERT_EQ(e1, id * e1);
  ASSERT_EQ(e2, id * e2);

  s_trans<2> t = {{-1, 1}, {1, 0}};
  s_trans<2> t_inv = t.inverse();
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
