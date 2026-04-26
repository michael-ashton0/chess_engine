#include "nps.h"

void NpsCounter::start() {
    nodes = 0;
    startTime = std::chrono::steady_clock::now();
}

void NpsCounter::addNodes(uint64_t n) {
    nodes += n;
}

uint64_t NpsCounter::getNodes() {
    return nodes;
}

double NpsCounter::elapsedSeconds() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - startTime;
    return elapsed.count();
}

uint64_t NpsCounter::nodesPerSecond() {
    double seconds = elapsedSeconds();
    if (seconds <= 0.0) return 0;
    return static_cast<uint64_t>(nodes / seconds);
}

uint64_t NpsCounter::nodes = 0;
std::chrono::steady_clock::time_point NpsCounter::startTime;