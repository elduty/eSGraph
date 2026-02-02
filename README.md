[![CI](https://github.com/elduty/eSGraph/actions/workflows/ci.yml/badge.svg)](https://github.com/elduty/eSGraph/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/elduty/eSGraph/branch/main/graph/badge.svg)](https://codecov.io/gh/elduty/eSGraph)

# eSGraph

A lightweight, high-performance C++ scene graph library built on [GLM](http://glm.g-truc.net/) (OpenGL Mathematics). eSGraph provides hierarchical node-based management of 3D transformations with support for multiple coordinate spaces.

## Features

- **Hierarchical Scene Graph** - Parent-child node relationships with automatic transform propagation
- **Multiple Coordinate Spaces** - LOCAL, PARENT, and WORLD coordinate systems
- **Efficient Caching** - Lazy matrix computation with dirty flag optimization
- **Modern C++20** - Uses smart pointers, move semantics, and modern language features
- **Minimal Dependencies** - Only requires GLM (header-only)
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

```bash
# Build with benchmarks
cmake -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release ..
make
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

## Performance

eSGraph uses lazy evaluation with dirty flags for optimal performance. Matrices are only recomputed when transformations change.

### Benchmarks

Run the performance benchmarks:

```bash
cmake -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release ..
make
./benchmarks/run_benchmarks
```

This generates a detailed report at `benchmark_report.md`. See [benchmark_report.md](benchmark_report.md) for latest results.

#### Key Performance Characteristics

| Operation | Typical Performance |
|-----------|---------------------|
| Cached matrix access | ~750M ops/sec |
| Matrix recomputation | ~34M ops/sec |
| Local position set | ~91M ops/sec |
| World position set | ~62M ops/sec |
| Deep hierarchy traversal (100 levels) | ~874K ops/sec |

> Benchmarks run on Release build. Results vary by hardware.

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

## License

MIT License - See [LICENSE](LICENSE.md) for details.
