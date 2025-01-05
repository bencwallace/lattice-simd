#pragma once

#include <array>
#include <cstdint>

struct v64_point2d {
  uint64_t data;

  v64_point2d() : data(0) {}
  v64_point2d(uint64_t data) : data(data) {}
  v64_point2d(int32_t x, int32_t y) : data((int64_t(y) << 32) | uint32_t(x)) {}
  v64_point2d(std::array<int32_t, 2> coords)
      : v64_point2d(coords[0], coords[1]) {}

  bool operator==(const v64_point2d &other) const { return data == other.data; }

  int32_t operator[](size_t i) const { return (data >> (32 * i)) & 0xFFFFFFFF; }
};
