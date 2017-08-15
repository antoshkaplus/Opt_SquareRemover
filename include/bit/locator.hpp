#pragma once

#include "board.hpp"
#include "location.hpp"


namespace bit {

class Locator {
public:
    void Init(const Board& b) {
        board_ = &b;
    }

    template<class Func>
    void ForEachLocation(Region reg, Func func) {
        reg.diff(-1, -1, 2, 2);
        reg = reg.intersection(board().region().diffed(1, 1, -2, -2));

        auto cs = [&](auto r, auto c) { return board_->color(r, c); };
        for (auto r = reg.row_begin(); r < reg.row_end(); ++r) {
            for (auto c = reg.col_begin(); c < reg.col_end(); ++c) {
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

    const Board& board() const {
        return *board_;
    }

private:
    const Board* board_;
};

}