#include <algorithm>
#include <vector>
#include <numeric>
#include <iterator>
#include <map>
#include <cstdlib>
#include <memory>
#include <unordered_set>
#include <cfloat>
#include <limits>

#define MAXFLOAT 9999999999

namespace ant {

typedef size_t Count;
typedef size_t Index;
typedef int    Int;


namespace d2 {

// make it possible to substruct
struct Indent {
    Indent() : Indent(0, 0) {}
    Indent(Int row, Int col) : row(row), col(col) {}

    void set(Int row, Int col) {
        this->row = row;
        this->col = col;
    }

    Int row, col;
};

struct Position {
    // operators see below
    Position() : Position(0, 0) {}
    Position(Int row, Int col) : row(row), col(col) {}

    void set(Int row, Int col) {
        this->row = row;
        this->col = col;
    }

    Int row, col;
    struct TopLeftComparator {
        bool operator()(const Position& p_0, const Position& p_1) {
            return p_0.row < p_1.row || (p_0.row == p_1.row && p_0.col < p_1.col);
        }
    };
    struct BottomRightComparator {
        bool operator()(const Position& p_0, const Position& p_1) {
            return p_0.row > p_1.row || (p_0.row == p_1.row && p_0.col > p_1.col);
        }
    };
};

Position operator-(const Position& p, const Indent& n);
Position operator+(const Position& p, const Indent& n);
bool operator==(const Position& p_0, const Position& p_1);
bool operator!=(const Position& p_0, const Position& p_1);


struct Rectangle {
    Rectangle() {}
    Rectangle(Int row, Int col, Int row_indent, Int col_indent)
    : position(row, col), indent(row_indent, col_indent) {}
    Rectangle(const Position& p, const Indent& d)
    : position(p), indent(d) {}

    void set(Int row, Int col, Int row_indent, Int col_indent) {
        position.set(row, col);
        indent.set(row_indent, col_indent);
    }
    void set(const Position& p, const Indent& d) {
        position = p;
        indent = d;
    }


    // maybe someone will find those getters useful
    Int row_begin() const { return position.row; }
    Int row_end()   const { return position.row + indent.row; }
    Int col_begin() const { return position.col; }
    Int col_end()   const { return position.col + indent.col; }

    void set_row_begin(Int r) {
        indent.row += position.row - r;
        position.row = r;
    }
    void set_row_end(Int r) {
        indent.row = r - position.row;
    }
    void set_col_begin(Int c) {
        indent.col += position.col - c;
        position.col = c;
    }
    void set_col_end(Int c) {
        indent.col = c - position.col;
    }

    bool isInside(const Position& p) {
        return p.row >= position.row &&
               p.row  < position.row+indent.row &&
               p.col >= position.col &&
               p.col  < position.col+indent.col;
    }

    Rectangle intersection(const Rectangle& r) {
        Rectangle n;
        n.position.row = std::max(position.row, r.position.row);
        n.position.col = std::max(position.col, r.position.col);
        n.indent.row = std::min(row_end(), r.row_end()) - n.position.row;
        n.indent.col = std::min(col_end(), r.col_end()) - n.position.col;

        if (n.indent.row < 0 || n.indent.col < 0) {
            n.indent.row = 0;
            n.indent.col = 0;
        }
        return n;
    }

    bool isEmpty() { return indent.row == 0 && indent.col == 0; }

    template<class ForwardIterator>
    static Rectangle unite(ForwardIterator first, ForwardIterator last) {
        Rectangle rect = *first;
        while (++first != last) {
            // should update "end" first because it dependent on "begin"
            rect.set_row_end(std::max(first->row_end(), rect.row_end()));
            rect.set_col_end(std::max(first->col_end(), rect.col_end()));
            rect.set_row_begin(std::min(first->row_begin(), rect.row_begin()));
            rect.set_col_begin(std::min(first->col_begin(), rect.col_begin()));

        }
        return rect;
    }

    Position position;
    Indent indent;
};

template<class T>
struct Grid {
    Grid() : Grid(0, 0) {}
    Grid(Count row_count, Count col_count)
    : row_count_(row_count),
      col_count_(col_count),
      grid_(row_count*col_count) {
    }

    bool isInside(const Position& p) {
        return isInside(p.row, p.col);
    }
    bool isInside(Int row, Int col) {
        return row >= 0 && row < row_count_ &&
               col >= 0 && col < col_count_;
    }

    void resize(Count row_count, Count col_count) {
        row_count_ = row_count;
        col_count_ = col_count;
        grid_.resize(row_count*col_count);
    }

    void fill(const T& t) {
        std::fill(grid_.begin(), grid_.end(), t);
    }

    Count row_count() const { return row_count_; }
    Count col_count() const { return col_count_; }

    T& operator()(const Position& p) {
        return grid_[p.row*col_count_ + p.col];
    }
    const T& operator()(const Position& p) const {
        return grid_[p.row*col_count_ + p.col];
    }

