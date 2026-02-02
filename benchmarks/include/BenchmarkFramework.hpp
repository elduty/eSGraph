//
//  BenchmarkFramework.hpp
//  eSGraph Benchmarks
//
//  Lightweight benchmarking utilities using std::chrono
//

#ifndef BenchmarkFramework_hpp
#define BenchmarkFramework_hpp

#include <chrono>
#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <ctime>

namespace eSGraph::Benchmark {

// Prevent compiler from optimizing away benchmark code
template<typename T>
inline void DoNotOptimize(T&& value) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "g"(value) : "memory");
#else
    volatile auto unused = value;
    (void)unused;
#endif
}

// Prevent reordering of memory operations
inline void ClobberMemory() {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : : "memory");
#endif
}

struct BenchmarkResult {
    std::string name;
    size_t iterations;
    double avgMicroseconds;
    double minMicroseconds;
    double maxMicroseconds;
    double opsPerSecond;
};

struct BenchmarkConfig {
    size_t warmupIterations = 100;
    size_t measureIterations = 10000;
    size_t repeatCount = 5;
};

class BenchmarkRunner {
public:
    using BenchmarkFunc = std::function<void()>;
    using SetupFunc = std::function<void()>;
    using TeardownFunc = std::function<void()>;

    static BenchmarkRunner& instance() {
        static BenchmarkRunner runner;
        return runner;
    }

    void setConfig(const BenchmarkConfig& config) {
        mConfig = config;
    }

    void registerBenchmark(const std::string& name,
                          BenchmarkFunc func,
                          SetupFunc setup = nullptr,
                          TeardownFunc teardown = nullptr) {
        mBenchmarks.push_back({name, func, setup, teardown});
    }

    void runAll() {
        printHeader();

        for (const auto& benchmark : mBenchmarks) {
            auto result = runSingle(benchmark);
            mResults.push_back(result);
            printResult(result);
        }

        printFooter();
    }

    const std::vector<BenchmarkResult>& getResults() const {
        return mResults;
    }

    const BenchmarkConfig& getConfig() const {
        return mConfig;
    }

    void generateMarkdownReport(const std::string& filepath) const {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filepath << "\n";
            return;
        }

        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream timestamp;
        timestamp << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

        // Header
        file << "# eSGraph Performance Benchmark Report\n\n";
        file << "**Generated:** " << timestamp.str() << "\n\n";

        // Configuration
        file << "## Configuration\n\n";
        file << "| Parameter | Value |\n";
        file << "|-----------|-------|\n";
        file << "| Warmup Iterations | " << mConfig.warmupIterations << " |\n";
        file << "| Measurement Iterations | " << mConfig.measureIterations << " |\n";
        file << "| Repeat Count | " << mConfig.repeatCount << " |\n";
#ifdef NDEBUG
        file << "| Build Type | Release |\n";
#else
        file << "| Build Type | Debug |\n";
#endif
        file << "\n";

        // Results table
        file << "## Results\n\n";
        file << "| Benchmark | Avg (us) | Min (us) | Max (us) | Ops/sec |\n";
        file << "|-----------|----------|----------|----------|----------|\n";

        for (const auto& result : mResults) {
            file << "| " << result.name
                 << " | " << std::fixed << std::setprecision(3) << result.avgMicroseconds
                 << " | " << result.minMicroseconds
                 << " | " << result.maxMicroseconds
                 << " | " << std::setprecision(0) << result.opsPerSecond
                 << " |\n";
        }
        file << "\n";

        // Categorized results
        file << "## Results by Category\n\n";

        // Local Matrix Operations
        file << "### Local Matrix Operations\n\n";
        file << "| Benchmark | Avg (us) | Ops/sec |\n";
        file << "|-----------|----------|----------|\n";
        for (const auto& result : mResults) {
            if (result.name.find("BM_GetMatrix") != std::string::npos) {
                file << "| " << result.name
                     << " | " << std::fixed << std::setprecision(3) << result.avgMicroseconds
                     << " | " << std::setprecision(0) << result.opsPerSecond << " |\n";
            }
        }
        file << "\n";

        // Global Matrix Operations
        file << "### Global Matrix Operations\n\n";
        file << "| Benchmark | Avg (us) | Ops/sec |\n";
        file << "|-----------|----------|----------|\n";
        for (const auto& result : mResults) {
            if (result.name.find("BM_GetGlobalMatrix") != std::string::npos) {
                file << "| " << result.name
                     << " | " << std::fixed << std::setprecision(3) << result.avgMicroseconds
                     << " | " << std::setprecision(0) << result.opsPerSecond << " |\n";
            }
        }
        file << "\n";

