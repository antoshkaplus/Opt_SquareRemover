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
    
    
using Color = short;

constexpr Count kColorMin = 4;
constexpr Count kColorMax = 6;
constexpr Count kSizeMin = 8;
constexpr Count kSizeMax = 16;
constexpr Count kMoveCount = 10000;

extern default_random_engine RNG;

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
    
    Move() {}
    Move(const Position& p, Direction d)
        : pos(p), dir(d) {}
    
    
    Position another() const {
        return pos.Shifted(dir);
    }
};


struct Problem {
    int color_count;
    int starting_seed;
    vector<string> board;
};

Problem ReadProblem(istream& cin);
void WriteProblem(ostream& out, const Problem& pr);
void WriteSolution(ostream& out, vector<Move>& moves);

Grid<Color> ToColorGrid(const vector<string>& b);   






#endif
