/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-12
 * @LastEditors: lize
 */
#include "utils/rbtree.h"

#include <gtest/gtest.h>

#include <random>
#include <ranges>
namespace lz {
namespace test {
namespace rbtree {
constexpr std::size_t START_NUM = 0;
constexpr std::size_t COUNT = 10;
constexpr std::size_t END_NUM = START_NUM + COUNT;
class RBTreeTest : public testing::Test {  // NOLINT
 protected:
  RBTreeTest() {
  }
  ~RBTreeTest() override {
  }
  void SetUp() override {
  }
  void TearDown() override {
  }

  auto generateRandomIntVecotr() {
    std::uniform_int_distribution<int> dis(0, 100);
    std::vector<int> vec;
    while (vec.size() < COUNT) {
      auto num = dis(g);
      if (std::find(vec.begin(), vec.end(), num) != vec.end()) {
        continue;
      }
      vec.push_back(num);
    }
    return vec;
  }
  RBTreeTest(const RBTreeTest&) = delete;
  RBTreeTest& operator=(const RBTreeTest&) = delete;
  RBTreeTest(RBTreeTest&&) = delete;
  RBTreeTest& operator=(RBTreeTest&&) = delete;

  // 创建随机数生成器
  std::random_device rd;            // NOLINT
  std::mt19937 g{rd()};             // NOLINT
  lz::rbtree::RBTree<int> _tree{};  // NOLINT
};

// Test Check func
TEST_F(RBTreeTest, Check) {
  auto root = _tree.root();
  root = std::make_shared<lz::rbtree::Node<int>>(0, lz::rbtree::TreeColor::RED);
  root->_left =
    std::make_shared<lz::rbtree::Node<int>>(-5, lz::rbtree::TreeColor::BLACK);
  root->_right =
    std::make_shared<lz::rbtree::Node<int>>(5, lz::rbtree::TreeColor::BLACK);
  ASSERT_TRUE(_tree.checkRbTree().first);
}

// Test one insert
TEST_F(RBTreeTest, InsertOne) {
  _tree.insert(1);
  ASSERT_EQ(_tree.size(), 1);
  ASSERT_EQ(_tree.root()->_value, 1);
  ASSERT_EQ(_tree.root()->_color, lz::rbtree::TreeColor::RED);
  ASSERT_TRUE(_tree.checkRbTree().first);
}

// Test multi insert
TEST_F(RBTreeTest, InsertMulti) {
  ASSERT_EQ(_tree.size(), 0);
  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    _tree.insert(i);
    ASSERT_TRUE(_tree.checkRbTree().first);
  }
  ASSERT_EQ(_tree.size(), COUNT);
}

// Test random insert
TEST_F(RBTreeTest, InsertRandom) {
  ASSERT_EQ(_tree.size(), 0);

  auto vec = generateRandomIntVecotr();
#ifdef DEBUG
  std::for_each(
    vec.begin(), vec.end(), [](const auto p) { std::cout << p << " "; });
#endif
  for (int i : vec) {
    _tree.insert(i);
#ifdef DEBUG
    _tree.printTree();
#endif
    ASSERT_TRUE(_tree.checkRbTree().first);
  }
  ASSERT_EQ(_tree.size(), COUNT);
}

// Test one remove
TEST_F(RBTreeTest, RemoveOne) {
  _tree.insert(1);
  _tree.remove(1);
  ASSERT_EQ(_tree.size(), 0);
  ASSERT_TRUE(_tree.checkRbTree().first);
}

// Test multi remove
TEST_F(RBTreeTest, RemoveMulti) {
  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    _tree.insert(i);
  }

  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    _tree.remove(i);

    ASSERT_TRUE(_tree.checkRbTree().first);
  }
  ASSERT_EQ(_tree.size(), 0);
}

// Test random remove
TEST_F(RBTreeTest, RemoveRandom) {
  std::vector<int> vec;

  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    vec.push_back(i);
  }
  // first random insert
  std::shuffle(vec.begin(), vec.end(), g);

#ifdef DEBUG
  auto printVec = [&](const auto vec) {
    std::for_each(
      vec.begin(), vec.end(), [](const auto p) { std::cout << p << " "; });
  };
  printVec(vec);
#endif

  std::for_each(vec.begin(), vec.end(), [&](int i) {
    _tree.insert(i);
    ASSERT_TRUE(_tree.checkRbTree().first);
  });

  // second random remove
  std::shuffle(vec.begin(), vec.end(), g);
#ifdef DEBUG
  printVec(vec);
#endif

  std::for_each(vec.begin(), vec.end(), [&](int i) {
    _tree.remove(i);
#ifdef DEBUG
    _tree.printTree();
#endif
    ASSERT_TRUE(_tree.checkRbTree().first);
  });
  ASSERT_EQ(_tree.size(), 0);
}

}  // namespace rbtree
}  // namespace test
}  // namespace lz