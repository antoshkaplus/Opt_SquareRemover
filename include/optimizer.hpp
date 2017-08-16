#pragma once

#include "tbb/tbb.h"
#include "tbb/parallel_for.h"

#include "ant/optimization/optimization.h"

#include "beam_search.hpp"
#include "local_sq_rm_v2.hpp"
#include "score.hpp"
#include "factors.hpp"


namespace beam {

constexpr Count kSampleCount = 2;
constexpr Count kWidth = 30;

template<class ScoreFactory>
inline double OptimizeFactor(Index group, ScoreFactory score_factory, string factor_title="factor", double min=0, double max=1, double eps=0.01) {
    vector<Problem> problems(kSampleCount);
    for_each(problems.begin(), problems.end(), [&](Problem& p) {
        p =  GenerateProblem(group);
    });

    auto func = [&](double factor) {
        cout << "start: group: " << group << " value: " << factor << endl;

        return -accumulate(problems.begin(), problems.end(), 0 , [&](Count total, const Problem& pr) {
            BeamSearch<LocalSqRm_v2, Score_v1> bm;
            bm.set_score(score_factory(factor));

            digit::HashedBoard b;
            b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);
            return total + bm.Remove(b, kMoveCount, kWidth).squares_removed();
        });
    };
    return opt::GoldenSectionSearch(min, max, func, eps);
}

inline void OptimizeLocsFactor() {


    vector<double> factors(kProblemGroupCount, 0);
    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {

        factors[group] = OptimizeFactor(group, [](double locs_factor) {
            return Score_v1(locs_factor, 0.01, 0);
        });

    });
    Println(cout, factors, string("group locs factors"));
}

inline void OptimizeTriplesFactor() {

    vector<double> factors(kProblemGroupCount, 0);
    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {

        factors[group] = OptimizeFactor(group, [&](double triple_factor) {
            return Score_v1(kLocFactors[group], triple_factor, 0);
        });

    });

    Println(cout, factors, string("group triples factors"));

}

inline void OptimizeDoublesFactor() {

    vector<double> factors(kProblemGroupCount, 0);
    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {

        factors[group] = OptimizeFactor(group, [&](double double_factor) {
            return Score_v1(kLocFactors[group], kTripleFactors[group], double_factor);
        });

    });

    Println(cout, factors, string("group doubles factors"));

}

}