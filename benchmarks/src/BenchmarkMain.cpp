//
//  BenchmarkMain.cpp
//  eSGraph Benchmarks
//
//  Entry point for benchmark executable
//

#include "BenchmarkFramework.hpp"
#include <iostream>

// Forward declaration from NodeBenchmarks.cpp
namespace eSGraph::Benchmark {
void registerNodeBenchmarks();
}

int main(int argc, char* argv[]) {
    using namespace eSGraph::Benchmark;

    std::cout << "\n";

    // Configure benchmark parameters
    BenchmarkConfig config;
    config.warmupIterations = 100;
    config.measureIterations = 10000;
    config.repeatCount = 5;

    // Allow command line override for quick tests
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--quick") {
            config.warmupIterations = 10;
            config.measureIterations = 1000;
            config.repeatCount = 3;
            std::cout << "Running in quick mode (reduced iterations)\n\n";
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options]\n";
            std::cout << "Options:\n";
            std::cout << "  --quick    Run with reduced iterations for faster results\n";
            std::cout << "  --help     Show this help message\n";
            return 0;
        }
    }

    BenchmarkRunner& runner = BenchmarkRunner::instance();
    runner.setConfig(config);

    // Register all benchmark scenarios
    registerNodeBenchmarks();

    // Run benchmarks and display results
    runner.runAll();

    std::cout << "\nNote: For accurate results, run in Release mode with optimizations enabled.\n";
    std::cout << "      cmake -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release ..\n\n";

    return 0;
}
