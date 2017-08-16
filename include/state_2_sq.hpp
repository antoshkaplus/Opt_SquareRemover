#pragma once

#include "board.hpp"


class _2_SqState {

    template<class Func>
    void ForEach_4x2(Region reg, Func func) {

        reg.diff(-3, -1, 3, 1);
        reg = reg.intersection(board_->region().diffed(0, 0, -3, -1));

        reg.ForEach([&](const Position& p) {
            // xo
            // ox
            // ox
            // xo
            if (board_->IsFour(p, p.shifted(1, 1), p.shifted(2, 1), p.shifted(3, 0))) {
                func();
            }
            // ox
            // xo
            // xo
            // ox
            if (board_->IsFour(p.shifted(0, 1), p.shifted(1, 0), p.shifted(2, 0), p.shifted(3, 1))) {
                func();
            }

            // ox
            // xo
            // ox
            // xo
            if (board_->IsFour(p.shifted(0, 1), p.shifted(1, 0), p.shifted(2, 1), p.shifted(3, 0))) {
                func();
            }

            // xo
            // ox
            // xo
            // ox
            if (board_->IsFour(p, p.shifted(1, 1), p.shifted(2, 0), p.shifted(3, 1))) {
                func();
            }

            // xo
            // oo
            // ox
            // xx
            if (board_->IsFour(p, p.shifted(2, 1), p.shifted(3, 0), p.shifted(3,1))) {
                func();
            }
            // ox
            // oo
            // xo
            // xx
            if (board_->IsFour(p.shifted(0,1), p.shifted(2,0), p.shifted(3,0), p.shifted(3,1))) {
                func();
            }

            // xx
            // xo
            // oo
            // ox
            if (board_->IsFour(p, p.shifted(0,1), p.shifted(1,0), p.shifted(3,1))) {
                func();
            }

            // xx
            // ox
            // oo
            // xo
            if (board_->IsFour(p, p.shifted(0,1), p.shifted(1,1), p.shifted(3,0))) {
                func();
            }

        });
    }

    template<class Func>
    void ForEach_2x4(Region reg, Func func) {

        reg.diff(-1, -3, 1, 3);
        reg = reg.intersection(board_->region().diffed(0, 0, -1, -3));

        reg.ForEach([&](const Position& p) {
            // oxxo
            // xoox
            if (board_->IsFour(p.shifted(1, 0), p.shifted(0, 1), p.shifted(0, 2), p.shifted(1, 3))) {
                func();
            }
            // xoox
            // oxxo
            if (board_->IsFour(p, p.shifted(1, 1), p.shifted(1, 2), p.shifted(0, 3))) {
                func();
            }

            // xoxo
            // oxox
            if (board_->IsFour(p, p.shifted(1, 1), p.shifted(0, 2), p.shifted(1, 3))) {
                func();
            }

            // oxox
            // xoxo
            if (board_->IsFour(p.shifted(1, 0), p.shifted(0, 1), p.shifted(1, 2), p.shifted(0, 3))) {
                func();
            }

            // xoox
            // xxoo
            if (board_->IsFour(p, p.shifted(1,0), p.shifted(1,1), p.shifted(0,3))) func();

            // xxoo
            // xoox
            if (board_->IsFour(p, p.shifted(1,0), p.shifted(0,1), p.shifted(1,3))) func();

            // xoox
            // ooxx
            if (board_->IsFour(p, p.shifted(1,2), p.shifted(0,3), p.shifted(1,3))) func();

            // ooxx
            // xoox
            if (board_->IsFour(p.shifted(1,0), p.shifted(0,2), p.shifted(0,3), p.shifted(1,3))) func();
        });
    }

    template<class Func>
    void ForEach_3x2(Region reg, Func func) {
        reg.diff(-2, -1, 2, 1);
        reg = reg.intersection(board_->region().diffed(0, 0, -2, -1));

        reg.ForEach([&](const Position& p) {
            auto r = p.row;
            auto c = p.col;
            // xo
            // xx
            // ox
            if (board_->IsFour({r,c}, {r+1,c}, {r+1,c+1}, {r+2,c+1})) {
                func();
            }
            // ox
            // xx
            // xo
            if (board_->IsFour({r,c+1}, {r+1,c+1}, {r+1,c}, {r+2,c})) {
                func();
            }

            // ox
            // ox
            // xx
            if (board_->IsFour({r,c+1}, {r+1,c+1}, {r+2,c}, {r+2,c+1})) {
                func();
            }
            // xo
            // xo
            // xx
            if (board_->IsFour({r,c}, {r+1,c}, {r+2,c}, {r+2,c+1})) {
                func();
            }

            // xx
            // ox
            // ox
            if (board_->IsFour({r,c}, {r,c+1}, {r+1,c+1}, {r+2,c+1})) {
                func();
            }
            // xx
            // xo
            // xo
            if (board_->IsFour({r,c}, {r,c+1}, {r+1,c}, {r+2,c})) {
                func();
            }

            // xx
            // oo
            // xx
            if (board_->IsFour({r,c}, {r,c+1}, {r+1,c}, {r+2,c})) {
                func();
            }
        });
    }

