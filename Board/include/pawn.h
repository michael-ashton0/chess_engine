#pragma once
#include "inclusions.h"

namespace PawnMoveGen {
    void generateWhite(Board& board,
                       std::vector<Move>& moves,
                       uint64_t enPassantSquare = 0ULL);

    void generateBlack(Board& board,
                              std::vector<Move>& moves,
                              uint64_t enPassantSquare = 0ULL);
    
    uint64_t movePawn(Board& board, int sq, bool color);
};
