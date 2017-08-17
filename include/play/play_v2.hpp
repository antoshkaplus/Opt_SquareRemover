#pragma once


template<class Board>
class Play_v2 {
public:

    struct DerivState : Deriv {
        Count triple_diff;
        Count double_diff;
    };

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
        sq_state_.ResetLocator(locator_);

        triple_state_ = play.triple_state_;
        triple_state_.ResetBoard(board_);
        double_state_ = play.double_state_;
        double_state_.ResetBoard(board_);
        return *this;
    }

    void Init(const Board& b) {
        board_ = b;
        locator_.Init(b);
        sq_state_.Init(locator_);

        triple_state_.Init(board_);
        double_state_.Init(board_);
    }

    template<class Func>
    void ForEachDeriv(Board& b, Func func) {
        // play and deriv are very correlated
        // call func with all move + deriv state + score + derivs
        // func is going to be called while board in the end state
    }

    auto& board() {
        return board_;
    }

    auto& board() const {
        return board_;
    }

private:


//    void AddSqLocDerivs(Play& play, vector<Derivative>& derivs) {
//        play.sq_state().ForEachSqMove([&](const Move& m) {
//
//            Count triple_diff_before;
//            Count triple_diff_after;
//            Count double_diff_before;
//            Count double_diff_after;
//            Count sq_removed = 0;
//            Count sq_loc_count;
//
//            Region rem_reg;
//
//            bool exists = false;
//
//            play.board().ForMove(m, [&]() {
//                // gonna be called with region described
//                local_sq_rm_.Init(play.board()).ForRemove(m, [&](const Region& reg) {
//                    if (board_hash_.count(play.board().hash())) {
//                        exists = true;
//                    } else {
//                        board_hash_.insert(play.board().hash());
//
//                        rem_reg = reg;
//
//                        sq_loc_count = play.sq_state().AfterChangeSqLocs(reg);
//                        triple_diff_after = play.triple_state().Count(reg);
//                        double_diff_after = play.double_state().Count(reg);
//                        sq_removed = play.board().squares_removed();
//                    }
//                });
//            });
//            if (exists) return;
//
//            triple_diff_before = play.triple_state().Count(rem_reg);
//            double_diff_before = play.double_state().Count(rem_reg);
//
//            auto score = score_(sq_removed, sq_loc_count,
//                                play.triple_state().triple_count() - triple_diff_before + triple_diff_after,
//                                play.double_state().double_count() - double_diff_before + double_diff_after);
//            derivs.emplace_back(play, m, score, - triple_diff_before + triple_diff_after,- double_diff_before + double_diff_after);
//        });
//    }
//
//    void AddSimpleDerivs(Play& play, vector<Derivative>& derivs) {
//        for (auto& m : play.board().moves()) {
//            if (play.sq_state().IsRemoveMove(m)) continue;
//
//            Region reg{m.pos, m.another()};
//
//            Count triple_diff_before = play.triple_state().Count(reg);
//            Count triple_diff_after;
//            Count double_diff_before = play.double_state().Count(reg);
//            Count double_diff_after;
//            Count sq_move_count;
//            Count sq_removed;
//
//            bool exists = false;
//            play.board().ForMove(m, [&]() {
//
//                if (board_hash_.count(play.board().hash())) {
//                    exists = true;
//                } else {
//                    board_hash_.insert(play.board().hash());
//
//                    sq_move_count = play.sq_state().AfterChangeSqLocs(reg);
//                    triple_diff_after = play.triple_state().Count(reg);
//                    double_diff_after = play.double_state().Count(reg);
//                    sq_removed = play.board().squares_removed();
//                }
//            });
//
//            if (exists) continue;
//
//            auto score = score_(sq_removed, sq_move_count,
//                                play.triple_state().triple_count() - triple_diff_before + triple_diff_after,
//                                play.double_state().double_count() - double_diff_before + double_diff_after);
//            derivs.emplace_back(play, m, score, - triple_diff_before + triple_diff_after, - double_diff_before + double_diff_after);
//        }
//    }


    digit::Locator locator_;
    Board board_;
    SqState<digit::Locator> sq_state_;

    TripleState triple_state_;
    DoubleState double_state_;
};
