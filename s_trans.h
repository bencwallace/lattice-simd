#include <array>
#include <cstddef>
#include <cstdint>

template <int N>
using s_point = std::array<int32_t, N>;

template <int N>
struct s_trans {
    int32_t signs[N];
    uint32_t perm[N];

    s_point<N> permute(const s_point<N> &p) const {
        s_point<N> q{};
        for (size_t i = 0; i < N; i++) {
            q[i] = p[perm[i]];
        }
        return q;
    }

    s_point<N> flip(const s_point<N> &p) const {
        s_point<N> q{};
        for (size_t i = 0; i < N; i++) {
            q[i] = p[i] * signs[i];
        }
        return q;
    }

    s_point<N> operator*(const s_point<N> &p) const {
        s_point<N> q{};
        for (size_t i = 0; i < N; i++) {
            // q[i] = p[perm[i]] * signs[i];
            q[perm[i]] = p[i] * signs[perm[i]];
        }
        return q;
    }
};
