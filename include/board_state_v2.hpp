#pragma once

#include "util.hpp"
#include "board.hpp"


class BoardState_v2 {
public:
    void OnRegionChanged(const Region& region) {
        ValidateOneMoves();
        UpdateOneMoves_2(region);
    }

    Count SquareMoveCount() const {
        return sq_locs_.size();
    }


private:
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

    void ValidateOneMoves() {

    }

    Board* board_;
    std::unordered_set<Location> sq_locs_;
};