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
#include "state_double.hpp"
#include "digit/board_hashed.hpp"
#include "digit/locator.hpp"
#include "play/play_v1.hpp"


template<class LocalSqRm, class Play>
class BeamSearch {

    using Board = digit::HashedBoard;
    using PlayDeriv = typename Play::Deriv;

    struct Deriv : PlayDeriv {
        Deriv(const Play& play, const PlayDeriv& deriv)
            : PlayDeriv(deriv), play(play) {}

        Play* play;
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
            board_hash_.clear();

            for_each(current.begin(), current.end(), [&](const Play& p) {
                p.ForEachDeriv([&](const PlayDeriv& d) {
                    derivs.emplace_back(p, d);
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

private:

    void SelectDerivatives(vector<Deriv>& bs, Count sz) {
        sz = min(sz, (Count)bs.size());
        nth_element(bs.begin(), bs.begin() + sz - 1, bs.end(), [&](const auto& i_0, const auto& i_1) {
            return i_0.score > i_1.score;
        });
        bs.resize(sz);
    }

    unordered_set<Board::HashFunction::value> board_hash_;
};

