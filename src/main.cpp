#include <algorithm>
#include <iostream>

#include "v128_walk2d.cpp"

int main() {
  int iters = 1000000;
  auto interval = static_cast<int>(
      std::pow(10, std::floor(std::log10(std::max(iters / 10, 1)))));
  std::unique_ptr<walk_tree> w = std::make_unique<walk_tree>(1000000);

  // std::vector<v128_point2d> endpoints;

  int num_success = 0;
  int total_success = 0;
  int num_iter = 0;
  while (true) {
    if (num_iter % interval == 0) {
      std::cout << "Iterations: " << num_iter
                << " / Successes: " << total_success << " / Success rate: "
                << num_success / static_cast<float>(interval) << std::endl;
      num_success = 0;
    }
    // if (require_success) {
    //   if (total_success == iters) {
    //     break;
    //   }
    // } else if (num_iter == iters) {
    //   break;
    // }
    if (num_iter >= iters) {
      break;
    }

    auto success = w->rand_pivot();
    if (success) {
      // endpoints.push_back(w->endpoint());
      ++num_success;
      ++total_success;
    }
    ++num_iter;
  }

  return 0;
}
