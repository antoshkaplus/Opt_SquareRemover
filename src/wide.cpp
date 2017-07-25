//
//  wide.cpp
//  SquareRemover
//
//  Created by Anton Logunov on 4/14/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#include "wide.hpp"

namespace square_remover {

    ostream& operator<<(ostream& output, const Wide::Stats& s) {
        return output
        << "el: " << s.eliminated_count
        << " fmd: " << s.four_move_degree
        << " bal: " << s.position_balance;
    }

    vector<int> Wide::playIt(int colors, vector<string> board, int startingSeed) {
        ofstream outstats("stats.txt");
        ofstream output("out.txt");
//            ofstream outans("ans.txt");

        Count same_move_count = 0;
        Count no_move_count = 0;
        vector<int> moves;

        init(colors, board, startingSeed);
        eliminate();
        updateMoves();
        Move m;
        Stats s_pre_f, s_f;
        for (auto i = 0; i < kMoveCount; ++i) {
            if (four_moves_.empty() && pre_four_moves_.empty()) {
                no_move_count++;
                m = bestFarMove();
                last_move = m;
                last_color_p = color_board_(m.pos);
                last_color_t = color_board_(m.another());
                makeMove(m);
                updateMovesAfterMove(m);
            }
            else {
                pair<Move, Stats>
                pre_f = bestPreFourMove(),
                f = bestFourMove();

                s_pre_f = pre_f.second;
                s_f = f.second;

                if (four_moves_.empty()
                    ||
                    (s_pre_f.eliminated_count + s_pre_f.four_move_degree-1 >
                     s_f.eliminated_count + s_f.four_move_degree &&
                     (pre_f.first != last_move ||
                      last_color_p != color_board_(last_move.another()) ||
                      last_color_t != color_board_(last_move.pos)
                      )
                     )
//                    ||
//                    (s_pre_f.eliminated_count + s_pre_f.four_move_degree-1 >
//                     s_f.eliminated_count + s_f.four_move_degree
//                     // bigger better
//                     &&
//                     pos_balance_coeff*s_pre_f.position_balance - board_balance_coeff*s_pre_f.board_balance - balance_coeff >
//                     pos_balance_coeff*s_best.position_balance - board_balance_coeff*s_best.board_balance)

                    ) {
                        m = pre_f.first;
                        last_move = m;
                        last_color_p = color_board_(m.pos);
                        last_color_t = color_board_(m.another());
                        makeMove(m);
                        updateMovesAfterMove(m);
                }
                else {
                    m = f.first;
                    last_move = m;
                    last_color_p = color_board_(m.pos);
                    last_color_t = color_board_(m.another());
                    makeMove(m);
                    updateMovesAfterEliminationMove(m, eliminate(m));

                }
            }
            //assert(m.direction == kRight || m.direction == kDown);

            moves.push_back(m.pos.row);
            moves.push_back(m.pos.col);
            moves.push_back(m.dir);

//            output
//            << "it: " << i << " four: " << four_moves_.size()
//            << " pre: " << pre_four_moves_.size()  << " " << endl;
//            outstats
//            << "it: " << i << " pre: " << s_pre_f
//            << " four: " << s_f << endl;
            outstats
            << i << " " << four_moves_.size()
            << " " << pre_four_moves_.size()
            << " " << positionBalance()
            << " " << boardBalance()
            << " " << removedCount() << endl;

        }
//
//        output << "random: " << no_move_count;
//        output << "same: " << same_move_count;
        return moves;
    }

}