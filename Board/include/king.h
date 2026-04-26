#pragma once
#include "inclusions.h"
#include "board.h"

namespace KingMoveGen {
   extern uint64_t kingMoves[64];

    void initKingMoves();

    void generateWhite(Board& board, 
                      std::vector<Move>& moves);
    
    void generateBlack(Board& board, 
                      std::vector<Move>& moves);
    
    void generateWhiteCastleMoves(Board& board,
                                std::vector<Move>& moves);
    
    void generateBlackCastleMoves(Board& board,
                                std::vector<Move>& moves);
}