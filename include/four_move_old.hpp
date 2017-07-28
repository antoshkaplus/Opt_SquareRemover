//
//  strict.h
//  SquareRemover
//
//  Created by Anton Logunov on 4/19/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __SquareRemover__strict__
#define __SquareRemover__strict__

#include <iostream>
#include <fstream>

#include "balance.hpp"
#include "base.hpp"

namespace square_remover {

    struct FourMoveSolver : Base {

        typedef u_char Degree;

        struct FourMove : Move {
            FourMove() {}
            FourMove(const Move& m)
                : Move(m) {}
            Degree degree;
            Position square[2];

            Degree update(FourMoveSolver& solver) {
                Count size = solver.board_.size();
                Position p = pos;
                bool b_0, b_1, c_0_0 = false, c_0_1 = false, c_1_0 = false, c_1_1 = false;
                solver.board_.MakeMove(*this);
                degree = 0;
                // here i want to know not only elimination degree, but eliminated squares
                switch(dir) {
                    case kDirUp:
                        p = another();
                    case kDirDown:
                        if (p.row > 0) {
                            c_0_0 = p.col > 0        && solver.isFourSquare(p + Indent{-1, -1});
                            c_0_1 = p.col < size-1   && solver.isFourSquare(p + Indent{-1,  0});
                        }
                        if (p.row < size-2) {
                            c_1_0 = p.col > 0        && solver.isFourSquare(p + Indent{ 1, -1});
                            c_1_1 = p.col < size-1   && solver.isFourSquare(p + Indent{ 1,  0});
                        }

                        if (c_0_0) square[degree++] = p + Indent{-1, -1};
                        else if (c_0_1) square[degree++] = p + Indent{-1,  0};

                        if (c_1_0) square[degree++] = p + Indent{ 1, -1};
                        else if (c_1_1) square[degree++] = p + Indent{ 1,  0};

                        break;
                    case kDirLeft:
                        p = another();
                    case kDirRight:
                        if (p.col > 0) {
                            c_0_0 = p.row > 0        && solver.isFourSquare(p + Indent{-1, -1});
                            c_0_1 = p.row < size-1   && solver.isFourSquare(p + Indent{ 0, -1});
                        }
                        if (p.col < size-2) {
                            c_1_0 = p.row > 0        && solver.isFourSquare(p + Indent{-1,  1});
                            c_1_1 = p.row < size-1   && solver.isFourSquare(p + Indent{ 0,  1});
                        }

                        if (c_0_0) square[degree++] = p + Indent{-1, -1};
                        else if (c_0_1) square[degree++] = p + Indent{0,  -1};

                        if (c_1_0) square[degree++] = p + Indent{ -1, 1};
                        else if (c_1_1) square[degree++] = p + Indent{ 0, 1};

                        break;
                    default: assert(false);
                }
                solver.board_.MakeMove(*this);
                return degree;
            }
        };

        vector<int> playIt(int colors, vector<string> board, int startingSeed) {
//            ofstream output("out.txt");
            vector<int> int_moves;
            init(colors, board, startingSeed);
            remover_.FindRemove();
            updateMoves();
            Move m, m_some;
            Count d, d_some;
            FourMoveSolver buffer;
            for (Index i = 0; i < kMoveCount; ++i) {
                if (four_moves_.size()) {
                    d = 0;
                    for (auto& p : four_moves_) {
                        m_some = p.second;
                        buffer = *this;
                        buffer.board_.MakeMove(m_some);
                        auto sqs = buffer.remover_.Remove(m_some);
                        buffer.updateMovesAfterEliminationMove(m_some, sqs);
                        d_some = sqs.size() + buffer.four_moves_.size();
                        if (d_some > d) {
                            m = m_some;
                            d = d_some;
                        }
                    }
//                    outputColorBoard(output);
//                    output << m << endl;
                    makeMove(m);
                    auto sqs = eliminate(m);
                    updateMovesAfterEliminationMove(m, sqs);
                }
                else {
                    m = randomMove();
                    makeMove(m);
                    updateMovesAfterMove(m);
                }
                int_moves.push_back(m.pos.row);
                int_moves.push_back(m.pos.col);
                int_moves.push_back(m.dir);
            }
            return int_moves;
        }

        // use update methods instead of validate
        void updateMoves() {
            updateMoves(boardRectangle());
        }

        void updateMoves(const Region& rect) {
            // need to validate
            Index ind;
            FourMove fm;
            validate(rect);
            for (auto& m : fourMoves(rect)) {
                ind = m.index();
                if (four_moves_.count(ind) == 0) {
                    fm = m;
                    fm.update(*this);
                    four_moves_.emplace(ind, fm);
                }
            }
        }

        void updateMovesAfterMove(const Move& m) {
            // first get to top left
            Int r = m.pos.row,
                c = m.pos.col;
            Region rect;
            switch (m.dir) {
                case kDirLeft:
                    rect.set(r-2, c-3, 5, 6);
                    break;
                case kDirRight:
                    rect.set(r-2, c-2, 5, 6);
                    break;
                case kDirUp:
                    rect.set(r-3, c-2, 6, 5);
                    break;
                case kDirDown:
                    rect.set(r-2, c-2, 6, 5);
                    break;
            }
            updateMoves(boardRectangle().intersection(rect));
        }

        void updateMovesAfterEliminationMove(const Move& m, const vector<Position>& sqs) {
            vector<Region> rect_sqs;
            for (auto& s : sqs) {
                rect_sqs.push_back(Region(s.row-2, s.col-2, 6, 6));
            }
            Position s;
            s = m.pos;
            rect_sqs.push_back(Region(s.row-2, s.col-2, 5, 5));
            s = m.another();
            rect_sqs.push_back(Region(s.row-2, s.col-2, 5, 5));

            // put in united rectangle
            if (rect_sqs.size()) {
                updateMoves(boardRectangle().intersection(
                    Region::unite(rect_sqs.begin(), rect_sqs.end())));
            }
        }

        void validate() {
            validate(boardRectangle());
        }

        void validate(const Region& rect) {
            FourMove m;
            vector<Index> remove_indices;
            for (auto& p : four_moves_) {
                m = p.second;
                if (!rect.hasInside(m.pos)) continue;
                if (m.update(*this) == 0) {
                    remove_indices.push_back(p.first);
                }
            }
            for (auto index : remove_indices) {
                four_moves_.erase(index);
            }
        }

        double totalDegree() {
            Int s = 0;
            vector<Region> sqs;
            for (auto p : four_moves_) {
                s += p.second.degree;
                for (int i = 0; i < p.second.degree; ++i) {
                    sqs.push_back(Region(p.second.square[i], Size{2, 2}));
                }
            }
            // would like also remove squares that have same moving point
            for (int i = 0; i < sqs.size(); ++i) {
                for (int j = i+1; j < sqs.size(); ++j) {
                    if (!sqs[i].intersection(sqs[j]).isEmpty()) {
                        //assert(s!=0);
                        --s;
                        break;
                    }
                }
            }
            return s;
        }

        // will be small number of those. so no need in unordered_map probably
        map<Index, FourMove> four_moves_;
    };
}

#endif /* defined(__SquareRemover__strict__) */
