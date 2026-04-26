#include <vector>

#include "search.h"
#include "evaluation.h"
#include "move.h"


static bool inCheck(Board& board)
{
    Board::Side us = board.side;
    Board::Side them = (us == Board::WHITE) ? Board::BLACK : Board::WHITE;

    int kingSq = board.kingSquare(us);

    return board.isAttacked(kingSq, them);
}

static std::vector<Move> generateLegalMoves(Board& board)
{
    std::vector<Move> pseudo;
    std::vector<Move> legal;

    generateMoves(board, pseudo);

    Board::Side us = board.side;

    for (const Move& move : pseudo) {
        Board next = board;
        next.makeMove(move);

        int kingSq = next.kingSquare(us);

        if (!next.isAttacked(kingSq, next.side)) {
            legal.push_back(move);
        }
    }

    return legal;
}

static int negamax(Board& board, int depth, int ply)
{
    if (depth == 0) {
        return evaluate(board);
    }

    std::vector<Move> moves = generateLegalMoves(board);

    if (moves.empty()) {
        if (inCheck(board)) {
            return -CHECKMATE + ply;
        }

        return 0; // stalemate
    }

    int bestScore = -CHECKMATE;

    for (const Move& move : moves) {
        board.makeMove(move);

        int score = -negamax(board, depth - 1, ply + 1);

        board.unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;
        }
    }

    return bestScore;
}


Move findBestMove(Board& board, int depth)
{
    std::vector<Move> moves = generateLegalMoves(board);

    Move bestMove;

    if (moves.empty()) {
        return bestMove;
    }

    int bestScore = -CHECKMATE;

    for (const Move& move : moves) {
        board.makeMove(move);

        int score = -negamax(board, depth - 1, 1);

        board.unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}
