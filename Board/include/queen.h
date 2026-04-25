#pragma once
#include "inclusions.h"
#include "board.h"
#include "bitboard.h"

namespace QueenMoveGen {
    void generateWhite(Board& board,
                        std::vector<Move>& moves);

    void generateBlack(Board& board,
                       std::vector<Move>& moves);

    uint64_t nonMagicQueenAttacks(int sq, uint64_t occupied);

    static uint64_t QMagic[64];
    static uint64_t QBlockers[64];
    static uint64_t QShift[64];
}