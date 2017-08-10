#pragma once

#include "board.hpp"


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

    void eliminate(Region rect) {
        for (;;) {
            AddEliminateCandidate(rect);

            if (heap_.empty()) break;

            Position p = heap_.top();
            heap_.pop();
            // when we come p may not to be a square
            if (board_->IsSquare(p)) {
                board_->Remove(p);
                squares_.push_back(p);
                rect = board_->region().intersection(Region(p.row - 1, p.col - 1, 3, 3));
            } else {
                rect = {};
            }
        }
    }

public:

    void Init(Board& board) {
        board_ = &board;
    }

    void Remove(const Move& m) {
        squares_.clear();

        auto ind = Indent{1,1};
        auto p = m.pos - ind;

        Region rect{p, m.another()};
        return eliminate(board_->region().intersection(rect));
    }

    void Eliminate() {
        squares_.clear();

        return eliminate(board_->region());
    }

    const vector<Position>& removed_squares() const {
        return squares_;
    }

private:
    Board *board_;
    priority_queue <Position, vector<Position>, Position::BottomRightComparator> heap_;
    vector <Position> squares_;
};