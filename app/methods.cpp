#include "naive.hpp"
#include "greedy.hpp"
//#include "wide.hpp"
#include "beam_search.hpp"


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

#ifdef SQR_REM_GREEDY

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    Grid<Color> g = ToColorGrid(board);
    Board b;
    b.Init(g, colors, startingSeed);

    Greedy naive;
    auto moves = naive.Solve(b, kMoveCount);
    return ToSolution(moves);
}

#endif

#ifdef SQR_REM_WIDE

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    return square_remover::Wide().playIt(colors, board, startingSeed);
}

#endif

#ifdef SQR_REM_FOUR_MOVE

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    return square_remover::FourMoveSolver().playIt(colors, board, startingSeed);
}

#endif

#ifdef SQR_REM_BEAM

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    Grid<DigitColor> g = ToColorGrid(board);
    Board b(g, colors, startingSeed);
    BeamSearch<LocalSqRm_v2> beam;
    beam.Remove(b, kMoveCount, 10);

    return vector<int>();
}

#endif