    T& operator()(Int row, Int col) {
        return grid_[row*col_count_ + col];
    }
    const T& operator()(Int row, Int col) const {
        return grid_[row*col_count_ + col];
    }
private:
    Count row_count_, col_count_;
    std::vector<T> grid_;
};
}


namespace d3 {

template<class T>
struct Cube {
    Cube() : Cube(0, 0, 0)  {}
    Cube(Count row_count, Count col_count, Count page_count)
    : row_count_(row_count), col_count_(col_count), page_count_(page_count),
      cube_(row_count*col_count*page_count) {}

    void resize(Count row_count, Count col_count, Count page_count) {
        row_count_ = row_count;
        col_count_ = col_count;
        page_count_ = page_count;
        cube_.resize(row_count*col_count*page_count);
    }

    void fill(const T& t) {
        std::fill(cube_.begin(), cube_.end(), t);
    }

    Count row_count() { return row_count_; }
    Count col_count() { return col_count_; }
    Count page_count() { return page_count_; }

    T& operator()(Int row, Int col, Int page) {
        return cube_[(row*col_count_ + col)*page_count_ + page];
    }
    const T& operator()(Int row, Int col, Int page) const {
        return cube_[(row*col_count_ + col)*page_count_ + page];
    }

private:
    Count row_count_, col_count_, page_count_;
    std::vector<T> cube_;
};
}



template<class T>
struct range {

    struct range_iterator : std::iterator<std::input_iterator_tag, T> {
        const range& _range;
        T _current;

        range_iterator(const range& range, T current)
        : _range(range), _current(current) {}
        const T operator*() const { return _current; }
        bool operator==(const range_iterator& it) const {
            return _current == *it;
        }
        bool operator!=(const range_iterator& it) const {
            return _current != *it;
        }
        range_iterator& operator++() {
            _current += _range._step;
            if (_range._step*(_current-_range._last) > 0) _current = _range._last;
            return *this;
        }
        range_iterator operator++(int) {
            range_iterator it(*this);
            operator++();
            return it;
        }


    };
    friend struct range_iterator;
    typedef range_iterator iterator;

private:
    T _first, _last, _step;

public:
    range(T last) : _first(0), _last(last), _step(1) {}
    range(T first, T last, T step = 1)
    : _first(first), _last(last), _step(step) {}

    iterator begin() const { return iterator(*this, _first); }
    iterator end()   const { return iterator(*this, _last); }
};

typedef range<Int> irange;
typedef range<double> frange;

}


namespace ant {
namespace d2 {
Position operator-(const Position& p, const Indent& n) {
    return Position(p.row-n.row, p.col-n.col);
}

Position operator+(const Position& p, const Indent& n) {
    return Position(p.row+n.row, p.col+n.col);
}

bool operator==(const Position& p_0, const Position& p_1) {
    return p_0.row == p_1.row && p_0.col == p_1.col;
}

bool operator!=(const Position& p_0, const Position& p_1) {
    return p_0.row != p_1.row || p_0.col != p_1.col;
}
}
}

#include <vector>
#include <cassert>
#include <random>
#include <array>
#include <queue>


namespace square_remover {
/* because FourSameColorSquareMoves or ThreeSameColorSquares
 * are long names and considering context of the problem we
 * will use names FourSquareMoves or ThreeSquares (SameColor
 * is clear from the context) */

using namespace std;
using namespace ant;
using namespace d2; // ant namespace

const int kUp     = 0;
const int kRight  = 1;
const int kDown   = 2;
const int kLeft   = 3;
const int kDirectionCount = 4;
const array<const int, kDirectionCount> kDirections = {{kUp, kRight, kDown, kLeft}};

typedef int Direction;
typedef u_char Degree;
typedef u_int64_t Seed;
typedef char Color;

array<Position, 4> squarePositions(const Position& square);
Direction oppositeDirection(Direction d);
string directionString(Direction d);
Direction randomDirection();
Rectangle squareRectangle(const Position& square);

/* if you want more "directed" move make subclass and rewrite index */
struct Move {
    Position position;
    Direction direction;
    Move() {}
    Move(int row, int col, Direction direction)
    : Move(Position(row, col), direction) {}
    Move(const Position& position, Direction direction)
    : position(position), direction(direction) {}
    bool operator==(const Move& m) const {
        return direction == m.direction && position == m.position;
    }
    Index index() {
        Index i = 0;
        Move m = direction == kDown || direction == kRight ? *this : opposite();
        i = m.position.row << 7 |
            m.position.col << 3 |
            m.direction    << 1;
        return i;
    }

