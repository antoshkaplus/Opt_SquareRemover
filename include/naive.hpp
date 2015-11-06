//
//  naive.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/5/15.
//
//

#pragma once

#include "local_sq_rm.hpp"

class NaiveSquareRemover {
public:
    vector<Move> Solve(Board& b, Count move_count) {
        vector<Move> res;
        for (Index i = 0; i < move_count; ++i) {
            
            vector<Move> best_ms;
            Count best_sq_rm = -1;
            Board new_b; 
            
            LocalSquareRemover local;
            local.Init(new_b);
            for (auto& m : b.moves()) {
                new_b = b;
                new_b.MakeMove(m);
                local.Remove(m);
                auto new_sq_rm = new_b.squares_removed();
                if (new_sq_rm == best_sq_rm) {
                    best_ms.push_back(m);
                } else if (new_sq_rm > best_sq_rm) {
                    best_ms.assign(1, m);
                    best_sq_rm = new_sq_rm;
                }
            }
            
            uniform_int_distribution<> distr(0, best_ms.size()-1);
            Index k = distr(RNG);
            b.MakeMove(best_ms[k]);
            res.push_back(best_ms[k]);
            if (best_sq_rm != 0) {
                local.Remove(best_ms[k]);
            }
        }
        return res;
    }


};