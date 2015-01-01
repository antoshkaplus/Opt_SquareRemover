//
//  wide.h
//  SquareRemover
//
//  Created by Anton Logunov on 4/14/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __SquareRemover__wide__
#define __SquareRemover__wide__

#include <fstream>

#include "four_move.h"
#include "balance.h"

namespace square_remover {
    
    struct Wide : FourMoveSolver {
        
        struct Stats {
            Stats() : 
                eliminated_count(0), 
                four_move_degree(0), 
                position_balance(0),
                board_balance(MAXFLOAT) {}
            Int eliminated_count;
            double four_move_degree;
            double position_balance;
            double board_balance;
        };
        
               
        Wide() : balance_coeff(MAXFLOAT), pos_balance_coeff(1.), board_balance_coeff(1.) {   
        }
        
        virtual vector<int> playIt(int colors, vector<string> board, int startingSeed) override;
        
        virtual void init(int color_count, const vector<string>& board, Seed starting_seed) {
            FourMoveSolver::init(color_count, board, starting_seed);
            four_buffer_ = unique_ptr<FourMoveSolver>(new FourMoveSolver());
            wide_buffer_ = unique_ptr<Wide>(new Wide());
        }
        
        pair<Move, Stats> bestFourMove() {
            FourMoveSolver &four_buffer = *four_buffer_.get();
            FourMoveSolver four_solver;
            Move m, m_best;
            Stats s, s_best;
            for (auto& p : four_moves_) {
                m = p.second;
                four_buffer = *this;
                four_buffer.makeMove(m);
                auto sqs = four_buffer.eliminate(m);
                // validation on update
                four_buffer.updateMovesAfterEliminationMove(m, sqs);
                s.eliminated_count = (int)sqs.size();
                if (s.eliminated_count < s_best.eliminated_count) {
                    continue;
                } 
                s.four_move_degree = four_buffer.totalDegree();
                if (s.eliminated_count == s_best.eliminated_count && s.four_move_degree < s_best.four_move_degree) {
                    continue;
                }  
                s.position_balance = four_buffer.positionBalance();
                s.board_balance = four_buffer.boardBalance();
                // bigger - better
                if (s.eliminated_count == s_best.eliminated_count && s.four_move_degree == s_best.four_move_degree 
                    && pos_balance_coeff*s.position_balance - board_balance_coeff*s.board_balance <=
                       pos_balance_coeff*s_best.position_balance - board_balance_coeff*s_best.board_balance) {
                    continue;
                }   
                
                s_best = s;
                m_best = m;

                
            }
            return {m_best, s_best};

        }
        
        
        pair<Move, Stats> bestPreFourMove() {
            FourMoveSolver &four_buffer = *four_buffer_.get();
            DoubleMove m, m_best;
            Stats s, s_best;
            for (auto p : pre_four_moves_) {
                m = p.second;
                four_buffer = *this;
                four_buffer.makeMove(m);
                four_buffer.updateMovesAfterMove(m);
                four_buffer.makeMove(m.next);
                auto sqs = four_buffer.eliminate(m.next);
                four_buffer.updateMovesAfterEliminationMove(m.next, sqs);
                s.eliminated_count = (int)sqs.size();
                if (s.eliminated_count < s_best.eliminated_count) {
                    continue;
                } 
                s.four_move_degree = four_buffer.totalDegree();
                if (s.eliminated_count == s_best.eliminated_count && s.four_move_degree < s_best.four_move_degree) {
                    continue;
                }  
                s.position_balance = four_buffer.positionBalance();
                s.board_balance = four_buffer.boardBalance();
                // bigger - better
                if (s.eliminated_count == s_best.eliminated_count && s.four_move_degree == s_best.four_move_degree 
                    && pos_balance_coeff*s.position_balance - board_balance_coeff*s.board_balance <=
                    pos_balance_coeff*s_best.position_balance - board_balance_coeff*s_best.board_balance) {
                    continue;
                }   
                
                s_best = s;
                m_best = m;
                
            }
            return {m_best, s_best};
        }
        
        Move bestFarMove() {
            Move m, m_best = randomMove();
            // count of pre four moves
            Count count = 0, count_best = 0;
            Move ms[2];
            Count ms_count;
            for (int r = 0; r < board_size_; ++r) {
                for (int c = 0; c < board_size_; ++c) {
                    // nothing to validate only hard watch
                    ms_count = 0;
                    if (c < board_size_-1) {
                        ms[ms_count++] = Move(r, c, kRight);
                    }
                    if (r < board_size_-1) {
                        ms[ms_count++] = Move(r, c, kDown);
                    }
                    while (ms_count) {
                        m = ms[--ms_count];
                        makeMove(m);
                        count = preFourMoves().size();
                        if (count > count_best) {
                            count_best = count;
                            m_best = m;
                        }
                        // back up
                        makeMove(m);
                    }    
                }
            }
            return m_best;
        }
        
