/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-10
 * @LastEditors: lize
 */

#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>
namespace lz {
enum class TreeColor : uint8_t { RED, BLACK };

template <typename Value, typename Compare = std::less<Value>()>
class RBTree {
 public:
  class Node;
  using NodeUPtr = std::unique_ptr<Node>;
  using NodeSPtr = std::shared_ptr<Node>;
  struct Node {
    Node() = default;
    explicit Node(Value value) : _value(value) {
    }
    NodeSPtr sibling() const {
      if (_parent == nullptr) {
        return nullptr;
      }
      if (this == _parent->_left.get()) {
        return _parent->_right;
      }
      return _parent->_left;
    }

    NodeSPtr _parent{};
    NodeSPtr _left{};
    NodeSPtr _right{};

    TreeColor _color = TreeColor::RED;
    Value _value{};
  };
  void insert(Value value) {
    // first insert as nomal BST
    auto node = insertValue(_root, value);

    // then rotate or change color to keep balance

    // case1 tree is empty. only insert RED node.
    // fine, is ok.
    if (_count == 1) {
      return;
    }

    // case2 only 2 node and red root node in tree.
    // need to change root color to black
    if (_count == 2 && _root->_color == TreeColor::RED) {
      _root->_color = TreeColor::BLACK;
      return;
    }
    // case3 only 2 node and  black root node in tree.
    // fine, is ok.
    if (_count == 2 && _root->_color == TreeColor::BLACK) {
      return;
    }

    auto parent = node->_parent;
    assert(parent != nullptr);
    auto grandParent = parent->_parent;
    assert(grandParent != nullptr);

    // if P is black, insert is ok. so wo only need to consider is U.
    if (parent->_color == TreeColor::BLACK) {
      return;
    }

    // if U is not exist
    if (grandParent->_left == nullptr) {
      rotateLeft(grandParent);
      grandParent->_color = TreeColor::RED;
      parent->_color = TreeColor::BLACK;
      return;
    } else if (grandParent->_right == nullptr) {
      rotateRight(grandParent);
      grandParent->_color = TreeColor::RED;
      parent->_color = TreeColor::BLACK;
      return;
    }

    // case4 if U is red.
    //      G(b)              G(r)
    //   P(r)  U(r)  ===>  P(b)  U(b)
    // N(r)               N(r)
    // need change color of G, P, U.  G'parent is unknow ,so recursively recolor
    if (grandParent->_left->_color == TreeColor::RED &&
        grandParent->_right->_color == TreeColor::RED) {
      recolor(node);
      return;
    }
    // if U is balck, according to the order of node, parent, grandParent
    // LL RR LR RL

    // case5 LR && RL.
    //      G(b)   L-rotate     G(b)
    //   P(r)  U(b)  ===>    P(r)  U(b)
    //     N(r)           N(r)
    // need rotate to trans case6
    if (node == parent->right && parent == grandParent->left) {
      rotateLeft(parent);
    } else if (node == parent->left && parent == grandParent->right) {
      rotateRight(parent);
    }

    // case6 LL && RR.  RR is dual operation of LL
    //      G(b)   R-rotate   P(r)      recolor   P(b)
    //   P(r)  U(b)  ===>  N(r)  G(b)     ===>  N(r)  G(r)
    // N(r)                        U(b)                  U(b)
    if (node == parent->left && parent == grandParent->left) {
      rotateRight(grandParent);
    } else if (node == parent->right && parent == grandParent->right) {
      rotateLeft(grandParent);
    }
    grandParent->_color = TreeColor::RED;
    parent->_color = TreeColor::BLACK;
  };
  void remove(Value value) {
    NodeSPtr node = find(value);
    if (node == nullptr) {
      return;
    }
    if (_count == 1 && _root->_value == value) {
      _root = nullptr;
      _count--;
      return;
    }
    // if node has two child, find the max value in left child
    // and replace node with max value node.
    if (node->_left && node->_right) {
      NodeSPtr lower_node = findRightestNode(node->_left);
      node->_value = lower_node->_value;
      lower_node->parent->_right = nullptr;
      return;
    };

    // if node has one child, replace node with child. and change color to black
    // not like front case to exchange value. because child is not leaf node.
    if (node->_left || node->_right) {
      auto child = node->_left ? node->_left : node->_right;
      auto parent = node->_parent;
      if (parent->_left == node) {
        parent->_left = child;
      } else {
        parent->_right = child;
      }
      child->_parent = parent;
      child->_color = TreeColor::BLACK;
      return;
    }
    // if node is red leaf node. just remove it.
    if (node->_color == TreeColor::RED) {
      node->_parent->_left = nullptr;
      node->_parent->_right = nullptr;
      return;
    }
    // if node is black leaf node
    removeBlackLeafNode(node);
    node->_parent->_left = nullptr;
    node->_parent->_right = nullptr;
    _count--;
  }

