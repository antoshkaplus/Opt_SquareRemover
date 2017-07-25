//
//  local_square_remover.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/3/15.
//
//

#pragma once

#include "board.hpp"

// class responsible for removing everything in the right order after move.
class LocalSquareRemover {
public:
    
    void Init(Board& board) {
        board_ = &board;
    }
    
    void Remove(const Move& m) {
        AffectedPositions(m, affected_positions_);
        Remove(affected_positions_);
    }
    
    Count squares_removed() const {
        return squares_removed_;
    }
    
private:

    void Remove(vector<Position>& ps) {
        auto& b = *board_;
        while (!ps.empty()) {
            auto p = ps.back();
            ps.pop_back();
            if (b.IsSquare(p)) {
                b.Remove(p);
                AddAffectedBySquare(p, ps);
            }
        }
    }

    void AddAffectedBySquare(const Position& sq, vector<Position>& ps) {
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
    void AffectedPositions(const Move& m, vector<Position>& ps) {
        Region reg = AffectedRegion(m);
        ps.reserve(reg.cell_count());
        auto func = [&](const Position& p) {
            ps.push_back(p);
        };
        reg.ForEach(func);
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
    vector<Position> affected_positions_;
};