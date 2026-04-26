#include "knight.h"

void KnightMoveGen::generateWhite(Board& board,
    std::vector<Move>& moves) {
        uint64_t knights = board.knightsWhite();
        uint64_t enemies = board.occBlack();

        while (knights) {
            int sq = pop_lsb(knights);
            
            uint64_t currMoves = knightMoves[sq] & ~board.occWhite();
            
            while (currMoves) {
                int to = pop_lsb(currMoves);
                Move candidate;
                candidate.from  = sq;
                candidate.to    = to;

                candidate.isCapture = (enemies & (1ULL << to)) != 0;
                if (candidate.isCapture) {
                    PieceType cap;
                    if (board.pieceTypeAtSquare(board.BLACK, to, cap)) {
                        candidate.capturedPiece = cap;
                    }
                }
                candidate.piece = KNIGHT;

                moves.push_back(candidate);
            }
        }
    }
    
void KnightMoveGen::generateBlack(Board& board,
    std::vector<Move>& moves) {
        uint64_t knights = board.knightsBlack();
        uint64_t enemies = board.occWhite();

        while (knights) {
            int sq = pop_lsb(knights);
            
            uint64_t currMoves = knightMoves[sq] & ~board.occBlack();
            
            while (currMoves) {
                int to = pop_lsb(currMoves);
                Move candidate;
                candidate.from  = sq;
                candidate.to    = to;

                candidate.isCapture = (enemies & (1ULL << to)) != 0;
                if (candidate.isCapture) {
                    PieceType cap;
                    if (board.pieceTypeAtSquare(board.WHITE, to, cap)) {
                        candidate.capturedPiece = cap;
                    }
                }
                candidate.piece = KNIGHT;
                moves.push_back(candidate);
            }
        }
    }

void KnightMoveGen::initKnightMoves() {
    for (int sq = 0; sq < 64; sq++) {
        uint64_t bb = 1ULL << sq;
        uint64_t moves = 0ULL;

        if (!(bb & LEFT2)) {
            moves |= bb << 6;
            moves |= bb >> 10;
        }

        if (!(bb & RIGHT2)) {
            moves |= bb >> 10;
            moves |= bb << 6;
        }

        if (!(bb & TOP2)) {
            moves |= bb << 15;
            moves |= bb << 17;
        }

        if (!(bb & BOT2)) {
            moves |= bb >> 17;
            moves |= bb >> 15;
        }

        knightMoves[sq] = moves;
    }
}