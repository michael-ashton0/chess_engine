#pragma once
#include "inclusions.h"
#include "move.h"

struct Board {
    enum Piece {
    pawn_w, knight_w, bishop_w, rook_w, queen_w, king_w,
    pawn_b, knight_b, bishop_b, rook_b, queen_b, king_b
    };
    
    enum Side {
        WHITE, BLACK
    };
    
    struct Undo {
        uint64_t prevEnPassantSq = 0ULL;

        bool wasCapture         = false;
        int capturedSq          = -1;
        PieceType capturedPiece;

        bool wasPromotion           = false;
        PieceType promotionPiece;

        Side prevSide = WHITE;

        bool prevWhiteCastleKingside;
        bool prevWhiteCastleQueenside;
        bool prevBlackCastleKingside;
        bool prevBlackCastleQueenside;
    };

    std::vector<Undo> history;

    std::array<uint64_t, 12> pieceBitboards{};

    bool whKingMoved;
    bool blKingMoved;
   
    bool whQueenRookMoved   = false;
    bool whKingRookMoved    = false;
    bool blQueenRookMoved   = false;
    bool blKingRookMoved    = false;
    
    bool whiteCastleKingside  = true;
    bool whiteCastleQueenside = true;
    bool blackCastleKingside  = true;
    bool blackCastleQueenside = true;
    
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
    bool isAttacked(int sq, Side attacker) const;
    bool pieceTypeAtSquare(Side s, int sq, PieceType& out) const;
    void removeEnemyPieceAt(uint64_t sq, Side enemy);
    void setup();
    void makeMove(const Move& move);
    void unmakeMove(const Move& move);
    void printBoard();
    void importFen(std::string fen);
    std::string exportFen();

    bool canWhiteCastleKingside();
    bool canWhiteCastleQueenside();
    bool canBlackCastleKingside();
    bool canBlackCastleQueenside();

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