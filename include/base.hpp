//
//  base.h
//  SquareRemover
//
//  Created by Anton Logunov on 4/12/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __SquareRemover__base__
#define __SquareRemover__base__

#include <vector>
#include <cassert>
#include <random>
#include <array>
#include <queue>
#include <unordered_set>

#include "ant/core/core.hpp"
#include "ant/grid/grid.hpp"

#include "util.hpp"
#include "local_sq_rm.hpp"



struct DoubleMove : Move {
    DoubleMove() {}
    DoubleMove(const Move& m, const Move& next)
        : Move(m), next(next) {}
    Index index() {
        Index i = 0;
        i = next.index() << 12 | Move::index();
        return i;
    }
    Move next;
};

ostream& operator<<(ostream& output, const Move& m);

/* uses rand() function, so be careful and probably init srand() */
struct Base {
    vector<int> playIt(int colors, vector<string> board, int startingSeed) {
        vector<int> moves;
        init(colors, board, startingSeed);
        remover_.FindRemove();
        for (auto i = 0; i < kMoveCount; ++i) {
            Move m = board_.randomMove();
            board_.MakeMove(m);
            moves.push_back(m.pos.row);
            moves.push_back(m.pos.col);
            moves.push_back(m.dir);
            remover_.FindRemove();
        }
        return moves;
    }

protected:

    virtual void init(int color_count, const vector<string>& board, Seed starting_seed) {
        srand((uint)starting_seed);

        Grid<Color> color_board_(board.size(), board.size());
        for (auto row = 0; row < board.size(); ++row) {
            for (auto col = 0; col < board.size(); ++col) {
                color_board_(row, col) = board[row][col] - '0';
            }
        }
        board_.Init(color_board_, color_count, starting_seed);
    }

    bool isSameColor(const Position& p_0,
                     const Position& p_1) {
        return board_.color(p_0) == board_.color(p_1);
    }

    Count countColorInSquare(const Position& p, Color c) {
        return (board_.color(p) == c) +
               (board_.color(p.row  , p.col+1) == c) +
               (board_.color(p.row+1, p.col  ) == c) +
               (board_.color(p.row+1, p.col+1) == c);
    }

    virtual bool isFourSquare(const Position& p) {
        return board_.IsSquare(p);
    }

    static bool isInsideSquare(const Position& square, const Position& pos) {
        return pos.row >= square.row   &&
               pos.row <  square.row+2 &&
               pos.col >= square.col   &&
               pos.col <  square.col+2;
    }

protected:

    int eliminationDegree(const Move& m) {
        Position p = m.pos;
        auto sz = board_.size();
        bool b_0, b_1;
        board_.ForMove(m, [&]() {
            Position p = m.pos;
            switch(m.dir) {
                case kDirUp:
                    p = m.another();
                case kDirDown:
                    b_0 =
                        (p.row > 0 &&
                         ((p.col > 0               && isFourSquare(p + Indent{-1, -1})) ||
                          (p.col < sz-1   && isFourSquare(p + Indent{-1,  0}))));
                    b_1 =
                        (p.row < sz-2 &&
                         ((p.col > 0               && isFourSquare(p + Indent{ 1, -1})) ||
                          (p.col < sz-1   && isFourSquare(p + Indent{ 1,  0}))));
                    break;
                case kDirLeft:
                    p = m.another();
                case kDirRight:
                    b_0 =
                        (p.col > 0 &&
                         ((p.row > 0               && isFourSquare(p + Indent{-1, -1})) ||
                          (p.row < sz-1   && isFourSquare(p + Indent{ 0, -1}))));
                    b_1 =
                        (p.col < sz-2 &&
                         ((p.row > 0               && isFourSquare(p + Indent{-1,  1})) ||
                          (p.row < sz-1   && isFourSquare(p + Indent{ 0,  1}))));
                    break;
                default: assert(false);
            }
        });
        return b_0 + b_1;
    }

    // validation in case previous move affected precomputed
    bool canEliminate(const Move& m) {
        return eliminationDegree(m) > 0;
    }

protected:
    Region boardRectangle() {
        return Region(0, 0, board_.size(), board_.size());
    }

