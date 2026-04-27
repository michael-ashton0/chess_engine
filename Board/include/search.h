#pragma once

#include "board.h"
#include "move.h"

std::vector<Move> generateLegalMoves(Board& board);
Move findBestMove(Board& board, int depth);