        // World Coordinate Operations
        file << "### World Coordinate Operations\n\n";
        file << "| Benchmark | Avg (us) | Ops/sec |\n";
        file << "|-----------|----------|----------|\n";
        for (const auto& result : mResults) {
            if ((result.name.find("Position") != std::string::npos ||
                 result.name.find("Rotation") != std::string::npos) &&
                result.name.find("BM_GetGlobalMatrix") == std::string::npos &&
                result.name.find("BM_GetMatrix") == std::string::npos) {
                file << "| " << result.name
                     << " | " << std::fixed << std::setprecision(3) << result.avgMicroseconds
                     << " | " << std::setprecision(0) << result.opsPerSecond << " |\n";
            }
        }
        file << "\n";

        // Dirty Flag Propagation
        file << "### Dirty Flag Propagation\n\n";
        file << "| Benchmark | Avg (us) | Ops/sec |\n";
        file << "|-----------|----------|----------|\n";
        for (const auto& result : mResults) {
            if (result.name.find("DirtyPropagation") != std::string::npos) {
                file << "| " << result.name
                     << " | " << std::fixed << std::setprecision(3) << result.avgMicroseconds
                     << " | " << std::setprecision(0) << result.opsPerSecond << " |\n";
            }
        }
        file << "\n";

        // Hierarchy Modifications
        file << "### Hierarchy Modifications\n\n";
        file << "| Benchmark | Avg (us) | Ops/sec |\n";
        file << "|-----------|----------|----------|\n";
        for (const auto& result : mResults) {
            if (result.name.find("AddChild") != std::string::npos ||
                result.name.find("RemoveChild") != std::string::npos) {
                file << "| " << result.name
                     << " | " << std::fixed << std::setprecision(3) << result.avgMicroseconds
                     << " | " << std::setprecision(0) << result.opsPerSecond << " |\n";
            }
        }
        file << "\n";

        // Analysis section
        file << "## Analysis\n\n";

        // Find specific benchmarks for comparison
        double cleanMatrix = 0, dirtyMatrix = 0;
        double localRotation = 0, worldRotation = 0;
        double localPosition = 0, worldPosition = 0;
        double cachedGlobal = 0, deep100Global = 0;

        for (const auto& result : mResults) {
            if (result.name == "BM_GetMatrix_Clean") cleanMatrix = result.avgMicroseconds;
            if (result.name == "BM_GetMatrix_Dirty") dirtyMatrix = result.avgMicroseconds;
            if (result.name == "BM_SetRotation_Local") localRotation = result.avgMicroseconds;
            if (result.name == "BM_SetRotation_World_DeepNode") worldRotation = result.avgMicroseconds;
            if (result.name == "BM_SetPosition_Local") localPosition = result.avgMicroseconds;
            if (result.name == "BM_SetPosition_World_DeepNode") worldPosition = result.avgMicroseconds;
            if (result.name == "BM_GetGlobalMatrix_Cached") cachedGlobal = result.avgMicroseconds;
            if (result.name == "BM_GetGlobalMatrix_DeepChain_100") deep100Global = result.avgMicroseconds;
        }

        file << "### Key Findings\n\n";

        if (cleanMatrix > 0 && dirtyMatrix > 0) {
            file << "- **Matrix Caching:** Clean matrix access is "
                 << std::fixed << std::setprecision(1) << (dirtyMatrix / cleanMatrix)
                 << "x faster than dirty recomputation\n";
        }

        if (localRotation > 0 && worldRotation > 0) {
            file << "- **Rotation Overhead:** World rotation is "
                 << std::fixed << std::setprecision(1) << (worldRotation / localRotation)
                 << "x slower than local rotation\n";
        }

        if (localPosition > 0 && worldPosition > 0) {
            file << "- **Position Overhead:** World position setting is "
                 << std::fixed << std::setprecision(1) << (worldPosition / localPosition)
                 << "x slower than local\n";
        }

        if (cachedGlobal > 0 && deep100Global > 0) {
            file << "- **Hierarchy Depth Impact:** 100-level deep traversal is "
                 << std::fixed << std::setprecision(0) << (deep100Global / cachedGlobal)
                 << "x slower than cached access\n";
        }

