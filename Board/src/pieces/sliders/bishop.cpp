#include "bishop.h"

//diagonal rook logic

void BishopMoveGen::generateWhite(Board &board,
                                  std::vector<Move> &moves) 
    {
        uint64_t bishops    = board.bishopsWhite();
        uint64_t pieces     = board.occupied();
        uint64_t friends    = board.occWhite();
        uint64_t enemies    = board.occBlack();

        while (bishops) {
            int sq = pop_lsb(bishops);

            uint64_t attacks = nonMagicBishopAttacks(sq, pieces);
            uint64_t attackable = attacks & ~friends;

            uint64_t bb = attackable;

            while (bb) {
                int toSq = pop_lsb(bb);

                Move candidate;
                candidate.from = sq;
                candidate.to = toSq;
                
                candidate.isCapture = (enemies & (1ULL << toSq)) != 0;
                candidate.piece = BISHOP;
                moves.push_back(candidate);
            }
        }
};

void BishopMoveGen::generateBlack(Board &board,
                                  std::vector<Move> &moves) 
    {
        uint64_t bishops    = board.bishopsBlack();
        uint64_t pieces     = board.occupied();
        uint64_t friends    = board.occBlack();
        uint64_t enemies    = board.occWhite();

        while (bishops) {
            int sq = pop_lsb(bishops);

            uint64_t attacks = nonMagicBishopAttacks(sq, pieces);
            uint64_t attackable = attacks & ~friends;

            uint64_t bb = attackable;

            while (bb) {
                int toSq = pop_lsb(bb);

                Move candidate;
                candidate.from = sq;
                candidate.to = toSq;

                candidate.isCapture = (enemies & (1ULL << toSq)) != 0;
                candidate.piece = BISHOP;
                moves.push_back(candidate);
            }
        }
};

uint64_t BishopMoveGen::nonMagicBishopAttacks(int sq, uint64_t occupied)
{

    uint64_t attacks = 0ULL;

    uint64_t bb = 1ULL << sq;

    // northeast
    uint64_t ray = bb;
    while (true) {
        // step in direction until blocked or off board
        ray = ne(ray);   
        if (ray == 0) break;
        attacks |= ray;              
        if (occupied & ray) break;
    }

    // northwest
    while(true) {
        ray = nw(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    // southeast
    while(true) {
        ray = se(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    // southwest
    while(true) {
        ray = sw(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    return attacks;
}