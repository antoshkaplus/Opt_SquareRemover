//
//  util.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 1/11/15.
//
//

#ifndef SquareRemover_util_hpp
#define SquareRemover_util_hpp

#include "ant/grid.h"


using namespace std;
using namespace ant;
using namespace ant::grid;
    
using Color = int8_t;
using Direction = int8_t;

constexpr int NextSeed(int current_seed) {
    return ((int64_t)current_seed * 48271) % 2147483647; 
}

constexpr char NextColor(int current_seed, char color_count) {
    return current_seed % color_count;
}

template<class Grid>
constexpr int ReplaceColors(Grid& board, char color_count, Position p, int current_seed) {
    // top left, top right, bot left, bot right
    board(p.row, p.col) = NextColor(current_seed, color_count);
    current_seed = NextSeed(current_seed);
    board(p.row, p.col+1) = NextColor(current_seed, color_count);
    current_seed = NextSeed(current_seed);
    board(p.row+1, p.col) = NextColor(current_seed, color_count);
    current_seed = NextSeed(current_seed);
    board(p.row+1, p.col+1) = NextColor(current_seed, color_count);
    current_seed = NextSeed(current_seed);
    return current_seed;
}

using Rectangle = Region;

const char kRight  = 0;
const char kDown   = 1;





//
//struct Move {
//    Position pos;
//    Direction dir;
//    Move() {}
//    Move(int row, int col, Direction direction) 
//        : Move(Position(row, col), direction) {}
//    Move(const Position& position, Direction direction) 
//        : pos(position), dir(direction) {}
//        
//    Position target() const {
//        return dir == kRight ? pos.shifted(1, 0) : pos.shifted(0, 1);
//    }
//};
//

   

#endif
