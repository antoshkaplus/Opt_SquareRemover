#pragma once

#include "board.hpp"
#include "location.hpp"


namespace digit {

class Locator {
public:
    void Init(const Board& b) {
        board_ = &b;
    }

    void ResetBoard(const Board& b) {
        board_ = &b;
    }

    template<class Func>
    void ForEachLocation(Region reg, Func func) const {
        // don't want to look at what I don't need
        reg.diff(-1, -1, 2, 2);
        reg = reg.intersection(board().region());

        auto cs = [&](auto r, auto c) { return board_->color(r, c); };
        for (auto r = reg.row_begin(); r < reg.row_end(); ++r) {
            for (auto c = reg.col_begin(); c < reg.col_end(); ++c) {
                Color color;
                // x
                // ox
                // x
                color = cs(r-1,c);
                if (color == cs(r,c+1) && color == cs(r+1,c)) {
                    if (color == cs(r-1, c+1)) {
                        func({r-1, c, Combo::BotLeBot});
                    }
                    if (color == cs(r+1, c+1)) {
                        func({r, c, Combo::TopLeTop});
                    }
                }
                // xox
                //  x
                color = cs(r,c+1);
                if (color == cs(r+1,c) && color == cs(r,c-1)) {
                    if (color == cs(r+1,c-1)) {
                        func({r, c-1, Combo::TopRiRi});
                    }
                    if (color == cs(r+1,c+1)) {
                        func({r, c, Combo::TopLeLe});
                    }
                }
                //  x
                // xo
                //  x
                color = cs(r+1,c);
                if (color == cs(r,c-1) && color == cs(r-1,c)) {
                    if (color == cs(r+1,c-1)) {
                        func({r, c-1, Combo::TopRiTop});
                    }
                    if (color == cs(r-1,c-1)) {
                        func({r-1, c-1, Combo::BotRiBot});
                    }
                }
                //  x
                // xox
                color = cs(r,c-1);
                if (color == cs(r-1,c) && color == cs(r,c+1)) {
                    if (color == cs(r-1,c-1)) {
                        func({r-1, c-1, Combo::BotRiRi});
                    }
                    if (color == cs(r-1,c+1)) {
                        func({r-1, c, Combo::BotLeLe});
                    }
                }
            }
        }
    }

    bool IsValid(const Location& loc) const {
        auto r = loc.row;
        auto c = loc.col;
        switch (loc.combo) {
            case Combo::BotRiBot:
                return board_->IsFour({r,c}, {r,c+1}, {r+1,c}, {r+2,c+1});
            case Combo::TopRiTop:
                return board_->IsFour({r,c}, {r-1,c+1}, {r+1,c}, {r+1,c+1});
            case Combo::TopLeLe:
                return board_->IsFour({r,c-1}, {r,c+1}, {r+1,c}, {r+1,c+1});
            case Combo::TopRiRi:
                return board_->IsFour({r,c}, {r,c+2}, {r+1,c}, {r+1,c+1});
            case Combo::TopLeTop:
                return board_->IsFour({r-1,c}, {r,c+1}, {r+1,c}, {r+1,c+1});
            case Combo::BotLeBot:
                return board_->IsFour({r,c}, {r,c+1}, {r+2,c}, {r+1,c+1});
            case Combo::BotLeLe:
                return board_->IsFour({r,c}, {r,c+1}, {r+1,c-1}, {r+1,c+1});
            case Combo::BotRiRi:
                return board_->IsFour({r,c}, {r,c+1}, {r+1,c}, {r+1,c+2});
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