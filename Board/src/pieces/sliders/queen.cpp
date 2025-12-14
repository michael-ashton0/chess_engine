#include "queen.h"

// Combines the other sliders, maybe not most optimal
// but will switch to magic bbs so not relevant for now
void QueenMoveGen::generateWhite(Board& board,
                                std::vector<Move>& moves) 
    {
        uint64_t queens     = board.queensWhite();
        uint64_t pieces     = board.occupied();
        uint64_t friends    = board.occWhite();
        uint64_t enemies    = board.occBlack();

        while (queens) {
            int sq = pop_lsb(queens);

            uint64_t attacks = nonMagicQueenAttacks(sq, pieces);
            uint64_t attackable = attacks & ~friends;

            uint64_t bb = attackable;

            while (bb) {
                int toSq = pop_lsb(bb);

                Move candidate;
                candidate.from = sq;
                candidate.to = toSq;

                candidate.isCapture = (enemies & (1ULL << toSq)) != 0;
                candidate.piece = QUEEN;
                moves.push_back(candidate);
            }
        }
};

void QueenMoveGen::generateBlack(Board& board,
                                std::vector<Move>& moves) 
    {
        uint64_t queens     = board.queensBlack();
        uint64_t pieces     = board.occupied();
        uint64_t friends    = board.occBlack();
        uint64_t enemies    = board.occWhite();

        while (queens) {
            int sq = pop_lsb(queens);

            uint64_t attacks = nonMagicQueenAttacks(sq, pieces);
            uint64_t attackable = attacks & ~friends;

            uint64_t bb = attackable;

            while (bb) {
                int toSq = pop_lsb(bb);

                Move candidate;
                candidate.from = sq;
                candidate.to = toSq;

                candidate.isCapture = (enemies & (1ULL << toSq)) != 0;
                candidate.piece = QUEEN;
                moves.push_back(candidate);
            }
    }
};

uint64_t QueenMoveGen::nonMagicQueenAttacks(int sq, uint64_t occupied) {

    uint64_t attacks = 0ULL;

    uint64_t bb = 1ULL << sq;

    // rook moves
    uint64_t ray = bb;
    while (true) {
        ray = north(ray);   
        if (ray == 0) break;
        attacks |= ray;              
        if (occupied & ray) break;
    }

    uint64_t ray = bb;
    while (true) {
        ray = south(ray);   
        if (ray == 0) break;
        attacks |= ray;              
        if (occupied & ray) break;
    }

    uint64_t ray = bb;
    while (true) {
        ray = east(ray);   
        if (ray == 0) break;
        attacks |= ray;              
        if (occupied & ray) break;
    }

    uint64_t ray = bb;
    while (true) {
        ray = west(ray);   
        if (ray == 0) break;
        attacks |= ray;              
        if (occupied & ray) break;
    }

    // bishop moves
    uint64_t ray = bb;
    while (true) {
        ray = ne(ray);   
        if (ray == 0) break;
        attacks |= ray;              
        if (occupied & ray) break;
    }

    while(true) {
        ray = nw(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    while(true) {
        ray = se(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    while(true) {
        ray = sw(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    return attacks;
}
