#pragma once



class Play {
public:

    struct DerivState {
        Count triple_diff;
        Count double_diff;
    };

    Play() {}
    Play(const Board& b) {
        Init(b);
    }
    Play(const Play& p) {
        *this = p;
    }

    const Play& operator=(const Play& play) {
        board_ = play.board_;
        locator_ = play.locator_;
        locator_.ResetBoard(board_);
        sq_state_ = play.sq_state_;
        sq_state_.locator_ = &locator_;
        triple_state_ = play.triple_state_;
        triple_state_.ResetBoard(board_);
        double_state_ = play.double_state_;
        double_state_.ResetBoard(board_);
        return *this;
    }

    void Init(const Board& b) {
        board_ = b;
        locator_.Init(b);
        sq_state_.Init(locator_);
        triple_state_.Init(board_);
        double_state_.Init(board_);
    }

    template<class Func>
    void ForEachDeriv(Board& b, Func func) {
        // play and deriv are very correlated
        // call func with all move + deriv state + score + derivs
        // func is going to be called while board in the end state
    }

    auto& board() {
        return board_;
    }

    auto& board() const {
        return board_;
    }

private:
    digit::Locator locator_;
    Board board_;
    SqState<digit::Locator> sq_state_;
    TripleState triple_state_;
    DoubleState double_state_;
};
