#include "king.h"

// corner cases
uint64_t TL_CORNER = FILE_A | RANK_8;
uint64_t TR_CORNER = FILE_H | RANK_8;
uint64_t BL_CORNER = FILE_A | RANK_1;
uint64_t BR_CORNER = FILE_H | RANK_8;

void KingMoveGen::generateWhite(Board& board, std::vector<Move>& moves) {
    uint64_t kings   = board.kingWhite();
    uint64_t enemies = board.occBlack();

    while (kings) {
        int sq = pop_lsb(kings);

        uint64_t currMoves = kingMoves[sq] & ~board.occWhite();

        while (currMoves) {
            int to = pop_lsb(currMoves);
            Move candidate;
            candidate.from = sq;
            candidate.to = to;
            candidate.piece = KING;

            candidate.isCapture = (enemies & (1ULL << to)) != 0;
            if (candidate.isCapture) {
                PieceType cap;
                if (board.pieceTypeAtSquare(board.BLACK, to, cap)) {
                    candidate.capturedPiece = cap;
                }
            }

            moves.push_back(candidate);
        }

        if (sq == E1) {
            generateWhiteCastleMoves(board, moves);
        }
    }
}
    
void KingMoveGen::generateBlack(Board& board,
    std::vector<Move>& moves) {
        uint64_t kings = board.kingBlack();
        uint64_t enemies = board.occWhite();

        while (kings) {
            int sq = pop_lsb(kings);
            
            uint64_t currMoves = kingMoves[sq] & ~board.occBlack();
            
            while (currMoves) {
                int to = pop_lsb(currMoves);
                Move candidate;
                candidate.from  = sq;
                candidate.to    = to;

                candidate.isCapture = (enemies & (1ULL << to)) != 0;
                if (candidate.isCapture) {
                    PieceType cap;
                    if (board.pieceTypeAtSquare(board.WHITE, to, cap)) {
                        candidate.capturedPiece = cap;
                    }
                }
                
                candidate.piece = KING;
                moves.push_back(candidate);
            }

            if (sq == E8){
                generateBlackCastleMoves(board, moves);
            }
        }
    }

void KingMoveGen::generateWhiteCastleMoves(Board& board,
                                           std::vector<Move>& moves)
{
    // kingside
    if (board.canWhiteCastleKingside() &&
        !(board.occupied() & ((1ULL << F1) | (1ULL << G1))) &&
        !board.isAttacked(E1, board.BLACK) &&
        !board.isAttacked(F1, board.BLACK) &&
        !board.isAttacked(G1, board.BLACK))
    {
        Move m;
        m.from = E1;
        m.to = G1;
        m.piece = KING;
        m.isCastle = true;
        m.isKingsideCastle = true;
        moves.push_back(m);
    }

    // queenside
    if (board.canWhiteCastleQueenside() &&
        !(board.occupied() & ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))) &&
        !board.isAttacked(E1, board.BLACK) &&
        !board.isAttacked(D1, board.BLACK) &&
        !board.isAttacked(C1, board.BLACK))
    {
        Move m;
        m.from = E1;
        m.to = C1;
        m.piece = KING;
        m.isCastle = true;
        m.isQueensideCastle = true;
        moves.push_back(m);
    }
}

void KingMoveGen::generateBlackCastleMoves(Board& board,
                                           std::vector<Move>& moves)
{
    // kingside
    if (board.canBlackCastleKingside() &&
        !(board.occupied() & ((1ULL << F8) | (1ULL << G8))) &&
        !board.isAttacked(E8, board.WHITE) &&
        !board.isAttacked(F8, board.WHITE) &&
        !board.isAttacked(G8, board.WHITE))
    {
        Move m;
        m.from = E8;
        m.to = G8;
        m.piece = KING;
        m.isCastle = true;
        m.isKingsideCastle = true;
        moves.push_back(m);
    }

    // queenside
    if (board.canBlackCastleQueenside() &&
        !(board.occupied() & ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))) &&
        !board.isAttacked(E8, board.WHITE) &&
        !board.isAttacked(D8, board.WHITE) &&
        !board.isAttacked(C8, board.WHITE))
    {
        Move m;
        m.from = E8;
        m.to = C8;
        m.piece = KING;
        m.isCastle = true;
        m.isQueensideCastle = true;
        moves.push_back(m);
    }
}

void KingMoveGen::initKingMoves() {
    for (int sq = 0; sq <64; sq++) {
        uint64_t bb = 1ULL << sq;

        uint64_t moves = 0ULL;
        
        // corner cases
        if (!(sq & TL_CORNER)) {
            moves |= ne(bb);
        }
        if (!(sq & TR_CORNER)) {
            moves |= nw(bb);
        }
        if (!(sq & BL_CORNER)) {
            moves |= se(bb);
        }
        if (!(sq & BR_CORNER)) {
            moves |= sw(bb);
        }

        //edge cases
        if (!(sq & FILE_A)) {
            moves |= east(bb);
        }
        if (!(sq & FILE_H)) {
            moves |= west(bb);
        }
        if (!(sq & RANK_1)) {
            moves |= south(bb);
        }
        if (!(sq & RANK_8)) {
            moves |= north(bb);
        }
        kingMoves[sq] = moves;
    }
}
