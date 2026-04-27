#include "uci.h"
#include "knight.h"
#include "king.h"
#include "selfPlay.h"

int main()
{
    KnightMoveGen::initKnightMoves();
    KingMoveGen::initKingMoves();

    uciLoop();

    return 0;

    // KnightMoveGen::initKnightMoves();
    // KingMoveGen::initKingMoves();

    // GameResult result = playSelfGame(7, 7, 300);

    // std::cout << "Result: " << result.result
    //           << " in " << result.plies << " plies\n";

    // return 0;
}