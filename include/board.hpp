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
        sq_region_ = Region(1, 1, board.size().row-1, board.size().col);

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

    // if it's practical MakeMove, add method Try
    virtual void MakeMove(const Move& m) {
        auto pos_2 = m.pos.Shifted(m.dir);
        assert(region().hasInside(m.pos) && region().hasInside(pos_2));
        swap(board_[m.pos], board_[pos_2]);
    }

    template<class Func>
    void ForMove(const Move& m, Func func) {
        auto pos_2 = m.pos.Shifted(m.dir);
        swap(board_[m.pos], board_[pos_2]);
        func();
        swap(board_[m.pos], board_[pos_2]);
    }

    bool IsIdentityMove(const Move& m) {
        return color(m.pos) == color(m.another());
    }
    
//    bool IsKillerMove(const Move& m) {
//        MakeMove(m);
//        array<Position, 4> ps;
//        if (m.isVertical()) {
//            ps = {{ m.pos.shifted(-1, -1),
//                    m.pos.shifted(-1, 0),
//                    m.another().shifted(0, -1),
//                    m.another() }};
//        } else {
//            ps = {{ m.pos.shifted(-1, -1),
//                    m.pos.shifted(0, -1),
//                    m.another().shifted(-1, 0),
//                    m.another() }};
//        }
//        bool res = std::any_of(ps.begin(), ps.end(), std::bind(&Board::IsSquare, this, std::placeholders::_1));
//        MakeMove(m);
//        return res;
//    }

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

    bool IsRemoveMove(const Move& m) {
        ForMove(m, [&]() {
            if (m.isHorizontal()) {

            }
        });
    }

private:

    Region region_;
    Region sq_region_;
    Grid<Color> board_;

    Index seed_;
    Count color_count_;
    Count squares_removed_;    
    // this one should not be stored here
    // ore may be computed everytime
    shared_ptr<vector<::Move>> moves_;

    friend class BoardState;
    friend class BoardRestore;
};