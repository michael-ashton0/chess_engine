#include "zobrist.h"
#include "bitboard.h"

#include <random>

namespace Zobrist {
    uint64_t piece[12][64];
    uint64_t sideToMove;
    uint64_t enPassant[64];
    uint64_t castling[16];

    static uint64_t randomU64()
    {
        static std::mt19937_64 rng(20260427);
        return rng();
    }

    void init()
    {
        for (int p = 0; p < 12; ++p) {
            for (int sq = 0; sq < 64; ++sq) {
                piece[p][sq] = randomU64();
            }
        }

        sideToMove = randomU64();

        for (int sq = 0; sq < 64; ++sq) {
            enPassant[sq] = randomU64();
        }

        for (int i = 0; i < 16; ++i) {
            castling[i] = randomU64();
        }
    }

    uint64_t computeHash(const Board& board)
    {
        uint64_t hash = 0;

        for (int p = 0; p < 12; ++p) {
            uint64_t bb = board.pieceBitboards[p];

            while (bb) {
                int sq = pop_lsb(bb);
                hash ^= piece[p][sq];
            }
        }

        if (board.side == Board::BLACK) {
            hash ^= sideToMove;
        }

        if (board.enPassantSq != 0ULL) {
            int epSq = __builtin_ctzll(board.enPassantSq);
            hash ^= enPassant[epSq];
        }

        return hash;
    }
}