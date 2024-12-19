#include <array>
#include <cstddef>
#include <cstdint>

using s_point4 = std::array<int32_t, 4>;

struct s_trans4 {
    int32_t signs[4];
    uint32_t perm[4];

    s_point4 permute(const s_point4 &p) const {
        s_point4 q;
        for (size_t i = 0; i < 4; i++) {
            q[i] = p[perm[i]];
        }
        return q;
    }

    s_point4 flip(const s_point4 &p) const {
        s_point4 q;
        for (size_t i = 0; i < 4; i++) {
            q[i] = p[i] * signs[i];
        }
        return q;
    }

    s_point4 operator()(const s_point4 &p) const {
        s_point4 q;
        for (size_t i = 0; i < 4; i++) {
            q[i] = p[perm[i]] * signs[i];
        }
        return q;
    }
};
