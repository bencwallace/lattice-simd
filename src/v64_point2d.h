#pragma once

#include <array>
#include <cstdint>

struct v64_point2 {
  uint64_t data;

  v64_point2() : data(0) {}
  v64_point2(uint64_t data) : data(data) {}
  v64_point2(int32_t x, int32_t y) : data((int64_t(y) << 32) | uint32_t(x)) {}
  v64_point2(std::array<int32_t, 2> coords)
      : v64_point2(coords[0], coords[1]) {}

  bool operator==(const v64_point2 &other) const { return data == other.data; }

  int32_t operator[](size_t i) const { return (data >> (32 * i)) & 0xFFFFFFFF; }
};
