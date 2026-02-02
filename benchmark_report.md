# eSGraph Performance Benchmark Report

**Generated:** 2026-02-02 23:10:13

## Configuration

| Parameter | Value |
|-----------|-------|
| Warmup Iterations | 100 |
| Measurement Iterations | 10000 |
| Repeat Count | 5 |
| Build Type | Release |

## Results

| Benchmark | Avg (us) | Min (us) | Max (us) | Ops/sec |
|-----------|----------|----------|----------|----------|
| BM_GetMatrix_Clean | 0.003 | 0.003 | 0.003 | 374181478 |
| BM_GetMatrix_Dirty | 0.029 | 0.026 | 0.034 | 34235836 |
| BM_GetGlobalMatrix_RootNode | 0.002 | 0.002 | 0.003 | 481932356 |
| BM_GetGlobalMatrix_DeepChain_10 | 0.144 | 0.139 | 0.148 | 6940268 |
| BM_GetGlobalMatrix_DeepChain_50 | 0.644 | 0.594 | 0.691 | 1551968 |
| BM_GetGlobalMatrix_DeepChain_100 | 1.144 | 1.076 | 1.190 | 873804 |
| BM_GetGlobalMatrix_Cached | 0.001 | 0.001 | 0.001 | 750931155 |
| BM_SetPosition_Local | 0.011 | 0.010 | 0.011 | 91150889 |
| BM_SetPosition_World_DeepNode | 0.016 | 0.016 | 0.016 | 62522899 |
| BM_GetPosition_World_DeepNode | 0.002 | 0.002 | 0.002 | 491593747 |
| BM_DirtyPropagation_Flat_100 | 0.637 | 0.623 | 0.650 | 1570358 |
| BM_DirtyPropagation_Flat_1000 | 7.507 | 7.384 | 7.729 | 133205 |
| BM_DirtyPropagation_Flat_10000 | 76.116 | 75.335 | 76.830 | 13138 |
| BM_DirtyPropagation_Deep_100 | 1.071 | 1.055 | 1.084 | 933937 |
| BM_DirtyPropagation_BinaryTree_10 | 1.707 | 1.633 | 1.747 | 585687 |
| BM_DirtyPropagation_BinaryTree_15 | 52.414 | 51.889 | 52.943 | 19079 |
| BM_GetRotation_World_DeepNode | 0.219 | 0.217 | 0.222 | 4567027 |
| BM_SetRotation_World_DeepNode | 0.181 | 0.179 | 0.186 | 5509743 |
| BM_SetRotation_Local | 0.010 | 0.010 | 0.010 | 97521396 |
| BM_AddChild | 0.027 | 0.027 | 0.028 | 36661380 |
| BM_RemoveChild | 1.434 | 1.408 | 1.456 | 697431 |

## Results by Category

### Local Matrix Operations

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_GetMatrix_Clean | 0.003 | 374181478 |
| BM_GetMatrix_Dirty | 0.029 | 34235836 |

### Global Matrix Operations

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_GetGlobalMatrix_RootNode | 0.002 | 481932356 |
| BM_GetGlobalMatrix_DeepChain_10 | 0.144 | 6940268 |
| BM_GetGlobalMatrix_DeepChain_50 | 0.644 | 1551968 |
| BM_GetGlobalMatrix_DeepChain_100 | 1.144 | 873804 |
| BM_GetGlobalMatrix_Cached | 0.001 | 750931155 |

### World Coordinate Operations

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_SetPosition_Local | 0.011 | 91150889 |
| BM_SetPosition_World_DeepNode | 0.016 | 62522899 |
| BM_GetPosition_World_DeepNode | 0.002 | 491593747 |
| BM_GetRotation_World_DeepNode | 0.219 | 4567027 |
| BM_SetRotation_World_DeepNode | 0.181 | 5509743 |
| BM_SetRotation_Local | 0.010 | 97521396 |

### Dirty Flag Propagation

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_DirtyPropagation_Flat_100 | 0.637 | 1570358 |
| BM_DirtyPropagation_Flat_1000 | 7.507 | 133205 |
| BM_DirtyPropagation_Flat_10000 | 76.116 | 13138 |
| BM_DirtyPropagation_Deep_100 | 1.071 | 933937 |
| BM_DirtyPropagation_BinaryTree_10 | 1.707 | 585687 |
| BM_DirtyPropagation_BinaryTree_15 | 52.414 | 19079 |

### Hierarchy Modifications

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_AddChild | 0.027 | 36661380 |
| BM_RemoveChild | 1.434 | 697431 |

## Analysis

### Key Findings

- **Matrix Caching:** Clean matrix access is 10.9x faster than dirty recomputation
- **Rotation Overhead:** World rotation is 17.7x slower than local rotation
- **Position Overhead:** World position setting is 1.5x slower than local
- **Hierarchy Depth Impact:** 100-level deep traversal is 859x slower than cached access

### Recommendations

1. **Cache global matrices** when possible - repeated access is very fast
2. **Prefer local coordinates** for frequent transformations
3. **Minimize hierarchy depth** for performance-critical nodes
4. **Batch updates** to reduce dirty flag propagation overhead
