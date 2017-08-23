#pragma once

#include "play/play_v1.hpp"
#include "local_sq_rm_v2.hpp"
#include "digit/board_hashed.hpp"
#include "beam_search_v2.hpp"
#include "play_factory.hpp"


namespace beam_v2 {


constexpr Count kWidth = 30;
constexpr Count kSampleCount = 5;

// returns scores
template<class PlayFactory, class Score, class ScoreFactory>
inline vector<double> ComputeFactorMesh(Index group, PlayFactory play_factory, ScoreFactory score_factory, string factor_title="factor", double min=0, double max=1, double step=0.1) {
    vector<Problem> problems(kSampleCount);
    for_each(problems.begin(), problems.end(), [&](Problem& p) {
        p =  GenerateProblem(group);
    });

    auto func = [&](double factor) {

        auto res = -accumulate(problems.begin(), problems.end(), 0 , [&](Count total, const Problem& pr) {
            BeamSearch_v2<PlayFactory, Score> bm;
            bm.set_score(score_factory(factor));
            bm.set_play_factory(play_factory);

            typename PlayFactory::Play::Board b;
            b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);
            return total + bm.Remove(b, kMoveCount, kWidth).squares_removed();
        });

        return res;
    };

    vector<double> mesh;
    for (auto i = min; i <= max+step/2; i += step) mesh.push_back(i);
    vector<double> res(mesh.size(), 0);
    atomic_uint progress{0};
    tbb::parallel_for(0, (Index)mesh.size(), 1, [&](Index i) {
        res[i] = func(mesh[i]);
        ++progress;
        cout << "gr: " << group << " pr: " << progress << "/" << mesh.size() << endl;
    });
    return res;
}

inline void CheckMinSqMoves() {
    using Play = play::Play_v1<digit::HashedBoard, LocalSqRm_v2>;

    RNG.seed(14);

    vector<vector<Count>> factors(kProblemGroupCount);
    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {

        try {
            Problem pr = GenerateProblem(group);

            for (auto k = 1; k <= 5; ++k) {
                Println(cout, "group: ", group, " it: ", k);

                BeamSearch_v2<PlayFactory<Play>, Score_v1> bm;
                bm.set_score(Score_v1(kLocFactors[group]));
                bm.set_play_factory(PlayFactory<Play>(k));

                typename Play::Board b;
                b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);
                auto removed = bm.Remove(b, kMoveCount, kWidth).squares_removed();
                factors[group].push_back(removed);
            }

        } catch (exception ex) {
            std::cout << "group: " << group << " " << ex.what() << endl;
        }
    });
    for (auto i = 0; i < kProblemGroupCount; ++i) {
        Println(cout, factors[i], "mesh for group " + std::to_string(i));
    }
}

inline void OptimizeLocsFactorMesh() {
    using PlayFactory = PlayFactory<play::Play_v1<digit::HashedBoard, LocalSqRm_v2>>;

    vector<vector<double>> factors(kProblemGroupCount);
    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {

        try {
            auto score_factory = [](double locs_factor) {
                return Score_v1(locs_factor);
            };

            PlayFactory pf;
            pf.min_sq_moves = kMinSqMoves[group];

            factors[group] = ComputeFactorMesh<PlayFactory, Score_v1, decltype(score_factory)>(
            group, pf, score_factory, "locs", kLocFactors[group]-0.05, kLocFactors[group]+0.05, 0.01);
        } catch (exception ex) {
            std::cout << "group: " << group << " " << ex.what() << endl;
        }
    });
    for (auto i = 0; i < kProblemGroupCount; ++i) {
        Println(cout, factors[i], "mesh for group " + std::to_string(i));
    }
}


inline void OptimizeTriplesFactorMesh() {
    using PlayFactory = PlayFactory<play::Play_v2<digit::HashedBoard, LocalSqRm_v2>>;

    vector<vector<double>> factors(kProblemGroupCount);
    tbb::parallel_for(0, kProblemGroupCount, 1, [&](Index group) {

        try {
            auto score_factory = [&](double triples_factor) {
                return Score_v2(kLocFactors[group], triples_factor, 0);
            };

            PlayFactory pf;
            pf.min_sq_moves = 3;

            factors[group] = ComputeFactorMesh<PlayFactory, Score_v2, decltype(score_factory)>(
            group, pf, score_factory, "triples", 0, 100, 10);
        } catch (exception ex) {
            std::cout << "group: " << group << " " << ex.what() << endl;
        }
    });
    for (auto i = 0; i < kProblemGroupCount; ++i) {
        Println(cout, factors[i], "mesh for group " + std::to_string(i));
    }
}

}