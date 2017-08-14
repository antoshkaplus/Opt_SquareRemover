#pragma once

#include "digit/board.hpp"

// TODO fix virtuality
// was created to take care of having loops in moves cuz of score board changes
namespace digit {

class HashedBoard : public Board {
public:
    using HashFunction = ZobristHashing<64>;


    void Init(const Grid<Color>& board, Count color_count, Index seed) override {
        digit::Board::Init(board, color_count, seed);
        hash_function_.reset(new HashFunction(board.size(), color_count));
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
        digit::Board::MakeMove(m);
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

    HashFunction::value hash_;
    shared_ptr<HashFunction> hash_function_;
};

}
