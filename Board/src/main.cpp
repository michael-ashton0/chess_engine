#include <iostream>
#include "board.h"
#include "perft.h"
#include "knight.h"
#include "king.h"
#include "nps.h"

int main()
{
    KnightMoveGen::initKnightMoves();
    KingMoveGen::initKingMoves();

    printBitboardMoves("knight a3", KnightMoveGen::knightMoves[A3]);
    printBitboardMoves("knight h3", KnightMoveGen::knightMoves[H3]);

    Board board;
    board.setup();
    board.update();

    for (int depth = 1; depth <= 6; depth++) {
        NpsCounter::start();

        uint64_t nodes = perft(board, depth);

        NpsCounter::addNodes(nodes);

        std::cout << "perft(" << depth << ") = "
                  << nodes
                  << " | nps = "
                  << NpsCounter::nodesPerSecond()
                  << std::endl;
    }

    positionsByMove(board, 6);
}