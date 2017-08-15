
#include "gtest/gtest.h"

#include "util.hpp"
#include "local_sq_rm_v2.hpp"
#include "local_sq_rm_v3.hpp"
#include "naive.hpp"
#include "greedy.hpp"
#include "digit/board_hashed.hpp"
#include "beam_search.hpp"


vector<string> b = {
        "001",
        "011",
        "101"};

TEST(local_sq_rm, removal) {

    digit::Board bb;


    bb.Init(ToColorGrid(b), 2, 1);
    LocalSqRm_v2 sq_rm;
    sq_rm.Init(bb);
    Move m{{2, 1}, kDirUp};
    //Move m{{2, 0}, kDirRight};
    bb.MakeMove(m);
    bb.Print(cout);
    sq_rm.Remove(m);
    bb.Print(cout);
}

TEST(naive, solve) {
    NaiveSquareRemover<digit::Board, LocalSqRm_v2> sq_rm;
    digit::Board bb;
    bb.Init(ToColorGrid(b), 2, 1);

    auto ms = sq_rm.Solve(bb, 1);
    bb.Print(cout);
}

TEST(local_sq_rm, elimination) {
    auto startingSeed = 1;
    auto colorCount = kColorMin;
    auto strBoard = GenerateStrBoard(kSizeMax, 1);

    Grid<DigitColor> g = ToColorGrid(strBoard);
    digit::Board b;
    b.Init(g, colorCount, startingSeed);

    LocalSqRm_v2 sq_rm;
    sq_rm.Init(b);
    sq_rm.Eliminate();
    cout << b.squares_removed() << endl;
    ASSERT_GT(b.squares_removed(), b.region().cell_count()/4);
}

TEST(local_sq_rm, naive) {
    auto startingSeed = 1;
    auto colorCount = kColorMin;
    auto strBoard = GenerateStrBoard(kSizeMax, colorCount);

    Grid<DigitColor> g = ToColorGrid(strBoard);
    digit::Board b;
    b.Init(g, colorCount, startingSeed);

    RNG.seed(0);
    NaiveSquareRemover<digit::Board, LocalSqRm_v2> solver_2;
    auto b_2 = b;
    auto moves_2 =  solver_2.Solve(b_2, kMoveCount);

    RNG.seed(0);
    NaiveSquareRemover<digit::Board, LocalSqRm_v3> solver_3;
    auto b_3 = b;
    auto moves_3 =  solver_3.Solve(b_3, kMoveCount);

    ASSERT_EQ(moves_2, moves_3);
}


TEST(local_sq_rm, identical) {
    auto startingSeed = 1;
    auto colorCount = kColorMin;
    auto strBoard = GenerateStrBoard(kSizeMax, colorCount);

    Grid<DigitColor> g = ToColorGrid(strBoard);
    digit::Board b;
    b.Init(g, colorCount, startingSeed);

    RNG.seed(0);
    Greedy<digit::Board, LocalSqRm_v2> solver_2;
    auto moves_2 =  solver_2.Solve(b, kMoveCount);

    RNG.seed(0);
    Greedy<digit::Board, LocalSqRm_v3> solver_3;
    auto moves_3 =  solver_3.Solve(b, kMoveCount);

    ASSERT_EQ(moves_2, moves_3);
}

TEST(board_state, _1) {
    // create random board
    // start removing from board
    // make sure that everything is the same in board state
}


TEST(board_state, correctness) {
    // you want to load a board and check number of sq_loc + what are they
}

TEST(beam_search, correctness) {
    auto startingSeed = 1;
    auto colorCount = kColorMin;
    auto strBoard = GenerateStrBoard(kSizeMax, colorCount);

    Grid<DigitColor> g = ToColorGrid(strBoard);
    digit::HashedBoard b;
    b.Init(g, colorCount, startingSeed);

    RNG.seed(0);
    BeamSearch<LocalSqRm_v2> beam;
    // can get a history out of it.. need to use tailing stuff
    auto res_board = beam.Remove(b, 1000, 10);

    cout << res_board.squares_removed() << endl;
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::FLAGS_gtest_filter = "*beam*";
    return RUN_ALL_TESTS();
}