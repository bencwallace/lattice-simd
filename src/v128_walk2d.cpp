#include <cmath>
#include <memory>
#include <optional>
#include <random>
#include <span>
#include <stack>
#include <vector>

#include "v128_trans2d.h"

std::vector<v128_point2d> line(int num_steps) {
  std::vector<v128_point2d> steps(num_steps);
  for (int i = 0; i < num_steps; ++i) {
    steps[i] = (i + 1) * v128_point2d::unit();
  }
  return steps;
}

class walk_node;

bool intersect(const walk_node *l_walk, const walk_node *r_walk,
               const v128_point2d &l_anchor, const v128_point2d &r_anchor,
               const v128_trans2d &l_symm, const v128_trans2d &r_symm);

class walk_tree;

class walk_node {

public:
  walk_node(const walk_node &) = default;

  static walk_node *balanced_rep(const std::vector<v128_point2d> &steps) {
    return balanced_rep(steps, 1, v128_trans2d());
  }

  bool is_leaf() const { return left_ == nullptr && right_ == nullptr; }

  void merge() {
    num_sites_ = left_->num_sites_ + right_->num_sites_;

    bbox_ = left_->bbox_ | (symm_ * right_->bbox_ + left_->end_);
    end_ = left_->end_ + symm_ * right_->end_;
  }

  walk_node *rotate_left(bool set_parent = true) {
    if (right_->is_leaf()) {
      throw std::invalid_argument("can't rotate left on a leaf node");
    }
    auto temp_tree = right_;

    // update pointers
    right_ = temp_tree->right_;
    if (set_parent && temp_tree->right_ != nullptr) {
      temp_tree->right_->parent_ = this;
    }
    temp_tree->right_ = temp_tree->left_;
    temp_tree->left_ = left_;
    if (set_parent && left_ != nullptr) {
      left_->parent_ = temp_tree;
    }
    left_ = temp_tree;

    // update symmetries
    auto temp_symm = symm_;
    symm_ = temp_symm * left_->symm_;
    left_->symm_ = temp_symm;

    // merge
    left_->merge();

    return this;
  }

  walk_node *rotate_right(bool set_parent = true) {
    if (left_->is_leaf()) {
      throw std::invalid_argument("can't rotate right on a leaf node");
    }
    auto temp_tree = left_;

    // update pointers
    left_ = temp_tree->left_;
    if (set_parent && temp_tree->left_ != nullptr) {
      temp_tree->left_->parent_ = this;
    }
    temp_tree->left_ =
        temp_tree->right_; // temp_tree->set_left(temp_tree->right_)
                           // sets parent unnecessarily
    temp_tree->right_ = right_;
    if (set_parent && right_ != nullptr) {
      right_->parent_ = temp_tree;
    }
    right_ = temp_tree; // set_right(temp_tree) sets parent unnecessarily

    // update symmetries
    auto temp_symm = symm_;
    symm_ = right_->symm_;
    right_->symm_ = symm_.inverse() * temp_symm;

    // merge
    right_->merge();

    return this;
  }

  walk_node *shuffle_up(int id) {
    if (id < left_->num_sites_) {
      left_->shuffle_up(id);
      rotate_right();
    } else if (id > left_->num_sites_) {
      right_->shuffle_up(id - left_->num_sites_);
      rotate_left();
    }

    return this;
  }

  walk_node *shuffle_down() {
    int id = std::floor((num_sites_ + 1) / 2.0);
    if (id < left_->num_sites_) {
      rotate_right();
      right_->shuffle_down();
    } else if (id > left_->num_sites_) {
      rotate_left();
      left_->shuffle_down();
    }

    return this;
  }

  bool intersect() const {
    return ::intersect(left_, right_, v128_point2d(), left_->end_,
                       v128_trans2d(), symm_);
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

  friend bool intersect(const walk_node *l_walk, const walk_node *r_walk,
                        const v128_point2d &l_anchor,
                        const v128_point2d &r_anchor,
                        const v128_trans2d &l_symm, const v128_trans2d &r_symm);
};

bool intersect(const walk_node *l_walk, const walk_node *r_walk,
               const v128_point2d &l_anchor, const v128_point2d &r_anchor,
               const v128_trans2d &l_symm, const v128_trans2d &r_symm) {
  auto l_box = l_symm * l_walk->bbox_ + l_anchor;
  auto r_box = r_symm * r_walk->bbox_ + r_anchor;
  if ((l_box & r_box).empty()) {
    return false;
  }

  if (l_walk->num_sites_ <= 2 && r_walk->num_sites_ <= 2) {
    return true;
  }

  if (l_walk->num_sites_ >= r_walk->num_sites_) {
    return intersect(l_walk->right_, r_walk,
                     l_anchor + l_symm * l_walk->left_->end_, r_anchor,
                     l_symm * l_walk->symm_, r_symm) ||
           intersect(l_walk->left_, r_walk, l_anchor, r_anchor, l_symm, r_symm);
  } else {
    return intersect(l_walk, r_walk->left_, l_anchor, r_anchor, l_symm,
                     r_symm) ||
           intersect(l_walk, r_walk->right_, l_anchor,
                     r_anchor + r_symm * r_walk->left_->end_, l_symm,
                     r_symm * r_walk->symm_);
  }
}

class walk_tree {

public:
  walk_tree(int num_sites, std::optional<unsigned int> seed = std::nullopt)
      : walk_tree(line(num_sites), seed) {}

  walk_tree(const std::vector<v128_point2d> &steps,
            std::optional<unsigned int> seed) {
    if (steps.size() < 2) {
      throw std::invalid_argument("walk must have at least 2 sites (1 step)");
    }

    root_ = std::unique_ptr<walk_node>(walk_node::balanced_rep(steps));

    rng_ = std::mt19937(seed.value_or(std::random_device()()));
    dist_ = std::uniform_int_distribution<int>(1, steps.size() - 1);
  }

  ~walk_tree() {
    std::stack<walk_node *> nodes;
    nodes.push(root_.release());
    while (!nodes.empty()) {
      auto curr = nodes.top();
      nodes.pop();
      if (curr->left_ && !curr->left_->is_leaf()) {
        nodes.push(curr->left_);
      }
      if (curr->right_ && !curr->right_->is_leaf()) {
        nodes.push(curr->right_);
      }
      curr->~walk_node();
    }
  }

  bool try_pivot(int n, const v128_trans2d &r) {
    if (r.is_identity()) {
      return false;
    }

    root_->shuffle_up(n);
    auto root_symm = root_->symm_;
    root_->symm_ = root_->symm_ * r;
    auto success = !root_->intersect();
    if (!success) {
      root_->symm_ = root_symm;
    } else {
      root_->merge();
    }
    root_->shuffle_down();
    return success;
  }

  bool rand_pivot() {
    auto site = dist_(rng_);
    auto r = v128_trans2d::rand(rng_);
    return try_pivot(site, r);
  }

private:
  std::unique_ptr<walk_node> root_;
  std::mt19937 rng_;
  std::uniform_int_distribution<int> dist_;
};
