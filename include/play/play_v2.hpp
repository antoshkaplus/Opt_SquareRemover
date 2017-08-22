#pragma once

#include "state/state_triple.hpp"
#include "state/state_double.hpp"
#include "play/deriv.hpp"
#include "play/play_v1.hpp"


namespace play {

template<class TBoard, class TLocalSqRm>
class Play_v2 {
    using BasePlay = Play_v1<TBoard, TLocalSqRm>;

public:

    using Board = TBoard;
    using LocalSqRm = TLocalSqRm;

    struct Deriv : BasePlay::Deriv {
        Count triples;
        Count doubles;

        Deriv() {}
        Deriv(const typename BasePlay::Deriv& d, Count triples, Count doubles)
            : BasePlay::Deriv(d), triples(triples), doubles(doubles) {

            assert(triples >= 0);
        }
    };

    Play_v2() {}
    Play_v2(const Board& b) {
        Init(b);
    }
    Play_v2(const Play_v2& p) {
        *this = p;
    }

    const Play_v2& operator=(const Play_v2& play) {
        base_play_ = play.base_play_;

        triple_state_ = play.triple_state_;
        triple_state_.ResetBoard(base_play_.board());
        double_state_ = play.double_state_;
        double_state_.ResetBoard(base_play_.board());
        return *this;
    }

    void Init(const Board& b) {
        base_play_.Init(b);

        triple_state_.Init(base_play_.board());
        double_state_.Init(base_play_.board());
    }

    void Apply(const Deriv& d) {
        base_play_.Apply(d);

        triple_state_.set(d.triples);
        double_state_.set(d.doubles);
    }

    template<class Func>
    void ForEachDeriv(Func func) {
        derivs_.clear();

        base_play_.ForEachDeriv([&](const typename BasePlay::Deriv& d_v1) {
            derivs_.emplace_back(d_v1, triple_state_.Count(d_v1.ch_reg), double_state_.Count(d_v1.ch_reg));
        });

        for (auto& d : derivs_) {
            d.triples += triple_state_.triple_count() - triple_state_.Count(d.ch_reg);
            d.doubles += double_state_.double_count() - double_state_.Count(d.ch_reg);

            assert(d.triples >= 0);

            func(d);
        }
    }

    auto& board() {
        return base_play_.board();
    }

    auto& board() const {
        return base_play_.board();
    }

    auto& base_play() {
        return base_play_;
    }

private:

    BasePlay base_play_;

    vector<Deriv> derivs_;
    state::TripleState triple_state_;
    state::DoubleState double_state_;
};

}