#pragma once

#include "digit/locator.hpp"
#include "state/state_sq.hpp"
#include "play/deriv.hpp"


namespace play {

template<class TBoard, class TLocalSqRm>
class Play_v1 {
public:
    using Board = TBoard;
    using LocalSqRm = TLocalSqRm;

    struct Deriv : play::Deriv {
        Count sq_removed;
        Count sq_move_count;
        Region ch_reg;
        bool is_remove_move;


        Deriv() {}
        Deriv(const Move& m, Count sq_removed, Count sq_move_count, Region ch_reg, bool is_remove_move)
            : play::Deriv(m), sq_removed(sq_removed), sq_move_count(sq_move_count), is_remove_move(is_remove_move) {}
    };

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

                    auto sq_move_count = sq_state_.AfterChangeSqLocs(reg);
                    auto sq_removed = board_.squares_removed();

                    func({m, sq_removed, sq_move_count, reg, true});
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

                func({m, sq_removed, sq_move_count, reg, false});
            });
        }
    }

    digit::Locator locator_;
    Board board_;
    state::SqState<digit::Locator> sq_state_;
    LocalSqRm local_sq_rm_;
};

}

