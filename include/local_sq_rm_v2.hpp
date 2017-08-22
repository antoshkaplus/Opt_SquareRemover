#pragma once

#include "board.hpp"
#include "board_restore.hpp"


class LocalSqRm_v2 {

    // don't think pushing everything at once will help in any way
    void AddEliminateCandidate(const Region& rect) {
        rect.ForEach([&](const Position& p) {
            if (board_->IsSquare(p)) {
                assert(board_->sq_region().hasInside(p));
                heap_.push(p);
            }
        });
    }

    template<class BoardRemoveFunc>
    void eliminate(Region rect, BoardRemoveFunc board_remove) {
        for (;;) {
            AddEliminateCandidate(rect);

            if (heap_.empty()) break;

            Position p = heap_.top();
            heap_.pop();
            // when we come p may not to be a square
            if (board_->IsSquare(p)) {
                board_remove(p);
                squares_.push_back(p);
                rect = board_->sq_region().intersection(Region(p.row - 1, p.col - 1, 3, 3));
            } else {
                rect = {};
            }
        }
    }

public:

    LocalSqRm_v2& Init(Board& board) {
        board_ = &board;
        return *this;
    }

    Region Remove(const Move& m) {
        return Remove(m, [&](const Position& p) { board_->Remove(p); });
    }

    template<class Func>
    void ForRemove(const Move& m, Func func) {
        board_restore_.Init(*board_);

        auto reg = Remove(m, [&](const Position& p) {
            board_restore_.Save(p);
            board_->Remove(p);
        });

        func(reg);

        board_restore_.Restore();
    }

    void Eliminate() {
        squares_.clear();

        return eliminate(board_->region(), [&](const Position& p) { board_->Remove(p); });
    }

    const vector<Position>& removed_squares() const {
        return squares_;
    }

private:

    template<class OnSquareFound>
    Region Remove(const Move& m, OnSquareFound on_square_found) {
        squares_.clear();

        Region rect{m.pos.shifted(-1,-1), m.another()};
        eliminate(board_->sq_region().intersection(rect), on_square_found);

        // has to be present to make sense
        if (squares_.empty()) {
            return {m.pos, m.another()};
        } else {

            auto another = m.another();

            auto min_row = m.pos.row;
            auto max_row = another.row;
            auto min_col = m.pos.col;
            auto max_col = another.col;

            auto minmax_row = minmax_element(squares_.begin(), squares_.end(), [](auto& p_1, auto& p_2) {return p_1.row < p_2.row;});
            auto minmax_col = minmax_element(squares_.begin(), squares_.end(), [](auto& p_1, auto& p_2) {return p_1.col < p_2.col;});

            min_row = min(min_row, minmax_row.first->row);
            max_row = max(max_row, minmax_row.second->row + 1);

            min_col = min(min_col, minmax_col.first->col);
            max_col = max(max_col, minmax_col.second->col + 1);

            auto reg = Region(Position{min_row, min_col}, Position{max_row, max_col});

            assert(reg.size.row >= 2 && reg.size.col >= 2);

            return reg;
        }
    }


    Board *board_;
    BoardRestore board_restore_;
    priority_queue <Position, vector<Position>, Position::BottomRightComparator> heap_;
    vector <Position> squares_;
};