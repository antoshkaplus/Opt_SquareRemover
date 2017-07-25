//
//  beam_search.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/9/15.
//
//

#pragma once 

#include "local_sq_rm.hpp"


class BeamSearch {
public:

    vector<Move> Solve(Board& b, Count move_count) {
        // vector of boards
        // vector of next boards
        // go to next boards
        // random move when nothing to prune.
        // need better score function
        // you have number

    }
    
    // client should look at history himself
    Board Remove(const Board& b, int move_count, int beam_width) {
        vector<Board> current;
        vector<Board> next;
        vector<Board> bs;
        current.push_back(b);
        for (int i = 0; i < move_count; ++i) {
            assert(!current.empty());
            for (auto& b : current) {
                // takes some time to create everyone of them
                bs = b.OneMoveDerivatives(); 
                next.insert(next.end(),
                            make_move_iterator(bs.begin()), 
                            make_move_iterator(bs.end()));
            }
            auto inds = SelectDerivatives(next, beam_width);
            sort(inds.begin(), inds.end());
            for (int i = 0; i < inds.size(); ++i) {
                swap(next[i], next[inds[i]]);
            }
            next.resize(inds.size());
            // should use move
            swap(next, current);
            next.clear();
        }
        // from current choose board with max number of removed squares
        auto& res = *max_element(current.begin(), current.end(), [](const Board& b_0, const Board& b_1) {
            return b_0.SquaresRemovedCount() < b_1.SquaresRemovedCount();
        });
        return res;
    }
    
    static double Score(const Board& b) {
        return - (b.SquaresRemovedCount() + b.OneMoveCount());
    }
    
    
    vector<Index> SelectDerivatives(const vector<Board>& bs, Count sz) {
        sz = min<Count>(sz, bs.size());
        vector<Index> inds(bs.size());
        iota(inds.begin(), inds.end(), 0);
        nth_element(inds.begin(), inds.begin() + sz - 1, inds.end(), [&](short i_0, short i_1) {
            return Score(bs[i_0]) < Score(bs[i_1]);
        });
        inds.resize(sz);
        return inds;
    }
};

