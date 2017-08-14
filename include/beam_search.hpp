//
//  beam_search.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/9/15.
//
//

#pragma once

#include "state_sq.hpp"
#include "state_triple.hpp"
#include "digit/board_hashed.hpp"


template<class LocalSqRm>
class BeamSearch {

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

        auto& sq_state() const {
            return sq_state_;
        }

        auto& triple_state() const {
            return triple_state_;
        }

        auto& board() {
            return board_;
        }

        auto& board() const {
            return board_;
        }

    private:
        digit::HashedBoard board_;
        SqState sq_state_;
        TripleState triple_state_;
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

public:
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

                // we need to do this stuff for each move
                play.state().ForEachSqLoc([&](const Location& loc) {
                    auto m = loc.toMove();
                    auto triple_count = 0;
                    auto removed_count = 0;
                    auto loc_count = 0;

                    Region saved_reg;

                    play.board().ForMove(m, [&]() {
                        // gonna be called with region described
                        local_sq_rm.Init(play.board()).ForRemove(loc.toMove(), [&](const Region& reg) {
                            int sq_move_count = play.state().AfterChangeSqLocs(reg);
                            derivs.emplace_back(play, loc.toMove(), sq_move_count + b.squares_removed());

                            // use some sort of temporary functions.
                            // this should also work on locations but for that you need to know how many locations on particular region.
                            // tedious computation. easier to just invalidate / validate, count
                            play.triple_state().OnRegionChanged(saved_reg = reg);
                        });
                    });
                    play.triple_state().OnBeforeRegionChanged(saved_reg);

                    triple_count = play.triple_state().triple_count();

                    // and now I need to get to previous state somehow
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

private:
    double Score(int sq_removed, int locs, int triples) {
        return sq_removed + 0.95 * locs + 0.01 * triples;
    }


    void SelectDerivatives(vector<Derivative>& bs, Count sz) {
        sz = min(sz, (Count)bs.size());
        nth_element(bs.begin(), bs.begin() + sz - 1, bs.end(), [&](const auto& i_0, const auto& i_1) {
            return i_0.score > i_1.score || (i_0.score == i_1.score && i_0.play->board().squares_removed() > i_1.play->board().squares_removed());
        });
        bs.resize(sz);
    }
};

