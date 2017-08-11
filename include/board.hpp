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
    
    void Init(const Grid<DigitColor>& board, Count color_count, Index seed) {
        // I need to redraw board
        InitBitBoard(board);

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
    
    // should I be able to go back in time???
    // it's not easy to spot how much
    void Remove(const Position& p) {
        seed_ = ReplaceBitColors(bit_board_, color_count_, p, seed_);
        ++squares_removed_;
    }

    BitColor color(const Position& p) const {
        return bit_board_[p];
    }

    BitColor color(Index r, Index c) const {
        return bit_board_(r, c);
    }

    // if it's practical MakeMove, add method Try
    void MakeMove(const Move& m) {
        auto pos_2 = m.pos.Shifted(m.dir);
        assert(region().hasInside(m.pos) && region().hasInside(pos_2));
        swap(bit_board_[m.pos], bit_board_[pos_2]);
    }

    template<class Func>
    void ForMove(const Move& m, Func func) {
        auto pos_2 = m.pos.Shifted(m.dir);
        swap(bit_board_[m.pos], bit_board_[pos_2]);
        func();
        swap(bit_board_[m.pos], bit_board_[pos_2]);
    }

    bool IsIdentityMove(const Move& m) {
        return color(m.pos) == color(m.another());
    }
    
    bool IsKillerMove(const Move& m) {
        MakeMove(m);
        array<Position, 4> ps;
        if (m.isVertical()) {
            ps = {{ m.pos.shifted(-1, -1),
                    m.pos.shifted(-1, 0),
                    m.another().shifted(0, -1),
                    m.another() }};
        } else {
            ps = {{ m.pos.shifted(-1, -1),
                    m.pos.shifted(0, -1),
                    m.another().shifted(-1, 0),
                    m.another() }};
        }
        bool res = std::any_of(ps.begin(), ps.end(), std::bind(&Board::IsSquare, this, std::placeholders::_1));
        MakeMove(m);
        return res;
    }

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

    bool IsTriple(const Position p_0, const Position& p_1, const Position& p_2) const {
        return static_cast<bool>(color(p_0) & color(p_1) & color(p_2));
    }
    
    const Grid<BitColor>& grid() const {
        return bit_board_;
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
    
    bool IsSquare(const Position& p) const {
        auto& b = bit_board_;
        // could use intrinsics but mostly gonna get out on first or second comparizon
        return static_cast<bool>(b(p.row  , p.col) & b(p.row  , p.col+1) & b(p.row+1, p.col) & b(p.row+1, p.col+1));
    }

    Count color_count() const {
        return color_count_;
    }

    Count extended_size() const {
        return bit_board_.size().row;
    }

    Index seed() const {
        return seed_;
    }

    void Print(ostream& out) {
        for (auto r = region().row_begin(); r < region().row_end(); ++r) {
            for (auto c = region().col_begin(); c < region().col_end(); ++c) {
                out << char(BitColorToDigit(bit_board_(r, c)) + '0');
            }
            out << endl;
        }
    }
    
private:

    void InitBitBoard(Grid<DigitColor> digit_board) {
        region_ = Region(1, 1, digit_board.size().row, digit_board.size().col);
        sq_region_ = Region(1, 1, digit_board.size().row-1, digit_board.size().col);

        bit_board_.resize(digit_board.size().row+2, digit_board.size().col+2);
        fill(bit_board_.begin(), bit_board_.end(), 0);
        auto func = [&](const Position& p) {
            bit_board_(p.shifted(1, 1)) = DigitColotToBit(digit_board(p));
        };
        digit_board.ForEachPosition(func);
    }

    Region region_;
    Region sq_region_;
    Grid<BitColor> bit_board_;

    Index seed_;
    Count color_count_;
    Count squares_removed_;    
    // this one should not be stored here
    // ore may be computed everytime
    shared_ptr<vector<::Move>> moves_;

    friend class BoardState;
    friend class BoardRestore;
};