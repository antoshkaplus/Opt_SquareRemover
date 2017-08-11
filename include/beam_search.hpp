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
        Play() {}
        Play(const Board& b) {
            Init(b);
        }
        Play(const Play& p) {
            *this = p;
        }

        const Play& operator=(const Play& play) {
            board_ = play.board_;
            state_ = play.state_;
            state_.board_ = &board_;
            return *this;
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

        auto& board() const {
            return board_;
        }

    private:
        Board board_;
        BoardState state_;
    };

    class Derivative {
    public:
        const Play* play;
        Move move;
        double score;

        Derivative() {}
        Derivative(const Play& play, const Move& move, double score)
        : play(&play), move(move), score(score) {}
    };


    // client should look at history himself
    Board Remove(const Board& b, int move_count, int beam_width) {
        LocalSqRm local_sq_rm;

        vector<Play> current;
        vector<Play> next;
        vector<Play> bs;
        vector<Derivative> derivs;
        current.emplace_back(b);
        for (int i = 0; i < move_count; ++i) {
            assert(!current.empty());
            for (auto& play : current) {
                // takes some time to create everyone of them
                play.state().ForEachSqLoc([&](const Location& loc) {
                    auto m = loc.toMove();
                    play.board().ForMove(m, [&]() {
                        local_sq_rm.Init(play.board()).ForRemove(loc.toMove(), [&](const Region& reg) {
                            int sq_move_count = play.state().AfterChangeSqLocs(reg);
                            derivs.emplace_back(play, loc.toMove(), sq_move_count + b.squares_removed());
                        });
                    });
                });
            }

            if (!derivs.empty()) {

                SelectDerivatives(derivs, beam_width);

            } else {

                // let try to find something with another strategy ??
                uniform_int_distribution<> play_distr(0, current.size()-1);
                uniform_int_distribution<> move_distr(0, b.moves().size()-1);
                for (auto i = 0; i < beam_width; ++i) {
                    derivs.emplace_back(current[play_distr(RNG)], b.moves()[move_distr(RNG)], 0);
                }
            }

            for_each(derivs.begin(), derivs.end(), [&](auto& d) {

                next.push_back(*(d.play));
                next.back().board().MakeMove(d.move);
                auto reg = local_sq_rm.Init(next.back().board()).Remove(d.move);
                next.back().state().OnRegionChanged(reg);

            });
            derivs.clear();

            // should use move
            swap(next, current);
            next.clear();
        }
        // from current choose board with max number of removed squares
        auto& res = *max_element(current.begin(), current.end(), [](const Play& b_0, const Play& b_1) {
            return b_0.board().squares_removed() < b_1.board().squares_removed();
        });
        return res.board();
    }
    
    void SelectDerivatives(vector<Derivative>& bs, Count sz) {
        sz = min(sz, (Count)bs.size());
        nth_element(bs.begin(), bs.begin() + sz - 1, bs.end(), [&](const auto& i_0, const auto& i_1) {
            return i_0.score > i_1.score || (i_0.score == i_1.score && i_0.play->board().squares_removed() > i_1.play->board().squares_removed());
        });
        bs.resize(sz);
    }
};

