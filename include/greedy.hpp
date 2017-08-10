//
//  greedy.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/10/15.
//
//

#pragma once

#include "score_calc.hpp"


template<class LocalSqRem>
class Greedy {
    using HashFunction = ZobristHashing<64>;

public:
    vector<Move> Solve(Board& b_in, Count move_count) {
        vector<Move> res;
        Board b;
        b.Init(b_in.grid(), b_in.color_count(), b_in.seed());
        
        // reuse object. less allocations
        Board new_b; 
        LocalSqRem local;
        ScoreCalculator score_calc;

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
            
            Move m;
            Index iteration = 0;
            for (;;) {
                uniform_int_distribution<> distr(0, best_ms.size()-1);
                Index k = distr(RNG);
                b.MakeMove(best_ms[k]);
                // after we made blind move
                if (best_sq_rm > 1) {
                    m = best_ms[k];
                    break;
                }
                b.MakeMove(best_ms[k]);
                if (++iteration == 10) {
                    // made move that didn't go to visited, the idea was probably to leave the main loop too
                    uniform_int_distribution<> distr(0, b.moves().size()-1);
                    for (;;) {
                        m = b.moves()[distr(RNG)];
                        if (b.IsIdentityMove(m)) continue;
                        b.MakeMove(m);
                        break;
                    }
                }
            }
            res.push_back(m);
            local.Init(b);
            local.Remove(m);
        }
        return res;
    }
    
private:
    
};

