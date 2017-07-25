//
//  greedy.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/10/15.
//
//

#pragma once

#include "local_sq_rm.hpp"
#include "score_calc.hpp"

class Greedy {
    using HashFunction = ZobristHashing<64>;

public:
    vector<Move> Solve(Board& b_in, Count move_count) {
        vector<Move> res;
        HashedBoard b;
        b.Init(b_in.grid(), b_in.color_count(), b_in.seed());
        
        // reuse object. less allocations
        Board new_b; 
        LocalSquareRemover local;
        ScoreCalculator score_calc;
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
            
            Move m;
            Index iteration = 0;
            for (;;) {
                uniform_int_distribution<> distr(0, best_ms.size()-1);
                Index k = distr(RNG);
                b.MakeMove(best_ms[k]);
                // after we made blind move
                if (best_sq_rm > 1 || visited.count(b.hash()) == 0) {
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
            visited.insert(b.hash());
            res.push_back(m);
            local.Init(b);
            local.Remove(m);
        }
        return res;
    }
    
private:
    
};

