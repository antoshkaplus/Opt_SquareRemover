//
//  board_simulation.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 1/15/15.
//
//

#ifndef SquareRemover_board_simulation_hpp
#define SquareRemover_board_simulation_hpp

#include <stack>

#include "board.hpp"

namespace square_remover {
    
struct Simulation {
    unordered_set<Location> removed_hashes;
    vector<Location> new_hashes;
};

// trying to put functionality of board_2 in separate class ???
struct BoardSimulation {
    
    struct Return {
        Position pos;
        char color;
    };

    OriginGrid<Grid<char>> colors_;
    stack<Return> return_colors_;
     unordered_set<Location> existing_hashes_;
    int seed_;
    int color_count_;

    Simulation sim_;


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

        for (auto p : removed_ps) {
            // value is changing inside RemoveLocation
            auto node = board_
        }
        UpdateOneMoves(update_one_moves);
    }


    void AddLocation(char r, char c, char combo) {
        AddLocation({r, c, combo});
    }

    void AddLocation(Location loc) {
        if (existing_hashes_.count(loc) != 0) return;
        sim_.new_hashes.push_back(loc);
    }

    // multiple loops
    // less work for each one
    void UpdateOneMoves_1(const Region& region) {
        const auto& cs = colors_;

        // o - (r,c)

        // o0x
        // 0x0
        // x0x
        for (int r = region.row_begin() - 1; r < region.row_end(); ++r) {
            for (int c = region.col_begin() - 2; c < region.col_end(); ++c) {
                if (cs(r, c) == cs(r, c+1) && cs(r, c) == cs(r+1, c)) {
                    if (cs(r, c) == cs(r+1, c+2)) {
                        AddLocation(r, c, 2);
                    }
                    if (cs(r, c) == cs(r+2, c+1)) {
                        AddLocation(r, c, 3);
                    }
                }
            }
        }
        // x0o
        // 0x0
        // x0x
        for (int r = region.row_begin() - 1; r < region.row_end(); ++r) {
            for (int c = region.col_begin(); c < region.col_end() + 2; ++c) {
                if (cs(r, c) == cs(r, c-1) && cs(r, c) == cs(r+1, c)) {
                    if (cs(r, c) == cs(r+1, c-2)) {
                        AddLocation(r, c-1, 5);
                    }
                    if (cs(r, c) == cs(r+2, c-1)) {
                        AddLocation(r, c-1, 4);
                    }
                }
            }
        }
        // x0x
        // 0x0
        // x0o
        for (int r = region.row_begin(); r < region.row_end() + 1; ++r) {
            for (int c = region.col_begin(); c < region.col_end() + 2; ++c) {
                if (cs(r, c) == cs(r, c-1) && cs(r, c) == cs(r-1, c)) {
                    if (cs(r, c) == cs(r-1, c-2)) {
                        AddLocation(r-1, c-1, 6);
                    }
                    if (cs(r, c) == cs(r-2, c-1)) {
                        AddLocation(r-1, c-1, 7);
                    }
                }
            }
        }
        // x0x
        // 0x0
        // o0x
        for (int r = region.row_begin(); r < region.row_end() + 1; ++r) {
            for (int c = region.col_begin() - 2; c < region.col_end() + 2; ++c) {
                if (cs(r, c) == cs(r-1, c) && cs(r, c) == cs(r, c+1)) {
                    if (cs(r, c) == cs(r-2, c+1)) {
                        AddLocation(r-1, c, 0);
                    }
                    if (cs(r, c) == cs(r-1, c+2)) {
                        AddLocation(r-1, c, 1);
                    }
                }
            }
        }
    }

    // one big loop
    // some extra work for each branch
    void UpdateOneMoves_2(const Region& region) {
        const auto& cs = colors_;

        for (int r = region.row_begin() -1; r < region.row_end() +1; ++r) {
            for (int c = region.col_begin() -2; c < region.col_end() +2; ++c) {
                if (cs(r, c) == cs(r, c+1) && cs(r, c) == cs(r+1, c)) {
                    if (cs(r, c) == cs(r+1, c+2)) {
                        AddLocation(r, c, 2);
                    }
                    if (cs(r, c) == cs(r+2, c+1)) {
                        AddLocation(r, c, 3);
                    }
                }
                if (cs(r, c) == cs(r, c-1) && cs(r, c) == cs(r+1, c)) {
                    if (cs(r, c) == cs(r+1, c-2)) {
                        AddLocation(r, c-1, 5);
                    }
                    if (cs(r, c) == cs(r+2, c-1)) {
                        AddLocation(r, c-1, 4);
                    }
                }
                if (cs(r, c) == cs(r, c-1) && cs(r, c) == cs(r-1, c)) {
                    if (cs(r, c) == cs(r-1, c-2)) {
                        AddLocation(r-1, c-1, 6);
                    }
                    if (cs(r, c) == cs(r-2, c-1)) {
                        AddLocation(r-1, c-1, 7);
                    }
                }
                if (cs(r, c) == cs(r-1, c) && cs(r, c) == cs(r, c+1)) {
                    if (cs(r, c) == cs(r-2, c+1)) {
                        AddLocation(r-1, c, 0);
                    }
                    if (cs(r, c) == cs(r-1, c+2)) {
                        AddLocation(r-1, c, 1);
                    }
                }

            }
        }
    }


    void UpdateOneMoves(const Region& region) {
        UpdateOneMoves_2(region);
    }



};




}



#endif
