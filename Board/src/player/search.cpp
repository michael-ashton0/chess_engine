#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "search.h"
#include "evaluation.h"
#include "movegen.h"
#include "tt.h"
#include "zobrist.h"

static TranspositionTable TT(64);

constexpr int CHECKMATE = 100000;

static constexpr int MAX_PLY = 128;

static Move killerMoves[MAX_PLY][2];
static int historyHeuristic[2][64][64] = {};

static int pieceValue(PieceType p)
{
    switch (p) {
        case PAWN:   return 100;
        case KNIGHT: return 320;
        case BISHOP: return 330;
        case ROOK:   return 500;
        case QUEEN:  return 900;
        case KING:   return 20000;
    }

    return 0;
}

void clearSearchTables()
{
    TT.clear();
}

static int sideIndex(Board::Side side)
{
    return side == Board::WHITE ? 0 : 1;
}

static bool sameMove(const Move& a, const Move& b)
{
    if (a.from < 0 || a.to < 0 || b.from < 0 || b.to < 0) {
        return false;
    }

    if (a.from != b.from) return false;
    if (a.to != b.to) return false;
    if (a.piece != b.piece) return false;
    if (a.isPromotion != b.isPromotion) return false;

    if (a.isPromotion) {
        return a.promotionPiece == b.promotionPiece;
    }

    return true;
}

static void addKillerMove(const Move& move, int ply)
{
    if (ply < 0 || ply >= MAX_PLY) {
        return;
    }

    if (move.isCapture) {
        return;
    }

    if (sameMove(move, killerMoves[ply][0])) {
        return;
    }

    killerMoves[ply][1] = killerMoves[ply][0];
    killerMoves[ply][0] = move;
}

static void addHistoryScore(Board::Side side, const Move& move, int depth)
{
    if (move.isCapture) {
        return;
    }

    if (move.from < 0 || move.from >= 64 || move.to < 0 || move.to >= 64) {
        return;
    }

    historyHeuristic[sideIndex(side)][move.from][move.to] += depth * depth;
}

struct SearchTimer {
    using Clock = std::chrono::steady_clock;

    Clock::time_point start;
    int limitMs;

    explicit SearchTimer(int ms)
        : start(Clock::now()), limitMs(ms)
    {}

    bool outOfTime() const {
        auto now = Clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - start
        ).count();

        return elapsed >= limitMs;
    }

    long long elapsedMs() const {
        auto now = Clock::now();

        return std::chrono::duration_cast<std::chrono::milliseconds>(
            now - start
        ).count();
    }
};

static bool inCheck(Board& board)
{
    Board::Side us = board.side;
    Board::Side them = (us == Board::WHITE) ? Board::BLACK : Board::WHITE;

    int kingSq = board.kingSquare(us);

    return board.isAttacked(kingSq, them);
}

void generateLegalMoves(Board& board, MoveList& legalMoves)
{
    MoveList pseudoMoves;
    legalMoves.clear();

    generateMoves(board, pseudoMoves);

    Board::Side us = board.side;

    for (const Move& move : pseudoMoves) {
        board.makeMove(move);

        int kingSq = board.kingSquare(us);

        if (!board.isAttacked(kingSq, board.side)) {
            legalMoves.push_back(move);
        }

        board.unmakeMove(move);
    }
}

static int scoreMove(
    const Move& move,
    const Move& ttMove,
    bool hasTTMove,
    int ply,
    Board::Side sideToMove
) {
    if (hasTTMove && sameMove(move, ttMove)) {
        return 1'000'000;
    }

    int score = 0;

    if (move.isCapture) {
        // mvvlva
        score += 100'000;
        score += 10 * pieceValue(move.capturedPiece);
        score -= pieceValue(move.piece);
    }

    if (move.isPromotion) {
        score += 80'000 + pieceValue(move.promotionPiece);
    }

    if (!move.isCapture) {
        if (ply >= 0 && ply < MAX_PLY) {
            if (sameMove(move, killerMoves[ply][0])) {
                score += 70'000;
            } else if (sameMove(move, killerMoves[ply][1])) {
                score += 60'000;
            }
        }

        if (move.from >= 0 && move.from < 64 && move.to >= 0 && move.to < 64) {
            score += historyHeuristic[sideIndex(sideToMove)][move.from][move.to];
        }
    }

    return score;
}

static void orderMoves(
    MoveList& moves,
    const Move& ttMove,
    bool hasTTMove,
    int ply,
    Board::Side sideToMove
) {
    std::sort(moves.begin(), moves.end(), [&](const Move& a, const Move& b) {
        return scoreMove(a, ttMove, hasTTMove, ply, sideToMove)
             > scoreMove(b, ttMove, hasTTMove, ply, sideToMove);
    });
}

