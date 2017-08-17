#pragma once

class _2_SqState_v2 {
public:

    void OnRegionChanged() {




    }

private:

    // it's much bigger than you think
    template<class Func>
    void ForEachMove(void Region& reg, Func func) {
        reg.diffed(-1, 0, 1, 0).ForEach([&](const Position& p) {
            ForEachMove({p, kDirDown}, func);
        });
        reg.diffed(0, -1, 0, 1).ForEach([&](const Position& p) {
            ForEachMove({p, kDirRight}, func);
        });
    }

    template<class Func>
    void ForEachMove(const Move& m, Func func) {
        // build a rectangle out of it ???
        auto p_0;
        auto p_1;
        // don't forget to add everything to our set
        if (m.isHorizontal()) {

            // touching horizontal
            func(m, {p_0.shifted(0, -1), kDirRight});
            func({p_0.shifted(0, -1), kDirRight, m});

            func(m, {p_1, kDirRight});
            func({p_1, kDirRight}, m);

            // touching vertical
            // top
            func(m, {p_0.shifted(-1, 0), kDirDown});
            func({p_0.shifted(-1, 0), kDirDown}, m);

            func(m, {p_1.shifted(-1, 0), kDirDown});
            func({p_1.shifted(-1, 0), kDirDown}, m);

            // bot
            func(m, {p_0, kDirDown});
            func({p_0, kDirDown}, m);

            func(m, {p_1, kDirDown});
            func({p_1, kDirDown}, m);

            for (auto c = p_0.col-1; c <= p_1.col+1; ++c) {
                func(m, {{p_0.row-2, c}, kDirDown});
                func(m, {{p_0.row+1, c}, kDirDown});
            }
            for (auto r = p_0.row-1; r <= p_0.row+1; ++r) {
                func(m, {{r, p_0.col-2}, kDirRight});
                func(m, {{r, p_1.col+1}, kDirRight});
            }
        }

        if (m.isVertical()) {
            // touching vertical
            func(m, {p_0.shifted(-1, 0), kDirDown});
            func({p_0.shifted(-1, 0), kDirDown, m});

            func(m, {p_1, kDirDown});
            func({p_1, kDirDown}, m);

            // touching horizontal
            // left
            func(m, {p_0.shifted(0, -1), kDirRight});
            func({p_0.shifted(0, -1), kDirRight}, m);

            func(m, {p_1.shifted(0, -1), kDirRight});
            func({p_1.shifted(0, -1), kDirRight}, m);

            // right
            func(m, {p_0, kDirRight});
            func({p_0, kDirRight}, m);

            func(m, {p_1, kDirRight});
            func({p_1, kDirRight}, m);

            for (auto r = p_0.row-1; r <= p_1.row+1; ++r) {
                func(m, {{r, p_0.col-2}, kDirRight});
                func(m, {{r, p_0.col+1}, kDirRight});
            }
            for (auto c = p_0.col-1; c <= p_0.col+1; ++c) {
                func(m, {{p_0.row-2, c}, kDirDown});
                func(m, {{p_1.row+1, c}, kDirDown});
            }
        }
    }

    // just use this and everything will be perfect
    unordered_set<DoubleMove> d_moves;
};