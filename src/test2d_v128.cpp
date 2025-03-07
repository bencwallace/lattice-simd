#include <gtest/gtest.h>
#include <random>

#include "v128_trans2d.h"

TEST(Vector128, Equal) {
  v128_point2d p = {1, 2};
  v128_point2d q = {1, 2};
  EXPECT_EQ(p, q);
}

TEST(Vector128, TransformPoint) {
  v128_point2d p = {1, 2};
  v128_trans2d t = {{1, -1}, {1, 0}};
  v128_point2d q = t * p;
  EXPECT_EQ(q, (v128_point2d{2, -1}));
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
