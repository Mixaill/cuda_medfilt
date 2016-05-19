#include "benchmark.h"

std::chrono::steady_clock::time_point benchmarkTime;

void Benchmark::Start(){
    benchmarkTime = std::chrono::steady_clock::now();
}

double Benchmark::Stop(){
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - benchmarkTime ).count() / 1000.0;
}