    template<class Func>
    void ForEach_2x3(Region reg, Func func) {
        reg.diff(-1, -2, 1, 2);
        reg = reg.intersection(board_->region().diffed(0, 0, -1, -2));

        reg.ForEach([&](const Position& p) {
            auto r = p.row;
            auto c = p.col;
            // xo
            // xx
            // ox
            if (board_->IsFour({r,c}, {r,c+1}, {r+1,c+1}, {r+1,c+2})) {
                func();
            }
            // ox
            // xx
            // xo
            if (board_->IsFour({r+1,c}, {r+1,c+1}, {r,c+1}, {r,c+2})) {
                func();
            }

            // ox
            // ox
            // xx
            if (board_->IsFour({r+1,c}, {r+1,c+1}, {r,c+2}, {r+1,c+2})) {
                func();
            }
            // xo
            // xo
            // xx
            if (board_->IsFour({r,c}, {r,c+1}, {r,c+2}, {r+1,c+2})) {
                func();
            }

            // xx
            // ox
            // ox
            if (board_->IsFour({r,c}, {r+1,c}, {r+1,c+1}, {r+1,c+2})) {
                func();
            }
            // xx
            // xo
            // xo
            if (board_->IsFour({r,c}, {r+1,c}, {r,c+1}, {r,c+2})) {
                func();
            }

            // xx
            // oo
            // xx
            if (board_->IsFour({r,c}, {r+1,c}, {r,c+1}, {r,c+2})) {
                func();
            }
        });
    }

    template<class Func>
    void ForEach_3x3(Region reg, Func func) {
        reg.diffed(-2, -2, 2, 2).intersection(board_->region().diffed(0, 0, -2, -2));

        reg.ForEach([&](const Position& p) {
            auto r = p.row;
            auto c = p.col;

            // xox
            // oxo
            // xoo
            if (board_->IsFour({r, c}, {r+1, c+1}, {r, c+2}, {r+2, c})) {
                func();
            }
            // oox
            // oxo
            // xox
            if (board_->IsFour({r+2, c+2}, {r+1, c+1}, {r, c+2}, {r+2, c})) {
                func();
            }
            // xoo
            // oxo
            // xox
            if (board_->IsFour({r+2, c+2}, {r+1, c+1}, {r, c}, {r+2, c})) {
                func();
            }
            // xox
            // oxo
            // oox
            if (board_->IsFour({r+2, c+2}, {r+1, c+1}, {r, c}, {r, c+2})) {
                func();
            }

            // x
            // oox
            // xx
            if (board_->IsFour({r, c}, {r+1, c+2}, {r+2, c}, {r+2, c+1})) {
                func();
            }
            // oox
            // xoo
            // oxx
            if (board_->IsFour({r, c+2}, {r+1, c}, {r+2, c+1}, {r+2, c+2})) {
                func();
            }

            // xox
            // oox
            // oxo
            if (board_->IsFour({r, c}, {r, c+2}, {r+1, c+2}, {r+2, c+1})) {
                func();
            }
            // ox
            // oox
            // xox
            if (board_->IsFour({r, c+1}, {r+1, c+2}, {r+2, c}, {r+2, c+2})) {
                func();
            }

            // xx
            // oox
            // xoo
            if (board_->IsFour({r, c}, {r, c+2}, {r+1, c+2}, {r+2, c+1})) {
                func();
            }
            // oxx
            // xo
            //  ox
            if (board_->IsFour({r, c+1}, {r+1, c+2}, {r+2, c}, {r+2, c+2})) {
                func();
            }


            // xox
            // oox
            // oxo
            if (board_->IsFour({r, c}, {r, c+2}, {r+1, c+2}, {r+2, c+1})) {
                func();
            }
            // ox
            // oox
            // xox
            if (board_->IsFour({r, c+1}, {r+1, c+2}, {r+2, c}, {r+2, c+2})) {
                func();
            }


            //  xx
            // oox
            // xoo
            if (board_->IsFour({r, c}, {r, c+2}, {r+1, c+2}, {r+2, c+1})) {
                func();
            }
            // oxx
            // xo
            //  ox
            if (board_->IsFour({r, c+1}, {r+1, c+2}, {r+2, c}, {r+2, c+2})) {
                func();
            }
        });



    }

    const Board *board_;
    Count _2_count_;
};