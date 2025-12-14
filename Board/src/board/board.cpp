#include <iostream>
#include "board.h"

// TODO::
//  Is it a good idea to add rank checkers? 
//      ie return a bitboard that represents each of the pieces in rank 5 or col 2
//  Overall give more information available on board obj
//  Check checker for legality and castling concerns

void Board::clear(){
        lastPerm = 0;
        for (int i=0; i<12; i++) {
            pieceBitboards[i] = 0ULL;
        }
    };

void Board::update(){
    /* 
    Rebuild the board piece by piece
    not sure this is best, but should work for now
    (does performance even matter in the board object??)
    */
    white = 0ULL;
    black = 0ULL;
    for (int i = 0; i < 6; i++) {
        white |= pieceBitboards[i];
        black |= pieceBitboards[i+6];
    }
    universal = white | black;

    };


void Board::place(Piece p, int file, int rank) {
        const int sq = toSquare(file, rank);
        pieceBitboards[p] |= (1ULL << sq);
    };

bool Board::isAttacked(uint64_t sq, Side side) {

}

void Board::removeEnemyPieceAt(uint64_t sq, Side enemy) {
    // mask is a bitboard with just that one square set
    // this could be slow? alternative is knowing what kind of piece you are capturing with each move
    // which honestly could be better on the evaluation side? need to look at this when im not so sleepy
    uint64_t mask = sq;

    if (enemy == WHITE) {
        if (pieceBitboards[pawn_w]   & mask) { pieceBitboards[pawn_w]   ^= mask; return; }
        if (pieceBitboards[knight_w] & mask) { pieceBitboards[knight_w] ^= mask; return; }
        if (pieceBitboards[bishop_w] & mask) { pieceBitboards[bishop_w] ^= mask; return; }
        if (pieceBitboards[rook_w]   & mask) { pieceBitboards[rook_w]   ^= mask; return; }
        if (pieceBitboards[queen_w]  & mask) { pieceBitboards[queen_w]  ^= mask; return; }
        if (pieceBitboards[king_w]   & mask) { pieceBitboards[king_w]   ^= mask; return; }
    } else {
        if (pieceBitboards[pawn_b]   & mask) { pieceBitboards[pawn_b]   ^= mask; return; }
        if (pieceBitboards[knight_b] & mask) { pieceBitboards[knight_b] ^= mask; return; }
        if (pieceBitboards[bishop_b] & mask) { pieceBitboards[bishop_b] ^= mask; return; }
        if (pieceBitboards[rook_b]   & mask) { pieceBitboards[rook_b]   ^= mask; return; }
        if (pieceBitboards[queen_b]  & mask) { pieceBitboards[queen_b]  ^= mask; return; }
        if (pieceBitboards[king_b]   & mask) { pieceBitboards[king_b]   ^= mask; return; }
    }
};

void Board::setup(){
    clear();
    
    //pawns
    for (int file=0; file<8; file++){
        place(pawn_w, file, 1);
        place(pawn_b, file, 6);
    }
    
    //rooks
    place(rook_w, 0,0);
    place(rook_w, 7,0);
    place(rook_b, 0,7);
    place(rook_b, 7,7);
    
    //knights
    place(knight_w, 1,0);
    place(knight_w, 6,0);
    place(knight_b, 1,7);
    place(knight_b, 6,7);
    
    //bishops
    place(bishop_w, 2,0);
    place(bishop_w, 5,0);
    place(bishop_b, 2,7);
    place(bishop_b, 5,7);
    
    // queens
    place(queen_w, 3,0);
    place(queen_b, 3,7);
    
    // kings
    place(king_b, 4,7);
    place(king_w, 4,0);
    };

void Board::makeMove(Move move) {
    // take the move.to, or it with the old board, how to examine specifics?
    // find the corresponding piece bitboard, update it with the move
    // if caputre, update the board of the captured piece too?
    // quiet move should be a simple or
    // does move need an attached piece type?

    Side them = (side == WHITE ? BLACK : WHITE);

    pieceBitboards[move.piece] ^= move.from;
    pieceBitboards[move.piece] |= move.to;

    if (move.isCapture == true) {
        removeEnemyPieceAt(move.to, them);
    };

    side = them;
    update();
    
};

void Board::printBoard() {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            const int sq = toSquare(file, rank);
            const uint64_t mask = (1ULL << sq);
            char c = '.';
            if (pieceBitboards[knight_w]        & mask) c = 'N';
            else if (pieceBitboards[bishop_w]   & mask) c = 'B';
            else if (pieceBitboards[rook_w]     & mask) c = 'R';
            else if (pieceBitboards[queen_w]    & mask) c = 'Q';
            else if (pieceBitboards[king_w]     & mask) c = 'K';
            else if (pieceBitboards[knight_b]   & mask) c = 'n';
            else if (pieceBitboards[bishop_b]   & mask) c = 'b';
            else if (pieceBitboards[rook_b]     & mask) c = 'r';
            else if (pieceBitboards[queen_b]    & mask) c = 'q';
            else if (pieceBitboards[king_b]     & mask) c = 'k';
            else if (pieceBitboards[pawn_w]     & mask) c = 'P';
            else if (pieceBitboards[pawn_b]     & mask) c = 'p';
            
            std::cout << c << ' ';
            // arr[sq] = c;
        }
        std::cout << '\n';
        }
    };

    // Returns a full FEN string. Assumptions: side to move = white, no castling rights,
    // no en-passant, halfmove clock = 0, fullmove number = 1.
    // AI for now...
std::string Board::exportFen() {
    std::string fen;
    for (int rank = 7; rank >= 0; --rank) {
        int empty = 0;
        for (int file = 0; file < 8; ++file) {
            const int sq = toSquare(file, rank);
            const uint64_t mask = (1ULL << sq);

            char c = 0;
            if (pieceBitboards[knight_w]        & mask) c = 'N';
            else if (pieceBitboards[bishop_w]   & mask) c = 'B';
            else if (pieceBitboards[rook_w]     & mask) c = 'R';
            else if (pieceBitboards[queen_w]    & mask) c = 'Q';
            else if (pieceBitboards[king_w]     & mask) c = 'K';
            else if (pieceBitboards[knight_b]   & mask) c = 'n';
            else if (pieceBitboards[bishop_b]   & mask) c = 'b';
            else if (pieceBitboards[rook_b]     & mask) c = 'r';
            else if (pieceBitboards[queen_b]    & mask) c = 'q';
            else if (pieceBitboards[king_b]     & mask) c = 'k';
            else if (pieceBitboards[pawn_w]     & mask) c = 'P';
            else if (pieceBitboards[pawn_b]     & mask) c = 'p';

            if (c == 0) {
                ++empty;
            } else {
                if (empty > 0) {
                    fen.push_back(char('0' + empty));
                    empty = 0;
                }
                fen.push_back(c);
            }
        }
        if (empty > 0) fen.push_back(char('0' + empty));
        if (rank > 0) fen.push_back('/');
    }
    // Append minimal metadata: side to move, castling, en-passant, halfmove, fullmove
    fen += " w - - 0 1";
    return fen;
};