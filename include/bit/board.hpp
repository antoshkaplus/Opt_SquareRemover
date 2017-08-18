#pragma once

#include "board.hpp"

namespace bit {

class Board : public ::Board {
public:
    void Init(const Grid<DigitColor>& board, Count color_count, Index seed) override {
        ::Board::Init(board, color_count, seed);

        auto func = [&](const Position& p) {
            board_(p.shifted(1, 1)) = DigitColotToBit(board(p));
        };
        board.ForEachPosition(func);
    }

    void Remove(const Position& p) {
        seed_ = ReplaceBitColors(board_, color_count_, p, seed_);
        ++squares_removed_;
    }

    bool IsTriple(const Position p_0, const Position& p_1, const Position& p_2) const {
        return static_cast<bool>(color(p_0) & color(p_1) & color(p_2));
    }

    bool IsSquare(const Position& p) const {
        auto &b = board_;
        // could use intrinsics but mostly gonna get out on first or second comparizon
        return static_cast<bool>(b(p.row, p.col) & b(p.row, p.col + 1) & b(p.row + 1, p.col) & b(p.row + 1, p.col + 1));
    }

    void Print(ostream& out) {
        for (auto r = region().row_begin(); r < region().row_end(); ++r) {
            for (auto c = region().col_begin(); c < region().col_end(); ++c) {
                out << char(BitColorToDigit(board_(r, c)) + '0');
            }
            out << endl;
        }
    }
};

}