#pragma once
#include "inclusions.h"
#include "board.h"
#include "movelist.h"

namespace KingMoveGen {
   extern uint64_t kingMoves[64];

    void initKingMoves();

    void generateWhite(Board& board, 
                      MoveList& moves);
    
    void generateBlack(Board& board, 
                      MoveList& moves);
    
    void generateWhiteCastleMoves(Board& board,
                                MoveList& moves);
    
    void generateBlackCastleMoves(Board& board,
                                MoveList& moves);
}