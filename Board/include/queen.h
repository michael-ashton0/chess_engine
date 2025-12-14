#pragma once
#include "inclusions.h"

namespace QueenMoveGen {
    void generateWhite(Board& board,
                       std::vector<Move>& moves);

    void generateBlack(Board& board,
                       std::vector<Move>& moves);

    uint64_t nonMagicQueenAttacks(int sq, uint64_t occupied);

    const uint64_t QMagic[64];
    const uint64_t QBlockers[64];
    const uint64_t QShift[64];
}