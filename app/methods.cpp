#include "local_sq_rm_v1.hpp"
#include "local_sq_rm_v2.hpp"
#include "naive.hpp"
#include "greedy.hpp"
//#include "wide.hpp"
#include "beam_search.hpp"
#include "score.hpp"
#include "factors.hpp"
#include "play/play_v1.hpp"


class SquareRemover {
public:
    std::vector<int> playIt(int colors, std::vector<std::string> board, int startingSeed);
};

#ifdef SQR_REM_NAIVE

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    Grid<Color> g = ToColorGrid(board);
    digit::Board b;
    b.Init(g, colors, startingSeed);

    NaiveSquareRemover<digit::Board, LocalSqRm_v2> naive;
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
    digit::Board b;
    b.Init(g, colors, startingSeed);
    BeamSearch<play::Play_v1<decltype(b), LocalSqRm_v2>, Score_v1> beam;
    Index group = ProblemGroup(colors, board.size());
    Score_v1 s(kLocFactors[group]);
    b = beam.Remove(b, kMoveCount, 16 * 16 * 100 / (board.size()*board.size()));

    auto vec = b.move_history();
    assert(vec.size() == kMoveCount);
    return ToSolution(vec);
}

#endif