#pragma once

#include "board.hpp"

class TripleState {
public:
    void Init(const ::Board& board) {
        ResetBoard(board);
        IncreaseChange(board_->sq_region());
    }

    void ResetBoard(const ::Board& board) {
        board_ = &board;
    }

    void IncreaseChange(const Region& reg) {
        ForEachTriple(reg, [&]() { ++triple_count_; });
    }

    void DecreaseChange(const Region& reg) {
        ForEachTriple(reg, [&]() { --triple_count_; });
    }

    ant::Count Count(const Region& reg) {
        auto res = 0;
        ForEachTriple(reg, [&]() { ++res; });
        return res;
    }

    void IncreaseBy(ant::Count count) {
        triple_count_ += count;
    }

    ant::Count triple_count() const {
        return triple_count_;
    }

private:

    template<class Func>
    void ForEachTriple(const Region& reg, Func func) {
        auto cs = [&](auto r, auto c) { return board_->color(r, c); };

        // only one possible triple at a time
        for (auto r = reg.row_begin()-1; r < reg.row_end(); ++r) {
            for (auto c = reg.col_begin()-1; c < reg.col_end(); ++c) {

                if (cs(r,c) == cs(r+1,c+1)) {

                    if (cs(r,c+1) == cs(r,c)) {
                        func();
                    } else if (cs(r+1,c) == cs(r,c)) {
                        func();
                    }

                } else if (cs(r+1,c) == cs(r,c+1)) {

                    if (cs(r,c) == cs(r+1,c)) {
                        func();
                    } else if (cs(r+1,c+1) == cs(r+1,c)) {
                        func();
                    }

                }

            }
        }
    }

    const ::Board* board_;
    ant::Count triple_count_ = 0;
};