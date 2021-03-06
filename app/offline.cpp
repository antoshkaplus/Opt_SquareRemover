
#include <iostream>
#include <fstream>

#include "util.hpp"
#include "square_remover.hpp"

using namespace std;
using namespace ant;

ifstream online_input("../output/online_input.txt");


int main(int argc, const char * argv[])
{
    auto pr = RandomProblem(kSizeMin, kColorMin);

    SquareRemover solver;
    auto v = solver.playIt(pr.color_count, pr.board, pr.starting_seed);

}