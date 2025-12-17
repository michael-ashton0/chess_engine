#include "rook.h"
#include "bishop.h"
#include "queen.h"
#include "knight.h"
#include "king.h"
#include "pawn.h"

void generateMoves(Board& board) {
    std::vector<Move> moves;
    
    if (board.side == 0) {
        RookMoveGen::generateWhite(board, moves);
        BishopMoveGen::generateWhite(board, moves);
        QueenMoveGen::generateWhite(board, moves);
        KnightMoveGen::generateWhite(board, moves);
        KingMoveGen::generateWhite(board, moves);
        PawnMoveGen::generateWhite(board, moves, board.enPassantSq);
    }
    
    else {
        RookMoveGen::generateBlack(board, moves);
        BishopMoveGen::generateBlack(board, moves);
        QueenMoveGen::generateBlack(board, moves);
        KnightMoveGen::generateBlack(board, moves);
        KingMoveGen::generateBlack(board, moves);
        PawnMoveGen::generateBlack(board, moves, board.enPassantSq);
    }
};