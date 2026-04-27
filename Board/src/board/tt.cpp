#include "tt.h"

TranspositionTable::TranspositionTable(std::size_t sizeMb)
{
    std::size_t bytes = sizeMb * 1024ULL * 1024ULL;
    std::size_t count = bytes / sizeof(TTEntry);

    if (count == 0) {
        count = 1;
    }

    table.resize(count);
}

void TranspositionTable::clear()
{
    for (TTEntry& entry : table) {
        entry.valid = false;
    }
}

bool TranspositionTable::probe(
    uint64_t key,
    int depth,
    int alpha,
    int beta,
    int& outScore,
    Move& outBestMove
) const {
    const TTEntry& entry = table[index(key)];

    if (!entry.valid || entry.key != key) {
        return false;
    }

    outBestMove = entry.bestMove;

    if (entry.depth < depth) {
        return false;
    }

    if (entry.flag == TT_EXACT) {
        outScore = entry.score;
        return true;
    }

    if (entry.flag == TT_LOWERBOUND && entry.score >= beta) {
        outScore = entry.score;
        return true;
    }

    if (entry.flag == TT_UPPERBOUND && entry.score <= alpha) {
        outScore = entry.score;
        return true;
    }

    return false;
}

bool TranspositionTable::probeMoveOnly(uint64_t key, Move& outBestMove) const
{
    const TTEntry& entry = table[index(key)];

    if (!entry.valid || entry.key != key) {
        return false;
    }

    outBestMove = entry.bestMove;
    return true;
}

void TranspositionTable::store(
    uint64_t key,
    int depth,
    int score,
    TTFlag flag,
    const Move& bestMove
) {
    TTEntry& entry = table[index(key)];

    // Simple replacement rule:
    // replace empty entries, same key, or shallower entries.
    if (!entry.valid || entry.key == key || depth >= entry.depth) {
        entry.key = key;
        entry.depth = depth;
        entry.score = score;
        entry.flag = flag;
        entry.bestMove = bestMove;
        entry.valid = true;
    }
}