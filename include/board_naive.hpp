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
    Board(Grid<Color>& board, Count color_count, Index seed)
        :  seed_(seed), color_count_(color_count), board_(board) {}
    
    void Remove(const Position& p) {
        auto& b = board_;
        b(p) = NextColor();
        UpdateSeed();
        b(p.row, p.col+1) = NextColor();
        UpdateSeed();
        b(p.row+1, p.col) = NextColor();
        UpdateSeed();
        b(p.row+1, p.col+1) = NextColor();
        UpdateSeed();
    }
    
    Color color(const Position& p) const {
        return board_[p];
    } 

private:
    
    void UpdateSeed() {
        seed_ = NextSeed();
    }
    
    Index NextSeed() {
        return ((int64_t)seed_ * 48271) % 2147483647; 
    }
    
    Color NextColor() {
        return seed_ % color_count_;
    }
    
    Index seed_;
    Count color_count_;    
    Grid<Color> board_;

};