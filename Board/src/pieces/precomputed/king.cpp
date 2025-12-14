#include "king.h"

// corner cases
uint64_t TL_CORNER = FILE_A | RANK_8;
uint64_t TR_CORNER = FILE_H | RANK_8;
uint64_t BL_CORNER = FILE_A | RANK_1;
uint64_t BR_CORNER = FILE_H | RANK_8;

void KingMoveGen::generateWhite(Board& board,
    std::vector<Move>& moves) {
        uint64_t kings      = board.kingWhite();
        uint64_t enemies    = board.occBlack();

        while (kings) {
            int sq = pop_lsb(kings);
    
            uint64_t currMoves = kingMoves[sq] & ~board.occWhite();
            
            while (currMoves) {
                int to = pop_lsb(currMoves);
                Move candidate;
                candidate.from  = sq;
                candidate.to    = to;

                candidate.isCapture = (enemies & (1ULL << to)) != 0;
                candidate.piece     = KING;

                moves.push_back(candidate);
            }
        }
    }
    
void KingMoveGen::generateBlack(Board& board,
    std::vector<Move>& moves) {
        uint64_t kings = board.kingBlack();
        uint64_t enemies = board.occWhite();

        while (kings) {
            int sq = pop_lsb(kings);
            
            uint64_t currMoves = kingMoves[sq] & ~board.occBlack();
            
            while (currMoves) {
                int to = pop_lsb(currMoves);
                Move candidate;
                candidate.from  = sq;
                candidate.to    = to;

                candidate.isCapture = (enemies & (1ULL << to)) != 0;
                candidate.piece     = KING;

                moves.push_back(candidate);
            }
        }
    }

void KingMoveGen::initKingMoves() {
    for (int sq; sq <64; sq++) {
        uint64_t bb = 1ULL << sq;

        uint64_t moves = 0ULL;
        
        // corner cases
        if (!(sq & TL_CORNER)) {
            moves |= ne(bb);
        }
        if (!(sq & TR_CORNER)) {
            moves |= nw(bb);
        }
        if (!(sq & BL_CORNER)) {
            moves |= se(bb);
        }
        if (!(sq & BR_CORNER)) {
            moves |= sw(bb);
        }

        //edge cases
        if (!(sq & FILE_A)) {
            moves |= east(bb);
        }
        if (!(sq & FILE_H)) {
            moves |= west(bb);
        }
        if (!(sq & RANK_1)) {
            moves |= south(bb);
        }
        if (!(sq & RANK_8)) {
            moves |= north(bb);
        }
        kingMoves[sq] = moves;
    }
}
