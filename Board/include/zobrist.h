#pragma once

#include <cstdint>
#include "board.h"
#include "move.h"

namespace Zobrist {
    extern uint64_t piece[12][64];
    extern uint64_t sideToMove;
    extern uint64_t enPassant[64];

    extern uint64_t castling[16];

    void init();

    uint64_t computeHash(const Board& board);
}