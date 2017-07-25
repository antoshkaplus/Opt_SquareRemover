#include "naive.hpp"


class SquareRemover {
public:
    std::vector<int> playIt(int colors, std::vector<std::string> board, int startingSeed);
};

#ifdef SQR_REM_NAIVE

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    Grid<Color> g = ToColorGrid(board);
    Board b;
    b.Init(g, colors, startingSeed);

    NaiveSquareRemover naive;
    auto moves = naive.Solve(b, kMoveCount);
    return ToSolution(moves);
}

#endif