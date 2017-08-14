//
//  naive.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/5/15.
//
//

#pragma once

#include "board.hpp"

template<class Board, class LocalSqRm>
class NaiveSquareRemover {
public:
    vector<Move> Solve(Board& b, Count move_count) {
        vector<Move> res;
        // reuse object. less allocations
        Board new_b;
        LocalSqRm local;
        for (Index i = 0; i < move_count; ++i) {
            
            vector<Move> best_ms;
            Count best_sq_rm = -1;

            local.Init(new_b);
            for (auto& m : b.moves()) {
                new_b = b;
                new_b.MakeMove(m);
                local.Remove(m);
                auto new_sq_rm = new_b.squares_removed();
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
            if (best_sq_rm != 0) {
                local.Init(b);
                local.Remove(best_ms[k]);
            }
        }
        return res;
    }


};









/*
class NestedMonteCarloSearch {
public:
    vector<Move> Solve(Board& b, Count move_count) {
        vector<Move> res;
        // reuse object. less allocations
        Board new_b; 
        LocalSquareRemover local;
        const Count kMaxSamples = 10;
        const Count kDepth = 100;
        vector<Board> bs;
        
        for (Index i = 0; i < move_count; ++i) {
            
            Move best_m;
            Count best_r = 0;
            Count best_r_d = 0;
            if (best_ms.size() > kMaxSamples) {
                shuffle(best_ms.begin(), best_ms.end(), RNG);
                best_ms.resize(kMaxSamples);   
            }
            bs.resize(best_ms.size());
            for (auto i = 0; i < best_ms.size(); ++i) {
                bs[i] = b;
                bs[i].MakeMove(best_ms[i]);
                if (best_sq_rm != 0) {
                    local.Init(bs[i]);
                    local.Remove(best_ms[i]);
                }
                NaiveSquareRemover rr;
                rr.Solve(bs[i], kDepth);
                auto r = bs[i].squares_removed();
                if (r > best_r) {
                    best = best_ms[i];
                    best_r = r;
                }
            }
            
            b.MakeMove(best_m);
            res.push_back(best_m);
            if (best_r != 0) {
                local.Init(b);
                local.Remove(best_m);
            }
        }
        return res;
    }

    
    void SelectMoves(const Board& b, LocalSquareRemover& sq_rm, vector<Move>& ms) {
        vector<Move> best_ms;
        Count best_sq_rm = -1;
        
        local.Init(new_b);
        for (auto& m : b.moves()) {
            new_b = b;
            new_b.MakeMove(m);
            local.Remove(m);
            auto new_sq_rm = new_b.squares_removed();
            if (new_sq_rm == best_sq_rm) {
                best_ms.push_back(m);
            } else if (new_sq_rm > best_sq_rm) {
                best_ms.resize(1);
                best_ms[0] = m;
                best_sq_rm = new_sq_rm;
            }
        }
    }
    
    
    
    
};
*/
