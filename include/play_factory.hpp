#pragma once

#include "play/play_v1.hpp"
#include "digit/board_hashed.hpp"
#include "local_sq_rm_v2.hpp"


template<class TPlay>
struct PlayFactory {

    using Play = TPlay;

    PlayFactory() {}
    PlayFactory(Count min_sq_moves) : min_sq_moves(min_sq_moves) {}

    Play operator()(typename Play::Board &b) {
        Play play(b);
        play.base_play().set_min_sq_moves(min_sq_moves);
        return play;
    }

    Count min_sq_moves;
};

template<>
struct PlayFactory<play::Play_v1<digit::HashedBoard, LocalSqRm_v2>> {

    using Play = play::Play_v1<digit::HashedBoard, LocalSqRm_v2>;

    PlayFactory() {}
    PlayFactory(Count min_sq_moves) : min_sq_moves(min_sq_moves) {}

    Play operator()(typename Play::Board &b) {
        Play play(b);
        play.set_min_sq_moves(min_sq_moves);
        return play;
    }

    Count min_sq_moves;
};