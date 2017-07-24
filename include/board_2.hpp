//
//  board.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 1/10/15.
//
//

#ifndef SquareRemover_board_hpp
#define SquareRemover_board_hpp


#include <forward_list>
#include <functional>
#include <unordered_set>
#include <array>


#include "ant/optimization.h"

#include "util.hpp"



namespace square_remover {
    
// could use Position actually, but this takes less memory
// used to perfectly identify 4 move.
struct  Location {
    char row;
    char col; 
    char combo;
}; 

constexpr bool operator==(const Location& loc_0, const Location& loc_1) {
    return loc_0.row == loc_1.row && loc_0.col == loc_1.col && loc_0.combo == loc_1.combo;
}


}


namespace std {
    
template <> 
struct hash<square_remover::Location> {
    size_t operator()(const square_remover::Location& x) const {
        return (x.row << 16) | (x.col << 8) | x.combo;
    }
};
    
}


namespace square_remover {

using namespace ant::opt;


// ?
//   
// 0 : top, right, top 
// 1 : top, right, right 
// 2 : bottom, right, right
// 3 : bottom, right, bottom
// 4 : bottom, left, bototm
// 5 : bottom, left, left
// 6 : top, left, left
// 7 : top, left, top
//

    
struct Move {
    
    Position p_0;
    Position p_1;
    
    Move() {}
    
    bool IsVertical() const {
        return p_0.col == p_1.col;
    }
    
    bool IsHorizontal() const {
        return p_0.row == p_1.row;
    }
    
    // positions are sorted by removing order 
    Move(Location loc) {
        char r = loc.row;
        char c = loc.col;
        switch (loc.combo) {
            case 0: {
                p_0.set(r-1,c+1);
                p_1.set(r, c+1);
                break;
            }
            case 1: {
                p_0.set(r,c+1);
                p_1.set(r,c+2);
                break;
            }
            case 2: {
                p_0.set(r+1,c+1);
                p_1.set(r+1,c+2);
                break;
            }
            case 3: {
                p_0.set(r+1,c+1);
                p_1.set(r+2,c+1);
                break;
            } 
            case 4: {
                p_0.set(r+1,c);
                p_1.set(r+2,c);
                break;
            } 
            case 5: {
                p_0.set(r+1,c-1);
                p_1.set(r+1,c);
                break;
            } 
            case 6: {
                p_0.set(r,c-1);
                p_1.set(r,c);
                break;
            } 
            case 7: {
                p_0.set(r-1,c);
                p_1.set(r,c);
                break;
            }
            default: assert(false);
        }
    }
};

constexpr array<Position, 4> OneMovePositions(const Location& loc) {
    char r = loc.row;
    char c = loc.col;
    switch (loc.combo) {
        case 0: return {{ {r,c}, {r+1,c}, {r+1,c+1}, {r-1,c+1} }};
        case 1: return {{ {r,c}, {r+1,c}, {r+1,c+1}, {r,c+2} }};
        case 2: return {{ {r,c}, {r+1,c}, {r,c+1}, {r+1,c+2} }};
        case 3: return {{ {r,c}, {r+1,c}, {r,c+1}, {r+2,c+1} }};
        case 4: return {{ {r,c}, {r,c+1}, {r+1,c+1}, {r+2,c} }};
        case 5: return {{ {r,c}, {r,c+1}, {r+1,c+1}, {r+1,c-1} }};
        case 6: return {{ {r,c+1}, {r+1,c+1}, {r+1,c}, {r,c-1} }};
        case 7: return {{ {r,c+1}, {r+1,c+1}, {r+1,c}, {r-1,c} }};
    }
    assert(false);
}



template<class Grid>
constexpr bool IsSquare(const Grid& g, const Position& p) {
    return g(p.row  , p.col) == g(p.row  , p.col+1) &&
           g(p.row+1, p.col) == g(p.row+1, p.col+1) &&
           g(p.row  , p.col) == g(p.row+1, p.col+1);
}


// should be template that depends on size. ??? why ???
template<Count BOARD_SIZE>
class Board {
    
    OriginGrid<Grid<char>> colors_;
    
    //Grid<char> colors_;
    
    // should be another class like hashed board or something like that
    Grid<shared_ptr<Node<Location>>> board_hashes_;
    unordered_set<Location> existing_hashes_;
    shared_ptr<Node<Move>> move_history_;
    
