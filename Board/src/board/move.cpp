#include "rook.h"
#include "bishop.h"
#include "queen.h"
#include "knight.h"
#include "king.h"
#include "pawn.h"


void generateMoves(Board& board) {
    std::vector<Move> moves;
    RookMoveGen::generateWhite(board, moves);
    RookMoveGen::generateBlack(board, moves);

    BishopMoveGen::generateWhite(board, moves);
    BishopMoveGen::generateBlack(board, moves);

    QueenMoveGen::generateWhite(board, moves);
    QueenMoveGen::generateBlack(board, moves);

    KnightMoveGen::generateWhite(board, moves);
    KnightMoveGen::generateBlack(board, moves);

    KingMoveGen::generateWhite(board, moves);
    KingMoveGen::generateBlack(board, moves);

    PawnMoveGen::generateWhite(board, moves, 0ULL);
    PawnMoveGen::generateBlack(board, moves, 0ULL);
};