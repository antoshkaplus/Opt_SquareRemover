#pragma once

#include "board.hpp"
#include "board_restore.hpp"


class LocalSqRm_v1 {
public:

    LocalSqRm_v1& Init(Board& board) {
        board_ = &board;
        return *this;
    }

    void Remove(const Move& m) {
        return Eliminate();
    }

    void Eliminate() {
        squares_.clear();

        auto sq_reg = board_->sq_region();
        bool removed = true;
        start:
        while (removed) {
            removed = false;
            for (auto r = sq_reg.row_begin(); r < sq_reg.row_end(); ++r) {
                for (auto c = sq_reg.col_begin(); c < sq_reg.col_end(); ++c) {
                    if (board_->IsSquare({r, c})) {
                        board_->Remove({r, c});
                        squares_.emplace_back(r, c);
                        removed = true;
                        goto start;
                    }
                }
            }
        }
    }

    const vector<Position>& removed_squares() const {
        return squares_;
    }

private:

    Board *board_;
    vector <Position> squares_;
};