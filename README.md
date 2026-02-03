[![CI](https://github.com/elduty/eSGraph/actions/workflows/ci.yml/badge.svg)](https://github.com/elduty/eSGraph/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/elduty/eSGraph/branch/main/graph/badge.svg)](https://codecov.io/gh/elduty/eSGraph)

# eSGraph

A lightweight, high-performance C++ scene graph library built on [GLM](http://glm.g-truc.net/) (OpenGL Mathematics). eSGraph provides hierarchical node-based management of 3D transformations with support for multiple coordinate spaces.

## Features

- **Hierarchical Scene Graph** - Parent-child node relationships with automatic transform propagation
- **Multiple Coordinate Spaces** - LOCAL, PARENT, and WORLD coordinate systems
- **High Performance** - Optimized dirty flag propagation, cache-friendly memory layout, inlined hot paths
- **Efficient Caching** - Lazy matrix computation with dirty flags, cached world rotation for direction vectors
- **Modern C++20** - Uses smart pointers, move semantics, and modern language features
- **Minimal Dependencies** - Only requires GLM (header-only)
- **SIMD Support** - Optional GLM SIMD optimizations via CMake
- **Well Tested** - Comprehensive test suite using Google Test

## Quick Start

```cpp
#include "Node.hpp"

using namespace eSGraph;

// Create a scene hierarchy
auto root = std::make_unique<Node>("root");
auto child = std::make_unique<Node>("child");

root->setPosition(glm::vec3(10.0f, 0.0f, 0.0f));
child->setPosition(glm::vec3(5.0f, 0.0f, 0.0f));

root->addChild(std::move(child));

// Get world position of child (15, 0, 0)
glm::vec3 worldPos = root->getChildren().front()->getPosition(Coordinates::WORLD);
```

## Building

### Prerequisites

- CMake 3.14+
- C++20 compatible compiler
- Git (for submodules)

### Clone and Build

```bash
git clone https://github.com/elduty/eSGraph.git
cd eSGraph
git submodule update --init --recursive

mkdir build && cd build
cmake ..
make
```

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | ON | Build unit tests |
| `BUILD_BENCHMARKS` | OFF | Build performance benchmarks |
| `ENABLE_GLM_SIMD` | ON | Enable GLM SIMD intrinsics (AVX/SSE4.1) |
| `ENABLE_ASAN` | OFF | Enable AddressSanitizer |
| `ENABLE_COVERAGE` | OFF | Enable code coverage |

```bash
# Build with benchmarks
cmake -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release ..
make

# Build without SIMD (for compatibility)
cmake -DENABLE_GLM_SIMD=OFF ..
```

## API Overview

### Coordinate Spaces

```cpp
enum class Coordinates {
    LOCAL,   // Relative to node's own origin
    PARENT,  // Relative to parent's origin
    WORLD    // Absolute world coordinates
};
```

### Transformations

```cpp
// Position
node->setPosition(glm::vec3(x, y, z), Coordinates::WORLD);
glm::vec3 pos = node->getPosition(Coordinates::LOCAL);
node->translate(glm::vec3(dx, dy, dz), Coordinates::PARENT);

// Rotation
node->setRotation(glm::quat(...), Coordinates::LOCAL);
node->setRotation(glm::vec3(pitch, yaw, roll));  // Euler angles
node->setRotation(axis, angle);                   // Axis-angle
node->rotate(glm::vec3(0, 1, 0), 0.5f);          // Incremental

// Scale (local space only)
node->setScale(glm::vec3(sx, sy, sz));
node->setScale(2.0f);  // Uniform scale
```

### Hierarchy Management

```cpp
// Adding children
parent->addChild(std::move(child));
Node::attachTo(std::move(node), parent);

// Removing children
auto removed = parent->removeChild(childPtr);
auto removed = parent->removeChild("child_name");
auto detached = node->detach();

// Queries
bool has = parent->hasChild(childPtr);
bool is = child->isChildOf(parent);
Node* p = child->getParent();
const auto& children = parent->getChildren();
```

### Matrix Access

```cpp
// Local transformation matrix (T * R * S)
const glm::mat4& local = node->getMatrix();

// World transformation matrix (parent's world * local)
const glm::mat4& world = node->getGlobalMatrix();
```

### Direction Vectors

```cpp
// Individual direction vectors (uses cached world rotation for WORLD coords)
glm::vec3 forward = node->getForward(Coordinates::WORLD);
glm::vec3 right = node->getRight(Coordinates::WORLD);
glm::vec3 up = node->getUp(Coordinates::WORLD);

// Batch query - more efficient when you need all three
DirectionVectors dirs = node->getDirections(Coordinates::WORLD);
// dirs.forward, dirs.right, dirs.up

// Look at a target point
node->lookAt(glm::vec3(0.0f, 0.0f, -10.0f));
```

### Hierarchy Traversal

```cpp
// Find nodes
Node* found = root->findByIdentifier("player");
Node* rootNode = node->getRoot();
size_t depth = node->getDepth();

// Sibling navigation
Node* next = node->getNextSibling();
Node* prev = node->getPreviousSibling();

// Traverse entire hierarchy
root->traverse([](Node& n) {
    std::cout << n.getIdentifier() << std::endl;
});

// Clone a hierarchy (deep copy)
auto cloned = root->clone();
```

## Performance

eSGraph uses lazy evaluation with dirty flags for optimal performance:

- **Thread-local stack** for dirty propagation with early-exit optimization
- **Cache-friendly memory layout** with hot data (dirty flags, parent pointer) placed first
- **Inlined trivial getters** for zero-overhead access
- **`std::vector` children container** for cache-friendly iteration
- **Cached world rotation** for fast direction vector queries

### Benchmarks

Run the performance benchmarks:

```bash
cmake -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release ..
make
./benchmarks/run_benchmarks
```

This generates a detailed report at `benchmark_report.md`.

#### Key Performance Characteristics

| Operation | Performance | Notes |
|-----------|-------------|-------|
| Cached matrix access | ~714M ops/sec | No recomputation needed |
| Matrix recomputation | ~59M ops/sec | After transform change |
| Local position/rotation set | ~280M ops/sec | Inlined, minimal overhead |
| World position set | ~110M ops/sec | Requires parent matrix inverse |
| Direction vectors (world) | ~490M ops/sec | Uses cached world rotation |
| Batch directions | ~283M ops/sec | 41% faster than 3 separate calls |
| AddChild | ~46M ops/sec | Vector push_back |
| RemoveChild | ~6.8M ops/sec | Vector erase |
| Dirty propagation (1K flat) | ~150K ops/sec | Early-exit optimization |
| Dirty propagation (32K tree) | ~3.3M ops/sec | Skips already-dirty subtrees |

> Benchmarks on Apple Silicon, Release build. Results vary by hardware.

## Testing

```bash
# Build and run tests
cmake -DBUILD_TESTS=ON ..
make
./tests/run_tests

# Or using CTest
ctest
```

## Project Structure

```
eSGraph/
├── include/
│   └── Node.hpp          # Main header
├── src/
│   └── Node.cpp          # Implementation
├── tests/
│   └── src/
│       └── NodeTests.cpp # Unit tests
├── benchmarks/           # Performance benchmarks
│   ├── include/
│   └── src/
└── external/
    ├── glm/              # GLM submodule
    └── googletest/       # Google Test submodule
```

## API Notes

### Children Container

Children are stored in a `std::vector<std::unique_ptr<Node>>` for cache-friendly iteration. Methods return vector references:

```cpp
const std::vector<std::unique_ptr<Node>>& children = node->getChildren();
std::vector<std::unique_ptr<Node>> removed = node->removeAllChildren();
```

### Ownership

- `addChild()` takes ownership via `std::unique_ptr`
- `removeChild()` and `removeAllChildren()` return ownership to caller
- **Important:** If you discard the returned `unique_ptr`, the node is deleted immediately

```cpp
// Wrong - node deleted, subsequent access is undefined behavior
auto child = parent->getChildren().front().get();
(void)parent->removeChild(child);
child->doSomething();  // UB!

// Correct - ownership preserved
auto removed = parent->removeChild(child);
child->doSomething();  // Safe - removed keeps it alive
```

## License

MIT License - See [LICENSE](LICENSE.md) for details.