  NodeSPtr find(Value value) {
    NodeSPtr node = _root;
    while (node) {
      if (value < node->_value) {
        node = node->_left;
      } else if (value > node->_value) {
        node = node->_right;
      } else {
        return node;
      }
    }
    return nullptr;
  };
  NodeSPtr findMin() {
    return findLeftestNode(_root);
  };
  std::pair<bool, int> checkRbTree() {
    auto count = checkEachPath(_root, 0);
    return count == -1 ? std::make_pair(false, count)
                       : std::make_pair(true, count);
  }
  void printTree(const Node* node,
                 int level = 0,
                 const std::string& prefix = "Root: ") {
    if (node != nullptr) {
      std::cout << std::setw(level * 4) << prefix << node->value
                << (node->color == TreeColor::RED ? "(R)" : "(B)") << std::endl;
      if (node->left != nullptr || node->right != nullptr) {
        if (node->left) {
          printTree(node->left, level + 1, "L--- ");
        } else {
          std::cout << std::setw((level + 1) * 4) << "L--- " << "None"
                    << std::endl;
        }
        if (node->right) {
          printTree(node->right, level + 1, "R--- ");
        } else {
          std::cout << std::setw((level + 1) * 4) << "R--- " << "None"
                    << std::endl;
        }
      }
    }
  }

