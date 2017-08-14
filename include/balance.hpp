//
//  balance.h
//  SquareRemover
//
//  Created by Anton Logunov on 4/20/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//
#pragma once

#include <iostream>
#include <fstream>

#include "util.hpp"


// for each position check what's around it.

// low value here is bad
template<class Board>
double positionBalance(const Board& b) {
    double balance = 0;
    double count = 0;
    Color color;
    for (auto r = 0; r < b.size(); ++r) {
        for (auto c = 0; c < b.size(); ++c) {
            count = 0;
            Region rect = b.boardRectangle().intersection(Region(r-1, c-1, 3, 3));
            color = b.color(r,c);
//                    for (auto rr = rect.row_begin(); rr < rect.row_end(); ++rr) {
//                        for (auto cc = rect.col_begin(); cc < rect.col_end(); ++cc) {
//                            if (color_board_(rr,cc) == color) count++;
//                        }
//                    }
            count-=1;

            for (auto p : {Position(r-1, c), Position(r+1, c), Position(r, c+1), Position(r, c-1)}) {
                if (b.boardRectangle().hasInside(p) && b.color(p) == b.color(r, c)) {
                    count += 1;
                }
            }
            balance += count*count;
        }
    }
    return sqrt(balance)/b.size();
}
