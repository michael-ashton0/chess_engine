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
        board.makeMove(move);

        int kingSq = board.kingSquare(us);

        // only count moves that do not leave your own king in check
        if (!board.isAttacked(kingSq, board.side))
        {
            nodes += perft(board, depth - 1);
        }

        board.unmakeMove(move);
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