#pragma once
#include "board.h"

uint64_t perft(Board& board, int depth);
void positionsByMove(Board& board, int depth);