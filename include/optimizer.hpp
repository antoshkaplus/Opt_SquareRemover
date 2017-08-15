#pragma once

#include "tbb/tbb.h"
#include "tbb/parallel_for.h"

#include "ant/optimization/optimization.h"

#include "beam_search.hpp"
#include "local_sq_rm_v2.hpp"
#include "score.hpp"
#include "factors.hpp"


namespace beam {

constexpr Count kSampleCount = 10;

inline double OptimizeLocsFactor(Index group) {
    vector<Problem> problems(kSampleCount);
    for_each(problems.begin(), problems.end(), [&](Problem& p) {
        p =  GenerateProblem(group);
    });

    auto func = [&](double factor_locs) {
        cout << "start: group: " << group << " value: " << factor_locs << endl;

        return -accumulate(problems.begin(), problems.end(), 0 , [&](Count total, const Problem& pr) {
            Score_v1 score_v1(factor_locs, 0.01);
            BeamSearch<LocalSqRm_v2, Score_v1> bm;
            bm.set_score(score_v1);

            digit::HashedBoard b;
            b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);
            return total + bm.Remove(b, kMoveCount, 30).squares_removed();
        });
    };
    return opt::GoldenSectionSearch(0, 1, func, 0.01);
}

inline void OptimizeLocsFactor() {

    vector<double> factors(kProblemGroupCount, 0);
    tbb::parallel_for(0, kProblemGroupCount-1, 1, [&](Index group) { factors[group] = OptimizeLocsFactor(group); });

    Println(cout, factors, string("group locs factors"));

}

inline double OptimizeTriplesFactor(Index group) {

    auto func = [&](double factor_triples) {
        cout << "start: group: " << group << " value: " << factor_triples << endl;
        Score_v1 score_v1(kLocFactors[group], factor_triples);
        BeamSearch<LocalSqRm_v2, Score_v1> bm;
        bm.set_score(score_v1);
        Problem pr = GenerateProblem(group);

        digit::HashedBoard b;
        b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);
        return -bm.Remove(b, kMoveCount, 30).squares_removed();
    };
    return opt::GoldenSectionSearch(0, 1, func, 0.01);
}

inline void OptimizeTriplesFactor() {

    vector<double> factors(kProblemGroupCount, 0);
    tbb::parallel_for(0, kProblemGroupCount-1, 1, [&](Index group) { factors[group] = OptimizeTriplesFactor(group); });

    Println(cout, factors, string("group triples factors"));

}


}