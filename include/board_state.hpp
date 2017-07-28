#pargma once

#include "util.hpp"
#include "location.hpp"
#include "board.hpp"

class BoardState {
public:

    void Init(const Board& b) {
        board_ = &b;
    }

    void OnRegionChanged(const Region& reg) {
        ValidateSqLocs();
        UpdateSqLocs(reg);
    }

    Count AfterChangeSqLocs(const Region& reg) {

    }

    bool HasSqLocs() {
        return !sq_locs_.empty();
    }

    template<class Func>
    void ForEachSqLoc(Func func) {

    }

private:

    void UpdateSqLocs(const Region& reg) {

        auto& cs = std::bind(board.color, std::placeholders::_1, std::placeholders::_2);
        for (auto r = reg.row_begin()-1; r < reg.row_end()+1; ++r) {
            for (auto c = reg.col_begin()-1; c < reg.col_end()+1; ++c) {
                char color;
                // x
                // ox
                // x
                if (color = cs(r-1,c) & cs(r,c+1) & cs(r+1,c)) {
                    if (color & cs(r-1, c+1)) {
                        AddLocation(r-1, c, Combo::BotLeBot);
                    }
                    if (color & cs(r+1, c+1)) {
                        AddLocation(r, c, Combo::TopLeTop);
                    }
                }
                // xox
                //  x
                if (color = cs(r,c+1) & cs(r+1,c) & cs(r,c-1)) {
                    if (color & cs(r+1,c-1)) {
                        AddLocation(r, c, Combo::TopRiRi);
                    }
                    if (color & cs(r+1,c+1)) {
                        AddLocation(r, c, Combo::TopLeLe);
                    }
                }
                //  x
                // xo
                //  x
                if (color = cs(r+1,c) & cs(r,c-1) & cs(r-1,c)) {
                    if (color & cs(r+1,c-1)) {
                        AddLocation(r, c-1, Combo::TopRiTop);
                    }
                    if (color & cs(r-1,c-1)) {
                        AddLocation(r-1, c-1, Combo::BotRiBot);
                    }

                }
            }
        }
    }

    void AddLocation(char row, char col, Combo combo) {
        sq_locs_.emplace(row, col, combo);
    }

    void ValidateSqLocs() {
        for (auto it = sq_locs_.begin(); it != sq_locs_.end();) {
            Location loc = *it;
            bool leave = false;
            switch (loc.combo) {
                case Combo::BotRiBot:
                    leave = cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+2,c+1);
                    break;
                case Combo::TopRiTop:
                    leave = cs(r,c)&cs(r-1,c+1)&cs(r+1,c)&cs(r+1,c+1);
                    break;
                case Combo::TopLeLe:
                    leave = cs(r,c-1)&cs(r,c+1)&cs(r+1,c)&cs(r+1,c+1);
                    break;
                case Combo::TopRiRi:
                    leave = cs(r,c)&cs(r,c+2)&cs(r+1,c)&cs(r+1,c+1);
                    break;
                case Combo::TopLeTop:
                    leave = cs(r-1,c)&cs(r,c+1)&cs(r+1,c)&cs(r+1,c+1);
                    break;
                case Combo::BotLeBot:
                    leave = cs(r,c)&cs(r,c+1)&cs(r+2,c)&cs(r+1,c+1);
                    break;
                case Combo::BotLeLe:
                    leave = cs(r,c)&cs(r,c+1)&cs(r+1,c-1)&cs(r+1,c+1);
                    break;
                case Combo::BotRiRi:
                    leave = cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+1,c+2);
                    break;
                default:
                    throw runtime_error("unknown Combo");
            }
            if (!leave) {
                it = sq_locs_.erase(it);
            } else ++it;
        }
    }

    std::unordered_set<Location> sq_locs_;
    const Board* board_;
};