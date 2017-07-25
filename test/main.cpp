
#include "gtest/gtest.h"

#include "util.hpp"
#include "local_sq_rm.hpp"
#include "naive.hpp"


vector<string> b = {
        "001",
        "011",
        "101"};

TEST(local_sq_rm, removal) {

    Board bb;


    bb.Init(ToColorGrid(b), 2, 1);
    LocalSquareRemover sq_rm;
    sq_rm.Init(bb);
    Move m{{2, 1}, kDirUp};
    //Move m{{2, 0}, kDirRight};
    bb.MakeMove(m);
    cout << bb.grid() << endl;
    sq_rm.Remove(m);
    cout << bb.grid() << endl;
}

TEST(naive, solve) {
    NaiveSquareRemover sq_rm;
    Board bb;
    bb.Init(ToColorGrid(b), 2, 1);

    auto ms = sq_rm.Solve(bb, 1);
    cout << bb.grid();
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::FLAGS_gtest_filter = "*naive*";
    return RUN_ALL_TESTS();
}