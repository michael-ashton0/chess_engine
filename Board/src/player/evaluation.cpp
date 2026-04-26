#include "evaluation.h"

static int popcount(uint64_t bb)
{
    return __builtin_popcountll(bb);
}

int evaluate(const Board& board)
{
    int whiteScore = 0;
    int blackScore = 0;

    whiteScore += 100 * __builtin_popcountll(board.pawnsWhite());
    whiteScore += 320 * __builtin_popcountll(board.knightsWhite());
    whiteScore += 330 * __builtin_popcountll(board.bishopsWhite());
    whiteScore += 500 * __builtin_popcountll(board.rooksWhite());
    whiteScore += 900 * __builtin_popcountll(board.queensWhite());

    blackScore += 100 * __builtin_popcountll(board.pawnsBlack());
    blackScore += 320 * __builtin_popcountll(board.knightsBlack());
    blackScore += 330 * __builtin_popcountll(board.bishopsBlack());
    blackScore += 500 * __builtin_popcountll(board.rooksBlack());
    blackScore += 900 * __builtin_popcountll(board.queensBlack());

    int score = whiteScore - blackScore;

    return board.side == Board::WHITE ? score : -score;
}