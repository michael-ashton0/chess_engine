#include <iostream>
#include "board.h"
#include "perft.h"
#include "knight.h"
#include "king.h"

int main()
{
    
    KnightMoveGen::initKnightMoves();
    KingMoveGen::initKingMoves();

    Board board;
    board.setup();
    board.update();

    for (int depth = 1; depth <= 5; depth++) {
        std::cout << "perft(" << depth << ") = "
                  << perft(board, depth)
                  << std::endl;
    }

    positionsByMove(board, 3);
}