    Move opposite() {
        return Move(target(), oppositeDirection(direction));
    }
    Position target() const {
        int r = 0, c = 0;
        switch (direction) {
            case kDown: r = 1; break;
            case kUp: r = -1; break;
            case kRight: c = 1; break;
            case kLeft: c = -1; break;
            default: assert(false);
        }
        return Position(position.row+r, position.col+c);
    }
    virtual vector<Position> positionsInvolved() {
        return {position};
    }
};

struct DoubleMove : Move {
    DoubleMove() {}
    DoubleMove(const Move& m, const Move& next)
    : Move(m), next(next) {}
    Index index() {
        Index i = 0;
        i = next.index() << 11 | Move::index();
        return i;
    }
    Move next;
};

ostream& operator<<(ostream& output, const Move& m);

// use this class to make solvers that doesn't depand on Base
struct Solver {
    virtual vector<int> playIt(int colors, vector<string> board, int startingSeed) = 0;
    static const Count kMoveCount = 1e+4;
};

/* uses rand() function, so be careful and probably init srand() */
struct Base : Solver {
    vector<int> playIt(int colors, vector<string> board, int startingSeed) override {
        vector<int> moves;
        init(colors, board, startingSeed);
        eliminate();
        for (auto i = 0; i < kMoveCount; ++i) {
            Move m = randomMove();
            makeMove(m);
            moves.push_back(m.position.row);
            moves.push_back(m.position.col);
            moves.push_back(m.direction);
            eliminate();
        }
        return moves;
    }



protected:

    Position randomPosition() {
        return Position(rand()%board_size_, rand()%board_size_);
    }

    Move randomMove() {
        Move m;
        Position p;
        do {
            m.position = randomPosition();
            m.direction = randomDirection();
            p = m.target();
        } while(!color_board_.isInside(p) ||
                color_board_(p) == color_board_(m.position));
        return m;
    }

    virtual void init(int color_count, const vector<string>& board, Seed starting_seed) {
        srand((uint)starting_seed);
        color_produced_ = false;
        color_seed_ = starting_seed;
        board_size_ = board.size();
        color_count_ = color_count;
        color_board_.resize(board_size_, board_size_);
        for (auto row = 0; row < board_size_; ++row) {
            for (auto col = 0; col < board_size_; ++col) {
                color_board_(row, col) = board[row][col] - '0';
            }
        }
    }

    bool isSameColor(const Position& p_0,
                     const Position& p_1,
                     const Position& p_2,
                     const Position& p_3) {
        Color c = color_board_(p_0);
        return c == color_board_(p_1) &&
               c == color_board_(p_2) &&
               c == color_board_(p_3);
    }

    bool isSameColor(const Position& p_0,
                     const Position& p_1,
                     const Position& p_2) {
        return color_board_(p_0) == color_board_(p_1) &&
               color_board_(p_1) == color_board_(p_2);
    }

    bool isSameColor(const Position& p_0,
                     const Position& p_1) {
        return color_board_(p_0) == color_board_(p_1);
    }

    Count countColorInSquare(const Position& p, Color c) {
        return (color_board_(p) == c) +
               (color_board_(p.row  , p.col+1) == c) +
               (color_board_(p.row+1, p.col  ) == c) +
               (color_board_(p.row+1, p.col+1) == c);
    }

    bool isFourSquare(const Position& p) {
        return isSameColor(p,
                           p+Indent{0, 1},
                           p+Indent{1, 0},
                           p+Indent{1, 1});
    }

    static bool isInsideSquare(const Position& square, const Position& pos) {
        return pos.row >= square.row   &&
               pos.row <  square.row+2 &&
               pos.col >= square.col   &&
               pos.col <  square.col+2;
    }

    void replaceColors(const Position& p) {
        color_board_(p) = nextColor();
        color_board_(p+Indent{0,1}) = nextColor();
        color_board_(p+Indent{1,0}) = nextColor();
        color_board_(p+Indent{1,1}) = nextColor();
    }

private:
    vector<Position> eliminateCandidates(const Rectangle& rect) {
        vector<Position> candidates;
        for (auto r = rect.row_begin(); r < rect.row_end()-1; ++r) {
            for (auto c = rect.col_begin(); c < rect.col_end()-1; ++c) {
                Position p(r, c);
                if (isFourSquare(p)) candidates.push_back(p);
            }
        }
        return candidates;
    }

protected:
    vector<Position> eliminate(Rectangle rect) {
        vector<Position> squares;
        priority_queue<Position, vector<Position>, Position::BottomRightComparator> heap;
        while(true) {
            for (auto& p : eliminateCandidates(rect)) {
                heap.push(p);
            }
            if (heap.empty()) break;
            Position p = heap.top();
            heap.pop();
            if (isFourSquare(p)) {
                replaceColors(p);
                squares.push_back(p);
                rect = boardRectangle().intersection(Rectangle(p.row-1, p.col-1, 4, 4));
            }
        }
        return squares;
    }

    Degree eliminationDegree(const Move& m) {
        Position p = m.position;
        bool b_0, b_1;
        makeMove(m);
        switch(m.direction) {
            case kUp:
                p = m.target();
            case kDown:
                b_0 =
                (p.row > 0 &&
                 ((p.col > 0               && isFourSquare(p + Indent{-1, -1})) ||
                  (p.col < board_size_-1   && isFourSquare(p + Indent{-1,  0}))));
                b_1 =
                (p.row < board_size_-2 &&
                 ((p.col > 0               && isFourSquare(p + Indent{ 1, -1})) ||
                  (p.col < board_size_-1   && isFourSquare(p + Indent{ 1,  0}))));
                break;
            case kLeft:
                p = m.target();
            case kRight:
                b_0 =
                (p.col > 0 &&
                 ((p.row > 0               && isFourSquare(p + Indent{-1, -1})) ||
                  (p.row < board_size_-1   && isFourSquare(p + Indent{ 0, -1}))));
                b_1 =
                (p.col < board_size_-2 &&
                 ((p.row > 0               && isFourSquare(p + Indent{-1,  1})) ||
                  (p.row < board_size_-1   && isFourSquare(p + Indent{ 0,  1}))));
                break;
            default: assert(false);
        }
        makeMove(m);
        return b_0 + b_1;
    }

