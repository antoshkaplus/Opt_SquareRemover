#pragma once


class Score_v1 {

    double factor_locs_;

public:
    Score_v1() {}
    Score_v1(double locs)
        : factor_locs_(locs) {}

    template<class Deriv>
    double operator()(const Deriv& d) {
        return d.sq_removed + d.sq_move_count * factor_locs_ ;
    }

};

class Score_v2 {

    double factor_locs_;
    double factor_triples_;
    double factor_doubles_;

public:
    Score_v2() {}
    Score_v2(double locs, double triples, double doubles)
        : factor_locs_(locs), factor_triples_(triples), factor_doubles_(doubles) {}

    template<class Deriv>
    double operator()(const Deriv& d) {
        // as more sq_move_count reduce influence of secondary factor
        return d.sq_removed + d.sq_move_count * factor_locs_ +  0.001 * d.triples * factor_triples_ / (d.sq_move_count +1) + d.doubles * factor_doubles_;
    }

};

class Score_v3 {

    double factor_locs_;
    double factor_adj_;

public:
    Score_v3() {}
    Score_v3(double locs, double adj)
        : factor_locs_(locs), factor_adj_(adj) {}

    template<class Deriv>
    double operator()(const Deriv& d) {
        return d.sq_removed + d.sq_move_count * factor_locs_ + factor_adj_ * 0.01 * sqrt(1.L * d.adj_amount);
    }
};