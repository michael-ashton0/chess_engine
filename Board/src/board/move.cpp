#include "inclusions.h"
#include "pieces.h"
#include "move.h"

void generateMoves(Board& board, std::vector<Move>& moves)
{
    moves.clear();

    if (board.side == Board::WHITE) {
        RookMoveGen::generateWhite(board, moves);
        BishopMoveGen::generateWhite(board, moves);
        QueenMoveGen::generateWhite(board, moves);
        KnightMoveGen::generateWhite(board, moves);
        KingMoveGen::generateWhite(board, moves);
        PawnMoveGen::generateWhite(board, moves, board.enPassantSq);
    } else {
        RookMoveGen::generateBlack(board, moves);
        BishopMoveGen::generateBlack(board, moves);
        QueenMoveGen::generateBlack(board, moves);
        KnightMoveGen::generateBlack(board, moves);
        KingMoveGen::generateBlack(board, moves);
        PawnMoveGen::generateBlack(board, moves, board.enPassantSq);
    }
}

std::string squareName(int sq)
{
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);
    return std::string{file, rank};
}

std::string moveName(const Move& m)
{
    return squareName(m.from) + squareName(m.to);
}