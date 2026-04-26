#pragma once
#include "inclusions.h"

struct Board;

enum PieceType {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

//why are pawns the hardest
struct Move {
    PieceType piece;
    uint8_t from;   //0-63
    uint8_t to;     //0-63
    bool isCapture = false;
    PieceType capturedPiece;
    
    // Pawn Specific
    // might get scrapped if i can't figure out 
    // passing values to a Move object from movePawn
    // want to keep for en Passant clearing ease of use
    bool isDoublePush   = false;
    bool isEnPassant    = false;
    bool isPromotion    = false;
    PieceType promotionPiece;
    
    // King Specific
    bool isCastle           = false;
    bool isKingsideCastle   = false;
    bool isQueensideCastle  = false;
};

void generateMoves(Board& board, std::vector<Move>& moves);

std::string squareName(int sq);

std::string moveName(const Move& m);