static int negamax(
    Board& board,
    int depth,
    int ply,
    int alpha,
    int beta,
    const SearchTimer* timer,
    bool& stopped
) {
    if (timer && timer->outOfTime()) {
        stopped = true;
        return 0;
    }

    if (depth == 0) {
        return evaluate(board); // later: quiescence(board, ply, alpha, beta)
    }

    int originalAlpha = alpha;

    uint64_t key = Zobrist::computeHash(board);

    int ttScore = 0;
    Move ttMove{};
    bool hasTTMove = TT.probeMoveOnly(key, ttMove);

    if (TT.probe(key, depth, alpha, beta, ttScore, ttMove)) {
        return ttScore;
    }

    MoveList moves;
    generateLegalMoves(board, moves);

    if (moves.empty()) {
        if (inCheck(board)) {
            return -CHECKMATE + ply;
        }

        return 0;
    }
    Board::Side us = board.side;
    orderMoves(moves, ttMove, hasTTMove, ply, us);

    int bestScore = -CHECKMATE;
    Move bestMove = moves[0];

    for (const Move& move : moves) {
        if (timer && timer->outOfTime()) {
            stopped = true;
            return 0;
        }

        board.makeMove(move);

        int score = -negamax(
            board,
            depth - 1,
            ply + 1,
            -beta,
            -alpha,
            timer,
            stopped
        );

        board.unmakeMove(move);

        if (stopped) {
            return 0;
        }

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        if (score > alpha) {
            alpha = score;
        }

        if (alpha >= beta) {
            break;
        }
    }

    TTFlag flag;

    if (bestScore <= originalAlpha) {
        flag = TT_UPPERBOUND;
    } else if (bestScore >= beta) {
        flag = TT_LOWERBOUND;
    } else {
        flag = TT_EXACT;
    }

    TT.store(key, depth, bestScore, flag, bestMove);

    return bestScore;
}

static Move findBestMoveTimedAtDepth(
    Board& board,
    int depth,
    const SearchTimer* timer,
    bool& stopped,
    int& completedScore
) {
    uint64_t key = Zobrist::computeHash(board);

    Move ttMove;
    bool hasTTMove = TT.probeMoveOnly(key, ttMove);

    Board::Side us = board.side;
    MoveList moves;
    generateLegalMoves(board, moves);
    orderMoves(moves, ttMove, hasTTMove, 0, us);

    Move bestMove;

    if (moves.empty()) {
        completedScore = 0;
        return bestMove;
    }

    bestMove = moves[0];

    int bestScore = -CHECKMATE;
    int alpha = -CHECKMATE;
    int beta = CHECKMATE;

    for (const Move& move : moves) {
        if (timer && timer -> outOfTime()) {
            stopped = true;
            return bestMove;
        }

        board.makeMove(move);

        int score = -negamax(
            board,
            depth - 1,
            1,
            -beta,
            -alpha,
            timer,
            stopped
        );

        board.unmakeMove(move);

        if (stopped) {
            return bestMove;
        }

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    completedScore = bestScore;
    return bestMove;
}

Move findBestMoveIterativeTimed(Board& board, int maxDepth, int timeMs)
{
    MoveList moves;
    generateLegalMoves(board, moves);

    if (moves.empty()) {
        return Move{};
    }

    if (moves.size() == 1) {
        return moves[0];
    }

    // Always have a legal fallback.
    Move bestMove = moves[0];

    // Avoid weird zero or negative time budgets.
    if (timeMs < 10) {
        timeMs = 10;
    }

    SearchTimer timer(timeMs);

    for (int depth = 1; depth <= maxDepth; ++depth) {
        bool stopped = false;
        int score = 0;

        Move currentBest = findBestMoveTimedAtDepth(
            board,
            depth,
            &timer,
            stopped,
            score
        );

        // Important:
        // Only trust the move if the full depth completed.
        if (stopped) {
            break;
        }

        bestMove = currentBest;

        std::cout << "info depth " << depth
                  << " score cp " << score
                  << " time " << timer.elapsedMs()
                  << " pv " << moveName(bestMove)
                  << std::endl;

        if (timer.outOfTime()) {
            break;
        }
    }

    return bestMove;
}

// Move findBestMove(Board& board, int depth)
// {
//     MoveList moves;
//     generateLegalMoves(board, moves);

//     uint64_t key = Zobrist::computeHash(board);

//     Move ttMove;
//     bool hasTTMove = TT.probeMoveOnly(key, ttMove);

//     Board::Side us = board.side;
//     orderMoves(moves, ttMove, hasTTMove, ply, us);

//     Move bestMove;

//     if (moves.empty()) {
//         return bestMove;
//     }

//     int bestScore = -CHECKMATE;
//     int alpha = -CHECKMATE;
//     int beta = CHECKMATE;

//     for (const Move& move : moves) {
//         board.makeMove(move);

//         int score = -negamax(board, depth - 1, 1, -beta, -alpha);

//         board.unmakeMove(move);

//         if (score > bestScore) {
//             bestScore = score;
//             bestMove = move;
//         }

//         if (score > alpha) {
//             alpha = score;
//         }
//     }

//     TT.store(key, depth, bestScore, TT_EXACT, bestMove);

//     return bestMove;
// }