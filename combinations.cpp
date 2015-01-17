// 

struct Move {
    uint8_t row, col, dir;
};

struct DoubleMove {
    Move m_0;
    Move m_1;
};





struct Move {
    uint8_t row;
    uint8_t col;
    // down or right
    uint8_t dir;
    // combo index
    uint8_t combo;
}

// has 2 arrays of moves
// single move
// double move
// first try all single moves
// then all double moves

// row, col, dir // no way with immediate move
// if single is double and vise versa
// hash single and double moves so i didn't repeat myself

// how to evaluate board after double move ??? 
// double moves go immediatly

// maybe third coeff not needed
// (v_single * n_single + v_double * n_double + v_destr * n_destroyed)/n_moves

// board hashing 

// could do much more actually

for (int r = 0; r < N-1; ++r) {
    for (int c = 0; c < N-1; ++c) {
        
        // combo
        
        // oox
        // xoo
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c+1)&cs(r+1,c+2) != 0) {
        
        }
        // ox
        // oo
        // xo
        if (cs(r,c)&cs(r+1,c)&cs(r+1,c+1)&cs(r+2,c+1) != 0) {
        
        }
        // xo
        // oo
        // ox
        if (cs(r,c+1)&cs(r+1,c+1)&cs(r+1,c)&cs(r+2,c) != 0) {
        
        }
        // xoo
        // oox
        if (cs(r,c+1)&cs(r,c+2)&cs(r+1,c)&cs(r+1,c+1) != 0) {
        
        }
        
        // combo
        
        // oox
        // oxo
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+1,c+2) != 0) {
        
        }
        // oo
        // ox
        // xo 
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+2,c+1) != 0) {
            
        }
        
        // xoo
        // oxo
        if (cs(r,c+1)&cs(r,c+2)&cs(r+1,c+2)&cs(r+1,c) != 0) {
        
        }
        //  oo
        //  xo
        //  ox
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c+1)&cs(r+2,c) != 0) {
            
        }
        // ox
        // xo
        // oo
        if (cs(r,c)&cs(r+1,c+1)&cs(r+2,c)&cs(r+2,c+1) != 0) {
        
        }
        // oxo
        // xoo
        if (cs(r,c)&cs(r,c+2)&cs(r+1,c+1)&cs(r+1,c+2) != 0) {
        
        }
        // oxo
        // oox
        if (cs(r,c)&cs(r+1,c)&cs(r+1,c+1)&cs(r,c+2) != 0) {
            
        }
        // xo
        // ox
        // oo
        if (cs(r,c+1)&cs(r+1,c)&cs(r+2,c)&cs(r+2,c+1) != 0) {
        
        }
        
        // combo
        
        // oxxo
        // xoox
        if (cs(r,c)&cs(r+1,c+1)&cs(r+1,c+2)&cs(r,c+3) != 0) {
        
        }
        // xoox
        // oxxo
        if (cs(r+1,c)&cs(r,c+1)&cs(r,c+2)&cs(r+1,c+3) != 0) {
        
        }
        // xo
        // ox
        // ox
        // xo
        if (cs(r,c+1)&cs(r+1,c)&cs(r+2,c)&cs(r+3,c+1) != 0) {
        
        }
        // ox
        // xo
        // xo
        // ox
        if (cs(r,c)&cs(r+1,c+1)&cs(r+2,c+1)&cs(r+3,c) != 0) {
        
        }
        
        // combo
        
        // ooo
        // xxo
        if (cs(r,c)&cs(r,c+1)&cs(r,c+2)&cs(r+1,c+2) != 0) {
        
        }
        // xxo
        // ooo
        if (cs(r,c+2)&cs(r+1,c)&cs(r+1,c+1)&cs(r+1,c+2) != 0) {
        
        }
        // ooo
        // oxx
        if (cs(r,c)&cs(r,c+1)&cs(r,c+2)&cs(r+1,c) != 0) {
        
        }
        // oxx
        // ooo
        if (cs(r,c)&cs(r+1,c)&cs(r+1,c+1)&cs(r+1,c+2) != 0) {
        
        }
        // xo
        // xo
        // oo
        if (cs(r,c+1)&cs(r+1,c+1)&cs(r+2,c)&cs(r+2,c+1) != 0) {
        
        }
        // ox
        // ox
        // oo
        if (cs(r,c)&cs(r+1,c)&cs(r+2,c)&cs(r+2,c+1) != 0) {
        
        }
        // oo
        // xo
        // xo
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c+1)&cs(r+2,c+1) != 0) {
        
        }
        // oo
        // ox
        // ox
        if (cs(r,c)&cs(r,c+1)&cs(r+1,c)&cs(r+2,c) != 0) {
            
        }
    }
}