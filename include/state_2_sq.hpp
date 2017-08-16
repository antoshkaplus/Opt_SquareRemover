#pragma once

#include "board.hpp"


class _2_SqState {

    void ForEach_4x2(const Region& reg) {

        reg.diff(-3, -1, 3, 1);
        reg = reg.intersection(board().region().diffed(0, 0, -3, -1));

        reg.ForEach([&](const Position& p) {
            // xo
            // ox
            // ox
            // xo
            if (board_->IsFour(p, p.shifted(1, 1), p.shifted(2, 1), p.shifted(3, 0))) {

            }
            // ox
            // xo
            // xo
            // ox
            if (board_->IsFour(p.shifted(0, 1), p.shifted(1, 0), p.shifted(2, 0), p.shifted(3, 1))) {

            }

            // ox
            // xo
            // ox
            // xo
            if (board_->IsFour(p.shifted(0, 1), p.shifted(1, 0), p.shifted(2, 1), p.shifted(3, 0))) {

            }

            // xo
            // ox
            // xo
            // ox
            if (board_->IsFour(p, p.shifted(1, 1), p.shifted(2, 0), p.shifted(3, 1))) {

            }
        });

    }

    ForEach_2x4(const Region& reg) {

        reg.diff(-1, -3, 1, 3);
        reg = reg.intersection(board().region().diffed(0, 0, -1, -3));

        reg.ForEach([&](const Position& p) {
            // oxxo
            // xoox
            if (board_->IsFour(p.shifted(1, 0), p.shifted(0, 1), p.shifted(0, 2), p.shifted(1, 3))) {

            }
            // xoox
            // oxxo
            if (board_->IsFour(p, p.shifted(1, 1), p.shifted(1, 2), p.shifted(0, 3))) {

            }

            // xoxo
            // oxox
            if (board_->IsFour(p, p.shifted(1, 1), p.shifted(0, 2), p.shifted(1, 3))) {

            }

            // oxox
            // xoxo
            if (board_->IsFour(p.shifted(1, 0), p.shifted(0, 1), p.shifted(1, 2), p.shifted(0, 3))) {

            }
        });

    }

    void ForEach_3x2(const Region& reg) {
        // xo
        // xx
        // ox
        if (cs(r,c)&cs(r+1,c)&cs(r+1,c+1)&cs(r+2,c+1) != 0) {

        }
        // ox
        // xx
        // xo
        if (cs(r,c+1)&cs(r+1,c+1)&cs(r+1,c)&cs(r+2,c) != 0) {

        }

        // ox
        // ox
        // xx
        if (cs(r,c+1)&cs(r+1,c+1)&cs(r+2,c)&cs(r+2,c+1) != 0) {

        }
        // xo
        // xo
        // xx
        if (cs(r,c)&cs(r+1,c)&cs(r+2,c)&cs(r+2,c+1) != 0) {

        }

        // xx
        // ox
        // ox
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c+1)&cs(r+2,c+1) != 0) {

        }
        // xx
        // xo
        // xo
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+2,c) != 0) {

        }

        // xx
        // oo
        // xx
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+2,c) != 0) {

        }
    }

    void ForEach_2x3(const Region& reg) {

    }



    const Board *board_;
};