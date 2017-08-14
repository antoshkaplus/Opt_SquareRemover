
#include "gtest/gtest.h"

#include "board_restore.hpp"
#include "digit/board.hpp"


class BoardRestoreTest : public ::testing::TestWithParam<vector<Position>> {
protected:
    virtual void SetUp() {

        RNG.seed(0);

        auto startingSeed = 1;
        auto colorCount = kColorMin;
        auto strBoard = GenerateStrBoard(kSizeMax, colorCount);

        Grid<DigitColor> g = ToColorGrid(strBoard);

        board.Init(g, colorCount, startingSeed);
        boardCopy = board;

        boardRestore.Init(board);
    }

    digit::Board board;
    digit::Board boardCopy;
    BoardRestore boardRestore;
};

// could go with random moves
INSTANTIATE_TEST_CASE_P(Predefined,
                        BoardRestoreTest,
                        ::testing::Values(vector<Position>{},
                                          vector<Position>{{1,1}},
                                          vector<Position>{{1,1},{2,3},{2,2},{3,3}},
                                          vector<Position>{{14,14},{14,13}}));

TEST_P(BoardRestoreTest, _1) {

    for (auto& p : GetParam()) {
        boardRestore.Save(p);
        board.Remove(p);
    }
    boardRestore.Restore();

    ASSERT_EQ(board, boardCopy);
}