#pragma once

#include "boost/iterator/zip_iterator.hpp"

#include "ant/grid/grid.hpp"


using namespace std;
using namespace ant;
using namespace ant::grid;
    

using BitColor = uint8_t;
using DigitColor = uint8_t;
using Color = uint8_t;
using Seed = int;

constexpr uint8_t kEmptyColor = 1 << 7;

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

constexpr uint8_t NextColor(int current_seed, char color_count) {
    return current_seed % color_count;
}

inline BitColor DigitColotToBit(DigitColor digit_color) {
    return 1 << digit_color;
}

inline DigitColor BitColorToDigit(BitColor bit_color) {
    switch (bit_color) {
        case 1 << 0: return 0;
        case 1 << 1: return 1;
        case 1 << 2: return 2;
        case 1 << 3: return 3;
        case 1 << 4: return 4;
        case 1 << 5: return 5;
        default: throw runtime_error("unknown bit color");
    }
}

// replace it with the loop around SquarePositions
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

template<class Grid>
int ReplaceBitColors(Grid& board, char color_count, Position p, int current_seed) {
    auto localNextColor = [=](int seed) {
        return DigitColotToBit(NextColor(seed, color_count));
    };
    board(p.row, p.col) = localNextColor(current_seed);
    current_seed = NextSeed(current_seed);
    board(p.row, p.col+1) = localNextColor(current_seed);
    current_seed = NextSeed(current_seed);
    board(p.row+1, p.col) = localNextColor(current_seed);
    current_seed = NextSeed(current_seed);
    board(p.row+1, p.col+1) = localNextColor(current_seed);
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
    Move(int row, int col, Direction d) : Move({row, col}, d) {}
    Move(const Position& p, Direction d)
        : pos(p), dir(d) {}

    // 2 bit per dir, 5 bits row / col (due to extended board)
    // 12 bits total

    // used to fully identify move in space but not the square!!!
    // doesn't take into account opposite moves
    int index() const {
        return (dir << 10) | (pos.row << 5) | pos.col;
    }

    bool operator==(const Move& m) const {
        return (pos == m.pos && dir == m.dir) || (pos == m.another() && dir == kDirOpposite[m.dir]);
    }

    bool operator!=(const Move& m) const {
        return !(*this == m);
    }
    
    Position another() const {
        return pos.Shifted(dir);
    }

    bool isVertical() const {
        return dir == kDirUp || dir == kDirDown;
    }

    bool isHorizontal() const {
        return dir == kDirRight || dir == kDirLeft;
    }

    template<class Func>
    void ForEachPossibleSquare(Func func) {
        func(pos.shifted(-1, -1));
        func(another());
        if (isVertical()) {
            func(pos.shifted(-1, 0));
            func(another().shifted(0, -1));
        } else {
            func(pos.shifted(0, -1));
            func(another().shifted(-1, 0));
        }
    }
};

namespace std {

template <>
struct hash<Move> {
    size_t operator()(const Move& x) const {
        return x.index();
    }
};

}

struct DoubleMove : Move {
    DoubleMove() {}
    DoubleMove(const Move& m, const Move& next)
    : Move(m), next(next) {}
    Index index() {
        Index i = 0;
        i = next.index() << 12 | Move::index();
        return i;
    }
    Move next;
};

template<class Board>
Index index(const Position& p, const Board& b) {
    throw runtime_error("not implemented");
}


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

Grid<DigitColor> ToColorGrid(const vector<string>& b);

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


namespace bit {


}