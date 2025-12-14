#include <cstdint>
#include <iostream>

using std::uint64_t;

uint64_t maskRook(int sq) {
    uint64_t mask = 0ULL;
    int r = sq / 8;
    int f = sq % 8;

    for (int r2 = r + 1; r2 <= 6; r2++) mask |= 1ULL << (r2 * 8 + f);
    for (int r2 = r - 1; r2 >= 1; r2--) mask |= 1ULL << (r2 * 8 + f);
    for (int f2 = f + 1; f2 <= 6; f2++) mask |= 1ULL << (r * 8 + f2);
    for (int f2 = f - 1; f2 >= 1; f2--) mask |= 1ULL << (r * 8 + f2);

    return mask;
}

void generateShiftArray() {
    uint64_t blockers[64];
    for (int i = 0; i < 64; i++ ) {
        blockers[i] = maskRook(i);
    }

    for (int i = 0; i < 64; i++) {
        std::cout << "0x" << std::hex << blockers[i] << "ULL" << ',' << std::dec << '\n';
    }
}

int main() {
    generateShiftArray();
    return 0;
}
