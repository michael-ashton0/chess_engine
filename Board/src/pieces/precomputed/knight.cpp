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
                candidate.piece     = KNIGHT;

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
                candidate.piece     = KNIGHT;
                
                moves.push_back(candidate);
            }
        }
    }

void KnightMoveGen::initKnightMoves() {

    
    for (int sq; sq < 64; sq++) {
        uint64_t bb = 1ULL << sq;

        uint64_t moves = 0ULL;
        
        // 2 left, up/down 1
        if (!(sq & LEFT2)) {
            moves |= bb << 6;
            moves |= bb >> 10;
        }
        
        // 2 right, up/down 1
        if (!(sq & RIGHT2)) {
            moves |= bb << 10;
            moves |= bb >> 6;
        }

        // 2 up, right/left 1
        if (!(sq & TOP2)) {
            moves |= bb << 15;
            moves |= bb << 17;
        }
        
        //2 down, right/left 1
        if (!(sq & BOT2)) {
            moves |= bb >> 17;
            moves |= bb >> 15;
        }

        knightMoves[sq] = moves;
    }
}