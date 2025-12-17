#pragma once
#include "inclusions.h"

struct Board {
    enum Piece {
    pawn_w, knight_w, bishop_w, rook_w, queen_w, king_w,
    pawn_b, knight_b, bishop_b, rook_b, queen_b, king_b
    };
    
    enum Side {
        WHITE, BLACK
    };

    std::array<uint64_t, 12> pieceBitboards{};

    bool whKingMoved;
    bool blKingMoved;
   
    bool whQueenRookMoved;
    bool whKingRookMoved;
    bool blQueenRookMoved;
    bool blKingRookMoved;
    
    bool whKingsideCastle;
    bool whQueensideCastle;
    bool blKingsideCastle;
    bool blQueensideCastle;
    
    uint64_t universal      = 0ULL;
    uint64_t white          = 0ULL;
    uint64_t black          = 0ULL;
    uint64_t enPassantSq    = 0ULL;
    
    int lastPerm = 0;
    Side side = WHITE;

    //square_index = rank * 8 + file
    //constexpr computes at compile time, as opposed to when the program is running
    //which is good for the setup function, but the game will be computed at runtime
    static constexpr int toSquare(int file, int rank) {
        return rank * 8 + file;
    };
    
    void clear();
    void update();
    void place(Piece p, int file, int rank);
    bool isAttacked(uint64_t sq, Side side);
    void removeEnemyPieceAt(uint64_t sq, Side enemy);
    void setup();
    void makeMove(Move move);
    void printBoard();
    void importFen(std::string fen);
    std::string exportFen();
    
    uint64_t occWhite()         const { return white; }
    uint64_t occBlack()         const { return black; }
    uint64_t occupied()         const { return universal; }
    uint64_t empty()            const { return ~universal; }
    uint64_t pawnsWhite()       const { return pieceBitboards[pawn_w]; }
    uint64_t pawnsBlack()       const { return pieceBitboards[pawn_b]; }
    uint64_t rooksWhite()       const { return pieceBitboards[rook_w]; }
    uint64_t rooksBlack()       const { return pieceBitboards[rook_b]; }
    uint64_t bishopsWhite()     const { return pieceBitboards[bishop_w]; }
    uint64_t bishopsBlack()     const { return pieceBitboards[bishop_b]; }
    uint64_t queensWhite()      const { return pieceBitboards[queen_w]; }
    uint64_t queensBlack()      const { return pieceBitboards[queen_b]; }
    uint64_t knightsWhite()     const { return pieceBitboards[knight_w]; }
    uint64_t knightsBlack()     const { return pieceBitboards[knight_b]; }
    uint64_t kingWhite()        const { return pieceBitboards[king_w]; }
    uint64_t kingBlack()        const { return pieceBitboards[king_b]; }
};