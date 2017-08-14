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
#include "digit/locator.hpp"


template<class LocalSqRm>
class BeamSearch {

    using Board = digit::HashedBoard;

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
            locator_ = play.locator_;
            locator_.ResetBoard(board_);
            sq_state_ = play.sq_state_;
            sq_state_.locator_ = &locator_;
            triple_state_ = play.triple_state_;
            triple_state_.ResetBoard(board_);
            return *this;
        }

        void Init(const Board& b) {
            board_ = b;
            locator_.Init(b);
            sq_state_.Init(locator_);
            triple_state_.Init(board_);
        }

        auto& sq_state() const {
            return sq_state_;
        }

        auto& sq_state() {
            return sq_state_;
        }

        auto& triple_state() const {
            return triple_state_;
        }

        auto& triple_state() {
            return triple_state_;
        }

        auto& board() {
            return board_;
        }

        auto& board() const {
            return board_;
        }

    private:
        digit::Locator locator_;
        Board board_;
        SqState<digit::Locator> sq_state_;
        TripleState triple_state_;
    };

    class Derivative {
    public:
        const Play* play;
        Move move;
        double score;
        Count triple_diff;

        Derivative() {}
        Derivative(const Play& play, const Move& move, double score=0, Count triple_diff=0)
        : play(&play), move(move), score(score), triple_diff(triple_diff) {}
    };

public:
    // client should look at history himself
    Board Remove(const Board& b, int move_count, int beam_width) {

        vector<Play> current;
        vector<Play> next;
        vector<Play> bs;
        vector<Derivative> derivs;
        current.emplace_back(b);

        auto addSqLocs = std::bind(&BeamSearch<LocalSqRm>::AddSqLocDerivs, this, placeholders::_1, std::ref(derivs));
        auto addSimple = std::bind(&BeamSearch<LocalSqRm>::AddSimpleDerivs, this, placeholders::_1, std::ref(derivs));

        for (int i = 0; i < move_count; ++i) {
            board_hash_.clear();

            for_each(current.begin(), current.end(), addSqLocs);

            if (derivs.empty()) {

                for_each(current.begin(), current.end(), addSimple);

                if (derivs.empty()) throw runtime_error("unable to find derivatives");
            }

            SelectDerivatives(derivs, beam_width);

            for_each(derivs.begin(), derivs.end(), [&](auto& d) {

                next.push_back(*(d.play));
                next.back().board().MakeMove(d.move);
                auto reg = local_sq_rm_.Init(next.back().board()).Remove(d.move);
                next.back().sq_state().OnRegionChanged(reg);
                next.back().triple_state().IncreaseBy(d.triple_diff);

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

    void AddSqLocDerivs(Play& play, vector<Derivative>& derivs) {
        play.sq_state().ForEachSqMove([&](const Move& m) {

            Count triple_diff_before;
            Count triple_diff_after;
            Count sq_removed = 0;
            Count sq_loc_count;

            Region rem_reg;

            bool exists = false;

            play.board().ForMove(m, [&]() {
                // gonna be called with region described
                local_sq_rm_.Init(play.board()).ForRemove(m, [&](const Region& reg) {
                    if (board_hash_.count(play.board().hash())) {
                        exists = true;
                    } else {
                        board_hash_.insert(play.board().hash());

                        rem_reg = reg;

                        sq_loc_count = play.sq_state().AfterChangeSqLocs(reg);
                        triple_diff_after = play.triple_state().Count(reg);
                        sq_removed = play.board().squares_removed();
                    }
                });
            });
            if (exists) return;

            triple_diff_before = play.triple_state().Count(rem_reg);

            auto score = Score(sq_removed, sq_loc_count, play.triple_state().triple_count() - triple_diff_before + triple_diff_after);
            derivs.emplace_back(play, m, score, triple_diff_before + triple_diff_after);
        });
    }

    void AddSimpleDerivs(Play& play, vector<Derivative>& derivs) {
        for (auto& m : play.board().moves()) {
            if (play.sq_state().IsRemoveMove(m)) continue;

            Region reg{m.pos, m.another()};

            Count triple_diff_before = play.triple_state().Count(reg);
            Count triple_diff_after;
            Count sq_move_count;
            Count sq_removed;

            bool exists = false;
            play.board().ForMove(m, [&]() {

                if (board_hash_.count(play.board().hash())) {
                    exists = true;
                } else {
                    board_hash_.insert(play.board().hash());

                    sq_move_count = play.sq_state().AfterChangeSqLocs(reg);
                    triple_diff_after = play.triple_state().Count(reg);
                    sq_removed = play.board().squares_removed();
                }
            });

            if (exists) continue;

            auto score = Score(sq_removed, sq_move_count, play.triple_state().triple_count() - triple_diff_before + triple_diff_after);
            derivs.emplace_back(play, m, score, triple_diff_before + triple_diff_after);
        }
    }

    double Score(int sq_removed, int locs, int triples) {
        return sq_removed + 0.95 * locs + 0.01 * triples;
    }

    void SelectDerivatives(vector<Derivative>& bs, Count sz) {
        sz = min(sz, (Count)bs.size());
        nth_element(bs.begin(), bs.begin() + sz - 1, bs.end(), [&](const auto& i_0, const auto& i_1) {
            return i_0.score > i_1.score;
        });
        bs.resize(sz);
    }

    LocalSqRm local_sq_rm_;
    unordered_set<Board::HashFunction::value> board_hash_;
};