    bool canEliminate(const Move& m) {
        return eliminationDegree(m) > 0;
    }

    vector<Position> eliminate(const Move& m) {
        Rectangle rect;
        switch(m.direction) {
            case kDown:
                rect.position = m.position + Indent{-1, -1};
                rect.indent = Indent{4, 3};
                break;
            case kUp:
                rect.position = m.position + Indent{-2, -1};
                rect.indent = Indent{4, 3};
                break;
            case kRight:
                rect.position = m.position + Indent{-1, -1};
                rect.indent = Indent{3, 4};
                break;
            case kLeft:
                rect.position = m.position + Indent{-1, -2};
                rect.indent = Indent{3, 4};
                break;
            default: assert(false);
        }
        return eliminate(boardRectangle().intersection(rect));
    }

    vector<Position> eliminate() {
        return eliminate(boardRectangle());
    }

    /* probably need something like factory
       to move from one color to another
      */
    Color nextColor() {
        if (color_produced_) {
            color_seed_ = (color_seed_ * 48271) % 2147483647;
        } else color_produced_ = true;
        return Color(color_seed_ % color_count_);
    }

    void makeMove(const Move& m) {
        swap(color_board_(m.position), color_board_(m.target()));
    }

    Rectangle boardRectangle() {
        return Rectangle(0, 0, (int)board_size_, (int)board_size_);
    }

    void outputColorBoard(ostream& output) {
        for (auto r : irange((int)board_size_)) {
            for (auto c : irange((int)board_size_)) {
                output << char(color_board_(r, c) + '0');
            }
            output << endl;
        }
    }

    /* you can use fourMoves methods where you want
       probably they belong to base class */
    vector<Move> fourMoves() {
        return fourMoves(Rectangle(0, 0, (int)board_size_, (int)board_size_));
    }

    vector<Move> fourMoves(const Rectangle& rect) {
        vector<Move> sqs;
        unordered_set<Index> sqs_inds;
        auto
        r_begin = rect.row_begin(),
        r_end   = rect.row_end(),
        c_begin = rect.col_begin(),
        c_end   = rect.col_end();
        Position p;
        for (auto r = r_begin; r < r_end-1; ++r) {
            for (auto c = c_begin; c < c_end-1; ++c) {
                p.set(r, c);
                searchFourMoves(p, rect, sqs, sqs_inds);
            }
        }
        return sqs;
    }


    // all four moves of special color
    vector<Move> fourMoves(const Rectangle& rect, Color color) {
        vector<Move> sqs;
        unordered_set<Index> sqs_inds;
        auto
        r_begin = rect.row_begin(),
        r_end   = rect.row_end(),
        c_begin = rect.col_begin(),
        c_end   = rect.col_end();
        Position p;
        for (auto r = r_begin; r < r_end-1; ++r) {
            for (auto c = c_begin; c < c_end-1; ++c) {
                p.set(r, c);
                if (countColorInSquare(p, color) == 3) {
                    searchFourMoves(p, rect, sqs, sqs_inds);
                }
            }
        }
        return sqs;
    }

