#include "local_sq_rm_v1.hpp"
#include "local_sq_rm_v2.hpp"
#include "naive.hpp"
#include "greedy.hpp"
//#include "wide.hpp"
#include "beam_search.hpp"
#include "beam_search_v2.hpp"
#include "score.hpp"
#include "factors.hpp"
#include "play/play_v1.hpp"
#include "play/play_v2.hpp"
#include "play/play_v3.hpp"
#include "play_factory.hpp"


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

#ifdef SQR_REM_BEAM_1

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    using PlayFactory = PlayFactory<play::Play_v1<digit::HashedBoard, LocalSqRm_v2>>;

    Grid<DigitColor> g = ToColorGrid(board);
    digit::HashedBoard b;
    b.Init(g, colors, startingSeed);
    BeamSearch_v2<PlayFactory, Score_v1_1> beam;
    Index group = ProblemGroup(colors, board.size());
    Score_v1_1 s(kLocRates[group]);
    beam.set_score(s);
    PlayFactory f;
    f.min_sq_moves = kMinSqMoves[group];
    beam.set_play_factory(f);
    b = beam.Remove(b, kMoveCount, 16 * 16 * 100 / (board.size()*board.size()));

    auto vec = b.move_history();
    assert(vec.size() == kMoveCount);
    return ToSolution(vec);
}

#endif

#ifdef SQR_REM_BEAM_2

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    using PlayFactory = PlayFactory<play::Play_v2<digit::HashedBoard, LocalSqRm_v2>>;

    Grid<DigitColor> g = ToColorGrid(board);
    digit::HashedBoard b;
    b.Init(g, colors, startingSeed);
    BeamSearch_v2<PlayFactory , Score_v2> beam;
    Index group = ProblemGroup(colors, board.size());
    Score_v2 s(kLocFactors[group], kTripleFactors[group], 0);
    beam.set_score(s);
    PlayFactory f;
    f.min_sq_moves = 3;

    beam.set_play_factory(f);
    b = beam.Remove(b, kMoveCount, 16 * 16 * 100 / (board.size()*board.size()));

    auto vec = b.move_history();
    assert(vec.size() == kMoveCount);
    return ToSolution(vec);
}

#endif

#ifdef SQR_REM_BEAM_3

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    using PlayFactory = PlayFactory<play::Play_v3<digit::HashedBoard, LocalSqRm_v2>>;

    Grid<DigitColor> g = ToColorGrid(board);
    digit::HashedBoard b;
    b.Init(g, colors, startingSeed);
    BeamSearch_v2<PlayFactory, Score_v3> beam;
    Index group = ProblemGroup(colors, board.size());
    Score_v3 s(kLocFactors[group]);
    beam.set_score(s);
    PlayFactory f;
    f.min_sq_moves = 2;
    beam.set_play_factory(f);
    b = beam.Remove(b, kMoveCount, 16 * 16 * 100 / (board.size()*board.size()));

    auto vec = b.move_history();
    assert(vec.size() == kMoveCount);
    return ToSolution(vec);
}

#endif