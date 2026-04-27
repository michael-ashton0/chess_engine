#include "movegen.h"

void generateMoves(Board& board, MoveList& moves)
{
    moves.clear();

    if (board.side == Board::WHITE) {
        PawnMoveGen::generateWhite(board, moves, board.enPassantSq);
        KnightMoveGen::generateWhite(board, moves);
        BishopMoveGen::generateWhite(board, moves);
        RookMoveGen::generateWhite(board, moves);
        QueenMoveGen::generateWhite(board, moves);
        KingMoveGen::generateWhite(board, moves);
    } else {
        PawnMoveGen::generateBlack(board, moves, board.enPassantSq);
        KnightMoveGen::generateBlack(board, moves);
        BishopMoveGen::generateBlack(board, moves);
        RookMoveGen::generateBlack(board, moves);
        QueenMoveGen::generateBlack(board, moves);
        KingMoveGen::generateBlack(board, moves);
    }
}