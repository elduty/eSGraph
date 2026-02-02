//
//  NodeBenchmarks.cpp
//  eSGraph Benchmarks
//
//  Benchmark scenarios for Node operations
//

#include "BenchmarkFramework.hpp"
#include "HierarchyBuilders.hpp"
#include "Node.hpp"
#include <memory>

using namespace eSGraph;
using namespace eSGraph::Benchmark;

// ============================================================================
// Global state for benchmarks (reset between runs via setup/teardown)
// ============================================================================

namespace {

std::unique_ptr<Node> g_root;
Node* g_targetNode = nullptr;

// Hierarchy configurations
constexpr size_t FLAT_SMALL = 100;
constexpr size_t FLAT_MEDIUM = 1000;
constexpr size_t FLAT_LARGE = 10000;
constexpr size_t DEEP_SMALL = 10;
constexpr size_t DEEP_MEDIUM = 50;
constexpr size_t DEEP_LARGE = 100;
constexpr size_t TREE_SMALL = 10;   // ~1K nodes
constexpr size_t TREE_MEDIUM = 15;  // ~32K nodes

// ============================================================================
// 1. Local Matrix Operations
// ============================================================================

void registerLocalMatrixBenchmarks() {
    // BM_GetMatrix_Clean - Cache hit scenario
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetMatrix_Clean",
        []() {
            auto& matrix = g_root->getMatrix();
            DoNotOptimize(matrix);
        },
        []() {
            g_root = std::make_unique<Node>("test");
            // Pre-compute matrix to ensure cache is warm
            (void)g_root->getMatrix();
        },
        []() {
            g_root.reset();
        }
    );

    // BM_GetMatrix_Dirty - Force recomputation
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetMatrix_Dirty",
        []() {
            g_root->setPosition(glm::vec3(1.0f, 2.0f, 3.0f));
            auto& matrix = g_root->getMatrix();
            DoNotOptimize(matrix);
        },
        []() {
            g_root = std::make_unique<Node>("test");
        },
        []() {
            g_root.reset();
        }
    );
}

// ============================================================================
// 2. Global Matrix Operations
// ============================================================================

void registerGlobalMatrixBenchmarks() {
    // BM_GetGlobalMatrix_RootNode - No parent traversal
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetGlobalMatrix_RootNode",
        []() {
            auto& matrix = g_root->getGlobalMatrix();
            DoNotOptimize(matrix);
        },
        []() {
            g_root = std::make_unique<Node>("root");
            (void)g_root->getGlobalMatrix(); // Warm cache
        },
        []() {
            g_root.reset();
        }
    );

    // BM_GetGlobalMatrix_DeepChain_10
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetGlobalMatrix_DeepChain_10",
        []() {
            g_root->setPosition(glm::vec3(1.0f)); // Dirty the hierarchy
            auto& matrix = g_targetNode->getGlobalMatrix();
            DoNotOptimize(matrix);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_SMALL);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_GetGlobalMatrix_DeepChain_50
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetGlobalMatrix_DeepChain_50",
        []() {
            g_root->setPosition(glm::vec3(1.0f));
            auto& matrix = g_targetNode->getGlobalMatrix();
            DoNotOptimize(matrix);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_GetGlobalMatrix_DeepChain_100
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetGlobalMatrix_DeepChain_100",
        []() {
            g_root->setPosition(glm::vec3(1.0f));
            auto& matrix = g_targetNode->getGlobalMatrix();
            DoNotOptimize(matrix);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_LARGE);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_GetGlobalMatrix_Cached - Repeated access without modification
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetGlobalMatrix_Cached",
        []() {
            auto& matrix = g_targetNode->getGlobalMatrix();
            DoNotOptimize(matrix);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
            (void)g_targetNode->getGlobalMatrix(); // Warm cache
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );
}

// ============================================================================
// 3. World Coordinate Operations
// ============================================================================

