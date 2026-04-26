#include "inclusions.h"
#include "pieces.h"
#include "move.h"

void generateMoves(Board& board, std::vector<Move>& moves)
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

std::string squareName(int sq)
{
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);
    return std::string{file, rank};
}

std::string moveName(const Move& m)
{
    std::string out = squareName(m.from) + squareName(m.to);

    if (m.isPromotion) {
        switch (m.promotionPiece) {
            case KNIGHT: out += 'n'; break;
            case BISHOP: out += 'b'; break;
            case ROOK:   out += 'r'; break;
            case QUEEN:  out += 'q'; break;
            default: break;
        }
    }

    return out;
}