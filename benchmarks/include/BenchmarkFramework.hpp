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
#include <iomanip>
#include <algorithm>
#include <numeric>

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
