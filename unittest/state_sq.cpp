
#include "gtest/gtest.h"

#include "state_sq.hpp"
#include "digit/locator.hpp"
#include "local_sq_rm_v1.hpp"
#include "local_sq_rm_v2.hpp"


static vector<string> board = {
    "001122",
    "010123",
    "001122",
    "444444",
    "040404",
    "000000"
};

vector<Move> moves = {
    {{0,1}, kDirDown}, {{0,2}, kDirDown}, {{0,5}, kDirDown},
    {{1,1}, kDirRight}, {{1,1}, kDirDown}, {{1,2}, kDirDown}, {{1,5}, kDirDown},
    {{4,0}, kDirRight}, {{4,1}, kDirRight}, {{4,2}, kDirRight}, {{4,3}, kDirRight}, {{4,4}, kDirRight}
};

vector<Location> locations = {
    {0, 0, Combo::BotRiBot}, {0, 0, Combo::BotRiRi}, {0, 2, Combo::BotLeLe}, {0, 2, Combo::BotLeBot}, {0, 4, Combo::BotRiBot},
    {1, 0, Combo::TopRiRi}, {1, 2, Combo::TopLeTop}, {1, 2, Combo::TopLeLe}, {1, 4, Combo::TopRiTop},
    {3, 1, Combo::BotRiRi}, {3, 2, Combo::BotLeLe}, {3, 3, Combo::BotRiRi}, {3, 4, Combo::BotLeLe},
    {4, 0, Combo::TopRiRi}, {4, 1, Combo::TopLeLe}, {4, 2, Combo::TopRiRi}, {4, 3, Combo::TopLeLe},
};

TEST(SqState, _1) {
    auto g = ToColorGrid(board);
    digit::Board b;
    b.Init(g, 5, 11);

    digit::Locator locator;
    locator.Init(b);
    SqState<decltype(locator)> sq_state;
    sq_state.Init(locator);

    for_each(moves.begin(), moves.end(), [](Move& m) {m.pos.shift(1,1);});
    for_each(locations.begin(), locations.end(), [](Location& loc) {loc.row+=1; loc.col+=1;});

    unordered_set<Move> expected_moves(moves.begin(), moves.end()), real_moves;
    unordered_set<Location> expected_locs(locations.begin(), locations.end()), real_locs;

    sq_state.ForEachSqLoc([&](const Location& loc) {real_locs.insert(loc);});
    sq_state.ForEachSqMove([&](const Move& m) {real_moves.insert(m);});

    Println(cout, vector<Move>(expected_moves.begin(), expected_moves.end()), string("exp m"));
    Println(cout, vector<Move>(real_moves.begin(), real_moves.end()), string("real m"));
    ASSERT_EQ(expected_moves, real_moves);


    //ASSERT_EQ(expected_locs, real_locs);

    LocalSqRm_v2 sq_rm;
    while (!sq_state.sq_moves_empty()) {
        bool first = true;
        Move M;
        sq_state.ForEachSqMove([&](const Move& m) {
            if (first) {
                first = false;
                M = m;
            }
        });
        b.MakeMove(M);
        auto reg = sq_rm.Init(b).Remove(M);
        sq_state.OnRegionChanged(reg);
        cout << endl << endl;
        b.Print(cout);

        unordered_set<Move> real_moves;
        sq_state.ForEachSqMove([&](const Move& m) {real_moves.insert(m);});
        Println(cout, vector<Move>(real_moves.begin(), real_moves.end()), string("real m"));
    }
    b.Print(cout);
}

TEST(SqState, _2) {
    auto color_count = 4;
    auto str_board = GenerateStrBoard(16, color_count);
    auto g = ToColorGrid(str_board);
    digit::Board b;
    b.Init(g, color_count, 11);

    digit::Locator locator;
    locator.Init(b);
    SqState<decltype(locator)> sq_state;
    sq_state.Init(locator);

    LocalSqRm_v2 sq_rm;
    while (!sq_state.sq_moves_empty()) {
        bool first = true;
        Move M;
        sq_state.ForEachSqMove([&](const Move& m) {
            if (first) {
                first = false;
                M = m;
            }
        });
        b.MakeMove(M);
        auto reg = sq_rm.Init(b).Remove(M);
        sq_state.OnRegionChanged(reg);
    }
    b.Print(cout);
}

TEST(SqState, _3) {
    auto color_count = 4;
    auto str_board = GenerateStrBoard(16, color_count);
    auto g = ToColorGrid(str_board);
    digit::Board b;
    b.Init(g, color_count, 11);

    digit::Locator locator;
    locator.Init(b);
    SqState<decltype(locator)> sq_state;
    sq_state.Init(locator);

    LocalSqRm_v2 sq_rm;
    LocalSqRm_v1 sq_rm_primitive;
    while (!sq_state.sq_moves_empty()) {
        bool first = true;
        Move M;
        sq_state.ForEachSqMove([&](const Move& m) {
            if (first) {
                first = false;
                M = m;
            }
        });
        b.MakeMove(M);

        digit::Board b_copy = b;
        auto reg = sq_rm.Init(b).Remove(M);
        sq_rm_primitive.Init(b_copy).Remove(M);

        ASSERT_EQ(b, b_copy);

        sq_state.OnRegionChanged(reg);
    }
    b.Print(cout);
}

TEST(SqState, _4) {
    auto color_count = 4;
    auto str_board = GenerateStrBoard(16, color_count);
    auto g = ToColorGrid(str_board);
    digit::Board b;
    b.Init(g, color_count, 11);

    digit::Locator locator;
    locator.Init(b);
    SqState<decltype(locator)> sq_state;
    sq_state.Init(locator);



    LocalSqRm_v2 sq_rm;
    LocalSqRm_v1 sq_rm_primitive;

    sq_rm.Init(b).Eliminate();
    for (auto i = 0; i < kMoveCount; ++i) {
        bool first = true;
        Move M;
        if (!sq_state.sq_moves_empty()) {
            sq_state.ForEachSqMove([&](const Move& m) {
                if (first) {
                    first = false;
                    M = m;
                }
            });
        } else {
            // check if really no sq moves
            for (auto m : b.moves()) {
                ASSERT_FALSE(b.IsRemoveMove(m));
            }

            uniform_int_distribution<> distr(0, b.moves().size()-1);
            M = b.moves()[distr(RNG)];
        }
        b.Print(cout);
        cout << "lol" <<endl;

        b.MakeMove(M);

        b.Print(cout);
        cout << "lol" <<endl;

        digit::Board b_copy = b;
        auto reg = sq_rm.Init(b).Remove(M);
        sq_rm_primitive.Init(b_copy).Remove(M);

        if (b != b_copy) {
            cout << endl;
            b.Print(cout);
            cout << endl;
            b_copy.Print(cout);
            cout << endl;
            cout << "lol" << endl;
        }
        ASSERT_EQ(b, b_copy);


        sq_state.OnRegionChanged(reg);
    }
    b.Print(cout);
}