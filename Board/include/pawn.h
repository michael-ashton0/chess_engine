#pragma once
#include "inclusions.h"
#include "board.h"
#include "movelist.h"

namespace PawnMoveGen {
    void generateWhite(Board& board,
                       MoveList& moves,
                       uint64_t enPassantSquare = 0ULL);

    void generateBlack(Board& board,
                              MoveList& moves,
                              uint64_t enPassantSquare = 0ULL);
    
    uint64_t movePawn(Board& board, int sq, bool color);
};
