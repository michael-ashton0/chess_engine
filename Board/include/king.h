#pragma once
#include "inclusions.h"

namespace KingMoveGen {
    uint64_t kingMoves[64];

    void initKingMoves();

    void generateWhite(Board& board, 
                      std::vector<Move>& moves);
    
    void generateBlack(Board& board, 
                      std::vector<Move>& moves);
}