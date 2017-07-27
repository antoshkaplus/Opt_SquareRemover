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

}