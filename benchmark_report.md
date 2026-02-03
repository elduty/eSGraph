# eSGraph Performance Benchmark Report

**Generated:** 2026-02-03 21:05:04

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
| BM_GetMatrix_Clean | 0.002 | 0.002 | 0.002 | 429800658 |
| BM_GetMatrix_Dirty | 0.017 | 0.017 | 0.017 | 58527929 |
| BM_GetGlobalMatrix_RootNode | 0.002 | 0.002 | 0.002 | 603318250 |
| BM_GetGlobalMatrix_DeepChain_10 | 0.115 | 0.113 | 0.116 | 8726763 |
| BM_GetGlobalMatrix_DeepChain_50 | 0.586 | 0.563 | 0.597 | 1707835 |
| BM_GetGlobalMatrix_DeepChain_100 | 1.058 | 1.048 | 1.079 | 945358 |
| BM_GetGlobalMatrix_Cached | 0.001 | 0.001 | 0.002 | 714275510 |
| BM_SetPosition_Local | 0.004 | 0.004 | 0.004 | 281425363 |
| BM_SetPosition_World_DeepNode | 0.009 | 0.009 | 0.009 | 109529265 |
| BM_GetPosition_World_DeepNode | 0.002 | 0.002 | 0.002 | 500200080 |
| BM_DirtyPropagation_Flat_100 | 0.476 | 0.462 | 0.487 | 2102648 |
| BM_DirtyPropagation_Flat_1000 | 6.653 | 6.460 | 6.715 | 150302 |
| BM_DirtyPropagation_Flat_10000 | 68.670 | 65.499 | 71.103 | 14562 |
| BM_DirtyPropagation_Deep_100 | 1.001 | 0.984 | 1.020 | 998543 |
| BM_DirtyPropagation_BinaryTree_10 | 0.158 | 0.141 | 0.166 | 6346151 |
| BM_DirtyPropagation_BinaryTree_15 | 0.300 | 0.293 | 0.310 | 3330160 |
| BM_GetRotation_World_DeepNode | 0.224 | 0.218 | 0.231 | 4460834 |
| BM_SetRotation_World_DeepNode | 0.222 | 0.219 | 0.226 | 4506196 |
| BM_SetRotation_Local | 0.004 | 0.004 | 0.004 | 275799681 |
| BM_GetForward_World_DeepNode | 0.002 | 0.002 | 0.002 | 490383578 |
| BM_GetForward_Local | 0.002 | 0.002 | 0.002 | 440847132 |
| BM_GetAllDirections_World | 0.004 | 0.004 | 0.004 | 283353546 |
| BM_GetThreeDirections_Separate | 0.005 | 0.005 | 0.005 | 200636419 |
| BM_GetWorldRotation_Cached | 0.222 | 0.220 | 0.225 | 4509532 |
| BM_AddChild | 0.022 | 0.019 | 0.032 | 46042296 |
| BM_RemoveChild | 0.148 | 0.113 | 0.157 | 6773691 |

## Results by Category

### Local Matrix Operations

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_GetMatrix_Clean | 0.002 | 429800658 |
| BM_GetMatrix_Dirty | 0.017 | 58527929 |

### Global Matrix Operations

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_GetGlobalMatrix_RootNode | 0.002 | 603318250 |
| BM_GetGlobalMatrix_DeepChain_10 | 0.115 | 8726763 |
| BM_GetGlobalMatrix_DeepChain_50 | 0.586 | 1707835 |
| BM_GetGlobalMatrix_DeepChain_100 | 1.058 | 945358 |
| BM_GetGlobalMatrix_Cached | 0.001 | 714275510 |

### World Coordinate Operations

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_SetPosition_Local | 0.004 | 281425363 |
| BM_SetPosition_World_DeepNode | 0.009 | 109529265 |
| BM_GetPosition_World_DeepNode | 0.002 | 500200080 |
| BM_GetRotation_World_DeepNode | 0.224 | 4460834 |
| BM_SetRotation_World_DeepNode | 0.222 | 4506196 |
| BM_SetRotation_Local | 0.004 | 275799681 |
| BM_GetWorldRotation_Cached | 0.222 | 4509532 |

### Dirty Flag Propagation

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_DirtyPropagation_Flat_100 | 0.476 | 2102648 |
| BM_DirtyPropagation_Flat_1000 | 6.653 | 150302 |
| BM_DirtyPropagation_Flat_10000 | 68.670 | 14562 |
| BM_DirtyPropagation_Deep_100 | 1.001 | 998543 |
| BM_DirtyPropagation_BinaryTree_10 | 0.158 | 6346151 |
| BM_DirtyPropagation_BinaryTree_15 | 0.300 | 3330160 |

### Hierarchy Modifications

| Benchmark | Avg (us) | Ops/sec |
|-----------|----------|----------|
| BM_AddChild | 0.022 | 46042296 |
| BM_RemoveChild | 0.148 | 6773691 |

## Analysis

### Key Findings

- **Matrix Caching:** Clean matrix access is 7.3x faster than dirty recomputation
- **Rotation Overhead:** World rotation is 61.2x slower than local rotation
- **Position Overhead:** World position setting is 2.6x slower than local
- **Hierarchy Depth Impact:** 100-level deep traversal is 756x slower than cached access

### Recommendations

1. **Cache global matrices** when possible - repeated access is very fast
2. **Prefer local coordinates** for frequent transformations
3. **Minimize hierarchy depth** for performance-critical nodes
4. **Batch updates** to reduce dirty flag propagation overhead