    void searchFourMoves(const Position& position, const Rectangle& rect,
                         vector<Move>& sqs, unordered_set<Index>& sqs_inds) {
        Int r = position.row,
        c = position.col;
        Position
        p_m,
        p_0 = {r, c},
        p_1 = {r, c+1},
        p_2 = {r+1, c},
        p_3 = {r+1, c+1};
        pair<Position, Move> sqs_pair;
        sqs_pair.first = p_0;
        bool
        b_0_1 = isSameColor(p_0, p_1),
        b_1_2 = isSameColor(p_1, p_2),
        b_2_3 = isSameColor(p_2, p_3),
        b_0_3 = isSameColor(p_0, p_3);

        Count count = 0;
        if (b_0_1 && b_1_2) {
            p_m = p_3+Indent{1, 0};
            if (p_3.row < rect.row_end()-1 && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kUp));
            }
            p_m = p_3+Indent{0, 1};
            if (p_3.col < rect.col_end()-1 && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kLeft));
            }
        }
        if (b_0_1 && b_0_3) {
            p_m = p_2+Indent{1, 0};
            if (p_2.row < rect.row_end()-1 && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kUp));
            }
            p_m = p_2+Indent{0,-1};
            if (p_2.col > rect.col_begin() && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kRight));
            }
        }
        if (b_2_3 && b_0_3) {
            p_m = p_1+Indent{-1,0};
            if (p_1.row > rect.row_begin() && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kDown));
            }
            p_m = p_1+Indent{0, 1};
            if (p_1.col < rect.col_end()-1 && isSameColor(p_0, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kLeft));
            }
        }
        if (b_1_2 && b_2_3) {
            p_m = p_0+Indent{-1,0};
            if (p_0.row > rect.row_begin() && isSameColor(p_1, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kDown));
            }
            p_m = p_0+Indent{0,-1};
            if (p_0.col > rect.col_begin() && isSameColor(p_1, p_m)) {
                ++count;
                sqs.push_back(Move(p_m, kRight));
            }
        }

        // remove repeating moves
        Move m;
        Index ind;
        for (Index i = sqs.size()-count; i < sqs.size();) {
            m = sqs[i];
            ind = m.index();
            if (sqs_inds.count(ind)) {
                swap(sqs[i], sqs.back());
                sqs.pop_back();
            }
            else {
                sqs_inds.insert(ind);
                ++i;
            }
        }
    }

    double balance() {

        vector<vector<char>> hor(board_size_);
        for (auto r = 0; r < board_size_; ++r) {
            Count count = 1;
            for (auto c = 1; c < board_size_; ++c) {
                if (color_board_(r, c-1) == color_board_(r, c)) {
                    ++count;
                }
                else {
                    hor[r].push_back(count);
                    count = 1;
                }
            }
            hor[r].push_back(count); // last one
        }

        vector<vector<char>> ver(board_size_);
        for (auto c = 0; c < board_size_; ++c) {
            Count count = 1;
            for (auto r = 1; r < board_size_; ++r) {
                if (color_board_(r-1, c) == color_board_(r, c)) {
                    ++count;
                }
                else {
                    ver[c].push_back(count);
                    count = 1;
                }
            }
            ver[c].push_back(count);
        }

        double res = 0;
        for (auto i = 0; i < board_size_; ++i) {
            double r = 0;
            for (auto h : hor[i]) r += (h-2)*(h-2);
            r /= hor.size();
            res += r;
            r = 0;
            for (auto v : ver[i]) r += (v-2)*(v-2);
            r /= ver.size();
            res += r;
        }
        return res;
    }



protected:
    bool color_produced_;
    Seed color_seed_;
    Grid<Color> color_board_;
    Count board_size_;
    Count color_count_;

    //friend struct Stingy;
    //friend struct Last;
    friend struct Wide;
};
}




namespace square_remover {

array<Position, 4>  squarePositions(const Position& square) {
    Int r = square.row, c = square.col;
    return {{Position(r, c), Position(r, c+1), Position(r+1, c), Position(r+1, c+1)}};
}

Direction oppositeDirection(Direction d) {
    return (d+2)%4;
}

string directionString(Direction d) {
    switch (d) {
        case kDown: return "DOWN";
        case kUp:   return "UP";
        case kLeft: return "LEFT";
        case kRight:return "RIGHT";
        default: assert(false);
    }
}

Direction randomDirection() {
    return rand()%kDirectionCount;
}

Rectangle squareRectangle(const Position& square) {
    return Rectangle(square, Indent{2, 2});
}

ostream& operator<<(ostream& output, const Move& m) {
    return output << "move: " << m.position.row << " " << m.position.col << " " << directionString(m.direction) << endl;
}



}


namespace square_remover {

struct FourMoveSolver : Base {

    typedef u_char Degree;

    struct FourMove : Move {
        FourMove() {}
        FourMove(const Move& m, Degree d)
        : Move(m), degree(d) {}
        Degree degree;
        Position square[2];

        bool update(FourMoveSolver& solver) {
            Count size = solver.board_size_;
            Position p = position;
            bool c_0_0 = false, c_0_1 = false, c_1_0 = false, c_1_1 = false;
            solver.makeMove(*this);
            degree = 0;
            switch(direction) {
                case kUp:
                    p = target();
                case kDown:
                    (p.row > 0 &&
                     ((c_0_0 = (p.col > 0        && solver.isFourSquare(p + Indent{-1, -1}))) ||
                      (c_0_1 = (p.col < size-1   && solver.isFourSquare(p + Indent{-1,  0})))));
                    (p.row < size-2 &&
                     ((c_1_0 = (p.col > 0        && solver.isFourSquare(p + Indent{ 1, -1}))) ||
                      (c_1_1 = (p.col < size-1   && solver.isFourSquare(p + Indent{ 1,  0})))));

                    if (c_0_0) square[degree++] = p + Indent{-1, -1};
                    else if (c_0_1) square[degree++] = p + Indent{-1,  0};

                    if (c_1_0) square[degree++] = p + Indent{ 1, -1};
                    else if (c_1_1) square[degree++] = p + Indent{ 1,  0};

                    break;
                case kLeft:
                    p = target();
                case kRight:
                    (p.col > 0 &&
                     ((c_0_0 = (p.row > 0        && solver.isFourSquare(p + Indent{-1, -1}))) ||
                      (c_0_1 = (p.row < size-1   && solver.isFourSquare(p + Indent{ 0, -1})))));

                    (p.col < size-2 &&
                     ((c_1_0 = (p.row > 0        && solver.isFourSquare(p + Indent{-1,  1}))) ||
                      (c_1_1 = (p.row < size-1   && solver.isFourSquare(p + Indent{ 0,  1})))));

                    if (c_0_0) square[degree++] = p + Indent{-1, -1};
                    else if (c_0_1) square[degree++] = p + Indent{0,  -1};

                    if (c_1_0) square[degree++] = p + Indent{ -1, 1};
                    else if (c_1_1) square[degree++] = p + Indent{ 0, 1};

                    break;
                default: assert(false);
            }
            solver.makeMove(*this);
            return degree > 0;
        }
    };

