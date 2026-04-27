#include "evaluation.h"
#include "bitboard.h"

static int popcount(uint64_t bb)
{
    return __builtin_popcountll(bb);
}

static int mirrorSquare(int sq)
{
    // Mirrors white-oriented PST square to black perspective.
    // A1 -> A8, B1 -> B8, etc.
    return sq ^ 56;
}

// Simple white-oriented piece-square tables.
// Positive = good for White.
// For Black, use mirrorSquare(sq).

static const int pawnPST[64] = {
     0,  0,  0,  0,  0,  0,  0,  0, 
     5,  5,  5,  0,  0,  5,  5,  5, 
     5,  5, 10, 15, 15, 10,  5,  5, 
     0,  0, 10, 25, 25, 10,  0,  0, 
     5,  5, 15, 30, 30, 15,  5,  5, 
    10, 10, 20, 30, 30, 20, 10, 10, 
    30, 30, 30, 30, 30, 30, 30, 30, 
     0,  0,  0,  0,  0,  0,  0,  0  
};

static const int knightPST[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

static const int bishopPST[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

static const int rookPST[64] = {
     0,  0,  5, 10, 10,  5,  0,  0,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  5,  5,  0,  0,  0
};

static const int queenPST[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

static const int kingMiddlePST[64] = {
     20, 30, 10,  0,  0, 10, 30, 20,
     20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

static int evalPSTFor(uint64_t pieces, const int pst[64], bool white)
{
    int score = 0;

    while (pieces) {
        int sq = pop_lsb(pieces);
        score += white ? pst[sq] : pst[mirrorSquare(sq)];
    }

    return score;
}

static int evaluateMaterial(const Board& board)
{
    int whiteScore = 0;
    int blackScore = 0;

    whiteScore += 100 * popcount(board.pawnsWhite());
    whiteScore += 320 * popcount(board.knightsWhite());
    whiteScore += 330 * popcount(board.bishopsWhite());
    whiteScore += 500 * popcount(board.rooksWhite());
    whiteScore += 900 * popcount(board.queensWhite());

    blackScore += 100 * popcount(board.pawnsBlack());
    blackScore += 320 * popcount(board.knightsBlack());
    blackScore += 330 * popcount(board.bishopsBlack());
    blackScore += 500 * popcount(board.rooksBlack());
    blackScore += 900 * popcount(board.queensBlack());

    // Small but useful bonus.
    if (popcount(board.bishopsWhite()) >= 2) whiteScore += 30;
    if (popcount(board.bishopsBlack()) >= 2) blackScore += 30;

    return whiteScore - blackScore;
}

static int evaluatePieceSquares(const Board& board)
{
    int score = 0;

    score += evalPSTFor(board.pawnsWhite(),   pawnPST,       true);
    score += evalPSTFor(board.knightsWhite(), knightPST,     true);
    score += evalPSTFor(board.bishopsWhite(), bishopPST,     true);
    score += evalPSTFor(board.rooksWhite(),   rookPST,       true);
    score += evalPSTFor(board.queensWhite(),  queenPST,      true);
    score += evalPSTFor(board.kingWhite(),   kingMiddlePST, true);

    score -= evalPSTFor(board.pawnsBlack(),   pawnPST,       false);
    score -= evalPSTFor(board.knightsBlack(), knightPST,     false);
    score -= evalPSTFor(board.bishopsBlack(), bishopPST,     false);
    score -= evalPSTFor(board.rooksBlack(),   rookPST,       false);
    score -= evalPSTFor(board.queensBlack(),  queenPST,      false);
    score -= evalPSTFor(board.kingBlack(),   kingMiddlePST, false);

    return score;
}

static int evaluateDevelopment(const Board& board)
{
    int score = 0;

    constexpr int undevelopedMinorPenalty = 8;
    constexpr int earlyQueenPenalty = 3;

    if (board.knightsWhite() & BB(B1)) score -= undevelopedMinorPenalty;
    if (board.knightsWhite() & BB(G1)) score -= undevelopedMinorPenalty;
    if (board.bishopsWhite() & BB(C1)) score -= undevelopedMinorPenalty;
    if (board.bishopsWhite() & BB(F1)) score -= undevelopedMinorPenalty;

    if (board.knightsBlack() & BB(B8)) score += undevelopedMinorPenalty;
    if (board.knightsBlack() & BB(G8)) score += undevelopedMinorPenalty;
    if (board.bishopsBlack() & BB(C8)) score += undevelopedMinorPenalty;
    if (board.bishopsBlack() & BB(F8)) score += undevelopedMinorPenalty;

    if (!(board.queensWhite() & BB(D1))) score -= earlyQueenPenalty;
    if (!(board.queensBlack() & BB(D8))) score += earlyQueenPenalty;

    return score;
}

static int pawnShieldScore(uint64_t pawns, int kingSq, bool white)
{
    int score = 0;

    int kFile = fileOf(kingSq);
    int kRank = rankOf(kingSq);

    for (int df = -1; df <= 1; ++df) {
        int f = kFile + df;
        if (f < 0 || f > 7) continue;

        int shieldRank = white ? kRank + 1 : kRank - 1;
        if (shieldRank < 0 || shieldRank > 7) continue;

        int shieldSq = shieldRank * 8 + f;

        if (pawns & BB(shieldSq)) {
            score += 10;
        } else {
            score -= 8;
        }
    }

    return score;
}

static bool kingNeedsPawnShield(int kingSq, bool white)
{
    int r = rankOf(kingSq);
    int f = fileOf(kingSq);

    if (white) {
        // White king on first rank and near queenside/kingside castled area.
        return r == 0 && (f <= 2 || f >= 5);
    } else {
        // Black king on eighth rank and near queenside/kingside castled area.
        return r == 7 && (f <= 2 || f >= 5);
    }
}

static int evaluateKingSafety(const Board& board)
{
    int score = 0;

    int whiteKingSq = board.kingSquare(Board::WHITE);
    int blackKingSq = board.kingSquare(Board::BLACK);

    if (kingNeedsPawnShield(whiteKingSq, true)) {
        score += pawnShieldScore(board.pawnsWhite(), whiteKingSq, true);
    }

    if (kingNeedsPawnShield(blackKingSq, false)) {
        score -= pawnShieldScore(board.pawnsBlack(), blackKingSq, false);
    }

    int whiteFile = fileOf(whiteKingSq);
    int blackFile = fileOf(blackKingSq);

    if (whiteFile >= 2 && whiteFile <= 5) score -= 8;
    if (blackFile >= 2 && blackFile <= 5) score += 8;

    return score;
}

static bool isWhitePassedPawn(int sq, uint64_t blackPawns)
{
    int file = fileOf(sq);
    int rank = rankOf(sq);

    for (int f = file - 1; f <= file + 1; ++f) {
        if (f < 0 || f > 7) continue;

        for (int r = rank + 1; r <= 7; ++r) {
            int checkSq = r * 8 + f;
            if (blackPawns & BB(checkSq)) {
                return false;
            }
        }
    }

    return true;
}

static bool isBlackPassedPawn(int sq, uint64_t whitePawns)
{
    int file = fileOf(sq);
    int rank = rankOf(sq);

    for (int f = file - 1; f <= file + 1; ++f) {
        if (f < 0 || f > 7) continue;

        for (int r = rank - 1; r >= 0; --r) {
            int checkSq = r * 8 + f;
            if (whitePawns & BB(checkSq)) {
                return false;
            }
        }
    }

    return true;
}

static int evaluatePassedPawns(const Board& board)
{
    int score = 0;

    uint64_t whitePawns = board.pawnsWhite();
    uint64_t blackPawns = board.pawnsBlack();

    uint64_t wp = whitePawns;
    while (wp) {
        int sq = pop_lsb(wp);

        if (isWhitePassedPawn(sq, blackPawns)) {
            int rank = rankOf(sq);
            score += 20 + rank * rank * 5;
        }
    }

    uint64_t bp = blackPawns;
    while (bp) {
        int sq = pop_lsb(bp);

        if (isBlackPassedPawn(sq, whitePawns)) {
            int rank = rankOf(sq);
            int advancement = 7 - rank;
            score -= 20 + advancement * advancement * 5;
        }
    }

    return score;
}

static int evaluatePawnStructure(const Board& board)
{
    int score = 0;

    uint64_t whitePawns = board.pawnsWhite();
    uint64_t blackPawns = board.pawnsBlack();

    constexpr uint64_t files[8] = {
        FILE_A, FILE_B, FILE_C, FILE_D,
        FILE_E, FILE_F, FILE_G, FILE_H
    };

    for (int f = 0; f < 8; ++f) {
        uint64_t whiteOnFile = whitePawns & files[f];
        uint64_t blackOnFile = blackPawns & files[f];

        int whiteCount = popcount(whiteOnFile);
        int blackCount = popcount(blackOnFile);

        // Doubled pawns.
        if (whiteCount > 1) score -= 12 * (whiteCount - 1);
        if (blackCount > 1) score += 12 * (blackCount - 1);

        // Isolated pawns.
        uint64_t adjacentFiles = 0ULL;
        if (f > 0) adjacentFiles |= files[f - 1];
        if (f < 7) adjacentFiles |= files[f + 1];

        if (whiteCount > 0 && !(whitePawns & adjacentFiles)) {
            score -= 10 * whiteCount;
        }

        if (blackCount > 0 && !(blackPawns & adjacentFiles)) {
            score += 10 * blackCount;
        }

        // Open/semi-open rook files.
        // A rook on a file with no friendly pawns is usually happier.
        if (whiteCount == 0 && (board.rooksWhite() & files[f])) {
            score += blackCount == 0 ? 15 : 8;
        }

        if (blackCount == 0 && (board.rooksBlack() & files[f])) {
            score -= whiteCount == 0 ? 15 : 8;
        }
    }

    return score;
}

int evaluate(const Board& board)
{
    int score = 0;

    score += evaluateMaterial(board);
    score += evaluatePieceSquares(board);
    score += evaluateDevelopment(board);
    score += evaluateKingSafety(board);
    score += evaluatePawnStructure(board);
    score += evaluatePassedPawns(board);

    return board.side == Board::WHITE ? score : -score;
}