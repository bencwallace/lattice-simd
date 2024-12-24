#include <array>
#include <cstddef>
#include <cstdint>

template <int N> using s_point = std::array<int32_t, N>;

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
};
