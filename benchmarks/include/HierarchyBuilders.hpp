//
//  HierarchyBuilders.hpp
//  eSGraph Benchmarks
//
//  Helper functions for building test hierarchies
//

#ifndef HierarchyBuilders_hpp
#define HierarchyBuilders_hpp

#include <memory>
#include <cstddef>

namespace eSGraph {
class Node;
}

namespace eSGraph::Benchmark {

// Build a flat hierarchy: root with N direct children
std::unique_ptr<Node> buildFlatHierarchy(size_t childCount);

// Build a deep hierarchy: linear chain N levels deep
// Returns the root; deepest node is N levels down
std::unique_ptr<Node> buildDeepHierarchy(size_t depth);

// Build a complete binary tree of given depth
// depth=1 means just root, depth=2 means root+2 children, etc.
std::unique_ptr<Node> buildBinaryTree(size_t depth);

// Get the deepest node in a linear chain (assumes buildDeepHierarchy structure)
Node* getDeepestNode(Node* root);

// Count total nodes in hierarchy
size_t countNodes(const Node* root);

} // namespace eSGraph::Benchmark

#endif /* HierarchyBuilders_hpp */