        file << "\n### Recommendations\n\n";
        file << "1. **Cache global matrices** when possible - repeated access is very fast\n";
        file << "2. **Prefer local coordinates** for frequent transformations\n";
        file << "3. **Minimize hierarchy depth** for performance-critical nodes\n";
        file << "4. **Batch updates** to reduce dirty flag propagation overhead\n";

        file.close();
        std::cout << "Markdown report generated: " << filepath << "\n";
    }

private:
    struct RegisteredBenchmark {
        std::string name;
        BenchmarkFunc func;
        SetupFunc setup;
        TeardownFunc teardown;
    };

    BenchmarkConfig mConfig;
    std::vector<RegisteredBenchmark> mBenchmarks;
    std::vector<BenchmarkResult> mResults;

    BenchmarkResult runSingle(const RegisteredBenchmark& benchmark) {
        std::vector<double> runTimes;
        runTimes.reserve(mConfig.repeatCount);

        for (size_t run = 0; run < mConfig.repeatCount; ++run) {
            // Setup if provided
            if (benchmark.setup) {
                benchmark.setup();
            }

            // Warmup phase
            for (size_t i = 0; i < mConfig.warmupIterations; ++i) {
                benchmark.func();
            }
            ClobberMemory();

            // Measurement phase
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < mConfig.measureIterations; ++i) {
                benchmark.func();
            }
            ClobberMemory();
            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            double microsPerOp = static_cast<double>(duration.count()) /
                                static_cast<double>(mConfig.measureIterations) / 1000.0;
            runTimes.push_back(microsPerOp);

            // Teardown if provided
            if (benchmark.teardown) {
                benchmark.teardown();
            }
        }

        // Calculate statistics
        double avg = std::accumulate(runTimes.begin(), runTimes.end(), 0.0) /
                     static_cast<double>(runTimes.size());
        double minTime = *std::min_element(runTimes.begin(), runTimes.end());
        double maxTime = *std::max_element(runTimes.begin(), runTimes.end());
        double opsPerSec = 1'000'000.0 / avg;

        return {
            benchmark.name,
            mConfig.measureIterations,
            avg,
            minTime,
            maxTime,
            opsPerSec
        };
    }

    void printHeader() const {
        std::cout << std::string(80, '=') << "\n";
        std::cout << std::setw(40 + 17) << "eSGraph Performance Benchmarks\n";
        std::cout << std::string(80, '=') << "\n";
        std::cout << std::left << std::setw(40) << "Name"
                  << std::right << std::setw(8) << "Iters"
                  << std::setw(10) << "Avg(us)"
                  << std::setw(10) << "Min(us)"
                  << std::setw(10) << "Max(us)"
                  << std::setw(12) << "Ops/s"
                  << "\n";
        std::cout << std::string(80, '-') << "\n";
    }

    void printResult(const BenchmarkResult& result) const {
        std::cout << std::left << std::setw(40) << result.name
                  << std::right << std::setw(8) << result.iterations
                  << std::fixed << std::setprecision(3)
                  << std::setw(10) << result.avgMicroseconds
                  << std::setw(10) << result.minMicroseconds
                  << std::setw(10) << result.maxMicroseconds
                  << std::setprecision(0)
                  << std::setw(12) << result.opsPerSecond
                  << "\n";
    }

    void printFooter() const {
        std::cout << std::string(80, '=') << "\n";
        std::cout << "Configuration: "
                  << mConfig.warmupIterations << " warmup, "
                  << mConfig.measureIterations << " iterations, "
                  << mConfig.repeatCount << " repeats\n";
    }
};

// Macro for easy benchmark registration
#define BENCHMARK(name) \
    static void name(); \
    namespace { \
        struct name##_Registrar { \
            name##_Registrar() { \
                eSGraph::Benchmark::BenchmarkRunner::instance().registerBenchmark(#name, name); \
            } \
        } name##_registrar_instance; \
    } \
    static void name()

#define BENCHMARK_WITH_SETUP(name, setup_func, teardown_func) \
    static void name(); \
    namespace { \
        struct name##_Registrar { \
            name##_Registrar() { \
                eSGraph::Benchmark::BenchmarkRunner::instance().registerBenchmark( \
                    #name, name, setup_func, teardown_func); \
            } \
        } name##_registrar_instance; \
    } \
    static void name()

} // namespace eSGraph::Benchmark

#endif /* BenchmarkFramework_hpp */