    int seed_;
    char color_count_;    
    // wtf is that????
    Count squares_removed_{0};

public:
    Board() {}

    // TODO watchout for existing squares
    Board(vector<string> board, int color_count, int starting_seed) {
        colors_.grid().resize(BOARD_SIZE + 4, BOARD_SIZE + 4);
        colors_.set_origin({-2, -2});
        board_hashes_.resize(BOARD_SIZE, BOARD_SIZE);
        Region reg({0, 0}, {BOARD_SIZE, BOARD_SIZE});
        for (int r = -2; r < BOARD_SIZE + 2; ++r) {
            for (int c = -2; c < BOARD_SIZE + 2; ++c) {
                if (reg.hasInside({r, c})) {
                    colors_(r, c) = board[r][c] - '0';
                } else {
                    colors_(r, c) = (r + c) % 2 == 0 ? -1 : -2;
                }
            }
        }
        seed_ = starting_seed;
        color_count_ = color_count;
        UpdateOneMoves(reg);
    }
    
    vector<Move> move_history() const {
        return Node<Move>::ToVector(move_history_);
    }
    
    // shared ptr may chancge
    vector<Board> OneMoveDerivatives() const {
        VerifyHashIdentity();
        // could just make all copies of b, then for each hash/board call remove
        vector<Board> res(existing_hashes_.size());
        Board b;
        int i = 0;
        for (auto loc : existing_hashes_) {
            b = *this;
            b.Remove(loc);
            swap(res[i++], b);
        }
        return res;
    }
    
    Count OneMoveCount() const {
        return existing_hashes_.size();
    }
  
    Count SquaresRemovedCount() const {
        return squares_removed_;
    }

    
    Index operator()(Index r, Index c) const {
        return (Index)colors_(r, c);
    }
    
    void VerifyHashIdentity() const {
        for (Location loc : existing_hashes_) {
            for (Position p : OneMovePositions(loc)) {
                assert(Node<Location>::Exists(board_hashes_(p), loc)); 
            }
        }
    }
    
    
private:

    constexpr static array<Position, 9> PotentialRemovals(const Position& p) {
        int r = p.row;
        int c = p.col;
        return {{   {r-1, c-1}, {r-1, c}, {r-1, c+1}, 
                    {r, c-1}, {r, c}, {r, c+1}, 
                    {r+1, c-1}, {r+1, c}, {r+1, c+1} }};
    }

    void Remove(Location loc) {
        VerifyHashIdentity();
        Move m{loc};
        // create move, put in history
        shared_ptr<Node<Move>> move;
        move.reset(new Node<Move>());
        move->value = m; 
        move->previous = move_history_;
        move_history_ = move;
        cout << *this;
        // execute move buy swapping colors
        swap(colors_(m.p_0), colors_(m.p_1));
        // don't need to remove loc right away
        // will be done in RemoveSquares
        
        vector<Position> squares;
        auto node = board_hashes_(m.p_0);
        while (node) {
            Location loc = node->value;
            Move mm(loc);
            if (m.p_1 == mm.p_1) {
                squares.emplace_back(loc.row, loc.col);
            }
            node = node->previous;
        }
        node = board_hashes_(m.p_1);
        while (node) {
            Location loc = node->value;
            Move mm(loc);
            if (m.p_0 == mm.p_0) {
                squares.emplace_back(loc.row, loc.col);
            }
            node = node->previous;
        }
        
        
        RemoveSquares(squares);
    }
    
    void RemoveSquares(vector<Position> squares) {
        vector<Position> removed_ps;
        Region update_one_moves(squares.front(), {2, 2});
        
        // we want to take positions from back
        auto order = [](const Position& p_0, const Position& p_1) {
            return p_0.row > p_1.row || (p_0.row == p_1.row && p_0.col > p_1.col);
        };
        sort(squares.begin(), squares.end(), order);
        while (!squares.empty()) {
            Position p = squares.back(); 
            squares.pop_back();
            seed_ = ReplaceColors(colors_, color_count_, p, seed_);
            ++squares_removed_;
            update_one_moves.Unite({p, {2, 2}});
            
            for (Position pp : SquarePositions(p)) {
                removed_ps.push_back(pp);
            }
            for (Position pp : PotentialRemovals(p)) {
                auto it = lower_bound(squares.begin(), squares.end(), pp, order);
                if (*it == pp) {
                    continue;
                } 
                squares.insert(it, pp);
            }
            while (!squares.empty() && !IsSquare(colors_, squares.back())) squares.pop_back(); 
        }
        
        for (auto p : removed_ps) {
            // value is changing inside RemoveLocation
            while (board_hashes_(p)) {
                RemoveLocation(board_hashes_(p)->value);
            }
        }
        VerifyHashIdentity();
        UpdateOneMoves(update_one_moves);
        VerifyHashIdentity();
    }

