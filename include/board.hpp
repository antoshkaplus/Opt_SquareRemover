//
//  board_naive.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 11/3/15.
//
//

#pragma once

#include <functional>

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
        array<Position, 4> ps;
        if (m.isVertical()) {
            ps = {{ m.pos.shifted(-1, -1),
                    m.pos.shifted(-1, 0),
                    m.another().shifted(0, -1),
                    m.another() }};
        } else {
            ps = {{ m.pos.shifted(-1, -1),
                    m.pos.shifted(0, -1),
                    m.another().shifted(-1, 0),
                    m.another() }};
        }
        bool res = std::any_of(ps.begin(), ps.end(), std::bind(&Board::IsSquare, this, std::placeholders::_1));
        MakeMove(m);
        return res;
    }
    
    bool IsTriple(const Position p_0, const Position& p_1, const Position& p_2) const {
        return color(p_0) == color(p_1) && color(p_1) == color(p_2);
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
        // could use intrinsics but mostly gonna get out on first or second comparizon
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


// TODO fix virtuality
// was created to take care of having loops in moves cuz of score board changes
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



class CalculatedBoard : public Board {

    struct TripleCounter {
        void Init(const Board& b) {
            board_ = &b;
        }
        
        template<Direction dir>
        Count Outer(const Position& p) {
            auto& b = *board_; 
            auto& g = board_->grid();
            Count count = 0;
            auto p_1 = p.Shifted(dir);
            if (!g.IsInside(p_1)) {
                return count;
            }
            auto& dd = kDirTurn[dir]; 
            auto p_2 = p_1.Shifted(dd[0]);
            if (g.IsInside(p_2) && b.IsTriple(p, p_1, p_2)) {
                ++count; 
            }
            p_2 = p_1.Shifted(dd[1]);
            if (g.IsInside(p_2) && b.IsTriple(p, p_1, p_2)) {
                ++count;
            }
            return count;
        }
        
        Count Outer(const Position& p, Direction dir) {
            auto& b = *board_; 
            auto& g = board_->grid();
            Count count = 0;
            auto p_1 = p.Shifted(dir);
            if (!g.IsInside(p_1)) {
                return count;
            }
            auto& dd = kDirTurn[dir]; 
            auto p_2 = p_1.Shifted(dd[0]);
            if (g.IsInside(p_2) && b.IsTriple(p, p_1, p_2)) {
                ++count; 
            }
            p_2 = p_1.Shifted(dd[1]);
            if (g.IsInside(p_2) && b.IsTriple(p, p_1, p_2)) {
                ++count;
            }
            return count;
        }
        
        Count Everything(const Position& p) {
            return OuterAll(p) + Center(p);
        }
        
        
        Count OuterAll(const Position& p) {
            return Outer<kDirUp>(p) + Outer<kDirDown>(p) + 
                Outer<kDirRight>(p) + Outer<kDirLeft>(p);
        }
        
        Count OuterSquare(const Position& p) {
            auto ps = SquarePositions(p);
            Count count = 0;
            Direction dir = kDirLeft;
            for (auto p : ps) {
                count += Center(p);
                count += Outer(p, dir);
                dir = kDirClockwise[dir];
                count += Outer(p, dir);
            }
            return count;
        }
        
        Count Center(const Position& p) {
            auto& b = *board_; 
            auto& g = board_->grid();
            Count count = 0;
            for (Direction d : kDirections) {
                auto p_1 = p.Shifted(d);
                if (!g.IsInside(p_1)) continue;
                auto p_2 = p.Shifted(kDirClockwise[d]);
                if (!g.IsInside(p_2) || !b.IsTriple(p, p_1, p_2)) {
                    continue;
                }
                ++count;
            }
            return count;
        }
        
        const Board* board_;
    };
    
    struct DoubleCounter {
        
        
    };
    
    

public:
    void Init(const Grid<Color>& board, Count color_count, Index seed) {
        Board::Init(board, color_count, seed);
        // count triples  here
        
        // count doubles here
    }

    void MakeMove(const Move& m) {
        assert(color(m.pos) != color(m.another()));
        triple_count -= counter.Everything(m.pos) + counter.Everything(m.another());
        Board::MakeMove(m);
        triple_count += counter.Everything(m.pos) + counter.Everything(m.another());
    }
    
    void Remove(const Position& p) {
        triple_count -= counter.OuterSquare(p);
        Board::Remove(p);
        triple_count += counter.OuterSquare(p);
    }

private:
    
    TripleCounter counter;
    //Count double_count;
    Count triple_count;
};

