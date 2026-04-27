#pragma once
#include "move.h"
#include <cassert>

struct MoveList {

    Move moves[256];

    int count = 0;

    void clear() {
        count = 0;
    }

    void push(const Move& move) {
        assert(count < 256);
        moves[count++] = move;
    }

    
    void push_back(const Move& move) {
    if (count >= 256) {
        std::cerr << "MoveList overflow: " << count << "\n";
        std::abort();
    }

    moves[count++] = move;
}

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    Move& operator[](int i) {
        return moves[i];
    }

    const Move& operator[](int i) const {
        return moves[i];
    }

    Move* begin() {
        return moves;
    }

    Move* end() {
        return moves + count;
    }

    const Move* begin() const {
        return moves;
    }

    const Move* end() const {
        return moves + count;
    }
};