void registerWorldCoordinateBenchmarks() {
    // BM_SetPosition_Local - Baseline
    BenchmarkRunner::instance().registerBenchmark(
        "BM_SetPosition_Local",
        []() {
            g_targetNode->setPosition(glm::vec3(1.0f, 2.0f, 3.0f), Coordinates::LOCAL);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_SetPosition_World_DeepNode - Matrix inversion required
    BenchmarkRunner::instance().registerBenchmark(
        "BM_SetPosition_World_DeepNode",
        []() {
            g_targetNode->setPosition(glm::vec3(1.0f, 2.0f, 3.0f), Coordinates::WORLD);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_GetPosition_World_DeepNode
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetPosition_World_DeepNode",
        []() {
            auto pos = g_targetNode->getPosition(Coordinates::WORLD);
            DoNotOptimize(pos);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
            // Set some positions to make it interesting
            g_root->setPosition(glm::vec3(10.0f, 0.0f, 0.0f));
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );
}

// ============================================================================
// 4. Dirty Flag Propagation
// ============================================================================

void registerDirtyPropagationBenchmarks() {
    // BM_DirtyPropagation_Flat_100
    BenchmarkRunner::instance().registerBenchmark(
        "BM_DirtyPropagation_Flat_100",
        []() {
            g_root->setPosition(glm::vec3(1.0f));
            // Force computation of all children's global matrices
            for (const auto& child : g_root->getChildren()) {
                auto& matrix = child->getGlobalMatrix();
                DoNotOptimize(matrix);
            }
        },
        []() {
            g_root = buildFlatHierarchy(FLAT_SMALL);
        },
        []() {
            g_root.reset();
        }
    );

    // BM_DirtyPropagation_Flat_1000
    BenchmarkRunner::instance().registerBenchmark(
        "BM_DirtyPropagation_Flat_1000",
        []() {
            g_root->setPosition(glm::vec3(1.0f));
            for (const auto& child : g_root->getChildren()) {
                auto& matrix = child->getGlobalMatrix();
                DoNotOptimize(matrix);
            }
        },
        []() {
            g_root = buildFlatHierarchy(FLAT_MEDIUM);
        },
        []() {
            g_root.reset();
        }
    );

    // BM_DirtyPropagation_Flat_10000
    BenchmarkRunner::instance().registerBenchmark(
        "BM_DirtyPropagation_Flat_10000",
        []() {
            g_root->setPosition(glm::vec3(1.0f));
            for (const auto& child : g_root->getChildren()) {
                auto& matrix = child->getGlobalMatrix();
                DoNotOptimize(matrix);
            }
        },
        []() {
            g_root = buildFlatHierarchy(FLAT_LARGE);
        },
        []() {
            g_root.reset();
        }
    );

    // BM_DirtyPropagation_Deep_100
    BenchmarkRunner::instance().registerBenchmark(
        "BM_DirtyPropagation_Deep_100",
        []() {
            g_root->setPosition(glm::vec3(1.0f));
            auto& matrix = g_targetNode->getGlobalMatrix();
            DoNotOptimize(matrix);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_LARGE);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_DirtyPropagation_BinaryTree_10 (~1K nodes)
    BenchmarkRunner::instance().registerBenchmark(
        "BM_DirtyPropagation_BinaryTree_10",
        []() {
            g_root->setPosition(glm::vec3(1.0f));
            (void)g_targetNode->getGlobalMatrix();
            DoNotOptimize(g_targetNode);
        },
        []() {
            g_root = buildBinaryTree(TREE_SMALL);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_DirtyPropagation_BinaryTree_15 (~32K nodes)
    BenchmarkRunner::instance().registerBenchmark(
        "BM_DirtyPropagation_BinaryTree_15",
        []() {
            g_root->setPosition(glm::vec3(1.0f));
            (void)g_targetNode->getGlobalMatrix();
            DoNotOptimize(g_targetNode);
        },
        []() {
            g_root = buildBinaryTree(TREE_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );
}

// ============================================================================
// 5. Rotation Operations
// ============================================================================

void registerRotationBenchmarks() {
    // BM_GetRotation_World_DeepNode
    BenchmarkRunner::instance().registerBenchmark(
        "BM_GetRotation_World_DeepNode",
        []() {
            auto rot = g_targetNode->getRotation(Coordinates::WORLD);
            DoNotOptimize(rot);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
            // Apply some rotations throughout the hierarchy
            Node* current = g_root.get();
            float angle = 0.1f;
            while (current) {
                current->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), angle);
                if (current->hasChildren()) {
                    current = current->getChildren().front().get();
                } else {
                    break;
                }
                angle += 0.1f;
            }
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_SetRotation_World_DeepNode
    BenchmarkRunner::instance().registerBenchmark(
        "BM_SetRotation_World_DeepNode",
        []() {
            glm::quat rotation = glm::angleAxis(0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
            g_targetNode->setRotation(rotation, Coordinates::WORLD);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );

    // BM_SetRotation_Local - Baseline comparison
    BenchmarkRunner::instance().registerBenchmark(
        "BM_SetRotation_Local",
        []() {
            glm::quat rotation = glm::angleAxis(0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
            g_targetNode->setRotation(rotation, Coordinates::LOCAL);
        },
        []() {
            g_root = buildDeepHierarchy(DEEP_MEDIUM);
            g_targetNode = getDeepestNode(g_root.get());
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );
}

// ============================================================================
// 6. Hierarchy Modifications
// ============================================================================

void registerHierarchyModificationBenchmarks() {
    // BM_AddChild
    BenchmarkRunner::instance().registerBenchmark(
        "BM_AddChild",
        []() {
            auto child = std::make_unique<Node>("new_child");
            g_root->addChild(std::move(child));
        },
        []() {
            g_root = std::make_unique<Node>("root");
        },
        []() {
            g_root.reset();
        }
    );

    // BM_RemoveChild - From hierarchy with many siblings
    BenchmarkRunner::instance().registerBenchmark(
        "BM_RemoveChild",
        []() {
            // Remove and re-add to maintain state for next iteration
            auto removed = g_root->removeChild(g_targetNode);
            g_root->addChild(std::move(removed));
        },
        []() {
            g_root = buildFlatHierarchy(FLAT_MEDIUM);
            // Get a child in the middle
            auto it = g_root->getChildren().begin();
            std::advance(it, FLAT_MEDIUM / 2);
            g_targetNode = it->get();
        },
        []() {
            g_root.reset();
            g_targetNode = nullptr;
        }
    );
}

// ============================================================================
// Registration function called from main
// ============================================================================

void registerAllBenchmarks() {
    registerLocalMatrixBenchmarks();
    registerGlobalMatrixBenchmarks();
    registerWorldCoordinateBenchmarks();
    registerDirtyPropagationBenchmarks();
    registerRotationBenchmarks();
    registerHierarchyModificationBenchmarks();
}

} // anonymous namespace

// Export the registration function
namespace eSGraph::Benchmark {
void registerNodeBenchmarks() {
    registerAllBenchmarks();
}
} // namespace eSGraph::Benchmark
