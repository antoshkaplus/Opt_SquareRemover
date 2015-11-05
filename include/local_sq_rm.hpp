//
//  local_square_remover.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/3/15.
//
//

#pragma once

#include "board.hpp"

class LocalSquareRemover {
public:
    
    void Init(Board& board) {
        board_ = &board;
    }
    
    // vs big rectangle
    
    // i have to sort other way around... or get problems while popping elements
    
    // possible points of removal
    void Remove(vector<Position>& ps) {
        auto& b = *board_;
        while (!ps.empty()) {
            auto p = ps.back();
            ps.pop_back();
            if (b.IsSquare(p)) {
                b.Remove(p);
               
                auto sq = SquarePositions(p);
                // also include nearby positions here!
                
                // need to implement algorithm going back one by one
                // can set be a good solution??? probably not
                ps.insert(ps.end(), sq.begin(), sq.end()); 
            }
        }
    }
    
    void Remove(const Move& m) {
        auto rm = AffectedPositions(m);
        Remove(rm);
    }
    
    Count squares_removed() const {
        return squares_removed_;
    }
    
private:

    vector<Position> AffectedPositions(Move m) {
        auto& b = *board_;
        Indent t{1, 1};
        Size sz{3, 3};
        Region b_reg{{0, 0}, b.grid().size()};
        Region r{m.pos - t, sz};
        Region r_2{m.another() - t, sz};
        r = r.intersection(b_reg);
        r_2 = r_2.intersection(b_reg);
        Position p = min(r.position, r_2.position, Position::TopLeftComparator());
        Position p_2 = max(r.position + static_cast<Indent>(r.size), r_2.position + static_cast<Indent>(r_2.size), Position::TopLeftComparator());
        Region reg{p, static_cast<Size>(p_2-p)};
        return vector<Position>(reg.begin(), reg.end());
    }
    
    Board* board_;
    Count squares_removed_;
};