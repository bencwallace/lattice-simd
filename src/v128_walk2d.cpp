#include <cmath>
#include <memory>
#include <span>
#include <vector>

#include "v128_trans2d.h"

std::vector<v128_point2d> line(int num_steps) {
  std::vector<v128_point2d> steps(num_steps);
  for (int i = 0; i < num_steps; ++i) {
    steps[i] = (i + 1) * v128_point2d::unit();
  }
  return steps;
}

class walk_tree;

class walk_node {

public:
  walk_node(const walk_node &) = default;

  static walk_node *balanced_rep(const std::vector<v128_point2d> &steps) {
    return balanced_rep(steps, 1, v128_trans2d());
  }

private:
  int num_sites_;
  walk_node *parent_{};
  walk_node *left_{};
  walk_node *right_{};
  v128_trans2d symm_;
  v128_box2d bbox_;
  v128_point2d end_;

  friend class walk_tree;

  static walk_node *balanced_rep(std::span<const v128_point2d> steps, int start,
                                 const v128_trans2d &glob_symm) {
    int num_sites = steps.size();
    if (num_sites < 1) {
      throw std::invalid_argument("num_sites must be at least 1");
    }
    if (num_sites == 1) {
      return &leaf();
    }

    int n = std::floor((1 + num_sites) / 2.0);
    auto abs_symm = v128_trans2d::from_points(steps[n - 1], steps[n]);
    auto glob_inv = glob_symm.inverse();
    auto rel_symm = glob_inv * abs_symm;
    auto rel_end =
        glob_inv * (steps.back() - steps.front()) + v128_point2d::unit();
    auto rel_box = (glob_inv * (v128_box2d(steps) - v128_point2d::unit())) +
                   v128_point2d::unit();
    int id = start + n - 1;
    walk_node *root = new walk_node(id, num_sites, rel_symm, rel_box, rel_end);

    if (n >= 1) {
      root->left_ = balanced_rep(steps.subspan(0, n), start, glob_symm);
      root->left_->parent_ = root;
    }
    if (num_sites - n >= 1) {
      root->right_ =
          balanced_rep(steps.subspan(n), start + n, glob_symm * rel_symm);
      root->right_->parent_ = root;
    }
    return root;
  }

  walk_node(int id, int num_sites, const v128_trans2d &symm,
            const v128_box2d &bbox, const v128_point2d &end)
      : num_sites_(num_sites), symm_(symm), bbox_(bbox), end_(end) {}

  static walk_node create_leaf() {
    std::array<v128_interval, 2> intervals;
    intervals[0] = v128_interval(1, 1);
    for (int i = 1; i < 2; ++i) {
      intervals[i] = v128_interval(0, 0);
    }
    return walk_node(0, 1, v128_trans2d(), v128_box2d(intervals),
                     v128_point2d::unit());
  }

  static walk_node &leaf() {
    static walk_node leaf = create_leaf();
    return leaf;
  }
};

class walk_tree {

public:
  walk_tree(int num_sites) : root_(walk_node::balanced_rep(line(num_sites))) {}

private:
  std::unique_ptr<walk_node> root_;
};
