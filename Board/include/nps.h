#pragma once
#include <chrono>
#include <cstdint>

namespace NpsCounter {
    void start();
    void addNodes(uint64_t n = 1);
    uint64_t getNodes();
    double elapsedSeconds();
    uint64_t nodesPerSecond();

    extern uint64_t nodes;
    extern std::chrono::steady_clock::time_point startTime;
}