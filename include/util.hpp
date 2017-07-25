#pragma once

#include "boost/iterator/zip_iterator.hpp"

#include "ant/grid/grid.hpp"


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

    bool isVertical() const {
        return dir == kDirUp || dir == kDirDown;
    }

    bool isHorizontal() const {
        return dir == kDirRight || dir == kDirLeft;
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

inline void WriteSolution(ostream& out, vector<int>& moves) {
    assert(moves.size() == 3*kMoveCount);
    for (auto i : moves) {
        ant::Println(out, i);
    }
}

inline vector<int> ToSolution(const vector<Move>& moves) {
    vector<int> res;
    res.reserve(3*moves.size());
    static const map<Direction, int> ds = {{kDirUp, 0}, {kDirRight, 1}, {kDirDown, 2}, {kDirLeft, 3}};
    for (auto& m : moves) {
        res.push_back(m.pos.row);
        res.push_back(m.pos.col);
        res.push_back(ds.at(m.dir));
    }
    return res;
}

Grid<Color> ToColorGrid(const vector<string>& b);

inline vector<string> GenerateStrBoard(Count sz, Count colors) {
    vector<string> b(sz);
    for (int r = 0; r < sz; ++r) {
        b[r].resize(sz);
        for (int c = 0; c < sz; ++c) {
            b[r][c] = rand()%colors + '0';
        }
    }
    return b;
}

inline Problem RandomProblem(Count board_size, Count color_count) {
    Problem res;
    std::uniform_int_distribution<> distr(0, color_count-1);
    res.color_count = color_count;
    auto& board = res.board;
    board.resize(board_size);
    for (auto r = 0; r < board_size; ++r) {
        string s;
        for (auto c = 0; c < board_size; ++c) {
            s.push_back(distr(RNG)+'0');
        }
        board[r] = s;
    }
    res.starting_seed = uniform_int_distribution<>(1)(RNG);
    return res;
}