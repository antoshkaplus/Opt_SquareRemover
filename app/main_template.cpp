/// main program takes two arguments:
/// -i, -o
/// input and output file paths
/// input and output format are described in problem statement
/// -m : mehtod. for now just NaiveSearch
/// -s : score function probably

#include <chrono>
#include <fstream>

#include "util.hpp"
#include "square_remover.hpp"


int main(int argc, const char * argv[]) {
    command_line_parser parser(argv, argc);
    string input, output;

    ifstream fin(input);
    ofstream fout(output);

    istream *in;
    ostream *out;

    bool timed = false;
    if (parser.exists("t")) {
        cerr << "timed" << endl;
        timed = true;
    }

    if (parser.exists("d")) {
        cerr << "direct" << endl;
        in = &cin;
        out = &cout;
    } else {
        if (parser.exists("i")) {
            input = parser.getValue("i");
            fin.open(input);
            in = &fin;
        } else {
            cerr << "unable to find input file path";
            return 1;
        }
        if (parser.exists("o")) {
            output = parser.getValue("o");
            fout.open(output);
            out = &fout;
        } else {
            cerr << "unable to find output file path";
            return 1;
        }
    }

    Problem problem = ReadProblem(*in);

    Timer timer(30000);
    SquareRemover solver;
    auto v = solver.playIt(problem.color_count, problem.board, problem.starting_seed);
    cerr << "done" << endl;
    if (timed && timer.timeout()) {
        std::cerr << "timeout";
        return 0;
    }
    for (Index i = 0; i < v.size(); i+=3) {
        --v[i];
        --v[i+1];
    }
    WriteSolution(*out, v);
    return 0;
}