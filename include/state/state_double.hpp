#pragma once

#include "board.hpp"

class DoubleState {
public:
    void Init(const ::Board &board) {
        ResetBoard(board);
        IncreaseChange(board_->region());
    }

    void ResetBoard(const ::Board &board) {
        board_ = &board;
    }

    void IncreaseChange(const Region &reg) {
        ForEachDouble(reg, [&]() { ++double_count_; });
    }

    void DecreaseChange(const Region &reg) {
        ForEachDouble(reg, [&]() { --double_count_; });
    }

    ant::Count Count(const Region &reg) {
        auto res = 0;
        ForEachDouble(reg, [&]() { ++res; });
        return res;
    }

    void IncreaseBy(ant::Count count) {
        double_count_ += count;
    }

    ant::Count double_count() const {
        return double_count_;
    }

private:

    template<class Func>
    void ForEachDouble(const Region& reg, Func func) const {
        reg.diffed(0, -1, 0, 1).ForEach([&](const Position& p) {
            if (board_->color(p) == board_->color(p.shifted(0, 1))) {
                func();
            }
        });
        reg.diffed(-1, 0, 1, 0).ForEach([&](const Position& p) {
            if (board_->color(p) == board_->color(p.shifted(1, 0))) {
                func();
            }
        });
    }

    const ::Board *board_;
    ant::Count double_count_ = 0;
};