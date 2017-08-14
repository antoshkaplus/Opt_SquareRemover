#pragma once

#include "util.hpp"
#include "location.hpp"
#include "board.hpp"


template<class T> class BeamSearch;

// need to keep track of Locations to easier know if move valid of not.
// otherwise have to check sq in 4 locations

// that's a lot
// have to add move thing for everything
template<class Locator>
class SqState {
public:

    void Init(const Locator& locator) {
        locator_ = &locator;
        // have to reduce region, otherwise will examine out of board cells.
        OnRegionChanged(locator_->board().region().diffed(1, 1, -2, -2));
    }

    void OnRegionChanged(const Region& reg) {
        ValidateSqLocs();
        UpdateSqLocs(reg);
    }

    Count AfterChangeSqLocs(const Region& reg) {
        static vector<std::unordered_map<Location, bool>::iterator> invalidated;
        // don't have to use iterator
        for (auto it = sq_locs_.begin(); it != sq_locs_.end(); ++it) {
            if (!locator_->IsValid(it->first)) {
                it->second = false;
                invalidated.push_back(it);
            }
        }
        Count diff = sq_locs_.size() - invalidated.size();
        locator_->ForEachNewLocation(reg, [&](const Location& loc) {
            auto it = sq_locs_.find(loc);
            if (it == sq_locs_.end() || !it->second) {
                ++diff;
            }
        });
        for_each(invalidated.begin(), invalidated.end(), [](std::unordered_map<Location, bool>::iterator& it) {
            it->second = true;
        });
        invalidated.clear();

        return diff;
    }

    bool sq_locs_empty() const {
        return sq_locs_.empty();
    }

    bool sq_moves_empty() const {
        return sq_moves_.empty();
    }

    template<class Func>
    void ForEachSqMove(Func func) {
        for (auto& m : sq_moves_) {
            func(m.first);
        }
    }

    template<class Func>
    void ForEachSqLoc(Func func) {
        for (auto& loc : sq_locs_) {
            func(loc.first);
        }
    }

    bool IsRemoveMove(const Move& m) {
        return sq_moves_.count(m) > 0;
    }

private:

    void UpdateSqLocs(const Region& reg) {
        locator->ForEachNewLocation(reg, [&](const Location& loc) {
            AddLocation(loc);
        });
    }


    void AddLocation(const Location& loc) {
        sq_locs_.emplace(loc, true);
        sq_moves_.increase(loc.toMove());
    }

    void ValidateSqLocs() {
        for (auto it = sq_locs_.begin(); it != sq_locs_.end();) {
            if (!locator->IsValid(it->first)) {
                it = sq_locs_.erase(it);
                sq_moves_.decrease(it->first.toMove());
            } else ++it;
        }
    }

    CountMap<Move, UnorderedMap> sq_moves_;
    std::unordered_map<Location, bool> sq_locs_;
    const Locator* locator_;

    template<class T> friend class BeamSearch;
};