 private:
  //     [3]                   4
  //  1      4      ==>    3      5
  //       2   5        1    2       6
  //             6
  void rotateLeft(NodeSPtr node) {
    NodeSPtr parent = node->_parent;
    NodeSPtr rightNode = node->right;
    NodeSPtr successor = node->right->left;

    rightNode->left = node;
    node->right = successor;

    rightNode->_parent = node->_parent;
    node->_parent = rightNode;

    if (successor) {
      successor->_parent = node;
    }
  }
  //         [5]                    3
  //       3      6     ==>      2      5
  //    2    4                1      4     6
  // 1
  // dual operation of rotateLeft. just swap text "right" with "left"
  void rotateRight(NodeSPtr node) {
    NodeSPtr parent = node->_parent;
    NodeSPtr leftNode = node->left;
    NodeSPtr successor = node->left->right;

    // update node's child
    leftNode->right = node;
    node->left = successor;

    // update node's parent
    leftNode->_parent = node->_parent;
    node->_parent = leftNode;

    if (successor) {
      successor->_parent = node;
    }
  }
  //        G(b)                G(r)
  //     P(r)  U(r)   ===>    P(b)  U(b)
  //   N(r)                 N(r)
  void recolor(NodeSPtr node) {
    // if node is root or node's parent is root, return
    // if (node == nullptr || node->parent == nullptr ||
    //     node->parent->parent == nullptr) {
    //   return;
    // }
    // if node's parent is black, is ok
    if (node->parent->_color == TreeColor::BLACK) {
      return;
    }
    node->parent->parent->_color = TreeColor::RED;
    node->parent->parent->_left->_color = TreeColor::BLACK;
    node->parent->parent->_right->_color = TreeColor::BLACK;
    recolor(node->_parent->_parent);
  }
  NodeSPtr insertValue(NodeSPtr node, Value value) {
    if (node == nullptr) {
      _root = std::make_shared<Node>(value);
      ++_count;
      return _root;
    }
    while (node) {
      if (value < node->_value) {
        if (node->_left == nullptr) {
          node->_left = std::make_shared<Node>(value);
          node->_left->_parent = node;
          ++_count;
          return node->_left;
        }
        node = node->_left;
      } else if (value > node->_value) {
        if (node->_right == nullptr) {
          node->_right = std::make_shared<Node>(value);
          node->_right->_parent = node;
          ++_count;
          return node->_right;
        }
        node = node->_right;
      } else {
        return node;
      }
    }
  }
  NodeSPtr findRightestNode(const Node* node) const {
    while (node->_right) {
      node = node->_right;
    }
    return node;
  }
  NodeSPtr findLeftestNode(const Node* node) const {
    while (node->_left) {
      node = node->_left;
    }
    return node;
  }
  void removeBlackLeafNode(NodeSPtr node) {
    auto parent = node->_parent;
    auto sibling = node->sibling();
    // Case 1: Sibling is RED, parent and nephews must be BLACK
    //   Step 1. If N is a left child, left rotate P;
    //           If N is a right child, right rotate P.
    //   Step 2. Paint S to BLACK, P to RED
    //   Step 3. Goto Case 2, 3, 4, 5
    //      [P]                   <S>               [S]
    //      / \    l-rotate(P)    / \    repaint    / \
    //    [N] <S>  ==========>  [P] [D]  ======>  <P> [D]
    //        / \               / \               /  \
    //      [C] [D]           [N] [C]           [N] [C]
    if (sibling->_color == TreeColor::RED) {
      if (node == parent->_left) {
        rotateLeft(parent);
      } else {
        rotateRight(parent);
      }
      sibling->_color = TreeColor::BLACK;
      parent->_color = TreeColor::RED;
      sibling = node->sibling();
    }

    // Case 2: Sibling and nephews are BLACK, parent is RED
    //   Swap the color of P and S
    //      <P>             [P]
    //      / \             / \
    //    [N] [S]  ====>  [N] <S>
    //        / \             / \
    //      [C] [D]         [C] [D]
    if (parent->_color == TreeColor::RED &&
        sibling->_color == TreeColor::BLACK &&
        (sibling->_left == nullptr ||
         sibling->_left->_color == TreeColor::BLACK) &&
        (sibling->_right == nullptr ||
         sibling->_right->_color == TreeColor::BLACK)) {
      parent->_color = TreeColor::BLACK;
      sibling->_color = TreeColor::RED;
      return;
    }

    // Case 3: Sibling, parent and nephews are all black
    //   Step 1. Paint S to RED
    //   Step 2. Recursively maintain P
    //      [P]             [P]
    //      / \             / \
    //    [N] [S]  ====>  [N] <S>
    //        / \             / \
    //      [C] [D]         [C] [D]

    if (parent->_color == TreeColor::BLACK &&
        sibling->_color == TreeColor::BLACK &&
        (sibling->_left == nullptr ||
         sibling->_left->_color == TreeColor::BLACK) &&
        (sibling->_right == nullptr ||
         sibling->_right->_color == TreeColor::BLACK)) {
      sibling->_color = TreeColor::RED;
      removeBlackLeafNode(parent);
      return;
    }
    // Case 4: Sibling is BLACK, close nephew is RED,
    //         distant nephew is BLACK
    //   Step 1. If N is a left child, right rotate S;
    //           If N is a right child, left rotate S.
    //   Step 2. Swap the color of close nephew and sibling
    //   Step 3. Goto case 5
    //                            {P}                {P}
    //      {P}                   / \                / \
    //      / \    r-rotate(S)  [N] <C>   repaint  [N] [C]
    //    [N] [S]  ==========>        \   ======>        \
    //        / \                     [S]                <S>
    //      <C> [D]                     \                  \
    //                                  [D]                [D]

    auto closeNephew = sibling->_left;
    auto distantNephew = sibling->_right;
    if (sibling->_color == TreeColor::BLACK &&
        (sibling->_left == nullptr ||
         sibling->_left->_color == TreeColor::BLACK) &&
        (sibling->_right != nullptr &&
         sibling->_right->_color == TreeColor::RED)) {
      if (node == parent->_left) {
        rotateRight(sibling);
      } else {
        rotateLeft(sibling);
      }
      sibling->_color = TreeColor::RED;
      closeNephew->_color = TreeColor::BLACK;
      sibling = node->sibling();

      if (node == parent->left) {
        closeNephew = sibling->_left;
        distantNephew = sibling->_right;
      } else {
        closeNephew = sibling->_right;
        distantNephew = sibling->_left;
      }
    }

    // Case 5: Sibling is BLACK, distant nephew is RED
    //   Step 1. If N is a left child, left rotate P;
    //           If N is a right child, right rotate P.
    //   Step 2. Swap the color of parent and sibling.
    //   Step 3. Paint distant nephew D to BLACK.
    //      {P}                   [S]               {S}
    //      / \    l-rotate(P)    / \    repaint    / \
    //    [N] [S]  ==========>  {P} <D>  ======>  [P] [D]
    //        / \               / \               / \
    //      {C} <D>           [N] {C}           [N] {C}
    if (node == parent->_left) {
      rotateLeft(parent);
    } else {
      rotateRight(parent);
    }
    std::swap(parent->_color, sibling->_color);
    distantNephew->_color = TreeColor::BLACK;
  }
  int checkEachPath(const Node* node, int black_count) {
    if (node == nullptr) {
      black_count++;
      return black_count;
    }
    if (node->_color == TreeColor::BLACK) {
      black_count++;
    }
    if ((node->left && node->_value < node->left->_value) ||
        (node->right && node->_value > node->right->_value)) {
      return -1;
    }
    auto leftCount = checkEachPath(node->_left, black_count);
    auto rightCount = checkEachPath(node->_right, black_count);
    return leftCount == rightCount ? leftCount : -1;
  }

 private:
  NodeSPtr _root{};
  size_t _count = 0;
};

}  // namespace lz