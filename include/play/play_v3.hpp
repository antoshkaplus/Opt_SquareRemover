#pragma once

#include "state/state_adj.hpp"
#include "play/deriv.hpp"
#include "play/play_v1.hpp"


namespace play {

template<class TBoard, class TLocalSqRm>
class Play_v3 {
    using BasePlay = Play_v1<TBoard, TLocalSqRm>;

public:

    using Board = TBoard;
    using LocalSqRm = TLocalSqRm;

    struct Deriv : BasePlay::Deriv {
        Count adj_amount;

        Deriv() {}
        Deriv(const typename BasePlay::Deriv& d, Count adj_amount)
        : BasePlay::Deriv(d), adj_amount(adj_amount) {}
    };

    Play_v3() {}
    Play_v3(const Board& b) {
        Init(b);
    }
    Play_v3(const Play_v3& p) {
        *this = p;
    }

    const Play_v3& operator=(const Play_v3& play) {
        base_play_ = play.base_play_;

        adj_state_ = play.adj_state_;
        adj_state_.ResetBoard(base_play_.board());
        return *this;
    }

    void Init(const Board& b) {
        base_play_.Init(b);

        adj_state_.Init(base_play_.board());
    }

    void Apply(const Deriv& d) {
        base_play_.Apply(d);
        adj_state_.set(d.adj_amount);
    }

    template<class Func>
    void ForEachDeriv(Func func) {
        derivs_.clear();

        base_play_.ForEachDeriv([&](const typename BasePlay::Deriv& d_v1) {
            derivs_.emplace_back(d_v1, adj_state_.Count(d_v1.ch_reg));
        });

        for (auto& d : derivs_) {
            d.adj_amount += adj_state_.adj_amount() - adj_state_.Count(d.ch_reg);

            func(d);
        }
    }

    auto& board() {
        return base_play_.board();
    }

    auto& board() const {
        return base_play_.board();
    }

private:

    BasePlay base_play_;

    vector<Deriv> derivs_;
    state::AdjState adj_state_;
};

}