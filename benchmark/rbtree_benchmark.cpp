/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-19
 * @LastEditors: lize
 */
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include "benchmark/benchmark.h"
#include "utils/rbtree.h"
namespace bc {
// 创建随机数引擎（使用默认种子）
static std::default_random_engine engine;
// 创建均匀分布对象，生成0到100之间的随机整数
static std::uniform_int_distribution<int> distribution(1, 1 << 21);

static void insert(benchmark::State& state) {
  lz::rbtree::RBTree<int> _tree{};
  std::vector<int> v(state.range(0));
  std::transform(
    v.begin(), v.end(), v.begin(), [](int) { return distribution(engine); });
  // build tree
  std::for_each(v.begin(), v.end(), [&_tree](int i) { _tree.insert(i); });

  for (auto _ : state) {
    _tree.insert(distribution(engine));
  }
  state.SetComplexityN(state.range(0));
}

static void remove(benchmark::State& state) {
  lz::rbtree::RBTree<int> _tree{};
  std::vector<int> v(state.range(0));
  std::transform(v.begin(), v.end(), v.begin(), [&state](int) {
    return distribution(engine) % state.range(0);
  });
  // build tree
  std::for_each(v.begin(), v.end(), [&_tree](int i) { _tree.insert(i); });

  for (auto _ : state) {
    _tree.remove(v[distribution(engine) % v.size()]);
  }
  state.SetComplexityN(state.range(0));
}

// 注册函数和输入规模
// BENCHMARK(insert)->RangeMultiplier(2)->Range(1 << 10, 1 << 20)->Complexity();
BENCHMARK(remove)->RangeMultiplier(2)->Range(1 << 10, 1 << 20)->Complexity();
BENCHMARK_MAIN();
}  // namespace bc