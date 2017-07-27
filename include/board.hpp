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
    Board(const Grid<Color>& board, Count color_count, Index seed) {
        Init(board, color_count, seed);
    }
    
    void Init(const Grid<Color>& board, Count color_count, Index seed) {
        board_ = board;
        color_count_ = color_count;
        seed_ = seed;
        squares_removed_ = 0;
        moves_.reset(new vector<Move>());
        
        Region reg{{0, 0}, board.size()};
        for (Position p : reg) {
            if (p.col < size()-1) {
                moves_->emplace_back(p, kDirRight);
            }
            if (p.row < size()-1) {
                moves_->emplace_back(p, kDirDown);
            }
        }
    }
    
    // should I be able to go back in time???
    void Remove(const Position& p) {
        seed_ = ReplaceColors(board_, color_count_, p, seed_);
        ++squares_removed_;
    }

    Color color(const Position& p) const {
        return board_[p];
    }

    Color color(Index r, Index c) const {
        return board_(r, c);
    }

    // if it's practical MakeMove, add method Try
    void MakeMove(const Move& m) {
        auto pos_2 = m.pos.Shifted(m.dir);
        assert(board_.IsInside(m.pos) && board_.IsInside(pos_2));
        swap(board_[m.pos], board_[pos_2]);
        moves_->push_back(m);
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
        return Position(rand()%board_.row_count(), rand()%board_.col_count());
    }

public:
    Move randomMove() {
        Move m;
        Position p;
        do {
            m.pos = randomPosition();
            m.dir = randomDirection();
            p = m.another();
        } while(!board_.IsInside(p) ||
                board_(p) == board_(m.pos));
        return m;
    }

    bool IsTriple(const Position p_0, const Position& p_1, const Position& p_2) const {
        return color(p_0) == color(p_1) && color(p_1) == color(p_2);
    }
    
    const Grid<Color>& grid() const {
        return board_;
    }
    
    const Region region() const {
         return Region(0, 0, size(), size());
    }
    
    const vector<::Move>& moves() const {
        return *moves_;
    }
    
    Count squares_removed() const {
        return squares_removed_;
    }
    
    bool IsSquare(const Position& p) const {
        auto& b = board_;
        // could use intrinsics but mostly gonna get out on first or second comparizon
        return p.row < size()-1 && p.col < size()-1 && 
                b(p.row  , p.col) == b(p.row  , p.col+1) &&
                b(p.row+1, p.col) == b(p.row+1, p.col+1) &&
                b(p.row  , p.col) == b(p.row+1, p.col+1);
    }
    
    Count size() const {
        return board_.size().row;
    }
    
    Count color_count() const {
        return color_count_;
    }
    
    Index seed() const {
        return seed_;
    }

    void Print(ostream& out) {
        for (auto r = 0; r < size(); ++r) {
            for (auto c = 0; c < size(); ++c) {
                out << char(board_(r, c) + '0');
            }
            out << endl;
        }
    }
    
private:        
    Index seed_;
    Count color_count_;
    Count squares_removed_;    
    Grid<Color> board_;
    // this one should not be stored here
    // ore may be computed everytime
    shared_ptr<vector<::Move>> moves_;
};