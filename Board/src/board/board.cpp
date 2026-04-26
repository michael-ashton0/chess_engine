#include <iostream>
#include "board.h"
#include "bitboard.h"
#include "pieces.h"

// TODO::
//  Is it a good idea to add rank checkers? 
//      ie return a bitboard that represents each of the pieces in rank 5 or col 2 [done]
//  Overall give more information available on board obj
//  Check checker for legality and castling concerns [done]

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

bool Board::pieceTypeAtSquare(Side s, int sq, PieceType& out) const {
    uint64_t mask = 1ULL << sq;
    int offset = (s == WHITE) ? 0 : 6;

    for (int p = 0; p < 6; ++p) {
        if (pieceBitboards[offset + p] & mask) {
            out = static_cast<PieceType>(p);
            return true;
        }
    }
    return false;
}

bool Board::isAttacked(int sq, Side attacker) const {
    uint64_t target = 1ULL << sq;
    uint64_t occupied = universal;

    // case Pawn
    if (attacker == WHITE) {
        uint64_t whitePawnAttacks =
            ne(pieceBitboards[pawn_w]) | nw(pieceBitboards[pawn_w]);
        if (whitePawnAttacks & target) return true;
    } else {
        uint64_t blackPawnAttacks =
            se(pieceBitboards[pawn_b]) | sw(pieceBitboards[pawn_b]);
        if (blackPawnAttacks & target) return true;
    }

    // case Knight
    uint64_t knights = (attacker == WHITE) ? pieceBitboards[knight_w]
                                           : pieceBitboards[knight_b];
    while (knights) {
        int from = pop_lsb(knights);
        if (KnightMoveGen::knightMoves[from] & target) return true;
    }

    // case King
    uint64_t king = (attacker == WHITE) ? pieceBitboards[king_w]
                                        : pieceBitboards[king_b];
    if (king) {
        int from = pop_lsb(king);
        if (KingMoveGen::kingMoves[from] & target) return true;
    }

    // case horizontal slider
    uint64_t rookers = (attacker == WHITE)
        ? (pieceBitboards[rook_w] | pieceBitboards[queen_w])
        : (pieceBitboards[rook_b] | pieceBitboards[queen_b]);

    uint64_t tmp = rookers;
    while (tmp) {
        int from = pop_lsb(tmp);
        if (RookMoveGen::nonMagicRookAttacks(from, occupied) & target) return true;
    }

    // case diagonal slider
    uint64_t bishops = (attacker == WHITE)
        ? (pieceBitboards[bishop_w] | pieceBitboards[queen_w])
        : (pieceBitboards[bishop_b] | pieceBitboards[queen_b]);

    tmp = bishops;
    while (tmp) {
        int from = pop_lsb(tmp);
        if (BishopMoveGen::nonMagicBishopAttacks(from, occupied) & target) return true;
    }

    return false;
}

int Board::kingSquare(Side s) const
{
    uint64_t king = (s == WHITE)
        ? pieceBitboards[king_w]
        : pieceBitboards[king_b];

    return __builtin_ctzll(king);
}

