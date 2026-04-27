#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "uci.h"
#include "board.h"
#include "perft.h"
#include "knight.h"
#include "king.h"
#include "search.h"
#include "movegen.h"
#include "nps.h"

static bool parseMove(Board& board, const std::string& text, Move& out)
{
    MoveList legalMoves;
    generateLegalMoves(board, legalMoves);

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
    board.update();
}

static void handlePosition(Board& board, std::istringstream& iss)
{
    std::string token;
    iss >> token;

    if (token == "startpos") {
        board.setup();
        board.update();

        if (iss >> token && token == "moves") {
            while (iss >> token) {
                Move move;
                if (parseMove(board, token, move)) {
                    board.makeMove(move);
                } else {
                    std::cerr << "Illegal or unknown move: "
                              << token << std::endl;
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
        board.update();

        if (iss >> token && token == "moves") {
            while (iss >> token) {
                Move move;
                if (parseMove(board, token, move)) {
                    board.makeMove(move);
                } else {
                    std::cerr << "Illegal or unknown move: "
                              << token << std::endl;
                    return;
                }
            }
        }

        return;
    }
}

static int chooseMoveTimeMs(
    Board::Side side,
    int movetime,
    int wtime,
    int btime,
    int winc,
    int binc
) {
    // UCI exact move time.
    if (movetime > 0) {
        // Leave a small safety buffer.
        return std::max(10, movetime - 50);
    }

    int sideTime = (side == Board::WHITE) ? wtime : btime;
    int sideInc  = (side == Board::WHITE) ? winc  : binc;

    // No clock info. This happens with plain "go".
    if (sideTime <= 0) {
        return 1000;
    }

    // Basic time management:
    // Use about 1/30th of remaining clock plus half the increment.
    int budget = sideTime / 30 + sideInc / 2;

    // Do not spend more than 20% of the remaining clock.
    int maxBudget = sideTime / 5;
    budget = std::min(budget, maxBudget);

    // Emergency low-time caps.
    if (sideTime < 1000) {
        budget = std::min(budget, 50);
    } else if (sideTime < 3000) {
        budget = std::min(budget, 150);
    } else if (sideTime < 10000) {
        budget = std::min(budget, 500);
    }

    return std::max(10, budget);
}

static void handleGo(Board& board, std::istringstream& iss)
{
    int depth = 64;
    int movetime = -1;
    int wtime = -1;
    int btime = -1;
    int winc = 0;
    int binc = 0;

    bool depthWasGiven = false;
    bool infinite = false;

    std::string token;
    while (iss >> token) {
        if (token == "depth") {
            iss >> depth;
            depthWasGiven = true;
        } else if (token == "movetime") {
            iss >> movetime;
        } else if (token == "wtime") {
            iss >> wtime;
        } else if (token == "btime") {
            iss >> btime;
        } else if (token == "winc") {
            iss >> winc;
        } else if (token == "binc") {
            iss >> binc;
        } else if (token == "infinite") {
            infinite = true;
        } else if (token == "ponder") {
            // Ignored for now.
        } else if (token == "nodes") {
            // Consume the value and ignore for now.
            int ignored;
            iss >> ignored;
        } else if (token == "mate") {
            // Consume the value and ignore for now.
            int ignored;
            iss >> ignored;
        } else if (token == "searchmoves") {
            // Not supported yet. Ignore the rest of the line.
            break;
        }
    }

    MoveList legalMoves;
    generateLegalMoves(board, legalMoves);

    if (legalMoves.empty()) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    if (legalMoves.size() == 1) {
        std::cout << "bestmove " << moveName(legalMoves[0]) << std::endl;
        return;
    }

    Move best;

    // Manual testing:
    // go depth 5
    //
    // This should complete exactly to that depth.
    if (depthWasGiven && movetime < 0 && wtime < 0 && btime < 0 && !infinite) {
        best = findBestMoveIterativeTimed(board, depth, movetime);
    } else {
        // Real game use:
        // go movetime N
        // go wtime N btime N winc N binc N
        //
        // Iterative deepening searches until time runs out.
        int searchTimeMs = chooseMoveTimeMs(
            board.side,
            movetime,
            wtime,
            btime,
            winc,
            binc
        );

        best = findBestMoveIterativeTimed(board, depth, searchTimeMs);
    }

    std::cout << "bestmove " << moveName(best) << std::endl;
}

static void handlePerft(Board& board, std::istringstream& iss)
{
    int depth = 1;
    iss >> depth;

    // positionsByMove(board, depth);

    NpsCounter::start();

    uint64_t nodes = perft(board, depth);

    double seconds = NpsCounter::elapsedSeconds();
    uint64_t nps = NpsCounter::nodesPerSecond();

    std::cout << "nodes " << nodes << std::endl;
    std::cout << "time " << seconds << " sec" << std::endl;
    std::cout << "nps " << nps << std::endl;
}

static void handleSetOption(std::istringstream& iss)
{
    // For now, accept and ignore all options.
    //
    // This lets lichess-bot/python-chess send options like:
    // setoption name Threads value 1
    // setoption name Hash value 16
    // setoption name Move Overhead value 100
    //
    // You can actually store/use these later.
    std::string ignored;
    while (iss >> ignored) {}
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

            // These are advertised mostly so lichess-bot can configure them
            // without python-chess rejecting the engine.
            // The engine currently accepts but ignores setoption.
            std::cout << "option name Threads type spin default 1 min 1 max 1" << std::endl;
            std::cout << "option name Hash type spin default 16 min 1 max 1024" << std::endl;
            std::cout << "option name Move Overhead type spin default 50 min 0 max 5000" << std::endl;

            std::cout << "uciok" << std::endl;
        }
        else if (command == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (command == "setoption") {
            handleSetOption(iss);
        }
        else if (command == "ucinewgame") {
            setStartPosition(board);
            clearSearchTables();
        }
        else if (command == "position") {
            handlePosition(board, iss);
        }
        else if (command == "go") {
            handleGo(board, iss);
        }
        else if (command == "stop") {
            // This engine searches synchronously right now.
            // By the time we read "stop", search is already done.
            // Safe to ignore for now.
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