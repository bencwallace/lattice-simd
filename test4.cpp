#include <gtest/gtest.h>

#include "s_trans.h"
#include "v_trans.h"

TEST(Scalar, TransformPoint) {
    s_point<4> p = {1, 2, 3, 4};
    s_trans<4> t = {{-1, -1, -1, -1}, {1, 0, 3, 2}};
    s_point<4> q = t * p;
    EXPECT_EQ(q, (s_point<4>{-2, -1, -4, -3}));
}

TEST(Vector, Equal) {
    v_point<4> p = {1, 2, 3, 4};
    v_point<4> q = {1, 2, 3, 4};
    EXPECT_EQ(p, q);
}

TEST(Vector, TransformPoint) {
    v_point<4> p = {1, 2, 3, 4};
    v_trans_mul<4> t = {{-1, 1, -1, 1}, {1, 0, 3, 2}};
    v_point<4> q = t * p;
    EXPECT_EQ(q, (v_point<4>{-2, 1, -4, 3}));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
