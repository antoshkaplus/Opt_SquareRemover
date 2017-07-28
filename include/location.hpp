#pragma once

enum class Combo : uint8_t {
    TopRiTop,
    TopRiRi,
    BotRiRi,
    BotRiBot,
    BotLeBot,
    BotLeLe,
    TopLeLe,
    TopLeTop
};


// could use Position actually, but this takes less memory
// used to perfectly identify 4 move.
struct  Location {
    char row;
    char col;
    Combo combo;
};

constexpr bool operator==(const Location& loc_0, const Location& loc_1) {
    return loc_0.row == loc_1.row && loc_0.col == loc_1.col && loc_0.combo == loc_1.combo;
}


namespace std {

template <>
struct hash<Location> {
    size_t operator()(const square_remover::Location& x) const {
        return (x.row << 16) | (x.col << 8) | x.combo;
    }
};

}



// 0 : top, right, top

//   x
// xo
// xx

// expanded region:
// reg.row_begin-1, reg.row_end
// reg.col_begin-2, reg.col_end



// 1 : top, right, right

// xox
// xx

// expanded region:
// reg.row_begin-1, reg.row_end
// reg.col_begin-2, reg.col_end



// 2 : bottom, right, right

// xx
// xox

// expanded region:
// reg.row_begin-1, reg.row_end
// reg.col_begin-2, reg.col_end



// 3 : bottom, right, bottom

// xx
// xo
//  x

// expanded region:
// reg.row_begin-2, reg.row_end
// reg.col_begin-1, reg.col_end



// 4 : bottom, left, bototm

// xx
// ox
// x

// expanded region:
// reg.row_begin-2, reg.row_end
// reg.col_begin-1, reg.col_end



// 5 : bottom, left, left

//  xx
// xox

// expanded region:
// reg.row_begin-1, reg.row_end
// reg.col_begin-1, reg.col_end-2



// 6 : top, left, left

// xox
//  xx

// expanded region:
// reg.row_begin-1, reg.row_end
// reg.col_begin-1, reg.col_end-2



// 7 : top, left, top

// x
// ox
// xx

// expanded region:
// reg.row_begin-1, reg.row_end
// reg.col_begin-1, reg.col_end-2
