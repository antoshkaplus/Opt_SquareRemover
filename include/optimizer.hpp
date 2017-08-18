#pragma once

#include "tbb/tbb.h"
#include "tbb/parallel_for.h"

#include "ant/optimization/optimization.h"

#include "local_sq_rm_v2.hpp"
#include "score.hpp"
#include "factors.hpp"
#include "beam_search.hpp"
#include "play/play_v1.hpp"
#include "play/play_v2.hpp"


namespace beam {

constexpr Count kSampleCount = 5;
constexpr Count kWidth = 30;

template<class Play, class Score, class ScoreFactory>
inline double OptimizeFactor(Index group, ScoreFactory score_factory, string factor_title="factor", double min=0, double max=1, double eps=0.01) {
    vector<Problem> problems(kSampleCount);
    for_each(problems.begin(), problems.end(), [&](Problem& p) {
        p =  GenerateProblem(group);
    });

    auto func = [&](double factor) {

        auto res = -accumulate(problems.begin(), problems.end(), 0 , [&](Count total, const Problem& pr) {
            BeamSearch<Play, Score> bm;
            bm.set_score(score_factory(factor));

            typename Play::Board b;
            b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);
            return total + bm.Remove(b, kMoveCount, kWidth).squares_removed();
        });

        cout << "start: group: " << group << " value: " << factor << " res: " << res <<  endl;
        return res;
    };
    return opt::GoldenSectionSearch(min, max, func, eps);
}

inline void OptimizeLocsFactor() {


    vector<double> factors(kProblemGroupCount, 0);
    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {

        try {
            auto score_factory = [](double locs_factor) {
                return Score_v1(locs_factor);
            };
            factors[group] = OptimizeFactor<play::Play_v1<digit::HashedBoard, LocalSqRm_v2>, Score_v1, decltype(score_factory)>(group, score_factory);
        } catch (exception ex) {
            std::cout << "group: " << group << " " << ex.what() << endl;
        }
    });
    Println(cout, factors, string("group locs factors"));
}

inline void OptimizeTriplesFactor() {

    vector<double> factors(kProblemGroupCount, 0);
    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {

        try {
            auto score_factory = [&](double triple_factor) {
                return Score_v2(kLocFactors[group], triple_factor, 0);
            };
            factors[group] = OptimizeFactor<play::Play_v2<digit::HashedBoard, LocalSqRm_v2>, Score_v2, decltype(score_factory)>(group, score_factory);
        } catch (exception ex) {
            std::cout << "group: " << group << " " << ex.what() << endl;
        }
    });

    Println(cout, factors, string("group triples factors"));
}

inline void CheckTriplesFactor() {
    using Play = play::Play_v2<digit::HashedBoard, LocalSqRm_v2>;

    for (auto group = 0; group < kProblemGroupCount; ++group) {

        cout << "start group: " << endl;
        for (auto i = 0; i < kSampleCount; ++i) {
            Problem pr = GenerateProblem(group);
            BeamSearch<Play, Score_v2> bm;

            typename Play::Board b;
            b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);

            bm.set_score(Score_v2(kLocFactors[group], kTripleFactors[group], 0));
            cout << "with coeff: " << bm.Remove(b, kMoveCount, kWidth).squares_removed() << endl;
            bm.set_score(Score_v2(kLocFactors[group], 0, 0));
            cout << "without coeff: " << bm.Remove(b, kMoveCount, kWidth).squares_removed() << endl;
        }
    }


}

//
//inline void OptimizeDoublesFactor() {
//
//    vector<double> factors(kProblemGroupCount, 0);
//    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {
//
//        factors[group] = OptimizeFactor(group, [&](double double_factor) {
//            return Score_v1(kLocFactors[group], kTripleFactors[group], double_factor);
//        });
//
//    });
//
//    Println(cout, factors, string("group doubles factors"));
//
//}
//
}