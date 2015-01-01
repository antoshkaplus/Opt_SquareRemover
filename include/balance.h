//
//  balance.h
//  SquareRemover
//
//  Created by Anton Logunov on 4/20/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __SquareRemover__balance__
#define __SquareRemover__balance__

#include <iostream>
#include <fstream>

#include "base.h"

namespace square_remover {
    
    struct Balance : virtual Base {
        
        // low value here is bad
        double positionBalance() {
            double balance = 0;
            double count = 0;
            Color color;
            for (auto r = 0; r < board_size_; ++r) {
                for (auto c = 0; c < board_size_; ++c) {
                    count = 0;
                    Rectangle rect = boardRectangle().intersection(Rectangle(r-1, c-1, 3, 3));
                    color = color_board_(r,c);
//                    for (auto rr = rect.row_begin(); rr < rect.row_end(); ++rr) {
//                        for (auto cc = rect.col_begin(); cc < rect.col_end(); ++cc) {
//                            if (color_board_(rr,cc) == color) count++;
//                        }
//                    }  
                    count-=1;
                    
                    for (auto p : {Position(r-1, c), Position(r+1, c), Position(r, c+1), Position(r, c-1)}) {
                        if (boardRectangle().hasInside(p) && color_board_(p) == color_board_(r, c)) {
                            count += 1;
                        }
                    }
                    balance += count*count;
                }
            }
            return sqrt(balance)/board_size_;
        }
        
        // low value here is good
        double boardBalance() {
            
            vector<vector<char>> hor(board_size_);
            for (auto r = 0; r < board_size_; ++r) {
                Count count = 1;
                for (auto c = 1; c < board_size_; ++c) {
                    if (color_board_(r, c-1) == color_board_(r, c)) {
                        ++count;
                    }
                    else {
                        hor[r].push_back(count);
                        count = 1;
                    }
                }
                hor[r].push_back(count); // last one
            }
            
            vector<vector<char>> ver(board_size_);
            for (auto c = 0; c < board_size_; ++c) {
                Count count = 1;
                for (auto r = 1; r < board_size_; ++r) {
                    if (color_board_(r-1, c) == color_board_(r, c)) {
                        ++count;
                    }
                    else {
                        ver[c].push_back(count);
                        count = 1;
                    }
                }
                ver[c].push_back(count);
            }
            
            int res = 0;
            for (auto i = 0; i < board_size_; ++i) {
                for (auto h : hor[i]) res += (h-2)*(h-2);
                for (auto v : ver[i]) res += (v-2)*(v-2);
            }
            return sqrt(res)/board_size_;
        }
    };
    
}



#endif /* defined(__SquareRemover__balance__) */
