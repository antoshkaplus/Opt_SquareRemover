#pragma once

#include "board.hpp"


class BoardRestore {
    Board* board_;
    Index seed_;
    Count sq_removed_;
    vector<BitColor> colors_;
    vector<Position> sqs_;

public:

    void Init(Board& board) {
        board_ = &board;
        seed_ = board.seed();
        sq_removed_ = board.squares_removed();
    }

    void Save(const Position& pos) {
        colors_.push_back(board_->color(pos));
        colors_.push_back(board_->color(pos.shifted(1, 0)));
        colors_.push_back(board_->color(pos.shifted(1, 0)));
        colors_.push_back(board_->color(pos.shifted(1, 1)));
        sqs_.push_back(pos);
    }

    void Restore() {
        board_->seed_ = seed_;
        board_->squares_removed_ = sq_removed_;
        while (!sqs_.empty()) {
            auto& sq = sqs_.back();

            board_->bit_board_(sq.shifted(1, 1)) = colors_.back();
            colors_.pop_back();
            board_->bit_board_(sq.shifted(1, 0)) = colors_.back();
            colors_.pop_back();
            board_->bit_board_(sq.shifted(1, 0)) = colors_.back();
            colors_.pop_back();
            board_->bit_board_(sq) = colors_.back();
            colors_.pop_back();

            sqs_.pop_back();
        }
    }
};