//
//  stingy.h
//  SquareRemover
//
//  Created by Anton Logunov on 4/15/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __SquareRemover__stingy__
#define __SquareRemover__stingy__

#include <set>
#include <fstream>

#include "base.hpp"

namespace square_remover {

    struct Stingy : Base {
        
        virtual void init(int color_count, const vector<string>& board, Seed starting_seed) override {
            Base::init(color_count, board, starting_seed);
            right_color_equality_.resize(board_size_, board_size_-1);
            bottom_color_equality_.resize(board_size_-1, board_size_);
            for (auto r = 0; r < board_size_; ++r) {
                for (auto c = 0; c < board_size_; ++c) {
                    if (c < board_size_-1) right_color_equality_(r, c) = color_board_(r, c) == color_board_(r, c+1);
                    if (r < board_size_-1) bottom_color_equality_(r, c) = color_board_(r, c) == color_board_(r+1, c);
                }
            }
        }
        
        void outputEqualityBoard(ostream& output) {
            output << "right" << endl;
            for (auto r = 0; r < board_size_; ++r) {
                for (auto c = 0; c < board_size_; ++c) {
                    output << char(right_color_equality_(r, c) + '0');
                } 
                output << endl;
            }
            output << "bottom" << endl;
            for (auto r = 0; r < board_size_; ++r) {
                for (auto c = 0; c < board_size_; ++c) {
                    output << char(bottom_color_equality_(r, c) + '0');
                } 
                output << endl;
            }
        }
        
        // need to keep structure synchronized
        // p.r < board_size-1 
        // p.c < board_size-1
        virtual void replaceColors(const Position& p) override {
            Int r, c;
            Base::replaceColors(p);
            for (c = max(0, p.col-1); c <= min<Int>(board_size_-2, p.col+1); ++c) {
                for (r = p.row; r <= p.row+1; ++r) {
                    right_color_equality_(r, c) = color_board_(r, c) == color_board_(r, c+1);
                }
            }
            for (r = max(0, p.row-1); r <= min<Int>(board_size_-2, p.row+1); ++r) {
                for (c = p.col; c <= p.col+1; ++c) {
                    bottom_color_equality_(r, c) = color_board_(r, c) == color_board_(r+1, c);
                }
            }        
//            right_color_equality_(r, c)     =   color_board_(r, c) == color_board_(r, c+1);
//            right_color_equality_(r+1, c)   =   color_board_(r+1, c) == color_board_(r+1, c+1);
//            right_color_equality_(r, c-1)   =   color_board_(r, c-1) == color_board_(r, c);
//            right_color_equality_(r+1, c-1) =   color_board_(r+1, c-1) == color_board_(r+1, c);
//            right_color_equality_(r, c+1)   =   color_board_(r, c+1) == color_board_(r, c+2);
//            right_color_equality_(r+1, c+1) =   color_board_(r+1, c+1) == color_board_(r+1, c+2);
//            
//            bottom_color_equality_(r-1, c)  =   color_board_(r-1, c) == color_board_(r, c);
//            bottom_color_equality_(r-1, c+1)=   color_board_(r-1, c+1) == color_board_(r, c+1);
//            bottom_color_equality_(r, c)    =   color_board_(r, c) == color_board_(r+1, c);
//            bottom_color_equality_(r, c+1)  =   color_board_(r, c+1) == color_board_(r+1, c+1);
//            bottom_color_equality_(r+1, c)  =   color_board_(r+1, c) == color_board_(r+2, c);
//            bottom_color_equality_(r+1, c+1)=  color_board_(r+1, c+1) == color_board_(r+2, c+1);
        }

