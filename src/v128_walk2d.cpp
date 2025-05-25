#include <memory>
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

  static walk_node *balanced_rep(const std::vector<v128_point2d> &steps);

private:
  int num_sites_;
  walk_node *parent_{};
  walk_node *left_{};
  walk_node *right_{};
  v128_trans2d symm_;
  v128_box2d bbox_;
  v128_point2d end_;

  friend class walk_tree;
};

class walk_tree {

public:
  walk_tree(int num_sites) : root_(walk_node::balanced_rep(line(num_sites))) {}

private:
  std::unique_ptr<walk_node> root_;
};
