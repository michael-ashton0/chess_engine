#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "uci.h"
#include "board.h"
#include "perft.h"
#include "knight.h"
#include "king.h"

static std::vector<Move> generateLegalMoves(Board& board)
{
    std::vector<Move> pseudo;
    std::vector<Move> legal;

    generateMoves(board, pseudo);

    Board::Side us = board.side;

    for (const Move& move : pseudo) {
        Board next = board;
        next.makeMove(move);

        int kingSq = next.kingSquare(us);

        if (!next.isAttacked(kingSq, next.side)) {
            legal.push_back(move);
        }
    }

    return legal;
}

static bool parseMove(Board& board, const std::string& text, Move& out)
{
    std::vector<Move> legalMoves = generateLegalMoves(board);

    for (const Move& move : legalMoves) {
        if (moveName(move) == text) {
            out = move;
            return true;
        }
    }

    return false;
}

static void setStartPosition(Board& board)
{
    board.setup();
}

static void handlePosition(Board& board, std::istringstream& iss)
{
    std::string token;
    iss >> token;

    if (token == "startpos") {
        board.setup();

        if (iss >> token && token == "moves") {
            while (iss >> token) {
                Move move;
                if (parseMove(board, token, move)) {
                    board.makeMove(move);
                } else {
                    std::cerr << "Illegal or unknown move: " << token << std::endl;
                    return;
                }
            }
        }

        return;
    }

    if (token == "fen") {
        std::string fenFields[6];

        for (int i = 0; i < 6; i++) {
            if (!(iss >> fenFields[i])) {
                return;
            }
        }

        std::string fen =
            fenFields[0] + " " +
            fenFields[1] + " " +
            fenFields[2] + " " +
            fenFields[3] + " " +
            fenFields[4] + " " +
            fenFields[5];

        board.importFen(fen);

        if (iss >> token && token == "moves") {
            while (iss >> token) {
                Move move;
                if (parseMove(board, token, move)) {
                    board.makeMove(move);
                } else {
                    std::cerr << "Illegal or unknown move: " << token << std::endl;
                    return;
                }
            }
        }

        return;
    }
}

static Move chooseMove(Board& board)
{
    std::vector<Move> legalMoves = generateLegalMoves(board);

    if (legalMoves.empty()) {
        Move none;
        return none;
    }

    // Temporary: first legal move.
    // Later this becomes search(board, depth/time).
    return legalMoves[0];
}

static void handleGo(Board& board, std::istringstream& iss)
{
    int depth = 3;

    std::string token;
    while (iss >> token) {
        if (token == "depth") {
            iss >> depth;
        }
    }

    std::vector<Move> legalMoves = generateLegalMoves(board);

    if (legalMoves.empty()) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    Move best = findBestMove(board, depth);

    std::cout << "bestmove " << moveName(best) << std::endl;
}

static void handlePerft(Board& board, std::istringstream& iss)
{
    int depth = 1;
    iss >> depth;

    positionsByMove(board, depth);

    uint64_t nodes = perft(board, depth);
    std::cout << "nodes " << nodes << std::endl;
}

void uciLoop()
{
    Board board;
    setStartPosition(board);

    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);

        std::string command;
        iss >> command;

        if (command == "uci") {
            std::cout << "id name 1ply" << std::endl;
            std::cout << "id author Michael" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (command == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (command == "ucinewgame") {
            setStartPosition(board);
        }
        else if (command == "position") {
            handlePosition(board, iss);
        }
        else if (command == "go") {
            handleGo(board, iss);
        }
        else if (command == "d") {
            board.printBoard();
            std::cout << board.exportFen() << std::endl;
        }
        else if (command == "perft") {
            handlePerft(board, iss);
        }
        else if (command == "quit") {
            break;
        }
    }
}