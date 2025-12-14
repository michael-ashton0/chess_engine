#include <iostream>
#include "board/board.cpp"
#include "../include/board.h"

int main() {
    Board br;
    br.setup();
    br.printBoard();
    Move move;
    move.to = 38;
    move.from = 13;
    br.makeMove(move);
    br.printBoard();
    std::cout << "plssss";
    std::string fen = br.exportFen();
    std::cout << '\n';
    // std::cout << fen;
    return 0;
}
