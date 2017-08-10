#pragma once

// don't store open_ variable. calculate everything that is there.
// maybe have open between 3 together guys and two together.

// or at least one guy has to be new. something like this
class ScoreCalculator {
public:

	void Init(const Board& b) {
		board_ = &b;
		open_.resize(b.extended_size(), b.extended_size());
	}

	// more is better
	double Compute() {
		fill(open_.begin(), open_.end(), true);
		Count triples = CountTriples();
		Count doubles = CountDoubles();
		return board_->squares_removed() + 0.0026 * (70.*triples + 17.*doubles);
	}

private:

	Count CountTriples() {
		auto& b = *board_;
		auto& g = b.grid();

		Count triples = 0;
		auto func = [&](const Position& p) {
			if (!open_(p)) return;
			Indent i_1{-1,0};
			Indent i_2{ 0,1};
			for (auto i = 0; i < 4; ++i) {
				// can use current p_2
				Position p_1 = p + i_1;
				Position p_2 = p + i_2;
				if (open_(p_1) && open_(p_2) && (g(p) & g(p_1) & g(p_2))) {

					++triples;
					open_(p) = open_(p_1) = open_(p_2) = false;
				}
                // it seems like it doesn't work as expected.
				tie(i_1, i_2) = make_tuple(i_2, i_1*=-1);
			}
		};
		board_->region().ForEach(func);
		return triples;
	}

	Count CountDoubles() {
		auto& b = *board_;
		auto& g = b.grid();

		Count doubles = 0;
		// try to go down and right
		auto func = [&](const Position& p) {
			if (!open_(p)) return;
			for (auto d : {kDirRight, kDirDown}) {
				auto pp = p.Shifted(d);
				if (open_(pp) && g(p) == g(pp)) {
					++doubles;
				}
			}
		};
		board_->region().ForEach(func);
		return doubles;
	}

	const Board* board_;
	Grid<char> open_;
};