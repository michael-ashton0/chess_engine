#pragma once
#include "inclusions.h"

uint64_t LEFT2  = FILE_A | FILE_B;
uint64_t RIGHT2 = FILE_G | FILE_H;
uint64_t TOP2   = RANK_7 | RANK_8;
uint64_t BOT2   = RANK_1 | RANK_2;
// knight move offsets:
// +17, +15, +10, +6
// -17, -15, -10, -6
namespace KnightMoveGen {
    uint64_t knightMoves[64];

    void initKnightMoves();

    void generateWhite(Board& board, 
                      std::vector<Move>& moves);
    
    void generateBlack(Board& board, 
                      std::vector<Move>& moves);
}