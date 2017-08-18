#pragma once

#include "bit/board.hpp"


namespace bit {

class HashedBoard : public Board {

public:
    using HashFunction = ZobristHashing<64>;

    void Init(const Grid<Color>& board, Count color_count, Index seed) override {
        bit::Board::Init(board, color_count, seed);

        digit_board_.resize(board_.size());
        auto func = [&](const Position& p) {
            digit_board_(p.shifted(1, 1)) = board(p);
        };
        board.ForEachPosition(func);

        hash_function_.reset(new HashFunction({extended_size(), extended_size()}, color_count));
        auto is_filled = [](const Position& p) { return true; };
        auto getter = [&](const Position& p) {
            return color(p);
        };
        hash_ = hash_function_->Hash(is_filled, getter);
    }



    void MakeMove(const Move& m) override {
        auto& h = *hash_function_;
        auto p = m.another();
        h.xorState(&hash_, m.pos, color(m.pos));
        h.xorState(&hash_, p, color(p));
        bit::Board::MakeMove(m);
        h.xorState(&hash_, m.pos, color(m.pos));
        h.xorState(&hash_, p, color(p));
    }

    template<class Func>
    void ForMove(const Move& m, Func func) {
        auto& h = *hash_function_;
        auto p = m.another();
        // out
        h.xorState(&hash_, m.pos, color(m.pos));
        h.xorState(&hash_, p, color(p));
        Board::ForMove(m, [&]() {
            // in
            h.xorState(&hash_, m.pos, color(m.pos));
            h.xorState(&hash_, p, color(p));
            func();
            // out
            h.xorState(&hash_, m.pos, color(m.pos));
            h.xorState(&hash_, p, color(p));
        });
        // in
        h.xorState(&hash_, m.pos, color(m.pos));
        h.xorState(&hash_, p, color(p));

    }

    void Remove(const Position& p) override {
        auto& h = *hash_function_;
        for (auto& pp : SquarePositions(p)) {
            h.xorState(&hash_, pp, color(pp));
        }
        digit::Board::Remove(p);
        for (auto& pp : SquarePositions(p)) {
            h.xorState(&hash_, pp, color(pp));
        }
    }

    HashFunction::value hash() const {
        return hash_;
    }

private:

    Grid<DigitColor> digit_board_;
    HashFunction::value hash_;
    shared_ptr<HashFunction> hash_function_;
};

}