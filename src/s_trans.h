#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <span>

template <size_t N> using s_point = std::array<int32_t, N>;

template <size_t N> static s_point<N> unit(int i = 0) {
  s_point<N> p{};
  p[i] = 1;
  return p;
}

template <size_t N>
s_point<N> operator+(const s_point<N> a, const s_point<N> b) {
  s_point<N> c{};
  for (size_t i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
  }
  return c;
}

template <size_t N>
s_point<N> operator-(const s_point<N> a, const s_point<N> b) {
  s_point<N> c{};
  for (size_t i = 0; i < N; i++) {
    c[i] = a[i] - b[i];
  }
  return c;
}

template <size_t N> inline s_point<N> operator*(int k, const s_point<N> &p) {
  s_point<N> q{};
  for (size_t i = 0; i < N; i++) {
    q[i] = k * p[i];
  }
  return q;
}

struct s_interval {
  int32_t left;
  int32_t right;

  bool operator==(const s_interval &other) const {
    return left == other.left && right == other.right;
  }

  template <class T> bool operator==(const T &other) const {
    return left == other.left && right == other.right;
  }
};

template <size_t N> struct s_box {
  std::array<s_interval, N> intervals;

  s_box() = default;
  s_box(std::array<s_interval, N> intervals) : intervals(intervals) {}

  s_box(std::span<const s_point<N>> points) {
    std::array<int, N> min;
    std::array<int, N> max;
    min.fill(std::numeric_limits<int>::max());
    max.fill(std::numeric_limits<int>::min());
    for (const auto &p : points) {
      for (size_t i = 0; i < N; ++i) {
        min[i] = std::min(min[i], p[i]);
        max[i] = std::max(max[i], p[i]);
      }
    }
    // anchor at (1, 0, ..., 0)
    intervals[0] =
        s_interval(min[0] - points[0][0] + 1, max[0] - points[0][0] + 1);
    for (size_t i = 1; i < N; ++i) {
      intervals[i] = s_interval(min[i] - points[0][i], max[i] - points[0][i]);
    }
  }

  bool operator==(const s_box<N> &other) const {
    return intervals == other.intervals;
  }

  s_interval operator[](size_t i) const { return intervals[i]; }

  s_box operator+(const s_point<N> &p) const {
    std::array<s_interval, N> new_intervals{};
    for (size_t i = 0; i < N; i++) {
      new_intervals[i] = {intervals[i].left + p[i], intervals[i].right + p[i]};
    }
    return {new_intervals};
  }

  s_box operator-(const s_point<N> &b) const {
    std::array<s_interval, N> new_intervals{};
    for (size_t i = 0; i < N; i++) {
      new_intervals[i] = {intervals[i].left - b[i], intervals[i].right - b[i]};
    }
    return {new_intervals};
  }

  s_box operator|(const s_box<N> &b) const {
    std::array<s_interval, N> new_intervals{};
    for (size_t i = 0; i < N; i++) {
      new_intervals[i] = {std::min(intervals[i].left, b.intervals[i].left),
                          std::max(intervals[i].right, b.intervals[i].right)};
    }
    return {new_intervals};
  }

  s_box operator&(const s_box<N> &b) const {
    std::array<s_interval, N> new_intervals{};
    for (size_t i = 0; i < N; i++) {
      new_intervals[i] = {std::max(intervals[i].left, b.intervals[i].left),
                          std::min(intervals[i].right, b.intervals[i].right)};
    }
    return {new_intervals};
  }

  bool empty() const {
    return std::any_of(intervals.begin(), intervals.end(),
                       [](const s_interval &interval) {
                         return interval.left > interval.right;
                       });
  }
};

template <size_t N> struct s_trans {
  std::array<int32_t, N> signs;
  std::array<uint32_t, N> perm;

  s_trans() {
    for (size_t i = 0; i < N; i++) {
      signs[i] = 1;
      perm[i] = i;
    }
  }

  s_trans(std::array<int32_t, N> signs, std::array<uint32_t, N> perm)
      : signs(signs), perm(perm) {}

  bool operator==(const s_trans<N> &other) const {
    return signs == other.signs && perm == other.perm;
  }

  static s_trans<N> from_points(const s_point<N> &p, const s_point<N> &q) {
    auto result = s_trans<N>();
    s_point<N> diff = q - p;
    int idx = -1;
    for (size_t i = 0; i < N; ++i) {
      if (std::abs(diff[i]) == 1) {
        if (idx == -1) {
          idx = i;
        } else { // a differing coordinate has already been found
          throw std::invalid_argument("Points are not adjacent");
        }
      }
    }
    if (idx == -1) {
      throw std::invalid_argument("Points are not adjacent");
    }

    // Construct the transform by modifying the identity transform.
    result.perm[0] = idx;
    result.perm[idx] = 0;
    result.signs[0] = -diff[idx]; // not strictly necessary
    result.signs[idx] = diff[idx];

    return result;
  }

  template <typename Gen> static s_trans<N> rand(Gen &gen) {
    static std::bernoulli_distribution flip_;

    std::array<uint32_t, 2> perm = {0, 1};
    std::array<int32_t, 2> signs;
    for (int i = 0; i < 2; ++i) {
      signs[i] = 2 * flip_(gen) - 1;
    }
    std::shuffle(perm.begin(), perm.end(), gen);
    return s_trans<N>{signs, perm};
  }

  static s_trans<N> rand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return rand(gen);
  }

  std::pair<int32_t, uint32_t> operator[](size_t i) const {
    return {signs[i], perm[i]};
  }

  bool is_identity() const {
    for (size_t i = 0; i < N; i++) {
      if (perm[i] != i || signs[i] != 1) {
        return false;
      }
    }
    return true;
  }

  s_trans<N> inverse() const {
    std::array<uint32_t, N> new_perm;
    std::array<int32_t, N> new_signs;
    for (size_t i = 0; i < N; ++i) {
      new_perm[perm[i]] = i;
      new_signs[i] = signs[perm[i]];
    }
    return s_trans<N>(new_signs, new_perm);
  }

  s_point<N> operator*(const s_point<N> &p) const {
    s_point<N> q{};
    for (size_t i = 0; i < N; i++) {
      q[perm[i]] = p[i] * signs[perm[i]];
    }
    return q;
  }

  s_trans<N> operator*(const s_trans<N> &t) const {
    std::array<int32_t, N> new_signs{};
    std::array<uint32_t, N> new_perm{};
    for (size_t i = 0; i < N; i++) {
      new_perm[i] = perm[t.perm[i]];
      new_signs[new_perm[i]] = signs[new_perm[i]] * t.signs[t.perm[i]];
    }
    return {new_signs, new_perm};
  }

  s_box<N> operator*(const s_box<N> &b) const {
    std::array<s_interval, N> intervals{};
    for (size_t i = 0; i < N; i++) {
      int x = signs[perm[i]] * b.intervals[i].left;
      int y = signs[perm[i]] * b.intervals[i].right;
      intervals[perm[i]] = {std::min(x, y), std::max(x, y)};
    }
    return {intervals};
  }
};
