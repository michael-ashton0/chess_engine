#include "rook.h"

// base slider logic

void RookMoveGen::generateWhite(Board &board,
                                std::vector<Move> &moves)
{
    uint64_t rooks      = board.rooksWhite();
    uint64_t pieces     = board.occupied();
    uint64_t friends    = board.occWhite();
    uint64_t enemies    = board.occBlack();

    while (rooks)
    {
        int sq = pop_lsb(rooks); // chew thru rooks
        
        uint64_t attacks    = nonMagicRookAttacks(sq, pieces);
        uint64_t attackable = attacks & ~friends;

        uint64_t bb = attackable;

        while (bb)
        {
            int toSq = pop_lsb(bb);

            Move candidate;
            candidate.from  = sq;
            candidate.to    = toSq;

            candidate.isCapture = (enemies & (1ULL << toSq)) != 0;
            candidate.piece = ROOK;
            moves.push_back(candidate);
        }
        
        // Shelved Magic Code
        // Move candidate;
        // candidate.from  = sq;
        // int lookupIndex = (pieces & RBlockers[sq]) >> RShift[sq];
        // // Somehow need to look at each possible square in each direction
        // // Def possible with a loop but has to be a better way
        //     // https://www.chessprogramming.org/Magic_Bitboards
        //     // More simply, https://www.chessprogramming.org/Sliding_Piece_Attacks

    }
};

void RookMoveGen::generateBlack(Board &board,
                                std::vector<Move> &moves)
{
    uint64_t rooks      = board.rooksBlack();
    uint64_t pieces     = board.occupied();
    uint64_t friends    = board.occBlack();
    uint64_t enemies    = board.occWhite();

    while (rooks)
    {
        int sq = pop_lsb(rooks);

        uint64_t attacks    = nonMagicRookAttacks(sq, pieces);
        uint64_t attackable = attacks & ~friends;

        uint64_t bb = attackable;

        while (bb)
        {
            int toSq = pop_lsb(bb);

            Move candidate;
            candidate.from  = sq;
            candidate.to    = toSq;

            candidate.isCapture = (enemies & (1ULL << toSq)) != 0;
            candidate.piece = ROOK;
            moves.push_back(candidate);
        }
    }
};

uint64_t RookMoveGen::nonMagicRookAttacks(int sq, uint64_t occupied)
{

    uint64_t attacks = 0ULL;

    uint64_t bb = 1ULL << sq;

    // north
    uint64_t ray = bb;
    while (true) {
        // step in direction until blocked or off board
        ray = north(ray);   
        if (ray == 0) break;
        attacks |= ray;              
        if (occupied & ray) break;
    }

    // south
    while(true) {
        ray = south(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    // east
    while(true) {
        ray = east(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    // west
    while(true) {
        ray = west(ray);
        if (ray == 0) break;
        attacks |= ray;
        if (occupied & ray) break;
    }

    return attacks;
}