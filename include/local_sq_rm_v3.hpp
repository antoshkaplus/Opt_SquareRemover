#pragma once

#include "board.hpp"


class LocalSqRm_v3 {

    // or push everything in and check later
    void AddEliminateCandidate(const Region& rect) {
        auto& ps = potential_squares_;
        auto sz = potential_squares_.size();
        for (auto r = rect.row_begin(); r < rect.row_end(); ++r) {
            for (auto c = rect.col_begin(); c < rect.col_end(); ++c) {
                Position p(r, c);
                if (board_->IsSquare(p)) ps.push_back(p);
            }
        }
        reverse(ps.begin() + sz, ps.end());
        inplace_merge(ps.begin(), ps.begin()+sz, ps.end(), Position::BottomRightComparator());
        ps.erase(unique(ps.begin(), ps.end()), ps.end());
    }

    void eliminate(Region rect) {
        for (;;) {
            AddEliminateCandidate(rect);

            if (potential_squares_.empty()) break;

            Position p = potential_squares_.back();
            potential_squares_.pop_back();
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

    LocalSqRm_v3& Init(Board& board) {
        board_ = &board;
        return *this;
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
    vector<Position> potential_squares_;
    vector<Position> squares_;
};