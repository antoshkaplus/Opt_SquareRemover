//
//  util.cpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/6/15.
//
//

#include "util.hpp"

#ifdef RNG

default_random_engine RNG(std::chrono::system_clock::now().time_since_epoch().count());

#else

default_random_engine RNG;

#endif


Problem ReadProblem(istream& cin) {
    Problem pr;
    cin >> pr.color_count;
    
    size_t boardSize;
    cin >> boardSize;
    
    pr.board.resize(boardSize);
    for (auto i = 0; i < boardSize; ++i) {
        cin >> pr.board[i];
    }
    
    cin >> pr.starting_seed;
    return pr;
}

void WriteProblem(ostream& out, const Problem& pr) {
    Println(out, pr.color_count);
    Println(out, pr.board.size());
    for (auto line : pr.board) {
        Println(out, line);
    }
    Println(out, pr.starting_seed);
}


void WriteSolution(ostream& out, const vector<Move>& moves) {
    static const map<Direction, int> ds = {{kDirUp, 0}, {kDirRight, 1}, {kDirDown, 2}, {kDirLeft, 3}};
    for (auto m : moves) {
        out << m.pos.row << endl;
        out << m.pos.col << endl;
        out << ds.at(m.dir) << endl;
    }
}


Grid<DigitColor> ToColorGrid(const vector<string>& b) {
    Grid<DigitColor> g;
    auto sz = b.size();
    g.resize(sz, sz);
    auto func = [&](const Position& p) {
        return g[p] = b[p.row][p.col] - '0';
    };
    g.ForEachPosition(func);
    return g;
}
