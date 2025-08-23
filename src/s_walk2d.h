#pragma once

#include <optional>
#include <vector>

#include "s_trans.h"

std::vector<s_point<2>> line(int num_steps);

class walk_node;

bool intersect(const walk_node *l_walk, const walk_node *r_walk,
               const s_point<2> &l_anchor, const s_point<2> &r_anchor,
               const s_trans<2> &l_symm, const s_trans<2> &r_symm);

class walk_tree;

class walk_node {

public:
  walk_node(const walk_node &) = default;

  static walk_node *balanced_rep(const std::vector<s_point<2>> &steps);

  s_trans<2> symm() const { return symm_; }
  s_point<2> endpoint() const { return end_; }
  s_box<2> bbox() const { return bbox_; }
  int num_sites() const { return num_sites_; }
  walk_node *left() const { return left_; }
  walk_node *right() const { return right_; }

  bool is_leaf() const;

  void merge();

  walk_node *rotate_left(bool set_parent = true);

  walk_node *rotate_right(bool set_parent = true);

  walk_node *shuffle_up(int id);

  walk_node *shuffle_down();

  bool intersect() const;

private:
  int num_sites_;
  walk_node *parent_{};
  walk_node *left_{};
  walk_node *right_{};
  s_trans<2> symm_;
  s_box<2> bbox_;
  s_point<2> end_;

  friend class walk_tree;

  static walk_node *balanced_rep(std::span<const s_point<2>> steps, int start,
                                 const s_trans<2> &glob_symm);

  walk_node(int id, int num_sites, const s_trans<2> &symm, const s_box<2> &bbox,
            const s_point<2> &end);

  std::vector<s_point<2>> steps() const;

  static walk_node create_leaf();

  static walk_node &leaf();

  friend bool intersect(const walk_node *l_walk, const walk_node *r_walk,
                        const s_point<2> &l_anchor, const s_point<2> &r_anchor,
                        const s_trans<2> &l_symm, const s_trans<2> &r_symm);
};

class walk_tree {

public:
  walk_tree(int num_sites, std::optional<unsigned int> seed = std::nullopt);

  walk_tree(const std::vector<s_point<2>> &steps,
            std::optional<unsigned int> seed = std::nullopt);

  ~walk_tree();

  walk_node *root() const { return root_.get(); }

  bool try_pivot(int n, const s_trans<2> &r);

  bool rand_pivot();

  bool self_avoiding() const;

  std::vector<s_point<2>> steps() const;

private:
  std::unique_ptr<walk_node> root_;
  std::mt19937 rng_;
  std::uniform_int_distribution<int> dist_;
};
