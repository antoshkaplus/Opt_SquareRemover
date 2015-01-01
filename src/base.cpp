//
//  base.cpp
//  SquareRemover
//
//  Created by Anton Logunov on 4/12/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#include "base.h"

namespace square_remover {
    
    array<Position, 4>  squarePositions(const Position& square) {
        Int r = square.row, c = square.col; 
        return {Position(r, c), Position(r, c+1), Position(r+1, c), Position(r+1, c+1)};
    }
    
    Direction oppositeDirection(Direction d) {
        return (d+2)%4;
    }
    
    string directionString(Direction d) {
        switch (d) {
        case kDown: return "DOWN";
        case kUp:   return "UP";
        case kLeft: return "LEFT";
        case kRight:return "RIGHT";
        default: assert(false);
        }
    }
    
    Direction randomDirection() {
        return rand()%kDirectionCount;
    }
    
    Rectangle squareRectangle(const Position& square) {
        return Rectangle(square, Size{2, 2}); 
    }
    
    ostream& operator<<(ostream& output, const Move& m) {
        return output << "move: " << m.position.row << " " << m.position.col << " " << directionString(m.direction) << endl;
    }
    
    
}
