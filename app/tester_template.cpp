
#include <iostream>
#include <fstream>

#include "util.hpp"
#include "square_remover.hpp"

using namespace std;
using namespace ant;

int main(int argc, const char * argv[])
{
    auto pr = RandomProblem(10, 4);

    SquareRemover solver;
    cerr << "started" << endl;
    auto v = solver.playIt(pr.color_count, pr.board, pr.starting_seed);
    Println(cout, v);
    cerr << "finished" << endl;
}