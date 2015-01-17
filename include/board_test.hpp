//
//  board_test.hpp
//  SquareRemover
//
//  Created by Anton Logunov on 1/14/15.
//
//

#ifndef SquareRemover_board_test_hpp
#define SquareRemover_board_test_hpp

#include "board.hpp"


namespace square_remover {
    
struct BoardTest {

    void Test() {
        vector<string> str_board = {
            "010223",
            "002023",
            "114211",
            "102141",
            "012310",
            "010223"
        };
        Board<6> b(str_board, 5, 1626);
        cout << b.OneMoveCount() << endl;
        auto derivs = b.OneMoveDerivatives();
        
        cout << "derivs" << endl;
        for (auto d : derivs) {
            cout << d.OneMoveCount() << endl;
            cout << d << endl;
        }
    }


}; 

}


#endif
