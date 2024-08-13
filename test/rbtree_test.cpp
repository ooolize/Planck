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
  RBTreeTest(const RBTreeTest&) = delete;
  RBTreeTest& operator=(const RBTreeTest&) = delete;
  RBTreeTest(RBTreeTest&&) = delete;
  RBTreeTest& operator=(RBTreeTest&&) = delete;

  // 创建随机数生成器
  std::random_device rd;            // NOLINT
  std::mt19937 g{rd()};             // NOLINT
  lz::rbtree::RBTree<int> _tree{};  // NOLINT
};

constexpr std::size_t START_NUM = 0;
constexpr std::size_t COUNT = 10;
constexpr std::size_t END_NUM = START_NUM + COUNT;

// Test Check func
TEST_F(RBTreeTest, Check) {
  auto root = _tree.root();
  root->_value = 0;
  root->_color = lz::rbtree::TreeColor::RED;
  root->_left =
    std::make_shared<lz::rbtree::Node<int>>(1, lz::rbtree::TreeColor::BLACK);
  EXPECT_TRUE(_tree.checkRbTree().first);
}

// Test one insert
TEST_F(RBTreeTest, InsertOne) {
  _tree.insert(1);
  EXPECT_EQ(_tree.size(), 1);
  EXPECT_EQ(_tree.root()->_value, 1);
  EXPECT_EQ(_tree.root()->_color, lz::rbtree::TreeColor::RED);
  EXPECT_TRUE(_tree.checkRbTree().first);
}

// Test multi insert
TEST_F(RBTreeTest, InsertMulti) {
  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    _tree.insert(i);
  }
  EXPECT_EQ(_tree.size(), COUNT);
  EXPECT_TRUE(_tree.checkRbTree().first);
}

// Test random insert
TEST_F(RBTreeTest, InsertRandom) {
  std::vector<int> vec;
  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    vec.push_back(i);
  }
  std::shuffle(vec.begin(), vec.end(), g);
  for (int i : vec) {
    _tree.insert(i);
  }
  EXPECT_EQ(_tree.size(), COUNT);
  EXPECT_TRUE(_tree.checkRbTree().first);
}

// Test one remove
TEST_F(RBTreeTest, RemoveOne) {
  _tree.insert(1);
  _tree.remove(1);
  EXPECT_EQ(_tree.size(), 0);
  EXPECT_TRUE(_tree.checkRbTree().first);
}

// Test multi remove
TEST_F(RBTreeTest, RemoveMulti) {
  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    _tree.insert(i);
  }
  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    _tree.remove(i);
  }
  EXPECT_EQ(_tree.size(), 0);
  EXPECT_TRUE(_tree.checkRbTree().first);
}

// Test random remove
TEST_F(RBTreeTest, RemoveRandom) {
  std::vector<int> vec;
  for (int i : std::ranges::iota_view(START_NUM, END_NUM)) {
    vec.push_back(i);
  }
  std::shuffle(vec.begin(), vec.end(), g);
  std::for_each(vec.begin(), vec.end(), [&](int i) { _tree.insert(i); });

  std::shuffle(vec.begin(), vec.end(), g);
  std::for_each(vec.begin(), vec.end(), [&](int i) { _tree.remove(i); });
  EXPECT_EQ(_tree.size(), 0);
  EXPECT_TRUE(_tree.checkRbTree().first);
}


}  // namespace rbtree
}  // namespace test
}  // namespace lz