//
//  board_naive.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/3/15.
//
//

#pragma once

#include "util.hpp"



class Board {
public:
    
    Board() {}
    Board(Grid<Color>& board, Count color_count, Index seed) {
        Init(board, color_count, seed);
    }
    
    void Init(Grid<Color>& board, Count color_count, Index seed) {
        board_ = board;
        color_count_ = color_count;
        seed_ = seed;
        
        Region reg{{0, 0}, board.size()};
        for (Position p : reg) {
            if (p.col < size()-1) {
                moves_.emplace_back(p, kDirRight);
            }
            if (p.row < size()-1) {
                moves_.emplace_back(p, kDirDown);
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

    void MakeMove(const Move& m) {
        auto pos_2 = m.pos.Shifted(m.dir);
        assert(board_.IsInside(m.pos) && board_.IsInside(pos_2));
        swap(board_[m.pos], board_[pos_2]);
    }
    
    const Grid<Color>& grid() const {
        return board_;
    }
    
    const vector<::Move>& moves() const {
        return moves_;
    }
    
    Count squares_removed() const {
        return squares_removed_;
    }
    
    bool IsSquare(const Position& p) const {
        auto& b = board_;
        return p.row < size()-1 && p.col < size()-1 && 
                b(p.row  , p.col) == b(p.row  , p.col+1) &&
                b(p.row+1, p.col) == b(p.row+1, p.col+1) &&
                b(p.row  , p.col) == b(p.row+1, p.col+1);
    }
    
    Count size() const {
        return board_.size().row;
    }
    
private:        
    Index seed_;
    Count color_count_;
    Count squares_removed_;    
    Grid<Color> board_;
    vector<::Move> moves_;
};