    // what's going on ???
    // getting rid of some combinations cuz of square removal
    void RemoveLocation(Location loc) {
        array<Position, 4> ps = OneMovePositions(loc);
        for (auto p : ps) {
            if (!board_hashes_(p)) continue;
            if (!board_hashes_(p)->previous) {
                board_hashes_(p).reset();
                continue;
            }
            auto copy = Node<Location>::DeepCopy(board_hashes_(p));
            board_hashes_(p) = Node<Location>::Erase(copy, loc);
        } 
        existing_hashes_.erase(loc);
    }
        
    void AddLocation(char r, char c, char combo) {
        Location h{r, c, combo};
        // already there
        // could just check bool from insert. works quicker
        if (existing_hashes_.count(h) == 1) return;
        existing_hashes_.insert(h);
        for (auto& p : OneMovePositions(h)) {
            shared_ptr<Node<Location>> n;
            n.reset(new Node<Location>());
            n->value = h;
            n->previous = board_hashes_(p);
            board_hashes_(p) = n;
        }
    }
    
    
    
    // multiple loops 
    // less work for each one
    void UpdateOneMoves_1(const Region& region) {
        const auto& cs = colors_; 
        
        // o - (r,c)
        
        // o0x
        // 0x0
        // x0x
        for (int r = region.row_begin() - 1; r < region.row_end(); ++r) {
            for (int c = region.col_begin() - 2; c < region.col_end(); ++c) {
                if (cs(r, c) == cs(r, c+1) && cs(r, c) == cs(r+1, c)) {
                    if (cs(r, c) == cs(r+1, c+2)) {
                        AddLocation(r, c, 2);
                    }
                    if (cs(r, c) == cs(r+2, c+1)) {
                        AddLocation(r, c, 3);
                    }
                }
            }
        }   
        // x0o
        // 0x0
        // x0x
        for (int r = region.row_begin() - 1; r < region.row_end(); ++r) {
            for (int c = region.col_begin(); c < region.col_end() + 2; ++c) {
                if (cs(r, c) == cs(r, c-1) && cs(r, c) == cs(r+1, c)) {
                    if (cs(r, c) == cs(r+1, c-2)) {
                        AddLocation(r, c-1, 5);
                    }
                    if (cs(r, c) == cs(r+2, c-1)) {
                        AddLocation(r, c-1, 4);
                    }
                }
            }
        }
        // x0x
        // 0x0
        // x0o
        for (int r = region.row_begin(); r < region.row_end() + 1; ++r) {
            for (int c = region.col_begin(); c < region.col_end() + 2; ++c) {
                if (cs(r, c) == cs(r, c-1) && cs(r, c) == cs(r-1, c)) {
                    if (cs(r, c) == cs(r-1, c-2)) {
                        AddLocation(r-1, c-1, 6);
                    }
                    if (cs(r, c) == cs(r-2, c-1)) {
                        AddLocation(r-1, c-1, 7);
                    }
                }
            }
        }
        // x0x
        // 0x0
        // o0x
        for (int r = region.row_begin(); r < region.row_end() + 1; ++r) {
            for (int c = region.col_begin() - 2; c < region.col_end() + 2; ++c) {
                if (cs(r, c) == cs(r-1, c) && cs(r, c) == cs(r, c+1)) {
                    if (cs(r, c) == cs(r-2, c+1)) {
                        AddLocation(r-1, c, 0);
                    }   
                    if (cs(r, c) == cs(r-1, c+2)) {
                        AddLocation(r-1, c, 1);
                    }
                }
            }
        }
    }
    
