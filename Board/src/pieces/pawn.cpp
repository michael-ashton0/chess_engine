#include "pawn.h"

// en passant TBAAAA
// promotions are here but not sure if optimal
void PawnMoveGen::generateWhite(Board& board,
                              std::vector<Move>& moves,
                              uint64_t enPassantSquare) 
{
    uint64_t pawns      = board.pawnsWhite();
    uint64_t enemies    = board.occBlack();

    while (pawns) {
        int sq = pop_lsb(pawns);

        uint64_t candidates = movePawn(board, sq, true);

        while (candidates) {
            int toSq = pop_lsb(candidates);
            Move candidate;
            candidate.from  = sq;
            candidate.to    = toSq; 
            
            candidate.isCapture = (enemies & (1ULL << toSq)) != 0;
            candidate.piece     = PAWN;
            // gotta be a better way
            if (rankOf(toSq) == RANK_8) {
                candidate.isPromotion = true;
                candidate.promotionPiece = KNIGHT;
                moves.push_back(candidate);
                candidate.promotionPiece = BISHOP;
                moves.push_back(candidate);
                candidate.promotionPiece = ROOK;
                moves.push_back(candidate);
                candidate.promotionPiece = QUEEN;
                moves.push_back(candidate);
            } else {
                moves.push_back(candidate);
            }
        }
    }
}

void PawnMoveGen::generateBlack(Board& board,
                                std::vector<Move>& moves,
                                uint64_t enPassantSquare) {

    uint64_t pawns      = board.pawnsBlack();
    uint64_t enemies    = board.occWhite();

    while (pawns) {
        int sq = pop_lsb(pawns);

        uint64_t candidates = movePawn(board, sq, false);

        while (candidates) {
            int toSq = pop_lsb(candidates);
            
            Move candidate;
            candidate.from  = sq;
            candidate.to    = toSq;
            
            candidate.isCapture = (enemies & (1ULL << toSq)) != 0;
            if (candidate.isCapture) {
                uint64_t toMask = 1ULL << toSq;

                for (int i = 0; i < 7; ++i) {
                    if (board.pieceBitboards[i] & toMask) {
                        candidate.capturedPiece = static_cast<PieceType>(i);
                        break;
                    }
                }
            }
            candidate.piece     = PAWN;
            if (rankOf(toSq) == RANK_1) {
                candidate.isPromotion = true;
                candidate.promotionPiece = KNIGHT;
                moves.push_back(candidate);
                candidate.promotionPiece = BISHOP;
                moves.push_back(candidate);
                candidate.promotionPiece = ROOK;
                moves.push_back(candidate);
                candidate.promotionPiece = QUEEN;
                moves.push_back(candidate);
            } else {
                moves.push_back(candidate);
            }
        }
    }
}

uint64_t PawnMoveGen::movePawn(Board& board, int sq, bool color) {
    uint64_t attacks    = 0ULL;
    uint64_t moves      = 0ULL;
    uint64_t occupied   = board.occupied();
    uint64_t empty      = ~occupied;
    uint64_t enemy      = color ? board.occBlack() : board.occWhite();

    int rank = rankOf(sq);
    uint64_t bb = 1ULL << sq;

    if (color) {
        //single move
        uint64_t first = north(bb) & empty;
        moves |= first;

        //double move
        if (rank == 2 && first) {
            uint64_t second = north(first) & empty;
            moves |= second;
            board.enPassantSq = first;
        }

        //capture
        uint64_t leftCapture    = nw(bb);
        uint64_t rightCapture   = ne(bb);

        moves |= (leftCapture   & (enemy | board.enPassantSq));
        moves |= (rightCapture  & (enemy | board.enPassantSq));

    } else {
        uint64_t first = south(bb) & empty;
        moves |= first;

        if (rank == 7 && first) {
            uint64_t second = south(first) & empty;
            moves |= second;
            board.enPassantSq = first;
        }

        uint64_t leftCapture    = sw(bb);
        uint64_t rightCapture   = se(bb);

        moves |= (leftCapture   & (enemy | board.enPassantSq));
        moves |= (rightCapture  & (enemy | board.enPassantSq));
    }

    return moves;
}