        virtual void makeMove(const Move& m) override {
            Base::makeMove(m);
            Position p = m.position;
            Int 
            &r = p.row, 
            &c = p.col;
            
            switch (m.direction) {
            case kLeft: p = m.target();
            case kRight:
                right_color_equality_(r, c)     =   color_board_(r, c) == color_board_(r, c+1);
                if (c > 0) right_color_equality_(r, c-1)   =   color_board_(r, c-1) == color_board_(r, c);
                if (c < board_size_-2) right_color_equality_(r, c+1)   =   color_board_(r, c+1) == color_board_(r, c+2);
                
                if (r > 0) { 
                    bottom_color_equality_(r-1, c)  =   color_board_(r-1, c) == color_board_(r, c);
                    bottom_color_equality_(r-1, c+1)=   color_board_(r-1, c+1) == color_board_(r, c+1);
                }
                if (r < board_size_-1) {
                    bottom_color_equality_(r, c)    =   color_board_(r, c) == color_board_(r+1, c);
                    bottom_color_equality_(r, c+1)  =   color_board_(r, c+1) == color_board_(r+1, c+1);
                }
                break;
            case kUp: p = m.target();
            case kDown:
                if (c < board_size_-1) { 
                    right_color_equality_(r, c)     =   color_board_(r, c) == color_board_(r, c+1);
                    right_color_equality_(r+1, c)   =   color_board_(r+1, c) == color_board_(r+1, c+1);
                }
                if (c > 0) {
                    right_color_equality_(r, c-1)   =   color_board_(r, c-1) == color_board_(r, c);
                    right_color_equality_(r+1, c-1) =   color_board_(r+1, c-1) == color_board_(r+1, c);
                }
                bottom_color_equality_(r, c)    =   color_board_(r, c) == color_board_(r+1, c);
                if (r > 0) bottom_color_equality_(r-1, c)  =   color_board_(r-1, c) == color_board_(r, c);
                if (r < board_size_-2) bottom_color_equality_(r+1, c)  =   color_board_(r+1, c) == color_board_(r+2, c);
                break;
            }
        }
        
        virtual bool isFourSquare(const Position& p) {
            Int r = p.row, c = p.col;
            return right_color_equality_(r, c) && right_color_equality_(r+1, c) && bottom_color_equality_(r, c); 
        }
        
        virtual void searchFourMoves(const Position& position, const Rectangle& rect, 
                                     vector<Move>& sqs, unordered_set<Index>& sqs_inds) {
            Int r = position.row, 
            c = position.col;
            Position
            p_m,
            p_0 = {r, c},
            p_1 = {r, c+1}, 
            p_2 = {r+1, c}, 
            p_3 = {r+1, c+1};
            pair<Position, Move> sqs_pair;
            sqs_pair.first = p_0;
            bool 
            b_0_1 = right_color_equality_(p_0),
            b_2_3 = right_color_equality_(p_2),
            b_0_2 = bottom_color_equality_(p_0),
            b_1_3 = bottom_color_equality_(p_1);
            
            Count count = 0;
            if (b_0_1 && b_0_2) {
                p_m = p_3+Indent{1, 0};
                if (p_3.row < rect.row_end()-1 && isSameColor(p_0, p_m)) {
                    ++count;
                    sqs.emplace_back(p_3, kDown);
                }
                p_m = p_3+Indent{0, 1};
                if (p_3.col < rect.col_end()-1 && isSameColor(p_0, p_m)) {
                    ++count;
                    sqs.emplace_back(p_3, kRight);
                }
            }
            if (b_0_1 && b_1_3) {
                p_m = p_2+Indent{1, 0};
                if (p_2.row < rect.row_end()-1 && isSameColor(p_0, p_m)) {
                    ++count;
                    sqs.emplace_back(p_2, kDown);
                }
                p_m = p_2+Indent{0,-1};
                if (p_2.col > rect.col_begin() && isSameColor(p_0, p_m)) {
                    ++count;
                    sqs.emplace_back(p_m, kRight);
                }
            }
            if (b_2_3 && b_0_2) {
                p_m = p_1+Indent{-1,0};
                if (p_1.row > rect.row_begin() && isSameColor(p_0, p_m)) {
                    ++count;
                    sqs.emplace_back(p_m, kDown);
                }
                p_m = p_1+Indent{0, 1};
                if (p_1.col < rect.col_end()-1 && isSameColor(p_0, p_m)) {
                    ++count;
                    sqs.emplace_back(p_1, kRight);
                }
            }
            if (b_1_3 && b_2_3) {
                p_m = p_0+Indent{-1,0};
                if (p_0.row > rect.row_begin() && isSameColor(p_1, p_m)) {
                    ++count;
                    sqs.emplace_back(p_m, kDown);
                }
                p_m = p_0+Indent{0,-1};
                if (p_0.col > rect.col_begin() && isSameColor(p_1, p_m)) {
                    ++count;
                    sqs.emplace_back(p_m, kRight);
                }
            }
            
            // remove repeating moves
            Move m;
            Index ind;
            for (Index i = sqs.size()-count; i < sqs.size();) {
                m = sqs[i];
                ind = m.index();
                if (sqs_inds.count(ind)) {
                    swap(sqs[i], sqs.back());
                    sqs.pop_back();
                }  
                else {
                    sqs_inds.insert(ind);
                    ++i;
                }
            }
        }
        
        Grid<char> right_color_equality_;
        Grid<char> bottom_color_equality_;
    };
}
    


#endif /* defined(__SquareRemover__stingy__) */



















