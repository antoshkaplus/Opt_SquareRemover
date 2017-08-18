//
//  beam_search.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/9/15.
//
//

#pragma once

#include "digit/board_hashed.hpp"
#include "digit/locator.hpp"
#include "play/play_v1.hpp"


// Score has to be able to accept deriv and give back result
template<class Play, class Score>
class BeamSearch {

    using Board = typename Play::Board;
    using PlayDeriv = typename Play::Deriv;
    using LocalSqRm = typename Play::LocalSqRm;

    struct Deriv : PlayDeriv {
        Deriv() {}
        Deriv(const Play& play, const PlayDeriv& deriv)
            : PlayDeriv(deriv), play(&play) {}

        const Play* play;
    };

public:
    // client should look at history himself
    Board Remove(Board b, int move_count, int beam_width) {
        local_sq_rm_.Init(b).Eliminate();

        vector<Play> current;
        vector<Play> next;
        vector<Play> bs;
        vector<Deriv> derivs;
        current.emplace_back(b);

        for (int i = 0; i < move_count; ++i) {
            hashes_.clear();

            for_each(current.begin(), current.end(), [&](Play& p) {
                p.ForEachDeriv([&](const PlayDeriv& d) {
                    //if (hashes_.count(p.board().hash())) return;
                    derivs.emplace_back(p, d);
                    //hashes_.insert(p.board().hash());
                });
            });

            SelectDerivatives(derivs, beam_width);

            for_each(derivs.begin(), derivs.end(), [&](auto& d) {

                next.push_back(*(d.play));
                next.back().Apply(d);

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

    template<class S>
    void set_score(S&& s) {
        score_ = forward<S>(s);
    }

private:

    void SelectDerivatives(vector<Deriv>& bs, Count sz) {
        sz = min(sz, (Count)bs.size());
        nth_element(bs.begin(), bs.begin() + sz - 1, bs.end(), [&](const auto& i_0, const auto& i_1) {
            return score_(i_0) > score_(i_1);
        });
        bs.resize(sz);
    }

    Score score_;
    LocalSqRm local_sq_rm_;
    unordered_set<typename Board::HashFunction::value> hashes_;
};

