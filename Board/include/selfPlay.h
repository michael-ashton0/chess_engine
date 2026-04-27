#pragma once

#include <iostream>
#include <vector>

#include "search.h"
#include "board.h"
#include "move.h"


struct GameResult {
    int result;       // 1 = white win, 0 = draw, -1 = black win
    int plies;
};

GameResult playSelfGame(int whiteDepth, int blackDepth, int maxPlies = 300);