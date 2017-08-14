#pragma once

#include "board.hpp"
#include "state_sq.hpp"
#include "local_sq_rm.hpp"


class FourMove {
public:
    void Solve(Board& b) {
        remover_.FindRemove();
        state_.UpdateSqLocs(board);

        Move m, m_some;
        Count d, d_some;
        for (Index i = 0; i < kMoveCount; ++i) {
            if (!state_.HasSqLocs()) {
                d = 0;
                state_.ForEachSqLoc([&](const Location& loc) {

                    m_some = loc.move();
                    b.ForMove(m_some, [&]() {
                        remover_.ForRemove(m_some, [&](const Region& rem_reg) {
                            auto sqs = state_.AfterChangeSqLocs(rem_reg);
                            d_some = sqs + b.squares_removed();
                            if (d_some > d) {
                                m = m_some;
                                d = d_some;
                            }
                        });
                    });
                });
                b.MakeMove(m);
                auto reg = remover_.Remove(m);
                state_.UpdateSqLocs(reg);
            }
            else {
                m = randomMove();
                b.MakeMove(m);
                state_.UpdateSqLocs(m.region());
            }
        }
    }

    BoardState state_;
    LocalSquareRemover remover_;

};