    vector<int> playIt(int colors, vector<string> board, int startingSeed) override {
//            ofstream output("out.txt");
        vector<int> int_moves;
        init(colors, board, startingSeed);
        eliminate();
        updateMoves();
        Move m, m_some;
        Count d, d_some;
        FourMoveSolver buffer;
        for (Index i = 0; i < kMoveCount; ++i) {
            if (four_moves_.size()) {
                d = 0;
                for (auto& p : four_moves_) {
                    m_some = p.second;
                    buffer = *this;
                    buffer.makeMove(m_some);
                    auto sqs = buffer.eliminate(m_some);
                    buffer.validate();
                    buffer.updateMovesAfterEliminationMove(m_some, sqs);
                    d_some = sqs.size() + buffer.four_moves_.size();
                    if (d_some > d) {
                        m = m_some;
                        d = d_some;
                    }
                }
//                    outputColorBoard(output);
//                    output << m << endl;
                makeMove(m);
                auto sqs = eliminate(m);
                validate();
                updateMovesAfterEliminationMove(m, sqs);
            }
            else {
                m = randomMove();
                makeMove(m);
                updateMovesAfterMove(m);
            }
            int_moves.push_back(m.position.row);
            int_moves.push_back(m.position.col);
            int_moves.push_back(m.direction);
        }
        return int_moves;
    }

    void updateMoves() {
        updateMoves(boardRectangle());
    }

    void updateMoves(const Rectangle& rect) {
        Index ind;
        for (auto& m : fourMoves(rect)) {
            ind = m.index();
            if (four_moves_.count(ind) == 0) {
                four_moves_.emplace(ind, FourMove{m, eliminationDegree(m)});
            }
        }
    }

    void updateMovesAfterMove(const Move& m) {
        // first get to top left
        Int r = m.position.row,
        c = m.position.col;
        Rectangle rect;
        switch (m.direction) {
            case kLeft:
                rect.set(r-2, c-3, 5, 6);
                break;
            case kRight:
                rect.set(r-2, c-2, 5, 6);
                break;
            case kUp:
                rect.set(r-3, c-2, 6, 5);
                break;
            case kDown:
                rect.set(r-2, c-2, 6, 5);
                break;
        }
        updateMoves(boardRectangle().intersection(rect));
    }

    void updateMovesAfterEliminationMove(const Move& m, const vector<Position>& sqs) {
        vector<Rectangle> rect_sqs;
        for (auto& s : sqs) {
            rect_sqs.push_back(Rectangle(s.row-2, s.col-2, 6, 6));
        }
        Position s;
        s = m.position;
        rect_sqs.push_back(Rectangle(s.row-2, s.col-2, 5, 5));
        s = m.target();
        rect_sqs.push_back(Rectangle(s.row-2, s.col-2, 5, 5));

        // put in united rectangle
        if (rect_sqs.size()) {
            updateMoves(boardRectangle().intersection(
            Rectangle::unite(rect_sqs.begin(), rect_sqs.end())));
        }
    }

    void validate() {
        FourMove m;
        Degree d;
        vector<Index> remove_indices;
        for (auto& p : four_moves_) {
            m = p.second;
            d = eliminationDegree(m);
            if (d == 0) remove_indices.push_back(p.first);
            else m.degree = d;
        }
        for (auto index : remove_indices) {
            four_moves_.erase(index);
        }
    }

    Int totalDegree() {
        Int s = 0;
        vector<Rectangle> sqs;
        for (auto p : four_moves_) {
            s += p.second.degree;
            p.second.update(*this);
            for (int i = 0; i < p.second.degree; ++i) {
                sqs.push_back(squareRectangle(p.second.square[i]));
            }
        }

        for (int i = 0; i < sqs.size(); ++i) {
            for (int j = i+1; j < sqs.size(); ++j) {
                if (!sqs[i].intersection(sqs[j]).isEmpty()) {
                    --s;
                    break;
                }
            }
        }

        return s;
    }

    // will be small number of those. so no need in unordered_map probably
    map<Index, FourMove> four_moves_;
};
}


namespace square_remover {


struct Wide : FourMoveSolver {

