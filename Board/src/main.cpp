#include <iostream>
#include "board/board.cpp"

int main() {
    Board br;
    br.setup();
    br.printBoard();
    Move move;
    // 1. e2 -> e4
    move.from = 12; move.to = 28; 
    move.piece = PAWN; br.makeMove(move);
    br.printBoard();
    // ... e7 -> e5
    move.from = 52; move.to = 36;
    move.piece = PAWN; br.makeMove(move);
    br.printBoard();
    // 2. Bf1 -> c4
    move.from = 5;  move.to = 26;
    move.piece = BISHOP; br.makeMove(move);
    br.printBoard();
    // ... Nb8 -> c6
    move.from = 57; move.to = 42;
    move.piece = KNIGHT; br.makeMove(move);
    br.printBoard();
    // 3. Qd1 -> h5
    move.from = 3;  move.to = 39;
    move.piece = QUEEN; br.makeMove(move);
    br.printBoard();
    // ... Ng8 -> f6
    move.from = 62; move.to = 45;
    move.piece = KNIGHT; br.makeMove(move);
    br.printBoard();
    // 4. Qh5 -> f7 (mate)
    move.from = 39; move.to = 53;
    move.piece = QUEEN; br.makeMove(move);
    br.printBoard(); 
    std::string fen = br.exportFen();
    std::cout << '\n';
    std::cout << fen;
    return 0;
}
