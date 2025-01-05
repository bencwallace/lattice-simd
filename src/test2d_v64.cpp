#include <gtest/gtest.h>
#include <random>

#include "v64_trans2d.h"

TEST(Vector64, Equal) {
  v64_point2d p = {2, -1};
  v64_point2d q = {2, -1};
  EXPECT_EQ(p, q);
}

TEST(Vector64, TransformPoint) {
  v64_point2d p = {1, 2};
  v64_trans2d t = {{1, -1}, true};
  v64_point2d q = t * p;
  EXPECT_EQ(q, (v64_point2d{2, -1}));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
