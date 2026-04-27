#pragma once
#include "inclusions.h"
#include "board.h"
#include "bitboard.h"
#include "movelist.h"

namespace QueenMoveGen {
    void generateWhite(Board& board,
                        MoveList& moves);

    void generateBlack(Board& board,
                       MoveList& moves);

    uint64_t nonMagicQueenAttacks(int sq, uint64_t occupied);

    static uint64_t QMagic[64];
    static uint64_t QBlockers[64];
    static uint64_t QShift[64];
}