    // one big loop
    // some extra work for each branch
    void UpdateOneMoves_2(const Region& region) {
        const auto& cs = colors_; 
        
        for (int r = region.row_begin() -2; r < region.row_end() +2; ++r) {
            for (int c = region.col_begin() -2; c < region.col_end() +2; ++c) {
                if (cs(r, c) == cs(r, c+1) && cs(r, c) == cs(r+1, c)) {
                    if (cs(r, c) == cs(r+1, c+2)) {
                        AddLocation(r, c, 2);
                    }
                    if (cs(r, c) == cs(r+2, c+1)) {
                        AddLocation(r, c, 3);
                    }
                }
                if (cs(r, c) == cs(r, c-1) && cs(r, c) == cs(r+1, c)) {
                    if (cs(r, c) == cs(r+1, c-2)) {
                        AddLocation(r, c-1, 5);
                    }
                    if (cs(r, c) == cs(r+2, c-1)) {
                        AddLocation(r, c-1, 4);
                    }
                }
                if (cs(r, c) == cs(r, c-1) && cs(r, c) == cs(r-1, c)) {
                    if (cs(r, c) == cs(r-1, c-2)) {
                        AddLocation(r-1, c-1, 6);
                    }
                    if (cs(r, c) == cs(r-2, c-1)) {
                        AddLocation(r-1, c-1, 7);
                    }
                }
                if (cs(r, c) == cs(r-1, c) && cs(r, c) == cs(r, c+1)) {
                    if (cs(r, c) == cs(r-2, c+1)) {
                        AddLocation(r-1, c, 0);
                    }   
                    if (cs(r, c) == cs(r-1, c+2)) {
                        AddLocation(r-1, c, 1);
                    }
                }
                // 00x
                // x00
                if (cs(r, c) == cs(r, c+1) && cs(r, c) == cs(r+1, c+1) && cs(r, c) == cs(r+1, c+2)) {
                    AddDoubleLocation();
                }  
                // 
                if ()
                
            }
        }
    }
    
        
        
        
        
    void UpdateOneMoves(const Region& region) {
        UpdateOneMoves_2(region);
    }
                    
// legacy code here        
//        // top
//        if (ver_equality_(r-1, c)) {
//            // left
//            if (hor_equality_(r, c-1)) {
//                if (colors_(r, c) == colors_(r-1, c-2)) {
//                    // r-1, c-1, 6
//                    InsertHash( Hash(r-1, c-1, 6) );
//                }
//                if (colors_(r, c) == colors_(r-2, c-1)) {
//                    // r-1, c-1, 7
//                    InsertHash( Hash(r-1, c-1, 7) );
//                }
//            }
//            // right 
//            if (hor_equality_(r, c)) {
//                if (colors_(r, c) == colors_(r-2, c+1)) {
//                    // r-1, c, 0
//                    InsertHash( Hash(r-1, c, 0) );
//                }
//                if (colors_(r, c) == colors_(r-1, c+2)) {
//                    // r-1, c, 1
//                    InsertHash( Hash(r-1, c, 0) );
//                }
//            }
//        }
//        // bottom
//        if (ver_equality_(r, c)) {
//            // left
//            if (hor_equality_(r, c-1)) {
//                if (colors_(r, c) == colors_(r+1, c-2)) {
//                    // r, c-1, 5
//                    InsertHash( Hash(r, c-1, 5) );
//                }
//                if (colors_(r, c) == colors_(r+2, c-1)) {
//                    // r, c-1, 4
//                    InsertHash( Hash(r, c-1, 4) );
//                }
//            }
//            // right
//            if (hor_equality_(r, c)) {
//                if (colors_(r, c) == colors_(r+1, c+2)) {
//                    // r, c, 2
//                    InsertHash( Hash(r, c, 2) );
//                }
//                if (colors_(r, c) == colors_(r+2, c+1)) {
//                    // r, c, 3
//                    InsertHash( Hash(r, c, 3) );
//                }
//            }
//        }

    
};

    
template<Count B_SIZE>
ostream& operator<<(ostream& output, const Board<B_SIZE>& b) {
    for (auto r = 0; r < B_SIZE; ++r) {
        for (auto c = 0; c < B_SIZE; ++c) {
            output << (int)b(r, c);
        }
        output << endl;
    }
    output << endl;
    return output;
}



}



#endif
