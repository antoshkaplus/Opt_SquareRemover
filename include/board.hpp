//
//  board_naive.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/3/15.
//
//

#pragma once

#include <functional>

#include "util.hpp"


class Board {
public:
    
    Board() {}
    Board(const Grid<DigitColor>& board, Count color_count, Index seed) {
        Init(board, color_count, seed);
    }
    
    virtual void Init(const Grid<DigitColor>& board, Count color_count, Index seed) {
        region_ = Region(1, 1, board.size().row, board.size().col);
        sq_region_ = Region(1, 1, board.size().row-1, board.size().col-1);

        board_.resize(board.size().row+2, board.size().col+2);
        board_.fill(kEmptyColor);

        color_count_ = color_count;
        seed_ = seed;
        squares_removed_ = 0;
        moves_.reset(new vector<Move>());

        for (Position p : region()) {
            if (p.col < region().col_end()-1) {
                moves_->emplace_back(p, kDirRight);
            }
            if (p.row < region().row_end()-1) {
                moves_->emplace_back(p, kDirDown);
            }
        }
    }

    BitColor color(const Position& p) const {
        return board_[p];
    }

    BitColor color(Index r, Index c) const {
        return board_(r, c);
    }

    vector<Move> move_history() const {
        return history_->Path();
    }

    virtual void MakeMove(const Move& m) {
        auto pos_2 = m.another();
        assert(region().hasInside(m.pos) && region().hasInside(pos_2));
        swap(board_[m.pos], board_[pos_2]);
        history_ = make_shared<TrailNode<Move>>(m, history_);
    }

// maybe
//    virtual void TryMakeMove(const Move& m) {
//
//    }

    template<class Func>
    void ForMove(const Move& m, Func func) {
        auto pos_2 = m.another();
        assert(region().hasInside(m.pos) && region().hasInside(pos_2));
        swap(board_[m.pos], board_[pos_2]);
        func();
        swap(board_[m.pos], board_[pos_2]);
    }

    bool IsIdentityMove(const Move& m) {
        return color(m.pos) == color(m.another());
    }
    
//    bool IsRemoveMove(const Move& m) {
//        auto res = false;
//        ForMove(m, [&]() {
//            array<Position, 4> ps;
//            if (m.isVertical()) {
//                ps = {{ m.pos.shifted(-1, -1),
//                      m.pos.shifted(-1, 0),
//                      m.another().shifted(0, -1),
//                      m.another() }};
//            } else {
//                ps = {{ m.pos.shifted(-1, -1),
//                      m.pos.shifted(0, -1),
//                      m.another().shifted(-1, 0),
//                      m.another() }};
//            }
//            bool res = std::any_of(ps.begin(), ps.end(), std::bind(&Board::IsSquare, this, std::placeholders::_1));
//        });
//        return res;
//    }

    virtual void Remove(const Position& p) = 0;
    virtual bool IsSquare(const Position& p) const = 0;
    virtual bool IsFour(const Position& p_1, const Position& p_2, const Position& p_3, const Position& p_4) const = 0;

private:
    Direction randomDirection() {
        return rand()%kDirCount;
    }

    Position randomPosition() {
        return Position(rand()%region_.row_count() +1, rand()%region_.col_count() +1);
    }

public:
    Move randomMove() {
        Move m;
        Position p;
        do {
            m.pos = randomPosition();
            m.dir = randomDirection();
            p = m.another();
        } while(!region().hasInside(p));
        return m;
    }
    
    const Grid<BitColor>& grid() const {
        return board_;
    }
    
    const Region region() const {
         return region_;
    }

    const Region sq_region() const {
        return sq_region_;
    }

    const Position origin() const {
        return region_.position;
    }

    const vector<::Move>& moves() const {
        return *moves_;
    }
    
    Count squares_removed() const {
        return squares_removed_;
    }

    Count color_count() const {
        return color_count_;
    }

    Count extended_size() const {
        return board_.size().row;
    }

    Index seed() const {
        return seed_;
    }

    bool operator==(const Board& b) const {
        return board_ == b.board_ && seed_ == b.seed_ && squares_removed_ == b.squares_removed_ && history_->value == b.history_->value;
    }

    bool operator!=(const Board& b) const {
        return !(*this == b);
    }
//    bool IsRemoveMove(const Move& m) {
//        ForMove(m, [&]() {
//            if (m.isHorizontal()) {
//
//            }
//        });
//        return true
//    }

protected:
    // should put a lot of stuff to common info.

    Region region_;
    Region sq_region_;
    Grid<Color> board_;

    Index seed_;
    Count color_count_;
    Count squares_removed_;    
    // this one should not be stored here
    // ore may be computed everytime
    shared_ptr<vector<::Move>> moves_;

    TrailNodePtr<Move> history_;

    friend class BoardState;
    friend class BoardRestore;
};