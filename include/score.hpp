#pragma once

class Score_v1 {

    double factor_locs_;
    double factor_triples_;

public:
    Score_v1() {}
    Score_v1(double locs, double triples)
        : factor_locs_(locs), factor_triples_(triples) {}

    double operator()(int sq_removed, int locs, int triples) {
        return sq_removed + locs * factor_locs_ + triples * factor_triples_;
    }

};