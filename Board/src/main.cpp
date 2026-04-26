#include "uci.h"
#include "knight.h"
#include "king.h"

int main()
{
    KnightMoveGen::initKnightMoves();
    KingMoveGen::initKingMoves();

    uciLoop();

    return 0;
}