    vector<int> playIt(int colors, vector<string> board, int startingSeed) override {
//            ofstream outans("ans.txt");

        Count no_move_count = 0;
        vector<int> moves;
        // you are mother fucker USE INT ONLY fuck you!!!!
        Int single_fm_count, double_fm_count, fm_count;
        Int single_el_count, double_el_count, el_count;
        double single_balance, double_balance, balance;
        Move single_move;
        DoubleMove double_move;

        Move last_move = Move(0, 0, kRight);
        Color last_color_sum = 0;

        init(colors, board, startingSeed);
        eliminate();
        updateMoves();
        Move m;
        DoubleMove d_m;
        Wide wideBuffer;
        FourMoveSolver fourBuffer;
        for (auto i = 0; i < kMoveCount; ++i) {
            single_fm_count = 0;
            single_el_count = 0;
            single_balance = MAXFLOAT;
            for (auto& p : four_moves_) {
                m = p.second;
                fourBuffer = *this;
                fourBuffer.makeMove(m);
                auto sqs = fourBuffer.eliminate(m);
                fourBuffer.validate();
                fourBuffer.updateMovesAfterEliminationMove(m, sqs);
                el_count = sqs.size();
                if (el_count < single_el_count) {
                    continue;
                }
                fm_count = fourBuffer.totalDegree();
                if (el_count == single_el_count && fm_count < single_fm_count) {
                    continue;
                }
                balance = fourBuffer.balance();
                if (el_count == single_el_count && fm_count == single_fm_count && balance >= single_balance) {
                    continue;
                }

                single_el_count = el_count;
                single_fm_count = fm_count;
                single_balance = balance;
                single_move = m;
            }

            double_fm_count = 0;
            double_el_count = 0;
            double_balance = MAXFLOAT;
            for (auto p : pre_four_moves_) {
                d_m = p.second;
                fourBuffer = *this;
                fourBuffer.makeMove(d_m);
                fourBuffer.updateMovesAfterMove(d_m);
                fourBuffer.makeMove(d_m.next);
                auto sqs = fourBuffer.eliminate(d_m.next);
                fourBuffer.validate();
                fourBuffer.updateMovesAfterEliminationMove(d_m.next, sqs);
                el_count = sqs.size();
                if (el_count < double_el_count) {
                    continue;
                }
                fm_count = fourBuffer.totalDegree();
                if (el_count == double_el_count && fm_count < double_fm_count) {
                    continue;
                }
                balance = fourBuffer.balance();
                if (el_count == double_el_count && fm_count == double_fm_count && balance >= double_balance) {
                    continue;
                }

                double_el_count = el_count;
                double_fm_count = fm_count;
                double_balance = balance;
                double_move = d_m;
            }

            if (four_moves_.empty() && pre_four_moves_.empty()) {
//                    output << "random ";
                Move three_move = randomMove();

                Count pre_move_count = 0, count;
                for (int r = 0; r < board_size_; ++r) {
                    for (int c = 0; c < board_size_; ++c) {
                        // cool, no move down... nice
                        // nothing to validate only hard watch
                        if (c < board_size_-1) {
                            m = Move(r, c, kRight);
                            wideBuffer = *this;
                            wideBuffer.makeMove(m);
                            wideBuffer.updateMovesAfterMove(m);
                            count = wideBuffer.pre_four_moves_.size();
                            if (count > pre_move_count) {
                                three_move = m;
                                pre_move_count = count;
                            }
                        }
                        if (r < board_size_-1) {
                            m = Move(r, c, kDown);
                            wideBuffer = *this;
                            wideBuffer.makeMove(m);
                            wideBuffer.updateMovesAfterMove(m);
                            count = wideBuffer.pre_four_moves_.size();
                            if (count > pre_move_count) {
                                three_move = m;
                                pre_move_count = count;
                            }
                        }
                    }
                }
                no_move_count++;
                makeMove(three_move);
                updateMovesAfterMove(three_move);

                moves.push_back(three_move.position.row);
                moves.push_back(three_move.position.col);
                moves.push_back(three_move.direction);
//                    outans << i << " " << 0 << endl;
                last_move = three_move;
                last_color_sum = color_board_(three_move.position) + color_board_(three_move.target());
            }
            else if (four_moves_.empty() ||
                     //                         (double_el_count > single_el_count ||
                     //                          (double_el_count == single_el_count &&
                     //                           double_fm_count-1 > single_fm_count))) {

                     ((double_el_count + double_fm_count - 1 >
                       single_el_count + single_fm_count) &&
                      (last_move.index() != double_move.Move::index() ||
                       last_color_sum != color_board_(double_move.position) + color_board_(double_move.target())))
            ) {

//                    output << "pre ";

                makeMove(double_move);
                updateMovesAfterMove(double_move);
//                    makeMove(pre_four_move.next);
//                    auto ps = eliminate(pre_four_move.next);
//                    updateMovesAfterEliminationMove(pre_four_move.next, ps);

                moves.push_back(double_move.position.row);
                moves.push_back(double_move.position.col);
                moves.push_back(double_move.direction);
                last_move = double_move;
                last_color_sum = color_board_(double_move.position) + color_board_(double_move.target());
                if (i == kMoveCount-1) continue;
//                    moves.push_back(pre_four_move.next.position.row);
//                    moves.push_back(pre_four_move.next.position.col);
//                    moves.push_back(pre_four_move.next.direction);
//                    ++i;
//                    outans << i << " " << 0 << endl;
            }
            else {
//                    output << "four ";
                makeMove(single_move);
                auto sqs = eliminate(single_move);
                updateMovesAfterEliminationMove(single_move, sqs);

                moves.push_back(single_move.position.row);
                moves.push_back(single_move.position.col);
                moves.push_back(single_move.direction);
                last_move = single_move;
                last_color_sum = color_board_(single_move.position) + color_board_(single_move.target());
//                    outans << i << " " << ps.size() << endl;
            }
//                stats << i << " "

//                output << "it: " << i << " four: " << four_moves_.size() << " pre: " << pre_four_moves_.size() << endl;
//                output << double_el_count << " " << double_fm_count << " " << double_balance << " vs "
//                       << single_el_count << " " << single_fm_count << " " << single_balance
//                       <<  endl;

        }

        // output << "random: " << no_move_count;
        return moves;
    }

