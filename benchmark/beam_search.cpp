
#include "benchmark/benchmark.h"

#include "local_sq_rm_v2.hpp"
#include "factors.hpp"
#include "score.hpp"
#include "play/play_v1.hpp"
#include "beam_search.hpp"


static void BeamSearchBenchmark(benchmark::State& state) {
    Index group = state.range(0);
    Problem pr = GenerateProblem(group);

    digit::HashedBoard b;
    b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);

    Score_v1 score_v1(kLocFactors[group]);
    BeamSearch<play::Play_v1<decltype(b), LocalSqRm_v2>, Score_v1> bm;
    bm.set_score(score_v1);

    while (state.KeepRunning()) {
        bm.Remove(b, kMoveCount, kBeamWidth[group]);
    }
}

BENCHMARK(BeamSearchBenchmark)->DenseRange(0, kProblemGroupCount-1)->Unit(benchmark::kMillisecond)->Iterations(5)->ThreadPerCpu();

