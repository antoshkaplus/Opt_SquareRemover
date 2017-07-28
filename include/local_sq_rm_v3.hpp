#pragma once

#include "board.hpp"


class LocalSqRm_v3 {

    Simulation Remove(Location loc) {
        Move m{loc};
        return_colors_.emplace(m.p_0, colors_(m.p_0));
        return_colors_.emplace(m.p_0, colors_(m.p_1));

        swap(colors_(m.p_0), colors_(m.p_1));
        // don't need to remove loc right away
        // will be done in RemoveSquares
        RemoveSquares({loc.row, loc.col});



    }

    constexpr static array<Position, 9> PotentialRemovals(const Position& p) {
        int r = p.row;
        int c = p.col;
        return {{   {r-1, c-1}, {r-1, c}, {r-1, c+1},
                    {r, c-1}, {r, c}, {r, c+1},
                    {r+1, c-1}, {r+1, c}, {r+1, c+1} }};
    }

    void RemoveSquares(const Position& p) {
        Region update_one_moves(p, {2, 2});

        vector<Position> ps;
        // we want to take positions from back
        auto order = [](const Position& p_0, const Position& p_1) {
            return p_0.row > p_1.row || (p_0.row == p_1.row && p_0.col > p_1.col);
        };
        vector<Position> removed_ps;
        ps.emplace_back(p.row, p.col);

        while (!ps.empty()) {
            Position p = ps.back();
            ps.pop_back();
            // emplace for goiing back
            return_colors_.emplace(p, ...);
            seed_ = ReplaceColors(colors_, color_count_, p, seed_);

            update_one_moves.Unite({p, {2, 2}});

            for (Position pp : SquarePositions(p)) {
                removed_ps.push_back(pp);
            }
            for (Position pp : PotentialRemovals(p)) {
                auto it = lower_bound(ps.begin(), ps.end(), pp, order);
                if (*it == pp) {
                    continue;
                }
                ps.insert(it, pp);
            }
            while (!ps.empty() && !IsSquare(colors_, ps.back())) ps.pop_back();
        }

        return
    }

};
