//
//  util.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 1/11/15.
//
//

#ifndef SquareRemover_util_hpp
#define SquareRemover_util_hpp

#include "ant/grid.hpp"


using namespace std;
using namespace ant;
using namespace ant::grid;
    
using Color = int8_t;

// using predefined constants
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

constexpr array<Position, 4> SquarePositions(const Position p) {
    return {{ {p.row, p.col}, {p.row, p.col+1}, 
        {p.row+1, p.col}, {p.row+1, p.col+1} }};
}

struct Move {
    Position pos;
    Direction dir;
    
    Position another() const {
        return pos.Shifted(dir);
    }
};




   

#endif
