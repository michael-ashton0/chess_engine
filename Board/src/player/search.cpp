#include <vector>
#include <algorithm>

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

std::vector<Move> generateLegalMoves(Board& board)
{
    std::vector<Move> pseudo;
    std::vector<Move> legal;

    pseudo.reserve(256);
    legal.reserve(256);

    generateMoves(board, pseudo);

    Board::Side us = board.side;

    for (const Move& move : pseudo) {
        board.makeMove(move);

        int kingSq = board.kingSquare(us);

        if (!board.isAttacked(kingSq, board.side)) {
            legal.push_back(move);
        }

        board.unmakeMove(move);
    }

    return legal;
}

int scoreMove(const Move& move)
{
    int score = 0;

    if (move.isPromotion) {
        if (move.promotionPiece == QUEEN) score += 9000;
        else if (move.promotionPiece == ROOK) score += 5000;
        else if (move.promotionPiece == BISHOP) score += 3300;
        else if (move.promotionPiece == KNIGHT) score += 3200;
    }

    if (move.isCapture) {
        score += 1000;
    }

    return score;
}

static int negamax_only(Board& board, int depth, int ply)
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

        int score = -negamax_only(board, depth - 1, ply + 1);

        board.unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;
        }
    }

    return bestScore;
}


Move findBestMove_ngo(Board& board, int depth)
{
    std::vector<Move> moves = generateLegalMoves(board);

    Move bestMove;

    if (moves.empty()) {
        return bestMove;
    }

    int bestScore = -CHECKMATE;

    for (const Move& move : moves) {
        board.makeMove(move);

        int score = -negamax_only(board, depth - 1, 1);

        board.unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

static int negamax_ab(Board& board, int depth, int ply, int alpha, int beta)
{
    if (depth == 0) {
        return evaluate(board);
    }

    std::vector<Move> moves = generateLegalMoves(board);

    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
    return scoreMove(a) > scoreMove(b);
    });

    if (moves.empty()) {
        if (inCheck(board)) {
            return -CHECKMATE + ply;
        }

        return 0; // stalemate
    }

    int bestScore = -CHECKMATE;

    for (const Move& move : moves) {
        board.makeMove(move);

        int score = -negamax_ab(board, depth - 1, ply + 1, -beta, -alpha);

        board.unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;
        }

        if (score > alpha) {
            alpha = score;
        }

        if (alpha >= beta) {
            break; // beta cutoff
        }
    }

    return bestScore;
}

Move findBestMove(Board& board, int depth)
{
    std::vector<Move> moves = generateLegalMoves(board);
    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        return scoreMove(a) > scoreMove(b);
    });

    Move bestMove;

    if (moves.empty()) {
        return bestMove;
    }

    int bestScore = -CHECKMATE;
    int alpha = -CHECKMATE;
    int beta = CHECKMATE;

    for (const Move& move : moves) {
        board.makeMove(move);

        int score = -negamax_ab(board, depth - 1, 1, -beta, -alpha);

        board.unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return bestMove;
}