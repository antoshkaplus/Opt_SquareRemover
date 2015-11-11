//
//  board_naive.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/3/15.
//
//

#pragma once

#include "util.hpp"



class Board {
public:
    
    Board() {}
    Board(const Grid<Color>& board, Count color_count, Index seed) {
        Init(board, color_count, seed);
    }
    
    void Init(const Grid<Color>& board, Count color_count, Index seed) {
        board_ = board;
        color_count_ = color_count;
        seed_ = seed;
        squares_removed_ = 0;
        moves_.reset(new vector<Move>());
        
        Region reg{{0, 0}, board.size()};
        for (Position p : reg) {
            if (p.col < size()-1) {
                moves_->emplace_back(p, kDirRight);
            }
            if (p.row < size()-1) {
                moves_->emplace_back(p, kDirDown);
            }
        }
    }
    
    // should I be able to go back in time???
    void Remove(const Position& p) {
        seed_ = ReplaceColors(board_, color_count_, p, seed_);
        ++squares_removed_;
    }
    
    Color color(const Position& p) const {
        return board_[p];
    } 

    void MakeMove(const Move& m) {
        auto pos_2 = m.pos.Shifted(m.dir);
        assert(board_.IsInside(m.pos) && board_.IsInside(pos_2));
        swap(board_[m.pos], board_[pos_2]);
    }
    
    bool IsIdentityMove(const Move& m) {
        return color(m.pos) == color(m.another());
    }
    
    bool IsKillerMove(const Move& m) {
        MakeMove(m);
        Position p_0 = m.pos - Indent{1,1};
        Position p_1 = m.pos - Indent{1,0};
        Position p_2 = m.another() - Indent{0,1};
        Position p_3 = m.another();
        bool res = false;
        if (IsSquare(p_0) || IsSquare(p_1) || IsSquare(p_2) || IsSquare(p_3)) {
            res = true;
        }
        MakeMove(m);
        return res;
    }
    
    const Grid<Color>& grid() const {
        return board_;
    }
    
    const Region region() const {
         return Region(0, 0, size(), size());
    }
    
    const vector<::Move>& moves() const {
        return *moves_;
    }
    
    Count squares_removed() const {
        return squares_removed_;
    }
    
    bool IsSquare(const Position& p) const {
        auto& b = board_;
        return p.row < size()-1 && p.col < size()-1 && 
                b(p.row  , p.col) == b(p.row  , p.col+1) &&
                b(p.row+1, p.col) == b(p.row+1, p.col+1) &&
                b(p.row  , p.col) == b(p.row+1, p.col+1);
    }
    
    Count size() const {
        return board_.size().row;
    }
    
    Count color_count() const {
        return color_count_;
    }
    
    Index seed() const {
        return seed_;
    }
    
private:        
    Index seed_;
    Count color_count_;
    Count squares_removed_;    
    Grid<Color> board_;
    // this one should not be stored here
    // ore may be computed everytime
    shared_ptr<vector<::Move>> moves_;
};


class HashedBoard : public Board {
public:
    using HashFunction = ZobristHashing<64>; 
    

    void Init(const Grid<Color>& board, Count color_count, Index seed) {
        Board::Init(board, color_count, seed);
        hash_function_.reset(new HashFunction(board.size(), color_count));
        auto is_filled = [](const Position& p) { return true; };
        auto getter = [&](const Position& p) {
            return color(p);
        };
        hash_ = hash_function_->Hash(is_filled, getter);
    }

    void MakeMove(const Move& m) {
        auto& h = *hash_function_;
        auto p = m.another();
        h.xorState(&hash_, m.pos, color(m.pos));
        h.xorState(&hash_, p, color(p));
        Board::MakeMove(m);
        h.xorState(&hash_, m.pos, color(m.pos));
        h.xorState(&hash_, p, color(p));
    }

    void Remove(const Position& p) {
        auto& h = *hash_function_;
        for (auto& pp : SquarePositions(p)) {
            h.xorState(&hash_, pp, color(pp));
        }
        Board::Remove(p);
        for (auto& pp : SquarePositions(p)) {
            h.xorState(&hash_, pp, color(pp));
        }
    }

    HashFunction::value hash() const {
        return hash_;
    }

private:
    
    HashFunction::value hash_;
    shared_ptr<HashFunction> hash_function_;
    
};




class ScoreCalculator {
public:

    void Init(const Board& b) {
        board_ = &b;
        open_.resize(b.size(), b.size());
    }

    // more is better
    double Compute() {
        fill(open_.begin(), open_.end(), true);
        Count triples = CountTriples();
        Count doubles = CountDoubles();
        return board_->squares_removed() + 0.0026 * (70.*triples + 17.*doubles);
    }
    
private:
    
    Count CountTriples() {
        auto& b = *board_;
        auto& g = b.grid();
    
        Count triples = 0;
        auto func = [&](const Position& p) {
            if (!open_(p)) return;
            Indent i_1{-1,0};
            Indent i_2{ 0,1};
            for (auto i = 0; i < 4; ++i) {
                // can use current p_2
                Position p_1 = p + i_1;
                Position p_2 = p + i_2;
                if (g.IsInside(p_1) && g.IsInside(p_2) &&
                    open_(p_1) && open_(p_2) &&
                    g(p) == g(p_1) && g(p) == g(p_2)) {
                    
                    ++triples;
                    open_(p) = open_(p_1) = open_(p_2) = false;
                }
                tie(i_1, i_2) = make_tuple(i_2, i_1*=-1);
            }
        };
        g.ForEachPosition(func);
        return triples;
    }
    
    Count CountDoubles() {
        auto& b = *board_;
        auto& g = b.grid();
        
        Count doubles = 0;
        // try to go down and right
        auto func = [&](const Position& p) {
            if (!open_(p)) return;
            for (auto d : {kDirRight, kDirDown}) {
                auto pp = p.Shifted(d);
                if (g.isInside(pp) && open_(pp) && g(p) == g(pp)) {
                    ++doubles; 
                }
            }
        };
        g.ForEachPosition(func);
        return doubles;
    }

    const Board* board_;
    Grid<char> open_;
};

