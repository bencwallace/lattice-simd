#pragma once

#include <cstdint>

struct v64_point2 {
    uint64_t data;

    v64_point2() : data(0) {}
    v64_point2(uint64_t data) : data(data) {}
    v64_point2(int32_t x, int32_t y) : data((int64_t(y) << 32) | uint32_t(x)) {}

    bool operator==(const v64_point2 &other) const {
        return data == other.data;
    }
};
