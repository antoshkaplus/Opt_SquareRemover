#pragma once

#include "board.hpp"


namespace state {

class AdjState {
public:

    void Init(const ::Board& board) {
        ResetBoard(board);
        adj_amount_ = Count(board_->region());
    }

    void ResetBoard(const ::Board& board) {
        board_ = &board;
    }

    ant::Count Count(Region reg) {
        auto res = 0;

        auto cs = [&](Index r, Index c) {
            return board_->color(r, c);
        };
        reg = reg.diffed(-1, -1, 2, 2).intersection(board_->region().diffed(1, 1, -2, -2));

        reg.ForEach([&](const Position& p) {
            auto r = p.row;
            auto c = p.col;
            auto count = 1;
            if (cs(r, c) == cs(r-1, c-1)) ++count;
            if (cs(r, c) == cs(r-1, c)) ++count;
            if (cs(r, c) == cs(r-1, c+1)) ++count;

            if (cs(r, c) == cs(r, c-1)) ++count;
            if (cs(r, c) == cs(r, c+1)) ++count;

            if (cs(r, c) == cs(r+1, c-1)) ++count;
            if (cs(r, c) == cs(r+1, c)) ++count;
            if (cs(r, c) == cs(r+1, c+1)) ++count;

            res += count * count;
        });
        return res;
    }

    ant::Count adj_amount() const {
        return adj_amount_;
    }

    double score() const {
        return sqrt(1.L * adj_amount_) / board_->region().size.cell_count();
    }

    void set(ant::Count adj_res) {
        adj_amount_ = adj_res;
    }

private:
    const ::Board* board_;
    ant::Count adj_amount_ = 0;
};

}