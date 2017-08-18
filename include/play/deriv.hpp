#pragma once

#include "util.hpp"


namespace play {

struct Deriv {
    Move move;

    Deriv() {}
    Deriv(const Move& m)
        : move(m) {}

};

}
