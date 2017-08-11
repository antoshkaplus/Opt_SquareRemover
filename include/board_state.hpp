#pragma once

#include "util.hpp"
#include "location.hpp"
#include "board.hpp"


template<class T> class BeamSearch;

// need to keep track of Locations to easier know if move valid of not.
// otherwise have to check sq in 4 locations

// that's a lot
class BoardState {
public:

    void Init(const Board& b) {
        board_ = &b;
        // have to reduce region, otherwise will examine out of board cells.
        OnRegionChanged(b.region().diffed(1, 1, -2, -2));
    }

    void OnRegionChanged(const Region& reg) {
        ValidateSqLocs();
        UpdateSqLocs(reg);
    }

    Count AfterChangeSqLocs(const Region& reg) {
        static vector<std::unordered_map<Location, bool>::iterator> invalidated;
        // don't have to use iterator
        for (auto it = sq_locs_.begin(); it != sq_locs_.end(); ++it) {
            if (!IsValid(it->first)) {
                it->second = false;
                invalidated.push_back(it);
            }
        }
        Count diff = sq_locs_.size() - invalidated.size();
        ForEachNewLocation(reg, [&](const Location& loc) {
            auto it = sq_locs_.find(loc);
            if (it == sq_locs_.end() || !it->second) {
                ++diff;
            }
        });
        for_each(invalidated.begin(), invalidated.end(), [](std::unordered_map<Location, bool>::iterator& it) {
            it->second = true;
        });
        invalidated.clear();

        return diff;
    }

    bool sq_locs_empty() const {
        return sq_locs_.empty();
    }

    template<class Func>
    void ForEachSqLoc(Func func) {
        for (auto loc : sq_locs_) {
            func(loc.first);
        }
    }

private:

    void UpdateSqLocs(const Region& reg) {
        ForEachNewLocation(reg, [&](const Location& loc) {
            AddLocation(loc);
        });
    }

    template<class Func>
    void ForEachNewLocation(const Region& reg, Func func) {
        auto cs = [&](auto r, auto c) { return board_->color(r, c); };
        for (auto r = reg.row_begin()-1; r < reg.row_end()+1; ++r) {
            for (auto c = reg.col_begin()-1; c < reg.col_end()+1; ++c) {
                char color;
                // x
                // ox
                // x
                if ((color = (cs(r-1,c) & cs(r,c+1) & cs(r+1,c)))) {
                    if (color & cs(r-1, c+1)) {
                        func({r-1, c, Combo::BotLeBot});
                    }
                    if (color & cs(r+1, c+1)) {
                        func({r, c, Combo::TopLeTop});
                    }
                }
                // xox
                //  x
                if ((color = (cs(r,c+1) & cs(r+1,c) & cs(r,c-1)))) {
                    if (color & cs(r+1,c-1)) {
                        func({r, c-1, Combo::TopRiRi});
                    }
                    if (color & cs(r+1,c+1)) {
                        func({r, c, Combo::TopLeLe});
                    }
                }
                //  x
                // xo
                //  x
                if ((color = (cs(r+1,c) & cs(r,c-1) & cs(r-1,c)))) {
                    if (color & cs(r+1,c-1)) {
                        func({r, c-1, Combo::TopRiTop});
                    }
                    if (color & cs(r-1,c-1)) {
                        func({r-1, c-1, Combo::BotRiBot});
                    }
                }
                //  x
                // xox
                if ((color = (cs(r,c-1) & cs(r-1,c) & cs(r,c+1)))) {
                    if (color & cs(r-1,c-1)) {
                        func({r-1, c-1, Combo::BotRiRi});
                    }
                    if (color & cs(r-1,c+1)) {
                        func({r-1, c, Combo::BotLeLe});
                    }
                }
            }
        }
    }

    void AddLocation(const Location& loc) {
        sq_locs_.emplace(loc, true);
    }

    void ValidateSqLocs() {
        for (auto it = sq_locs_.begin(); it != sq_locs_.end();) {
            if (!IsValid(it->first)) {
                it = sq_locs_.erase(it);
            } else ++it;
        }
    }

    bool IsValid(const Location& loc) {
        auto cs = [&](auto r, auto c) { return board_->color(r, c); };

        auto r = loc.row;
        auto c = loc.col;
        switch (loc.combo) {
            case Combo::BotRiBot:
                return cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+2,c+1);
            case Combo::TopRiTop:
                return cs(r,c)&cs(r-1,c+1)&cs(r+1,c)&cs(r+1,c+1);
            case Combo::TopLeLe:
                return cs(r,c-1)&cs(r,c+1)&cs(r+1,c)&cs(r+1,c+1);
            case Combo::TopRiRi:
                return cs(r,c)&cs(r,c+2)&cs(r+1,c)&cs(r+1,c+1);
            case Combo::TopLeTop:
                return cs(r-1,c)&cs(r,c+1)&cs(r+1,c)&cs(r+1,c+1);
            case Combo::BotLeBot:
                return cs(r,c)&cs(r,c+1)&cs(r+2,c)&cs(r+1,c+1);
            case Combo::BotLeLe:
                return cs(r,c)&cs(r,c+1)&cs(r+1,c-1)&cs(r+1,c+1);
            case Combo::BotRiRi:
                return cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+1,c+2);
            default:
                throw runtime_error("unknown Combo");
        }
    }

    std::unordered_map<Location, bool> sq_locs_;
    const Board* board_;

    template<class T> friend class BeamSearch;
};