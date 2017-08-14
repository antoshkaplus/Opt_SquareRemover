#pragma once

#include "board.hpp"


namespace bit {

class Locator {

    void Init(const Board& b) {
        board_ = &b;
    }

    template<class Func>
    void ForEachLocation(const Region& reg, Func func) {
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

private:
    const Board* board_;
};

}