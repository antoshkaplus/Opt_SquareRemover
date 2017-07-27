#pragma once

class LocalSqRm_v2 {

    // can't you just eliminate online?? why create this stupid vector??
    // so stupid!!!
    vector<Position> eliminateCandidates(const Region& rect) {
        vector<Position> candidates;
        for (auto r = rect.row_begin(); r < rect.row_end()-1; ++r) {
            for (auto c = rect.col_begin(); c < rect.col_end()-1; ++c) {
                Position p(r, c);
                if (isFourSquare(p)) candidates.push_back(p);
            }
        }
        return candidates;
    }

    // need function that receives just position
    virtual vector<Position> eliminate(Region rect) {
        vector<Position> squares;
        priority_queue<Position, vector<Position>, Position::BottomRightComparator> heap;
        while(true) {
            for (auto& p : eliminateCandidates(rect)) {
                heap.push(p);
            }
            if (heap.empty()) break;
            Position p = heap.top();
            heap.pop();
            // when we come p maybe already not to be a square
            if (isFourSquare(p)) {
                ++eliminated_count_;
                color_seed_ = ReplaceColors(color_board_, color_count_, p, color_seed_);
                squares.push_back(p);
                rect = boardRectangle().intersection(Region(p.row-1, p.col-1, 4, 4));
            }
        }
        return squares;
    }

    // better to throw exception if different from Down, Right
    vector<Position> eliminate(const Move& m) {
        Region rect;
        rect.position = m.pos;
        switch(m.dir) {
            case kDirDown:
                rect.shift(-1, -1);
                rect.size.set(4, 3);
                break;
            case kDirUp:
                rect.shift(-2, -1);
                rect.size.set(4, 3);
                break;
            case kDirRight:
                rect.shift(-1, -1);
                rect.size.set(3, 4);
                break;
            case kDirLeft:
                rect.shift(-1, -2);
                rect.size.set(3, 4);
                break;
            default: assert(false);
        }
        return eliminate(boardRectangle().intersection(rect));
    }

    vector<Position> eliminate() {
        return eliminate(boardRectangle());
    }
};