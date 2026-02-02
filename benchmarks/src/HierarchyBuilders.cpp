//
//  HierarchyBuilders.cpp
//  eSGraph Benchmarks
//

#include "HierarchyBuilders.hpp"
#include "Node.hpp"
#include <string>

namespace eSGraph::Benchmark {

std::unique_ptr<Node> buildFlatHierarchy(size_t childCount) {
    auto root = std::make_unique<Node>("root");

    for (size_t i = 0; i < childCount; ++i) {
        auto child = std::make_unique<Node>("child_" + std::to_string(i));
        root->addChild(std::move(child));
    }

    return root;
}

std::unique_ptr<Node> buildDeepHierarchy(size_t depth) {
    auto root = std::make_unique<Node>("node_0");

    Node* current = root.get();
    for (size_t i = 1; i < depth; ++i) {
        auto child = std::make_unique<Node>("node_" + std::to_string(i));
        Node* childPtr = child.get();
        current->addChild(std::move(child));
        current = childPtr;
    }

    return root;
}

std::unique_ptr<Node> buildBinaryTree(size_t depth) {
    auto root = std::make_unique<Node>("node_0");

    if (depth <= 1) {
        return root;
    }

    // Use a simple iterative approach with a queue-like structure
    std::vector<Node*> currentLevel;
    currentLevel.push_back(root.get());
    size_t nodeIndex = 1;

    for (size_t level = 1; level < depth; ++level) {
        std::vector<Node*> nextLevel;
        nextLevel.reserve(currentLevel.size() * 2);

        for (Node* parent : currentLevel) {
            // Add left child
            auto leftChild = std::make_unique<Node>("node_" + std::to_string(nodeIndex++));
            Node* leftPtr = leftChild.get();
            parent->addChild(std::move(leftChild));
            nextLevel.push_back(leftPtr);

            // Add right child
            auto rightChild = std::make_unique<Node>("node_" + std::to_string(nodeIndex++));
            Node* rightPtr = rightChild.get();
            parent->addChild(std::move(rightChild));
            nextLevel.push_back(rightPtr);
        }

        currentLevel = std::move(nextLevel);
    }

    return root;
}

Node* getDeepestNode(Node* root) {
    if (!root) {
        return nullptr;
    }

    Node* current = root;
    while (current->hasChildren()) {
        // Get first child (for linear chain, there's only one)
        current = current->getChildren().front().get();
    }

    return current;
}

size_t countNodes(const Node* root) {
    if (!root) {
        return 0;
    }

    size_t count = 1; // Count this node
    for (const auto& child : root->getChildren()) {
        count += countNodes(child.get());
    }

    return count;
}

} // namespace eSGraph::Benchmark
