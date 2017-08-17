#pragma once


struct Deriv {
    double score;
    Move move;

    Deriv() {}
    Deriv(const Move& m, double s)
        : score(s), move(m) {}

};


template<class Board, class LocalSqRm, class Score>
class Play_v1 {
    using Deriv = ::Deriv;

    Play_v1() {}
    Play_v1(const Board& b) {
        Init(b);
    }
    Play_v1(const Play_v1& p) {
        *this = p;
    }

    void Init(const Board& b) {
        board_ = b;
        locator_.Init(b);
        sq_state_.Init(locator_);
    }

    const Play_v1& operator=(const Play_v1& play) {
        board_ = play.board_;
        locator_ = play.locator_;
        locator_.ResetBoard(board_);
        sq_state_ = play.sq_state_;
        sq_state_.ResetLocator(locator_);
        return *this;
    }

    void Apply(const Deriv& d) {
        board_.MakeMove(d.move);
        auto reg = local_sq_rm_.Init(board_).Remove(d.move);
        sq_state_.OnRegionChanged(reg);
    }

    template<class Func>
    void ForEachDeriv(Func func) {

        if (sq_state_.sq_moves_empty()) {
            AddSimpleDerivs(func);
        } else {
            AddSqLocDerivs(func);
        }

    }

    void set_score(const Score& score) {
        score_ = score;
    }

    auto& board() const {
        return board_;
    }

private:

    template<class Func>
    void AddSqLocDerivs(Func func) {
        sq_state_.ForEachSqMove([&](const Move& m) {

            board_.ForMove(m, [&]() {
                // gonna be called with region described
                local_sq_rm_.Init(board_).ForRemove(m, [&](const Region& reg) {

                    auto sq_loc_count = sq_state_.AfterChangeSqLocs(reg);
                    auto sq_removed = board_.squares_removed();

                    auto score = score_(sq_removed, sq_loc_count);
                    func({m, score});
                });
            });
        });
    }

    template<class Func>
    void AddSimpleDerivs(Func func) {
        for (auto& m : board_.moves()) {

            Region reg{m.pos, m.another()};

            board_.ForMove(m, [&]() {

                auto sq_move_count = sq_state_.AfterChangeSqLocs(reg);
                auto sq_removed = board_.squares_removed();

                auto score = score_(sq_removed, sq_move_count);
                func({m, score});
            });
        }
    }

    digit::Locator locator_;
    Board board_;
    SqState<digit::Locator> sq_state_;
    LocalSqRm local_sq_rm_;
    Score score_;
};

