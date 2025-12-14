#pragma once
#include <cstdint>
#include <iostream>

constexpr uint64_t FILE_A = 0x0101010101010101ULL;
constexpr uint64_t FILE_B = 0x0202020202020202ULL;
constexpr uint64_t FILE_C = 0x0303030303030303ULL;
constexpr uint64_t FILE_D = 0x0404040404040404ULL;
constexpr uint64_t FILE_E = 0x0505050505050505ULL;
constexpr uint64_t FILE_F = 0x0606060606060606ULL;
constexpr uint64_t FILE_G = 0x0707070707070708ULL;
constexpr uint64_t FILE_H = 0x8080808080808080ULL;

constexpr uint64_t RANK_1 = 0x00000000000000FFULL;
constexpr uint64_t RANK_2 = 0x000000000000FF00ULL;
constexpr uint64_t RANK_3 = 0x0000000000FF0000ULL;
constexpr uint64_t RANK_4 = 0x00000000FF000000ULL;
constexpr uint64_t RANK_5 = 0x000000FF00000000ULL;
constexpr uint64_t RANK_6 = 0x0000FF0000000000ULL;
constexpr uint64_t RANK_7 = 0x00FF000000000000ULL;
constexpr uint64_t RANK_8 = 0xFF00000000000000ULL;

// movement masks
inline uint64_t north(uint64_t bb) { return bb << 8; }
inline uint64_t south(uint64_t bb) { return bb >> 8; }
inline uint64_t east(uint64_t bb)  { return (bb & ~FILE_H) << 1; }
inline uint64_t west(uint64_t bb)  { return (bb & ~FILE_A) >> 1; }

inline uint64_t ne(uint64_t bb) { return (bb & ~FILE_H) << 9; }
inline uint64_t nw(uint64_t bb) { return (bb & ~FILE_A) << 7; }
inline uint64_t se(uint64_t bb) { return (bb & ~FILE_H) >> 7; }
inline uint64_t sw(uint64_t bb) { return (bb & ~FILE_A) >> 9; }

// Get info
inline int fileOf(int sq) { return sq & 7; }
inline int rankOf(int sq) { return sq >> 3; }

// pops and clears lsb of a board
// Avoid iterating all bits, check only where a piece is
inline int pop_lsb(uint64_t &bb) {
    int sq = __builtin_ctzll(bb);   // Compiler specific operation to check for trailing zeroes?? Lmao
    bb &= (bb - 1);
    return sq;
}

// For debugging, by chatGPT
// ===== DEBUG =====
inline void printBitboard(uint64_t bb) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int sq = rank * 8 + file;
            std::cout << ((bb & (1ULL << sq)) ? "1 " : ". ");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}