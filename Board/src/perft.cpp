#include <cstdint>
#include "perft.h"
#include "move.h"

uint64_t perft(Board& board, int depth)
{
    if (depth == 0)
        return 1;

    std::vector<Move> moves;
    generateMoves(board, moves);

    uint64_t nodes = 0;

    Board::Side us = board.side;

    for (const Move& move : moves)
    {
        Board next = board;
        next.makeMove(move);

        int kingSq = next.kingSquare(us);

        // after makeMove, next.side should be the opponent
        if (!next.isAttacked(kingSq, next.side))
        {
            nodes += perft(next, depth - 1);
        }
    }

    return nodes;
}

void positionsByMove(Board& board, int depth)
{
    std::vector<Move> moves;
    generateMoves(board, moves);

    for (const Move& move : moves)
    {
        Board::Side us = board.side;

        board.makeMove(move);

        int kingSq = board.kingSquare(us);

        if (!board.isAttacked(kingSq, board.side))
        {
            uint64_t nodes = perft(board, depth - 1);
            std::cout << moveName(move) << ": " << nodes << "\n";
        }

        board.unmakeMove(move);
    }
}

std::string squareToString(int sq) {
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);

    std::string out;
    out += file;
    out += rank;
    return out;
}

void printBitboardMoves(const std::string& label, uint64_t moves) {
    std::cout << label << ": ";

    while (moves) {
        int toSq = __builtin_ctzll(moves);
        moves &= moves - 1;

        std::cout << squareToString(toSq) << " ";
    }

    std::cout << std::endl;
}