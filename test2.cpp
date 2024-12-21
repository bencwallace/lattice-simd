#include <gtest/gtest.h>

#include "s_trans.h"
#include "v_trans.h"

TEST(Scalar, PermutePoint) {
    s_point<2> p = {1, 2};
    s_trans<2> t = {{1, -1}, {1, 0}};
    s_point<2> q = t.permute(p);
    EXPECT_EQ(q, (s_point<2>{2, 1}));
}

TEST(Scalar, FlipPoint) {
    s_point<2> p = {1, 2};
    s_trans<2> t = {{-1, 1}, {1, 0}};
    s_point<2> q = t.flip(p);
    EXPECT_EQ(q, (s_point<2>{-1, 2}));
}

TEST(Scalar, TransformPoint) {
    s_point<2> p = {1, 2};
    s_trans<2> t = {{1, -1}, {1, 0}};
    s_point<2> q = t * p;
    EXPECT_EQ(q, (s_point<2>{2, -1}));
}

TEST(VectorShort, Equal) {
    v_point_short p = {2, -1};
    v_point_short q = {2, -1};
    EXPECT_EQ(p, q);
}

TEST(VectorShort, PermutePoint) {
    v_point_short p = {1, 2};
    v_trans_short t = {{1, -1}, true};
    v_point_short q = t.permute(p);
    EXPECT_EQ(q, (v_point_short{2, 1}));
}

TEST(VectorShort, FlipPoint) {
    v_point_short p = {1, 2};
    v_trans_short t = {{-1, 1}, 0};
    v_point_short q = t.flip(p);
    EXPECT_EQ(q, (v_point_short{-1, 2}));
}

TEST(VectorShort, TransformPoint) {
    v_point_short p = {1, 2};
    v_trans_short t = {{1, -1}, true};
    v_point_short q = t * p;
    EXPECT_EQ(q, (v_point_short{2, -1}));
}

TEST(Vector, Equal) {
    v_point<2> p = {1, 2};
    v_point<2> q = {1, 2};
    EXPECT_EQ(p, q);
}

TEST(Vector, PermutePoint) {
    v_point<2> p = {1, 2};
    v_trans<2> t = {{1, -1}, {1, 0}};
    v_point<2> q = t.permute(p);
    EXPECT_EQ(q, (v_point<2>{2, 1}));
}

TEST(Vector, FlipPoint) {
    v_point<2> p = {1, 2};
    v_trans<2> t = {{-1, 1}, {1, 0}};
    v_point<2> q = t.flip(p);
    EXPECT_EQ(q, (v_point<2>{-1, 2}));
}

TEST(Vector, TransformPoint) {
    v_point<2> p = {1, 2};
    v_trans<2> t = {{1, -1}, {1, 0}};
    v_point<2> q = t * p;
    EXPECT_EQ(q, (v_point<2>{2, -1}));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
