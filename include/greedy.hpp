//
//  greedy.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/10/15.
//
//

#pragma once

#include "local_sq_rm.hpp"


class Greedy {
    using HashFunction = ZobristHashing<64>;

public:
    vector<Move> Solve(Board& b, Count move_count) {
        vector<Move> res;
        // reuse object. less allocations
        Board new_b; 
        LocalSquareRemover local;
        ScoreCalculator score_calc;
        HashFunction zobrist_hashing({b.size(), b.size()}, b.color_count());
        // now it should inside board... to keep hash
        unordered_set<HashFunction::value> visited;
        
        for (Index i = 0; i < move_count; ++i) {
            
            vector<Move> best_ms;
            double best_sq_rm = -1;
            
            local.Init(new_b);
            for (auto& m : b.moves()) {               
                if (b.IsIdentityMove(m)) {
                    continue;
                }
                new_b = b;
                new_b.MakeMove(m);
                local.Remove(m);
                score_calc.Init(new_b);
                auto new_sq_rm = score_calc.Compute();
                if (new_sq_rm == best_sq_rm) {
                    best_ms.push_back(m);
                } else if (new_sq_rm > best_sq_rm) {
                    best_ms.resize(1);
                    best_ms[0] = m;
                    best_sq_rm = new_sq_rm;
                }
            }
            
            uniform_int_distribution<> distr(0, best_ms.size()-1);
            Index k = distr(RNG);
            b.MakeMove(best_ms[k]);
            res.push_back(best_ms[k]);
            local.Init(b);
            local.Remove(best_ms[k]);
        }
        return res;
    }
    
private:
    
};

