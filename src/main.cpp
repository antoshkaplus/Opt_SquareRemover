//
//  main.cpp
//  SquareRemover
//
//  Created by Anton Logunov on 4/9/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#include <iostream>

#include "wide.h"
#include "four_move.h"
#include "balance.h"
#include "stingy.h"

using namespace std;
using namespace ant;

struct SquareRemover : square_remover::Solver {
    vector<int> playIt(int colors, vector<string> board, int startingSeed) {
        vector<int> moves;
        moves = sr.playIt(colors, board, startingSeed);
        return moves;
    }
    
    Count removedCount() override {
        return sr.removedCount();
    }
    
    square_remover::Wide sr; 
};

void random_input(Count board_size, Count color_count, vector<string>& board, Count& seed) {
    //srand((uint)time(nullptr));
    board.resize(board_size);
    for (auto r : irange((int)board_size)) {
        string s;
        for (auto c: irange((int)board_size)) {
            s.push_back(rand()%color_count+'0');
        }
        board[r] = s;
    }
    seed = rand();
}

void test_input(size_t& colorCount, vector<string>& board, size_t& startingSeed) {
    colorCount = 6;
    size_t board_size = 16;
    board.resize(board_size);
    for (auto r : ant::irange(board_size)) {
        string s;
        for (auto c: ant::irange(board_size)) {
            s.push_back(rand()%colorCount+'0');
        }
        board[r] = s;
    }
    startingSeed = rand();
}

void outer_input(size_t& colorCount, vector<string>& board, size_t& startingSeed) {
    
    cin >> colorCount;
    
    size_t boardSize;
    cin >> boardSize;
    
    board.resize(boardSize);
    for (auto i : ant::irange(boardSize)) {
        cin >> board[i];
    }
    
    cin >> startingSeed;
}

int main(int argc, const char * argv[])
{
    size_t colorCount;
    vector<string> board;
    size_t startSeed;
    
    size_t starting_seed;
    bool is_outer = false;
    Count board_size;
    Count color_count;
    Count test_count;
    bool is_special = false;
    SquareRemover sr;
    
    if (argc == 1) {
        test_input(colorCount, board, startSeed);
        vector<int> ret;
        ret = sr.playIt((int)colorCount, board, (int)startSeed);
        cout << "finished" << endl;
    }
    else {
        for (int i = 1; i < argc; i+=2) {
            if (argv[i][0] == '-') {
                if (strcmp(argv[i]+1, "exec") == 0) {
                    if (strcmp(argv[i+1], "outer") == 0) {
                        is_outer = true;
                    }
                    if (strcmp(argv[i+1], "special") == 0) {
                        is_special = true;
                    }
                }
                if (strcmp(argv[i]+1, "size") == 0) {
                    board_size = atoi(argv[i+1]);
                }
                if (strcmp(argv[i]+1, "colors") == 0) {
                    color_count = atoi(argv[i+1]);
                }
                if (strcmp(argv[i]+1, "tests") == 0) {
                    test_count = atoi(argv[i+1]);
                }
            }
        }
        if (is_outer) {
            outer_input(colorCount, board, startSeed);
            vector<int> ret;
            ret = sr.playIt((int)colorCount, board, (int)startSeed);
            assert(ret.size() == 30000);
            for (auto i : ant::irange(30000)) {
                cout << ret[i] << endl;
            }
            cout.flush();
        }
        else {
            if (is_special) {
                ofstream lol("score.txt");
                double score = 0;
                for (Count board_size = 8; board_size <= 16; ++board_size) {
                    cout << "start board size " << board_size << endl << endl;
                    for (Count color_count = 4; color_count <= 6; ++color_count) {
                        cout << "start color count " << color_count << endl << endl;
                        for (double d_p = 0.; d_p <= 1.; d_p+=0.1) {
                            for (double d_b = 0.; d_b <= 1.; d_b+=0.1) {
                                cout << "d_p: " << d_p << " d_b: " << d_b << endl;
                                //sr.sr.balance_coeff = d;
                                score = 0;
                                for (Count n = 0; n < 30; ++n) {
                                    random_input(board_size, color_count, board, starting_seed);
                                    sr.sr.board_balance_coeff = d_b;
                                    sr.sr.pos_balance_coeff = d_p;
                                    sr.playIt((int)color_count, board, (uint)starting_seed);
                                    score += sr.sr.removedCount();
                                }
                                score /= 30.;
                                lol << board_size << " " << color_count 
                                    << " " << "d_p: " << d_p << " d_b: " << d_b  
                                    << " score: " << score << endl;
                                lol.flush();
                            }
                        }
                    }
                }
            
            } else {
                cout << "inner test begins" << endl
                << "test count: " << test_count << endl
                << "board size: " << board_size << endl
                << "color count: " << color_count << endl;
                
                size_t result = 0;
                for (uint i = 0; i < test_count; ++i) {
                    random_input(board_size, color_count, board, starting_seed);
                    sr.playIt(color_count, board, starting_seed);
                    result += sr.removedCount();
                }
                cout << result/test_count << endl;
            }
        }
    }
    return 0;
}

















