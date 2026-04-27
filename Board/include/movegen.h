#pragma once

#include "move.h"
#include "movelist.h"
#include "pieces.h"

void generateMoves(Board& board, MoveList& moves);
void generateLegalMoves(Board& board, MoveList& legalMoves);