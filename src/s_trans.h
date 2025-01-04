#include <array>
#include <cstddef>
#include <cstdint>

template <int N> using s_point = std::array<int32_t, N>;

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

template <int N> struct s_box {
  std::array<s_interval, N> intervals;

  s_box(std::array<s_interval, N> intervals) : intervals(intervals) {}

  s_interval operator[](size_t i) const { return intervals[i]; }
};

template <int N> struct s_trans {
  std::array<int32_t, N> signs;
  std::array<uint32_t, N> perm;

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
