#pragma once

#include "board.hpp"
#include "board_restore.hpp"


class LocalSqRm_v2 {

    // don't think pushing everything at once will help in any way
    void AddEliminateCandidate(const Region& rect) {
        for (auto r = rect.row_begin(); r < rect.row_end(); ++r) {
            for (auto c = rect.col_begin(); c < rect.col_end(); ++c) {
                Position p(r, c);
                if (board_->IsSquare(p)) {
                    assert(board_->sq_region().hasInside(p));
                    heap_.push(p);
                }
            }
        }
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
                rect = board_->region().intersection(Region(p.row - 1, p.col - 1, 3, 3));
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
        eliminate(board_->region().intersection(rect), on_square_found);

        // has to be present to make sense
        if (squares_.empty()) {
            return {m.pos, m.another()};
        } else {
            auto comp = Position::TopLeftComparator();

            auto minmax_sq = minmax_element(squares_.begin(), squares_.end(), comp);
            auto topleft = min(*(minmax_sq.first), m.pos, comp);
            auto botright = max(minmax_sq.second->shifted(1, 1), m.another(), comp);

            return Region(topleft, botright);
        }
    }


    Board *board_;
    BoardRestore board_restore_;
    priority_queue <Position, vector<Position>, Position::BottomRightComparator> heap_;
    vector <Position> squares_;
};