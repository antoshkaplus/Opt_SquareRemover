//
//  main.cpp
//  SquareRemover
//
//  Created by Anton Logunov on 4/9/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "board.hpp"
//#include "beam_search.hpp"
#include "board_simulation.hpp"
#include "combinations.hpp"
#include "board.hpp"
#include "local_sq_rm.hpp"
#include "naive.hpp"
#include "greedy.hpp"

using namespace std;
using namespace ant;

ofstream online_input("../output/online_input.txt");


int main(int argc, const char * argv[])
{
    auto pr = ReadProblem(cin);
    WriteProblem(online_input, pr);

    Grid<Color> g = ToColorGrid(pr.board);
    
    Board b;
    b.Init(g, pr.color_count, pr.starting_seed);
    
    Greedy naive;
    auto moves = naive.Solve(b, kMoveCount);
    
    WriteSolution(cout, moves);
}

















