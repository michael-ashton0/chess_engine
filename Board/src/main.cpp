#include "uci.h"
#include "knight.h"
#include "king.h"
#include "selfPlay.h"
#include "zobrist.h"

int main()
{
    Zobrist::init();

    KnightMoveGen::initKnightMoves();
    KingMoveGen::initKingMoves();

    uciLoop();

    return 0;

    // GameResult result = playSelfGame(7, 7, 300);

    // std::cout << "Result: " << result.result
    //           << " in " << result.plies << " plies\n";

    // return 0;
}