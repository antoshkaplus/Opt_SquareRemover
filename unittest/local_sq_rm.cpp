
#include "gtest/gtest.h"

#include "local_sq_rm_v2.hpp"


class Test {
    Grid<Color> color_grid;
    int sq_loc_count;
    int sq_move_count;
};

class LocalSqRmTest : public ::testing::TestWithParam<vector<Position>> {



};

// could go with random moves
INSTANTIATE_TEST_CASE_P(Predefined,
                        LocalSqRmTest,
                        ::testing::Values(Test{{0,0,1,1,2,2},
                                               {0,1,0,1,2,3},
                                               {}},
                                          ));