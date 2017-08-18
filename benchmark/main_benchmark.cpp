
#include "benchmark/benchmark.h"

#include "util.hpp"

static void BitColors(benchmark::State& state) {
    Count sz = state.range(0);
    Grid<uint8_t> grid(sz, sz);
    Count sq_count = 0;

    while (state.KeepRunning()) {
        for (auto r = 0; r < sz - 1; ++r) {
            for (auto c = 0; c < sz - 1; ++c) {
                if (grid(r, c) & grid(r, c + 1) & grid(r + 1, c) & grid(r + 1, c + 1)) {
                    ++sq_count;
                }
            }
        }
    }
}

static void NumbColors(benchmark::State& state) {
    Count sz = state.range(0);
    Grid<uint8_t> grid(sz, sz);
    Count sq_count = 0;

    while (state.KeepRunning()) {
        for (auto r = 0; r < sz - 1; ++r) {
            for (auto c = 0; c < sz - 1; ++c) {
                if (grid(r, c) == grid(r, c + 1) && grid(r, c) == grid(r + 1, c) &&
                    grid(r + 1, c) == grid(r + 1, c + 1)) {
                    ++sq_count;
                }
            }
        }
    }
}

BENCHMARK(BitColors)->DenseRange(8, 17, 4);
BENCHMARK(NumbColors)->DenseRange(8, 17, 4);

//static void PreCompColors(Count sz, Count color_count) {
//    Grid<uint8_t> grid(sz);
//    Grid<char> grid_row(sz-1);
//    Grid<char> grid_col(sz-1);
//
//    for (auto r = 0; r < sz-1; ++r) {
//        for (auto c = 0; c < sz-1; ++c) {
//            if (grid(r, c) == grid(r, c+1) && grid(r, c) == grid(r+1, c) && grid(r+1, c) == grid(r+1, c+1)) {
//                ++sq_count;
//            }
//        }
//    }
//}




int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}