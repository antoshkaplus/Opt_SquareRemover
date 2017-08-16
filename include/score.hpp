#pragma once

class Score_v1 {

    double factor_locs_;
    double factor_triples_;
    double factor_doubles_;

public:
    Score_v1() {}
    Score_v1(double locs, double triples, double doubles)
        : factor_locs_(locs), factor_triples_(triples), factor_doubles_(doubles) {}

    double operator()(int sq_removed, int locs, int triples, int doubles) {
        return sq_removed + locs * factor_locs_ + triples * factor_triples_ + doubles * factor_doubles_;
    }

};