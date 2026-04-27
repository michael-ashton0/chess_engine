#pragma once

#include <cstdint>
#include <vector>
#include "move.h"

enum TTFlag {
    TT_EXACT,
    TT_LOWERBOUND,
    TT_UPPERBOUND
};

struct TTEntry {
    uint64_t key = 0;
    int depth = -1;
    int score = 0;
    TTFlag flag = TT_EXACT;
    Move bestMove;
    bool valid = false;
};

class TranspositionTable {
public:
    explicit TranspositionTable(std::size_t sizeMb = 64);

    void clear();

    bool probe(
        uint64_t key,
        int depth,
        int alpha,
        int beta,
        int& outScore,
        Move& outBestMove
    ) const;

    bool probeMoveOnly(uint64_t key, Move& outBestMove) const;

    void store(
        uint64_t key,
        int depth,
        int score,
        TTFlag flag,
        const Move& bestMove
    );

private:
    std::vector<TTEntry> table;

    std::size_t index(uint64_t key) const {
        return key % table.size();
    }
};