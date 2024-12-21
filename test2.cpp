#include <gtest/gtest.h>

#include "s_trans.h"
#include "v128_point.h"
#include "v128_trans.h"
#include "v64_point2.h"
#include "v64_trans2.h"

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

TEST(Vector64, Equal) {
    v64_point2 p = {2, -1};
    v64_point2 q = {2, -1};
    EXPECT_EQ(p, q);
}

TEST(Vector64, PermutePoint) {
    v64_point2 p = {1, 2};
    v64_trans2 t = {{1, -1}, true};
    v64_point2 q = t.permute(p);
    EXPECT_EQ(q, (v64_point2{2, 1}));
}

TEST(Vector64, FlipPoint) {
    v64_point2 p = {1, 2};
    v64_trans2 t = {{-1, 1}, 0};
    v64_point2 q = t.flip(p);
    EXPECT_EQ(q, (v64_point2{-1, 2}));
}

TEST(Vector64, TransformPoint) {
    v64_point2 p = {1, 2};
    v64_trans2 t = {{1, -1}, true};
    v64_point2 q = t * p;
    EXPECT_EQ(q, (v64_point2{2, -1}));
}

TEST(Vector128, Equal) {
    v128_point<2> p = {1, 2};
    v128_point<2> q = {1, 2};
    EXPECT_EQ(p, q);
}

TEST(Vector128, PermutePoint) {
    v128_point<2> p = {1, 2};
    v128_trans<2> t = {{1, -1}, {1, 0}};
    v128_point<2> q = t.permute(p);
    EXPECT_EQ(q, (v128_point<2>{2, 1}));
}

TEST(Vector128, FlipPoint) {
    v128_point<2> p = {1, 2};
    v128_trans<2> t = {{-1, 1}, {1, 0}};
    v128_point<2> q = t.flip(p);
    EXPECT_EQ(q, (v128_point<2>{-1, 2}));
}

TEST(Vector128, TransformPoint) {
    v128_point<2> p = {1, 2};
    v128_trans<2> t = {{1, -1}, {1, 0}};
    v128_point<2> q = t * p;
    EXPECT_EQ(q, (v128_point<2>{2, -1}));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