    vector<DoubleMove> preFourMoves() {
        return preFourMoves(boardRectangle());
    }


    // dont expect possible four moves here
    vector<DoubleMove> preFourMoves(const Rectangle& rect) {
        vector<DoubleMove> pre_sqs;
        Rectangle rect_search;
        Indent d;
        Move m;
        bool b;
        array<tuple<bool, Move, Indent>, 2> items;
        for (auto r = rect.row_begin(); r < rect.row_end(); ++r) {
            for (auto c = rect.col_begin(); c < rect.col_end(); ++c) {
                rect_search.position = {r-2, c-2};
                if (r < board_size_-1 && color_board_(r, c) != color_board_(r+1, c)) {
                    items[0] = make_tuple(true, Move(r, c, kDown), Indent(6, 5));
                }
                else get<0>(items[0]) = false;

                if (c < board_size_-1 && color_board_(r, c) != color_board_(r, c+1)) {
                    items[1] = make_tuple(true, Move(r, c, kRight), Indent(5, 6));
                }
                else get<0>(items[1]) = false;

                for (auto i : {0, 1}) {
                    tie(b, m, d) = items[i];
                    if (!b || four_moves_.count(m.index())) continue;
                    rect_search.indent = d;
                    makeMove(m);
                    for (auto four_move : fourMoves(boardRectangle().intersection(rect_search))) {
                        if (four_moves_.count(four_move.index()) == 0) {
                            pre_sqs.push_back(DoubleMove(m, four_move));
                        }
                    }
                    makeMove(m);
                }
            }
        }
        return pre_sqs;
    }

    // probably should return vector of new moves
    void updateMoves() {
        updateMoves(boardRectangle());
    }

    // need to run validation first (need to have right four_moves) or reimplement preFourMoves
    void updateMoves(const Rectangle& rect) {
        validate();
        FourMoveSolver::updateMoves(rect);
        validatePreFourMoves();
        updatePreFourMoves(rect);
    }

    void updatePreFourMoves(const Rectangle& rect) {
        Index ind;
        for (auto& m : preFourMoves(rect)) {
            ind = m.index();
            if (pre_four_moves_.count(ind) == 0) {
                pre_four_moves_.emplace(ind, m);
            }
        }
    }

    void updateMovesAfterMove(const Move& m) {
        // first get to top left
        Int r = m.position.row,
        c = m.position.col;
        Rectangle rect;
        switch (m.direction) {
            case kLeft:
                rect.set(r-3, c-4, 7, 8);
                break;
            case kRight:
                rect.set(r-3, c-3, 7, 8);
                break;
            case kUp:
                rect.set(r-4, c-3, 8, 7);
                break;
            case kDown:
                rect.set(r-3, c-3, 8, 7);
                break;
        }
        updateMoves(boardRectangle().intersection(rect));
    }

    void updateMovesAfterEliminationMove(const Move& m, const vector<Position>& sqs) {
        vector<Rectangle> rect_sqs;
        for (auto& s : sqs) {
            rect_sqs.push_back(Rectangle(s.row-3, s.col-3, 8, 8));
        }
        Position s;
        s = m.position;
        rect_sqs.push_back(Rectangle(s.row-3, s.col-3, 7, 7));
        s = m.target();
        rect_sqs.push_back(Rectangle(s.row-3, s.col-3, 7, 7));

        // put in united rectangle
        if (rect_sqs.size()) {
            updateMoves(boardRectangle().intersection(
            Rectangle::unite(rect_sqs.begin(), rect_sqs.end())));
        }
    }

    void validatePreFourMoves() {
        DoubleMove pre_m;
        vector<Index> remove_indices;
        for (auto& p : pre_four_moves_) {
            pre_m = p.second;
            makeMove(pre_m);
            if (four_moves_.count(pre_m.Move::index()) || !canEliminate(pre_m.next)) {
                remove_indices.push_back(p.first);
            }
            makeMove(pre_m);
        }
        for (auto index : remove_indices) {
            pre_four_moves_.erase(index);
        }
    }

    map<Index, DoubleMove> pre_four_moves_;
};

}

class SquareRemover {
    std::vector<int> playIt(int colors, std::vector<std::string> board, int startingSeed);
};

std::vector<int> SquareRemover::playIt(int colors, std::vector<std::string> board, int startingSeed) {
    std::vector<int> moves;
    square_remover::Wide sr;
    moves = sr.playIt(colors, board, startingSeed);
    return moves;
}

