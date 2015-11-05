//
//  base.h
//  SquareRemover
//
//  Created by Anton Logunov on 4/12/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __SquareRemover__base__
#define __SquareRemover__base__

#include <vector>
#include <cassert>
#include <random>
#include <array>
#include <queue>
#include <unordered_set>

#include "ant/core/core.hpp"
#include "ant/grid.hpp"

#include "util.hpp"

//// namespace square_remover {
//    // could just use v_2...3...
//    /* because FourSameColorSquareMoves or ThreeSameColorSquares 
//     * are long names and considering context of the problem we 
//     * will use names FourSquareMoves or ThreeSquares (SameColor 
//     * is clear from the context) */
//    
////using namespace std;
////using namespace ant;
////using namespace ant::grid;
////
////
//namespace square_remover {
//
//
//    using namespace std;
//    using namespace ant;
//    using namespace ant::grid;    
//    array<Position, 4>  squarePositions(const Position& square);
//    Direction           oppositeDirection(Direction d);
//    string              directionString(Direction d);
//    Direction           randomDirection();
//    Rectangle           squareRectangle(const Position& square);
//}
            
//    /* if you want more "directed" move make subclass and rewrite index */
//        
//    struct DoubleMove : Move {
//        DoubleMove() {}
//        DoubleMove(const Move& m, const Move& next)
//            : Move(m), next(next) {}
//        Index index() {
//            Index i = 0;
//            i = next.index() << 11 | Move::index();
//            return i;
//        }
//        Move next;
//    };
//    
//    ostream& operator<<(ostream& output, const Move& m);
//    
//    // use this class to make solvers that doesn't depand on Base
//    struct Solver {
//        virtual vector<int> playIt(int colors, vector<string> board, int startingSeed) = 0;
//        virtual Count removedCount() = 0;
//        static const Count kMoveCount = 1e+4;
//    };
//    
//    /* uses rand() function, so be careful and probably init srand() */
//    struct Base : Solver {
//        vector<int> playIt(int colors, vector<string> board, int startingSeed) override {
//            vector<int> moves;
//            init(colors, board, startingSeed);
//            eliminate();
//            for (auto i = 0; i < kMoveCount; ++i) {
//                Move m = randomMove();
//                makeMove(m);
//                moves.push_back(m.position.row);
//                moves.push_back(m.position.col);
//                moves.push_back(m.direction);
//                eliminate();
//            }
//            return moves;
//        }
//
//    protected:
//        
//        Position randomPosition() {
//            return Position(rand()%board_size_, rand()%board_size_);
//        }
//        
//        Move randomMove() {
//            Move m; 
//            Position p;
//            do {
//                m.position = randomPosition();
//                m.direction = randomDirection();
//                p = m.target();
//            } while(!color_board_.isInside(p) || 
//                     color_board_(p) == color_board_(m.position));
//            return m;
//        }
//        
//        virtual void init(int color_count, const vector<string>& board, Seed starting_seed) {
//            srand((uint)starting_seed);
//            color_produced_ = false;
//            color_seed_ = starting_seed;
//            board_size_ = board.size();
//            color_count_ = color_count;
//            eliminated_count_ = 0;
//            color_board_.resize(board_size_, board_size_);
//            for (auto row = 0; row < board_size_; ++row) {
//                for (auto col = 0; col < board_size_; ++col) {
//                    color_board_(row, col) = board[row][col] - '0';
//                }
//            }
//        }
//        
//        bool isSameColor(const Position& p_0, 
//                         const Position& p_1,
//                         const Position& p_2,
//                         const Position& p_3) {
//            Color c = color_board_(p_0);
//            return c == color_board_(p_1) &&
//                   c == color_board_(p_2) &&
//                   c == color_board_(p_3);
//        }
//        
//        bool isSameColor(const Position& p_0, 
//                         const Position& p_1,
//                         const Position& p_2) {
//            return color_board_(p_0) == color_board_(p_1) &&
//                   color_board_(p_1) == color_board_(p_2);
//        }
//        
//        bool isSameColor(const Position& p_0,
//                         const Position& p_1) {
//            return color_board_(p_0) == color_board_(p_1);
//        }
//        
//        Count countColorInSquare(const Position& p, Color c) {
//            return (color_board_(p) == c) + 
//                   (color_board_(p.row  , p.col+1) == c) + 
//                   (color_board_(p.row+1, p.col  ) == c) +
//                   (color_board_(p.row+1, p.col+1) == c);
//        }
//        
//        virtual bool isFourSquare(const Position& p) {
//            return isSameColor(p, 
//                               p+Indent{0, 1}, 
//                               p+Indent{1, 0}, 
//                               p+Indent{1, 1}); 
//        }
//        
//        static bool isInsideSquare(const Position& square, const Position& pos) {
//            return pos.row >= square.row   && 
//                   pos.row <  square.row+2 && 
//                   pos.col >= square.col   && 
//                   pos.col <  square.col+2;
//        }
//                
//        virtual void replaceColors(const Position& p) {
//            Int r = p.row, c = p.col;
//            ++eliminated_count_;
//            color_board_(r, c)      = nextColor();
//            color_board_(r, c+1)    = nextColor();
//            color_board_(r+1, c)    = nextColor();
//            color_board_(r+1, c+1)  = nextColor();
//        }
//            
//    private:
//        vector<Position> eliminateCandidates(const Rectangle& rect) {
//            vector<Position> candidates;
//            for (auto r = rect.row_begin(); r < rect.row_end()-1; ++r) {
//                for (auto c = rect.col_begin(); c < rect.col_end()-1; ++c) {
//                    Position p(r, c);
//                    if (isFourSquare(p)) candidates.push_back(p);
//                }
//            }
//            return candidates;  
//        }
//        
//    protected:
//        // need function that receives just position
//        virtual vector<Position> eliminate(Rectangle rect) {
//            vector<Position> squares;
//            priority_queue<Position, vector<Position>, Position::BottomRightComparator> heap;
//            while(true) {
//                for (auto& p : eliminateCandidates(rect)) {
//                    heap.push(p);
//                }
//                if (heap.empty()) break;
//                Position p = heap.top();
//                heap.pop();
//                // when we come p maybe already not to be a square
//                if (isFourSquare(p)) {
//                    replaceColors(p);
//                    squares.push_back(p);
//                    rect = boardRectangle().intersection(Rectangle(p.row-1, p.col-1, 4, 4));
//                }
//            }
//            return squares;
//        }
//        
//        Degree eliminationDegree(const Move& m) {
//            Position p = m.position;
//            bool b_0, b_1;
//            makeMove(m);
//            switch(m.direction) {
//                case kUp:
//                    p = m.target();
//                case kDown: 
//                    b_0 = 
//                    (p.row > 0 &&
//                        ((p.col > 0               && isFourSquare(p + Indent{-1, -1})) ||
//                         (p.col < board_size_-1   && isFourSquare(p + Indent{-1,  0}))));
//                    b_1 =
//                    (p.row < board_size_-2 && 
//                        ((p.col > 0               && isFourSquare(p + Indent{ 1, -1})) || 
//                         (p.col < board_size_-1   && isFourSquare(p + Indent{ 1,  0}))));
//                    break;
//                case kLeft:
//                    p = m.target();
//                case kRight:
//                    b_0 = 
//                    (p.col > 0 && 
//                        ((p.row > 0               && isFourSquare(p + Indent{-1, -1})) ||
//                         (p.row < board_size_-1   && isFourSquare(p + Indent{ 0, -1}))));
//                    b_1 = 
//                    (p.col < board_size_-2 &&
//                        ((p.row > 0               && isFourSquare(p + Indent{-1,  1})) ||
//                         (p.row < board_size_-1   && isFourSquare(p + Indent{ 0,  1}))));
//                    break;
//                default: assert(false);
//            }
//            makeMove(m);
//            return b_0 + b_1;
//        }
//        
//        bool canEliminate(const Move& m) {
//            return eliminationDegree(m) > 0;
//        }
//        
//        vector<Position> eliminate(const Move& m) {
//            Rectangle rect;
//            rect.position = m.position;
//            switch(m.direction) {
//            case kDown: 
//                rect.shift(-1, -1);
//                rect.size.set(4, 3);
//                break;
//            case kUp:
//                rect.shift(-2, -1);
//                rect.size.set(4, 3);
//                break;
//            case kRight:
//                rect.shift(-1, -1);
//                rect.size.set(3, 4);
//                break;
//            case kLeft:
//                rect.shift(-1, -2);
//                rect.size.set(3, 4);
//                break;
//            default: assert(false);
//            }
//            return eliminate(boardRectangle().intersection(rect));
//        }
//        
//        vector<Position> eliminate() {
//            return eliminate(boardRectangle());
//        }
//        
//        /* probably need something like factory
//           to move from one color to another
//          */                         
//        Color nextColor() {
//            if (color_produced_) { 
//                color_seed_ = (color_seed_ * 48271) % 2147483647;
//            } else color_produced_ = true;
//            return Color(color_seed_ % color_count_);
//        }
//        
//        virtual void makeMove(const Move& m) {
//            swap(color_board_(m.position), color_board_(m.target()));
//        }
//      
//        Rectangle boardRectangle() {
//            return Rectangle(0, 0, (int)board_size_, (int)board_size_);
//        }
//        
//        void outputColorBoard(ostream& output) {
//            for (auto r = 0; r < board_size_; ++r) {
//                for (auto c = 0; c < board_size_; ++c) {
//                    output << char(color_board_(r, c) + '0');
//                } 
//                output << endl;
//            }
//        }
//        
//        /* you can use fourMoves methods where you want
//           probably they belong to base class */
//        vector<Move> fourMoves() {
//            return fourMoves(Rectangle(0, 0, (int)board_size_, (int)board_size_));
//        }
//        
//        vector<Move> fourMoves(const Rectangle& rect) {
//            vector<Move> sqs;
//            unordered_set<Index> sqs_inds;
//            auto 
//            r_begin = rect.row_begin(), 
//            r_end   = rect.row_end(),
//            c_begin = rect.col_begin(),
//            c_end   = rect.col_end();
//            Position p;
//            for (auto r = r_begin; r < r_end-1; ++r) {
//                for (auto c = c_begin; c < c_end-1; ++c) {
//                    p.set(r, c);
//                    searchFourMoves(p, rect, sqs, sqs_inds);
//                }
//            }
//            return sqs; 
//        }
//        
//        
//        // all four moves of special color
//        vector<Move> fourMoves(const Rectangle& rect, Color color) {
//            vector<Move> sqs;
//            unordered_set<Index> sqs_inds;
//            auto 
//            r_begin = rect.row_begin(), 
//            r_end   = rect.row_end(),
//            c_begin = rect.col_begin(),
//            c_end   = rect.col_end();
//            Position p;
//            for (auto r = r_begin; r < r_end-1; ++r) {
//                for (auto c = c_begin; c < c_end-1; ++c) {
//                    p.set(r, c);
//                    if (countColorInSquare(p, color) == 3) { 
//                        searchFourMoves(p, rect, sqs, sqs_inds);
//                    }
//                }
//            }
//            return sqs; 
//        }
//        
//        virtual void searchFourMoves(const Position& position, const Rectangle& rect, 
//                             vector<Move>& sqs, unordered_set<Index>& sqs_inds) {
//            Int r = position.row, 
//            c = position.col;
//            Position
//            p_m,
//            p_0 = {r, c},
//            p_1 = {r, c+1}, 
//            p_2 = {r+1, c}, 
//            p_3 = {r+1, c+1};
//            pair<Position, Move> sqs_pair;
//            sqs_pair.first = p_0;
//            bool 
//            b_0_1 = isSameColor(p_0, p_1),
//            b_1_2 = isSameColor(p_1, p_2),
//            b_2_3 = isSameColor(p_2, p_3),
//            b_0_3 = isSameColor(p_0, p_3);
//            
//            Count count = 0;
//            if (b_0_1 && b_1_2) {
//                p_m = p_3+Indent{1, 0};
//                if (p_3.row < rect.row_end()-1 && isSameColor(p_0, p_m)) {
//                    ++count;
//                    sqs.push_back(Move(p_3, kDown));
//                }
//                p_m = p_3+Indent{0, 1};
//                if (p_3.col < rect.col_end()-1 && isSameColor(p_0, p_m)) {
//                    ++count;
//                    sqs.push_back(Move(p_3, kRight));
//                }
//            }
//            if (b_0_1 && b_0_3) {
//                p_m = p_2+Indent{1, 0};
//                if (p_2.row < rect.row_end()-1 && isSameColor(p_0, p_m)) {
//                    ++count;
//                    sqs.push_back(Move(p_2, kDown));
//                }
//                p_m = p_2+Indent{0,-1};
//                if (p_2.col > rect.col_begin() && isSameColor(p_0, p_m)) {
//                    ++count;
//                    sqs.push_back(Move(p_m, kRight));
//                }
//            }
//            if (b_2_3 && b_0_3) {
//                p_m = p_1+Indent{-1,0};
//                if (p_1.row > rect.row_begin() && isSameColor(p_0, p_m)) {
//                    ++count;
//                    sqs.push_back(Move(p_m, kDown));
//                }
//                p_m = p_1+Indent{0, 1};
//                if (p_1.col < rect.col_end()-1 && isSameColor(p_0, p_m)) {
//                    ++count;
//                    sqs.push_back(Move(p_1, kRight));
//                }
//            }
//            if (b_1_2 && b_2_3) {
//                p_m = p_0+Indent{-1,0};
//                if (p_0.row > rect.row_begin() && isSameColor(p_1, p_m)) {
//                    ++count;
//                    sqs.push_back(Move(p_m, kDown));
//                }
//                p_m = p_0+Indent{0,-1};
//                if (p_0.col > rect.col_begin() && isSameColor(p_1, p_m)) {
//                    ++count;
//                    sqs.push_back(Move(p_m, kRight));
//                }
//            }
//            
//            // remove repeating moves
//            Move m;
//            Index ind;
//            for (Index i = sqs.size()-count; i < sqs.size();) {
//                m = sqs[i];
//                ind = m.index();
//                if (sqs_inds.count(ind)) {
//                    swap(sqs[i], sqs.back());
//                    sqs.pop_back();
//                }  
//                else {
//                    sqs_inds.insert(ind);
//                    ++i;
//                }
//            }
//        }
//    public:
//        Count removedCount() override {
//            return eliminated_count_;
//        }
//        
//    protected:
//        bool color_produced_;
//        Seed color_seed_;
//        Grid<Color> color_board_;
//        Count board_size_;
//        Count color_count_;
//        Count eliminated_count_;
//    
//    //friend struct Stingy;
//    //friend struct Last;
//    friend struct Wide;
//    };
//}
//
#endif /* defined(__SquareRemover__base__) */
