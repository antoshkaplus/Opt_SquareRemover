#pragma once

#include "board.hpp"

class TripleState {
public:
    void Init(::Board& board) {
        change_region_ = board_->region();
        OnRegionChanged();
    }

    // too early to substract.
    // we have incorrect result between Before and After region change
    void OnBeforeRegionChanged(const Region& reg) {
        change_region_ = reg;
        ForEachTriple([&]() { --triple_count_; });
    }

    void OnRegionChanged() {
        ForEachTriple([&]() { ++triple_count_; });
    }

    Count triple_count() const {
        return triple_count_;
    }

private:

    template<class Func>
    void ForEachTriple(Func func) {
        auto& reg = change_region_;
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


    Region change_region_;
    ::Board* board_;
    Count triple_count_;
};