#include "selfPlay.h"
#include "sstream"

std::string positionKey(const Board& board)
{
    std::ostringstream out;

    out << board.side << "|";

    for (int i = 0; i < 12; ++i) {
        out << board.pieceBitboards[i] << ",";
    }

    out << "|ep:" << board.enPassantSq;

    // Later include castling rights too:
    // out << "|castle:" << board.castlingRights;

    return out.str();
}

GameResult playSelfGame(int whiteDepth, int blackDepth, int maxPlies)
{
    Board board;
    board.setup();
    board.update();

    int halfmoveClock = 0;

    std::unordered_map<std::string, int> repetitions;

    for (int ply = 0; ply < maxPlies; ++ply) {
        std::string key = positionKey(board);
        repetitions[key]++;

        if (repetitions[key] >= 3) {
            return {0, ply}; // draw by repetition
        }

        int depth = (board.side == Board::WHITE) ? whiteDepth : blackDepth;

        std::vector<Move> legalMoves = generateLegalMoves(board);

        if (legalMoves.empty()) {
            Board::Side sideToMove = board.side;
            Board::Side enemy = sideToMove == Board::WHITE ? Board::BLACK : Board::WHITE;

            int kingSq = board.kingSquare(sideToMove);

            if (board.isAttacked(kingSq, enemy)) {
                if (sideToMove == Board::WHITE) {
                    return {-1, ply};
                } else {
                    return {1, ply};
                }
            }

            return {0, ply};
        }

        Move best = findBestMove(board, depth);

        if (best.isCapture || best.piece == PAWN) {
            halfmoveClock = 0;
        } else {
            halfmoveClock++;
        }

        if (halfmoveClock >= 100) {
            return {0, ply}; // draw by fifty-move rule
        }

        std::cout << ply + 1 << ". "
                  << (board.side == Board::WHITE ? "White " : "Black ")
                  << moveName(best) << "\n";

        board.makeMove(best);
    }

    return {0, maxPlies};
}