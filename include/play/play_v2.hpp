#pragma once

#include "play/deriv.hpp"
#include "play/play_v1.hpp"


template<class Board>
class Play_v2 {
public:

    struct Deriv : typename Play_v1::Deriv {
        Count triples;
        Count doubles;

        Deriv() {}
        Deriv(typename Play_v1::Deriv& d, Count triples, Count doubles)
            : typename Play_v1::Deriv(d), triples(tiples), doubles(doubles) {}
    };

    Play_v2() {}
    Play_v2(const Board& b) {
        Init(b);
    }
    Play_v2(const Play& p) {
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

        triple_state_.Init(board_);
        double_state_.Init(board_);
    }

    void Apply(const Deriv& d) {
        base_play_.Apply(d);
        triple_state_.set(d.triples);
        double_state_.set(d.doubles);
    }

    template<class Func>
    void ForEachDeriv(Func func) {
        derivs_.clear();

        base_play_.ForEachDeriv([&](const typename Play_v1::Deriv& d_v1) {
            derivs_.emplace({d_v1, triple_state_.Count(d_v1.ch_reg), double_state_.Count(d_v1.ch_reg)});
        });

        for (auto& d : derivs_) {
            d.triples += triple_state_.triple_count() - triple_state_.Count(d.ch_reg);
            d.doubles += double_state_.double_count() - double_state_.Count(d.ch_reg);

            func(d);
        }
    }

    auto& board() {
        return board_;
    }

    auto& board() const {
        return board_;
    }

private:

    Play_v1 base_play_;

    vector<Deriv> derivs_;
    TripleState triple_state_;
    DoubleState double_state_;
};
