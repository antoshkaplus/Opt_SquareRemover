
#include "benchmark/benchmark_api.h"

#include "beam_search.hpp"
#include "local_sq_rm_v2.hpp"
#include "factors.hpp"
#include "score.hpp"


static void BeamSearchBenchmark(benchmark::State& state) {
    Index group = state.range(0);
    Problem pr = GenerateProblem(group);

    Score_v1 score_v1(kLocFactors[group], kTripleFactors[group]);
    BeamSearch<LocalSqRm_v2, Score_v1> bm;
    bm.set_score(score_v1);

    digit::HashedBoard b;
    b.Init(ToColorGrid(pr.board), pr.color_count, pr.starting_seed);

    while (state.KeepRunning()) {
        bm.Remove(b, kMoveCount, kBeamWidth[group]);
    }
}

BENCHMARK(BeamSearchBenchmark)->DenseRange(0, kProblemGroupCount-1)->Unit(benchmark::kMillisecond)->Iterations(5)->ThreadPerCpu();

