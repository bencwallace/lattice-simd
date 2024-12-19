#include <gtest/gtest.h>

#include "s_trans.h"
#include "v_trans.h"

TEST(Scalar, TransformPoint) {
    s_point<2> p = {1, 2};
    s_trans<2> t = {{-1, -1}, {1, 0}};
    s_point<2> q = t * p;
    EXPECT_EQ(q, (s_point<2>{-2, -1}));
}

TEST(Vector, Equal) {
    v_point<2> p = {1, 2};
    v_point<2> q = {1, 2};
    EXPECT_EQ(p, q);
}

TEST(Vector, TransformPoint) {
    v_point<2> p = {1, 2};
    v_trans_mul<2> t = {{1, 1}, {1, 0}};
    v_point<2> q = t * p;
    EXPECT_EQ(q, (v_point<2>{2, 1}));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