        vector<DoubleMove> preFourMoves() {
            return preFourMoves(boardRectangle());
        }
        
        
        // dont expect possible four moves here
        vector<DoubleMove> preFourMoves(const Rectangle& rect) {
            vector<DoubleMove> pre_sqs;
            Rectangle rect_search;
            Size d;
            Move m;
            bool b;
            array<tuple<bool, Move, Size>, 2> items;
            for (auto r = rect.row_begin(); r < rect.row_end(); ++r) {
                for (auto c = rect.col_begin(); c < rect.col_end(); ++c) {
                    rect_search.position = {r-2, c-2};
                    if (r < board_size_-1 && color_board_(r, c) != color_board_(r+1, c)) {
                        items[0] = make_tuple(true, Move(r, c, kDown), Size(6, 5));
                    }
                    else get<0>(items[0]) = false;
                    
                    if (c < board_size_-1 && color_board_(r, c) != color_board_(r, c+1)) {
                        items[1] = make_tuple(true, Move(r, c, kRight), Size(5, 6));
                    }
                    else get<0>(items[1]) = false;
                    
                    for (auto i : {0, 1}) {
                        tie(b, m, d) = items[i];
                        if (!b || four_moves_.count(m.index())) continue;
                        rect_search.size = d;
                        makeMove(m);
                        for (auto four_move : fourMoves(boardRectangle().intersection(rect_search))) {
                            if (four_moves_.count(four_move.index()) == 0) { 
                                pre_sqs.push_back(DoubleMove(m, four_move));
                            }
                        }
                        makeMove(m);
                    }
                }
            }
            return pre_sqs;
        }
        
        // probably should return vector of new moves
        void updateMoves() {
            updateMoves(boardRectangle());
        }
        
        // need to run validation first (need to have right four_moves) or reimplement preFourMoves 
        void updateMoves(const Rectangle& rect) {
            FourMoveSolver::updateMoves(rect);
            validatePreFourMoves();
            updatePreFourMoves(rect);            
        }
                
        void updatePreFourMoves(const Rectangle& rect) {
            Index ind;
            for (auto& m : preFourMoves(rect)) {
                ind = m.index();
                if (pre_four_moves_.count(ind) == 0) {
                    pre_four_moves_.emplace(ind, m);
                }
            }
        }
        
        void updateMovesAfterMove(const Move& m) {
            // first get to top left
            Int r = m.position.row,
            c = m.position.col;
            Rectangle rect;
            switch (m.direction) {
                case kLeft:
                    rect.set(r-3, c-4, 7, 8);
                    break;
                case kRight:
                    rect.set(r-3, c-3, 7, 8);
                    break;
                case kUp:
                    rect.set(r-4, c-3, 8, 7);
                    break;
                case kDown:
                    rect.set(r-3, c-3, 8, 7);
                    break;
            }
            updateMoves(boardRectangle().intersection(rect));
        }
        
        void updateMovesAfterEliminationMove(const Move& m, const vector<Position>& sqs) {
            vector<Rectangle> rect_sqs;
            for (auto& s : sqs) {
                rect_sqs.push_back(Rectangle(s.row-3, s.col-3, 8, 8));
            }
            Position s;
            s = m.position;
            rect_sqs.push_back(Rectangle(s.row-3, s.col-3, 7, 7));
            s = m.target();
            rect_sqs.push_back(Rectangle(s.row-3, s.col-3, 7, 7));
            
            // put in united rectangle
            if (rect_sqs.size()) { 
                updateMoves(boardRectangle().intersection(
                    Rectangle::unite(rect_sqs.begin(), rect_sqs.end())));
            }
        }
        
        void validatePreFourMoves() {
            DoubleMove pre_m; 
            vector<Index> remove_indices;   
            for (auto& p : pre_four_moves_) {
                pre_m = p.second;
                makeMove(pre_m);
                if (four_moves_.count(pre_m.Move::index()) || !canEliminate(pre_m.next)) {
                    remove_indices.push_back(p.first);
                } 
                makeMove(pre_m);
            }
            for (auto index : remove_indices) {
                pre_four_moves_.erase(index);
            }
        }
        
        Move last_move;
        Color last_color_p, last_color_t;
        double balance_coeff, pos_balance_coeff, board_balance_coeff;
        map<Index, DoubleMove> pre_four_moves_;
        unique_ptr<FourMoveSolver> four_buffer_;
        unique_ptr<Wide> wide_buffer_;
    };
    
    ostream& operator<<(ostream& output, const Wide::Stats& s);
}



#endif /* defined(__SquareRemover__wide__) */
