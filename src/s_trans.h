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

  s_trans<N> operator*(const s_trans<N> &t) const {
    std::array<int32_t, N> new_signs{};
    std::array<uint32_t, N> new_perm{};
    for (size_t i = 0; i < N; i++) {
      new_perm[i] = perm[t.perm[i]];
      new_signs[new_perm[i]] = signs[new_perm[i]] * t.signs[t.perm[i]];
    }
    return {new_signs, new_perm};
  }
};