    /* you can use fourMoves methods where you want
       probably they belong to base class */
    vector<Move> fourMoves() {
        return fourMoves(boardRectangle());
    }

    vector<Move> fourMoves(const Region& rect) {
        vector<Move> sqs;
        unordered_set<Index> sqs_inds;
        auto
        r_begin = rect.row_begin(),
        r_end   = rect.row_end(),
        c_begin = rect.col_begin(),
        c_end   = rect.col_end();
        for (auto r = r_begin; r < r_end-1; ++r) {
            for (auto c = c_begin; c < c_end-1; ++c) {
                Position p{r, c};
                searchFourMoves(p, rect, sqs, sqs_inds);
            }
        }
        return sqs;
    }


    // all four moves of special color
    vector<Move> fourMoves(const Region& rect, Color color) {
        vector<Move> sqs;
        unordered_set<Index> sqs_inds;
        auto
        r_begin = rect.row_begin(),
        r_end   = rect.row_end(),
        c_begin = rect.col_begin(),
        c_end   = rect.col_end();
        Position p;
        for (auto r = r_begin; r < r_end-1; ++r) {
            for (auto c = c_begin; c < c_end-1; ++c) {
                p.set(r, c);
                if (countColorInSquare(p, color) == 3) {
                    // call will retrigger comparison procedure
                    searchFourMoves(p, rect, sqs, sqs_inds);
                }
            }
        }
        return sqs;
    }

    // we look for for moves inside a rectangle
    // sqs - existing moves
    // sqs_inds - existsing indexes of those moves
    // we guard / protect against duplicates
    virtual void searchFourMoves(const Position& position, const Region& rect,
                         vector<Move>& sqs, unordered_set<Index>& sqs_inds) {
        Int r = position.row,
        c = position.col;
        Position
        p_m,
        p_0 = {r, c},
        p_1 = {r, c+1},
        p_2 = {r+1, c},
        p_3 = {r+1, c+1};
        pair<Position, Move> sqs_pair;
        sqs_pair.first = p_0;
        bool
        b_0_1 = isSameColor(p_0, p_1),
        b_1_2 = isSameColor(p_1, p_2),
        b_2_3 = isSameColor(p_2, p_3),
        b_0_3 = isSameColor(p_0, p_3);

        Count count = 0;
        if (b_0_1 && b_1_2) {
            p_m = p_3+Indent{1, 0};
            if (p_3.row < rect.row_end()-1 && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_3, kDirDown));
            }
            p_m = p_3+Indent{0, 1};
            if (p_3.col < rect.col_end()-1 && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_3, kDirRight));
            }
        }
        if (b_0_1 && b_0_3) {
            p_m = p_2+Indent{1, 0};
            if (p_2.row < rect.row_end()-1 && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_2, kDirDown));
            }
            p_m = p_2+Indent{0,-1};
            if (p_2.col > rect.col_begin() && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kDirRight));
            }
        }
        if (b_2_3 && b_0_3) {
            p_m = p_1+Indent{-1,0};
            if (p_1.row > rect.row_begin() && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kDirDown));
            }
            p_m = p_1+Indent{0, 1};
            if (p_1.col < rect.col_end()-1 && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_1, kDirRight));
            }
        }
        if (b_1_2 && b_2_3) {
            p_m = p_0+Indent{-1,0};
            if (p_0.row > rect.row_begin() && isSameColor(p_1, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kDirDown));
            }
            p_m = p_0+Indent{0,-1};
            if (p_0.col > rect.col_begin() && isSameColor(p_1, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kDirRight));
            }
        }

        // remove repeating moves
        for (Index i = sqs.size()-count; i < sqs.size();) {
            auto m = sqs[i];
            auto ind = m.index();
            if (sqs_inds.count(ind)) {
                swap(sqs[i], sqs.back());
                sqs.pop_back();
            }
            else {
                sqs_inds.insert(ind);
                ++i;
            }
        }
    }
public:
    Count removedCount() {
        return board_.squares_removed();
    }

protected:
    Board board_;
    LocalSquareRemover remover_;

//friend struct Stingy;
//friend struct Last;
friend struct Wide;
};

#endif /* defined(__SquareRemover__base__) */
