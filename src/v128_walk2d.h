#pragma once

#include <optional>
#include <vector>

#include "v128_trans2d.h"

std::vector<v128_point2d> line(int num_steps);

class walk_node;

bool intersect(const walk_node *l_walk, const walk_node *r_walk,
               const v128_point2d &l_anchor, const v128_point2d &r_anchor,
               const v128_trans2d &l_symm, const v128_trans2d &r_symm);

class walk_tree;

class walk_node {

public:
  walk_node(const walk_node &) = default;

  static walk_node *balanced_rep(const std::vector<v128_point2d> &steps);

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
  v128_trans2d symm_;
  v128_box2d bbox_;
  v128_point2d end_;

  friend class walk_tree;

  static walk_node *balanced_rep(std::span<const v128_point2d> steps, int start,
                                 const v128_trans2d &glob_symm);

  walk_node(int id, int num_sites, const v128_trans2d &symm,
            const v128_box2d &bbox, const v128_point2d &end);

  std::vector<v128_point2d> steps() const;

  static walk_node create_leaf();

  static walk_node &leaf();

  friend bool intersect(const walk_node *l_walk, const walk_node *r_walk,
                        const v128_point2d &l_anchor,
                        const v128_point2d &r_anchor,
                        const v128_trans2d &l_symm, const v128_trans2d &r_symm);
};

class walk_tree {

public:
  walk_tree(int num_sites, std::optional<unsigned int> seed = std::nullopt);

  walk_tree(const std::vector<v128_point2d> &steps,
            std::optional<unsigned int> seed);

  ~walk_tree();

  bool try_pivot(int n, const v128_trans2d &r);

  bool rand_pivot();

  bool self_avoiding() const;

private:
  std::unique_ptr<walk_node> root_;
  std::mt19937 rng_;
  std::uniform_int_distribution<int> dist_;
};