void Board::removeEnemyPieceAt(int sq, Side enemy) {
    // mask is a bitboard with just that one square set
    // this could be slow? alternative is knowing what kind of piece you are capturing with each move
    // which honestly could be better on the evaluation side? need to look at this when im not so sleepy
    uint64_t mask = 1ULL << sq;

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

void Board::makeMove(const Move& move) {
    //heavily audited by chat
    Undo undo;
    undo.prevEnPassantSq = enPassantSq;
    undo.prevSide = side;

    Side us     = side;
    Side them   = (side == WHITE ? BLACK : WHITE);

    uint64_t fromMask   = 1ULL << move.from;
    uint64_t toMask     = 1ULL << move.to;

    enPassantSq = 0ULL;

    auto idx = [&](PieceType p, Side s) -> int {
        return (s == WHITE) ? (int)p : (int)p + 6;
    };
    
    undo.prevWhiteCastleKingside    = whiteCastleKingside;
    undo.prevWhiteCastleQueenside   = whiteCastleQueenside;
    undo.prevBlackCastleKingside    = blackCastleKingside;
    undo.prevBlackCastleQueenside   = blackCastleQueenside;

    if (move.isCapture) {
        undo.wasCapture = true;
        if (move.isEnPassant) {
            int captureSq           = (us == WHITE) ? (move.to - 8) : (move.to + 8);
            uint64_t captureMask    = 1ULL << captureSq;
            
            pieceBitboards[idx(PAWN, them)] ^= captureMask;
            
            undo.capturedSq     = captureSq;
            undo.capturedPiece  = PAWN;
        }

        else {
            removeEnemyPieceAt(move.to, them);
            undo.capturedSq     = move.to;
            undo.capturedPiece  = move.capturedPiece;
        }
    }

    if (move.isPromotion) {
        pieceBitboards[idx(PAWN, us)]                   ^= fromMask;
        pieceBitboards[idx(move.promotionPiece, us)]    |= toMask;

        undo.wasPromotion   = true;
        undo.promotionPiece = move.promotionPiece;
    }
    else {
        int pieceIdx = idx(move.piece, us);
        pieceBitboards[pieceIdx] ^= fromMask;
        pieceBitboards[pieceIdx] |= toMask;
    }
    if (move.piece == PAWN && move.isDoublePush) {
        int epSq = (us == WHITE) ? (move.from + 8) : (move.from - 8);
        enPassantSq = 1ULL << epSq;
    }

    if (move.piece == KING) {
        if (us == WHITE) {
            whiteCastleKingside = false;
            whiteCastleQueenside = false;
        } else {
            blackCastleKingside = false;
            blackCastleQueenside = false;
        }
    }

    if (move.piece == ROOK) {
        if (us == WHITE) {
            if (move.from == A1) whiteCastleQueenside = false;
            if (move.from == H1) whiteCastleKingside = false;
        } else {
            if (move.from == A8) blackCastleQueenside = false;
            if (move.from == H8) blackCastleKingside = false;
        }
    }

    if (move.isCapture && move.capturedPiece == ROOK) {
        if (them == WHITE) {
            if (move.to == A1) whiteCastleQueenside = false;
            if (move.to == H1) whiteCastleKingside = false;
        } else {
            if (move.to == A8) blackCastleQueenside = false;
            if (move.to == H8) blackCastleKingside = false;
        }
    }

    if (move.isCastle) {
        if (us == WHITE) {
            if (move.isKingsideCastle) {
                pieceBitboards[rook_w] ^= (1ULL << H1);
                pieceBitboards[rook_w] |= (1ULL << F1);
            } else {
                pieceBitboards[rook_w] ^= (1ULL << A1);
                pieceBitboards[rook_w] |= (1ULL << D1);
            }
        } else {
            if (move.isKingsideCastle) {
                pieceBitboards[rook_b] ^= (1ULL << H8);
                pieceBitboards[rook_b] |= (1ULL << F8);
            } else {
                pieceBitboards[rook_b] ^= (1ULL << A8);
                pieceBitboards[rook_b] |= (1ULL << D8);
            }
        }
    }

    history.push_back(undo);

    side = them;
    update();
};

void Board::unmakeMove(const Move& move) {
    Undo u = history.back();
    history.pop_back();

    Side us   = u.prevSide;
    Side them = (us == WHITE ? BLACK : WHITE);

    auto idx = [&](PieceType p, Side s) -> int {
        return (s == WHITE) ? (int)p : (int)p + 6;
    };

    uint64_t fromMask = 1ULL << move.from;
    uint64_t toMask   = 1ULL << move.to;

    side = us;
    enPassantSq = u.prevEnPassantSq;

    if (u.wasPromotion) {
        pieceBitboards[idx(u.promotionPiece, us)] ^= toMask;
        pieceBitboards[idx(PAWN, us)] |= fromMask;
    } else {
        int pIndex = idx(move.piece, us);
        pieceBitboards[pIndex] ^= toMask;
        pieceBitboards[pIndex] |= fromMask;
    }

    if (u.wasCapture) {
        uint64_t capMask = 1ULL << u.capturedSq;
        pieceBitboards[idx(u.capturedPiece, them)] |= capMask;
    }

    if (move.isCastle) {
        if (us == WHITE) {
            if (move.isKingsideCastle) {
                // rook F1 -> H1
                pieceBitboards[rook_w] ^= (1ULL << F1);
                pieceBitboards[rook_w] |= (1ULL << H1);
            } else {
                // rook D1 -> A1
                pieceBitboards[rook_w] ^= (1ULL << D1);
                pieceBitboards[rook_w] |= (1ULL << A1);
            }
        } else {
            if (move.isKingsideCastle) {
                // rook F8 -> H8
                pieceBitboards[rook_b] ^= (1ULL << F8);
                pieceBitboards[rook_b] |= (1ULL << H8);
            } else {
                // rook D8 -> A8
                pieceBitboards[rook_b] ^= (1ULL << D8);
                pieceBitboards[rook_b] |= (1ULL << A8);
            }
        }
    }

    whiteCastleKingside  = u.prevWhiteCastleKingside;
    whiteCastleQueenside = u.prevWhiteCastleQueenside;
    blackCastleKingside  = u.prevBlackCastleKingside;
    blackCastleQueenside = u.prevBlackCastleQueenside;

    update();
}

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
    int length = 50;
    for (int i = 0; i < length; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
};

void Board::importFen(std::string fen) {
    return;
}
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
    // Append metadata
    fen += " w - - 0 1";
    return fen;
};

bool Board::canWhiteCastleKingside() {
    return whiteCastleKingside
        && (pieceBitboards[king_w] & (1ULL << E1))
        && (pieceBitboards[rook_w] & (1ULL << H1));
}
bool Board::canWhiteCastleQueenside() {
    return whiteCastleQueenside
        && (pieceBitboards[king_w] & (1ULL << E1))
        && (pieceBitboards[rook_w] & (1ULL << A1));
}

bool Board::canBlackCastleKingside() {
    return blackCastleKingside
        && (pieceBitboards[king_b] & (1ULL << E8))
        && (pieceBitboards[rook_b] & (1ULL << H8));
}
bool Board::canBlackCastleQueenside() {
    return blackCastleQueenside
        && (pieceBitboards[king_b] & (1ULL << E8))
        && (pieceBitboards[rook_b] & (1ULL << A8));
}