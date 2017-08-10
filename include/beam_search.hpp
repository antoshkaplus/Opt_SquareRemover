//
//  beam_search.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/9/15.
//
//

#pragma once

#include "board_state.hpp"


template<class LocalSqRm>
class BeamSearch {
public:

    class Play {
    public:
        Play(const Board& b) {
            Init(b);
        }

        void Init(const Board& b) {
            board_ = b;
            state_.Init(board_);
        }

        auto& state() {
            return state_;
        }

        auto& board() {
            return board_;
        }

    private:
        Board board_;
        BoardState state_;
    };

    class Derivative {
        Play* play;
        Move move;
        double score;
    };


    // client should look at history himself
    Board Remove(const Board& b, int move_count, int beam_width) {
        vector<Play> current;
        vector<Play> next;
        vector<Play> bs;
        vector<Derivative> derivs;
        current.push_back(Play(b));
        for (int i = 0; i < move_count; ++i) {
            assert(!current.empty());
            for (auto& b : current) {
                // takes some time to create everyone of them
                b.state().ForEachSqLoc([&](Location& loc) {
                    b.board().ForMove(loc.toMove(), [&]() {
                        b.remover().ForRemove(loc.toMove(), [&](const Region& reg) {
                            int move_count = b.state().AfterChangeSqLocs(reg);
                            derivs.emplace_back(&b, loc.toMove(), move_count + b.board().squares_removed());
                        });
                    });
                });
            }
            auto inds = SelectDerivatives(derivs, beam_width);
            sort(inds.begin(), inds.end());
            for (int i = 0; i < inds.size(); ++i) {
                // for each index we want to copy Play there and make corresponding move
                // fill out next array
            }
            // should use move
            swap(next, current);
            next.clear();
        }
        // from current choose board with max number of removed squares
        auto& res = *max_element(current.begin(), current.end(), [](const Play& b_0, const Play& b_1) {
            return b_0.SquaresRemovedCount() < b_1.SquaresRemovedCount();
        });
        return res;
    }

    // usually is different class
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

