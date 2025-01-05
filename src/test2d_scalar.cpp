#include <gtest/gtest.h>
#include <random>

#include "s_trans.h"

TEST(Scalar, TransformPoint) {
  s_point<2> p = {1, 2};
  s_trans<2> t = {{1, -1}, {1, 0}};
  s_point<2> q = t * p;
  EXPECT_EQ(q, (s_point<2>{2, -1}));
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
