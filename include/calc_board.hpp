#pragma once

#include "board.hpp"

class CalculatedBoard : public Board {

    struct TripleCounter {
        void Init(const Board& b) {
            board_ = &b;
        }

        template<Direction dir>
        Count Outer(const Position& p) {
            auto& b = *board_;
            auto& g = board_->grid();
            Count count = 0;
            auto p_1 = p.Shifted(dir);
            if (!g.IsInside(p_1)) {
                return count;
            }
            auto& dd = kDirTurn[dir];
            auto p_2 = p_1.Shifted(dd[0]);
            if (g.IsInside(p_2) && b.IsTriple(p, p_1, p_2)) {
                ++count;
            }
            p_2 = p_1.Shifted(dd[1]);
            if (g.IsInside(p_2) && b.IsTriple(p, p_1, p_2)) {
                ++count;
            }
            return count;
        }

        Count Outer(const Position& p, Direction dir) {
            auto& b = *board_;
            auto& g = board_->grid();
            Count count = 0;
            auto p_1 = p.Shifted(dir);
            if (!g.IsInside(p_1)) {
                return count;
            }
            auto& dd = kDirTurn[dir];
            auto p_2 = p_1.Shifted(dd[0]);
            if (g.IsInside(p_2) && b.IsTriple(p, p_1, p_2)) {
                ++count;
            }
            p_2 = p_1.Shifted(dd[1]);
            if (g.IsInside(p_2) && b.IsTriple(p, p_1, p_2)) {
                ++count;
            }
            return count;
        }

        Count Everything(const Position& p) {
            return OuterAll(p) + Center(p);
        }


        Count OuterAll(const Position& p) {
            return Outer<kDirUp>(p) + Outer<kDirDown>(p) +
                   Outer<kDirRight>(p) + Outer<kDirLeft>(p);
        }

        Count OuterSquare(const Position& p) {
            auto ps = SquarePositions(p);
            Count count = 0;
            Direction dir = kDirLeft;
            for (auto p : ps) {
                count += Center(p);
                count += Outer(p, dir);
                dir = kDirClockwise[dir];
                count += Outer(p, dir);
            }
            return count;
        }

        Count Center(const Position& p) {
            auto& b = *board_;
            auto& g = board_->grid();
            Count count = 0;
            for (Direction d : kDirections) {
                auto p_1 = p.Shifted(d);
                if (!g.IsInside(p_1)) continue;
                auto p_2 = p.Shifted(kDirClockwise[d]);
                if (!g.IsInside(p_2) || !b.IsTriple(p, p_1, p_2)) {
                    continue;
                }
                ++count;
            }
            return count;
        }

        const Board* board_;
    };

    struct DoubleCounter {


    };



public:
    void Init(const Grid<Color>& board, Count color_count, Index seed) {
        Board::Init(board, color_count, seed);
        // count triples  here

        // count doubles here
    }

    void MakeMove(const Move& m) {
        assert(color(m.pos) != color(m.another()));
        triple_count -= counter.Everything(m.pos) + counter.Everything(m.another());
        Board::MakeMove(m);
        triple_count += counter.Everything(m.pos) + counter.Everything(m.another());
    }

    void Remove(const Position& p) {
        triple_count -= counter.OuterSquare(p);
        Board::Remove(p);
        triple_count += counter.OuterSquare(p);
    }

private:

    TripleCounter counter;
    //Count double_count;
    Count triple_count;
};