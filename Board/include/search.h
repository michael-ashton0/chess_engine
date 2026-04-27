#pragma once

#include <vector>

#include "board.h"
#include "move.h"

int scoreMove(const Move& move);

void clearSearchTables();

Move findBestMoveIterativeTimed(Board& board, int maxDepth, int timeMs);