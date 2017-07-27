#pragma once

#include "ant/grid/grid.hpp"

#include "util.hpp"

class BoardSquareRemover {


public:

    void Init(Board& board) {
        board_ = &board;
    }

    void Remove(const Move& m) {
        AddPossibleSquaresPositions(m);
        Remove();
    }

    Count squares_removed() const {
        return squares_removed_;
    }

private:

    void Remove() {
        auto& b = *board_;
        auto& ps = possible_squares_;
        while (!ps.empty()) {
            auto p = ps.back();
            ps.pop_back();
            if (b.IsSquare(p)) {
                b.Remove(p);
                AddAffectedBySquare(p, ps);
            }
        }
    }

    // after we removed this square
    void AddPossibleSquaresPositions(const Position& sq, vector<Position>& ps) {
        auto sz = ps.size();
        auto reg = AffectedRegionBySquare(sq);
        ps.reserve(sz + reg.cell_count());
        ps.insert(ps.end(), reg.begin(), reg.end());
        reverse(ps.begin() + sz, ps.end());
        inplace_merge(ps.begin(), ps.begin()+sz, ps.end(), Position::BottomRightComparator());
        unique(ps.begin(), ps.end());
    }

    vector<Position> AffectedPositions(Move m) {
        vector<Position> res;
        AffectedPositions(m, res);
        return res;
    }

    // we expect empty ps. why do we reverse at the end, we should check in provided order
    // we reverse because we gonna take out from the back.. like stack. (should rename data structure then?)
    void AddPossibleSquaresPositions(const Move& m) {
        vector<Position>& ps = possible_squares_;
        m.ForEachPossibleSquare([&](const Position& p) {
            ps.emplace(p);
            is_possible_square_(index(p, board_)) = true;
        });
        reverse(ps.begin(), ps.end());
    }

    Region AffectedRegionBySquare(const Position& sq) {
        auto& b = *board_;
        auto aff_reg = Region(sq - Indent{1, 1}, Size{3, 3});
        return b.region().intersection(aff_reg);
    }

    Region AffectedRegion(const Move& m) {
        // more optimal to use real positions
        // can have a boolean array that keeps track of all positions
        // if position is already marked for visiting no need to bother for second time visit
        auto ind = Indent{1,1};
        auto p = m.pos - ind;
        if (p.row < 0) p.row = 0;
        if (p.col < 0) p.col = 0;
        return {p, m.another()};
    }


    Board* board_;
    Count squares_removed_;

    // buffer
    vector<Position> possible_squares_;
    // after checking the value put back false in value field
    vector<bool> is_possible_square_(board.size()*board.size(), false);
};


template<class Remover>
class BoardNew {


    Grid<uint8_t> grid_;
    // take least possible guy
    GridView<Grid<uint8_t>> view_(grid_);



};

