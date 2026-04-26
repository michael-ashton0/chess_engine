#pragma once
#include "board.h"

uint64_t perft(Board& board, int depth);
void positionsByMove(Board& board, int depth);
std::string squareToString(int sq);
void printBitboardMoves(const std::string& label, uint64_t moves);