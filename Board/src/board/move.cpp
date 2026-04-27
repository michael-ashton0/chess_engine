#include "inclusions.h"
#include "pieces.h"
#include "move.h"

std::string squareName(int sq)
{
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);
    return std::string{file, rank};
}

std::string moveName(const Move& m)
{
    std::string out = squareName(m.from) + squareName(m.to);

    if (m.isPromotion) {
        switch (m.promotionPiece) {
            case KNIGHT: out += 'n'; break;
            case BISHOP: out += 'b'; break;
            case ROOK:   out += 'r'; break;
            case QUEEN:  out += 'q'; break;
            default: break;
        }
    }

    return out;
}