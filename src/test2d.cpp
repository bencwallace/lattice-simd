#include <gtest/gtest.h>

#include "s_trans.h"
#include "v32_trans2d16.h"
#include "v64_trans2d.h"
#include "v128_trans2d.h"

TEST(Consistency, TransformPoint) {
    std::array<int32_t, 2> coords = {1, 2};
    s_point<2> s_p = coords;
    v64_point2 v64_p = coords;
    v128_point2 v128_p = coords;

    std::array<int32_t, 2> signs = {1, -1};
    std::array<uint32_t, 2> perm = {1, 0};
    s_trans<2> s_t = {signs, perm};
    v64_trans2 v64_t = {signs, perm == std::array<uint32_t, 2>{1, 0}};
    v128_trans2 v128_t = {signs, perm};

    s_point<2> s_q = s_t * s_p;
    v64_point2 v64_q = v64_t * v64_p;
    v128_point2 v128_q = v128_t * v128_p;

    EXPECT_EQ(s_q[0], v64_q[0]);
    EXPECT_EQ(s_q[1], v64_q[1]);

    EXPECT_EQ(s_q[0], v128_q[0]);
    EXPECT_EQ(s_q[1], v128_q[1]);
}

TEST(Scalar, TransformPoint) {
    s_point<2> p = {1, 2};
    s_trans<2> t = {{1, -1}, {1, 0}};
    s_point<2> q = t * p;
    EXPECT_EQ(q, (s_point<2>{2, -1}));
}

TEST(Vector32, Coords) {
    v32_point2d16 p = {1, 2};
    EXPECT_EQ(p[0], 1);
    EXPECT_EQ(p[1], 2);
}

TEST(Vector64, Equal) {
    v64_point2 p = {2, -1};
    v64_point2 q = {2, -1};
    EXPECT_EQ(p, q);
}

TEST(Vector64, TransformPoint) {
    v64_point2 p = {1, 2};
    v64_trans2 t = {{1, -1}, true};
    v64_point2 q = t * p;
    EXPECT_EQ(q, (v64_point2{2, -1}));
}

TEST(Vector128, Equal) {
    v128_point2 p = {1, 2};
    v128_point2 q = {1, 2};
    EXPECT_EQ(p, q);
}

TEST(Vector128, TransformPoint) {
    v128_point2 p = {1, 2};
    v128_trans2 t = {{1, -1}, {1, 0}};
    v128_point2 q = t * p;
    EXPECT_EQ(q, (v128_point2{2, -1}));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
