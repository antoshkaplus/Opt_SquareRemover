
#include <iostream>
#include <fstream>

#include "board.hpp"
#include "board_simulation.hpp"
#include "combinations.hpp"
#include "board.hpp"
#include "local_sq_rm.hpp"
#include "naive.hpp"

using namespace std;
using namespace ant;

ifstream online_input("../output/online_input.txt");


int main(int argc, const char * argv[])
{
    auto pr = ReadProblem(online_input);
    
    auto sz = pr.board.size();
    Grid<Color> g;
    g.resize(sz, sz);
    auto func = [&](const Position& p) {
        return g[p] = pr.board[p.row][p.col] - '0';
    };
    g.ForEachPosition(func);
    
    Board b;
    b.Init(g, pr.color_count, pr.starting_seed);
    
    NaiveSquareRemover naive;
    auto moves = naive.Solve(b, kMoveCount);
}