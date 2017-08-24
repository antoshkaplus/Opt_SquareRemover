#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cstring>
#include <set>
#include <tuple>
#include <algorithm>

using namespace std;

// #define	LOCAL

class XorShift {
    unsigned int x = 123456789;
    unsigned int y = 362436069;
    unsigned int z = 521288629;
    unsigned int w = 88675123;

public:
    int xor128() {
        int t;
        t = x ^ (x << 11);
        x = y; y = z; z = w;
        return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8))) >> 1;
    }

    inline int nextInt() { return xor128(); }
};

template<typename T, T def, int w, int h>
class Cookie2D {
public:
    Cookie2D() : gid(0) {}

    inline void clear() { gid++; }
    inline T get(int x, int y) { return id[y][x] == gid ? val[y][x] : def; }
    inline void set(int x, int y, T v) {
        id[y][x] = gid;
        val[y][x] = v;
    }

private:
    unsigned int gid;
    unsigned int id[h][w];
    T val[h][w];
};

template<typename T, T def, int w, int h, int d>
class Cookie3D {
public:
    Cookie3D() : gid(0) {}

    inline void clear() { gid++; }
    inline T get(int x, int y, int z) { return id[z][y][x] == gid ? val[z][y][x] : def; }
    inline void set(int x, int y, int z, T v) {
        id[z][y][x] = gid;
        val[z][y][x] = v;
    }

private:
    unsigned int gid;
    unsigned int id[d][h][w];
    T val[d][h][w];
};


template<int size>
class Graph {
public:
    int add(int t, unsigned short v) {
#ifdef LOCAL
        if (index >= size) { throw 0; }
#endif
        move[index] = v;
        next[index] = t;
        return index++;
    }

    Graph() : index(0) { }
    inline void clear() { index = 0; }

    int index;
    unsigned short move[size];
    int next[size];
};

// CONSTANT
const int MIN_COLOR = 4;
const int MAX_COLOR = 6;

const int MIN_N = 8;
const int MAX_N = 16;

const int MAX_TURN = 10000;

// PARAMETER
const int MAX_LIMIT = 5;
// const int MAX_BEAM_SIZE = 20;
// const int MAX_BEAM_KOUHO_SIZE = 10;
int MAX_BEAM_SIZE = 20;
int MAX_BEAM_KOUHO_SIZE = 10;

// @todo MAX_MOVE_SIZEの最大値後で考える予定
// const int MAX_MOVE_SIZE = MAX_TURN * MAX_BEAM_SIZE * MAX_BEAM_KOUHO_SIZE;
const int MAX_MOVE_SIZE = MAX_TURN * 20 * 10;

// unyo~n
const int dx[4] = { 1, 0, -1, 0, };
const int dy[4] = { 0, -1, 0, 1, };
long long hs[MAX_COLOR][MAX_N][MAX_N];

// global var
int n;
int colors;
XorShift rndm;
Graph<MAX_MOVE_SIZE> g;
Graph<MAX_MOVE_SIZE> g2;

void init() {
    // dx[0] = 1;
    // dx[1] = 0;
    // dx[2] = -1;
    // dx[3] = 0;

    // dy[0] = 0;
    // dy[1] = -1;
    // dy[2] = 0;
    // dy[3] = 1;

    for (int i = 0; i < MAX_COLOR; i++) {
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                hs[i][y][x] = (long long)rndm.nextInt() << 32 | rndm.nextInt();
            }
        }
    }

    // for (int i = 0; i <= n + 1; i++) {
    // 	isWall[i][0] = isWall[i][n + 1] = isWall[0][i] = isWall[n + 1][i] = true;
    // }
}

//
#if 0
struct Board {
	inline void swapCell(int x, int y, int dir) {
		check(x, y);
		check(x + dx[dir], y + dy[dir]);
		const int c1 = board[y][x];
		const int c2 = board[y + dy[dir]][x + dx[dir]];
		h ^= hs[c1][y][x];
		h ^= hs[c2][y + dy[dir]][x + dx[dir]];
		swap(board[y][x], board[y + dy[dir]][x + dx[dir]]);
		h ^= hs[c2][y][x];
		h ^= hs[c1][y + dy[dir]][x + dx[dir]];
	}

	void init() {
		h = 0;
		memset(board, 0, sizeof(board));
		for (int y = 0; y < n; y++) {
			for (int x = 0; x < n; x++) {
				h ^= hs[0][y][x];
			}
		}
	}

	inline void check(int x, int y) {
#ifdef LOCAL
		if (x < 0 || y < 0 || x >= n || y >= n) {
			throw 0;
		}
#endif
	}

	inline int get(int x, int y) {
		return board[(y*n + x) / 10] >> ((y*n + x) % 10) & 7;
	}

	inline int func(int x, int y) {
		check(x, y);
		check(x + 1, y + 1);
		return
			(board[y][x] ^ board[y][x + 1]) |
			(board[y + 1][x + 1] ^ board[y][x + 1]) |
			(board[y + 1][x + 1] ^ board[y + 1][x]);
	}

	inline void put(int x, int y, int c) {
		h ^= hs[board[y][x]][y][x];
		board[y][x] = c;
		h ^= hs[c][y][x];
	}

	inline int get(int x, int y) const {
		return board[y][x];
	}

	inline void copyFrom(const Board &b) {
		h = b.h;
		memcpy(board, b.board, sizeof(board));
	}

	inline long long hash() { return h; }

	void dump() {
		cerr << "dump" << endl;
		for (int y = 0; y < n; y++) {
			for (int x = 0; x < n; x++) {
				cerr << board[y][x];
			}
			cerr << endl;
		}
	}

private:
	long long h;
	// 3bit / cell
	unsigned int board[(MAX_N * MAX_N * 3 + 31) / 32];
};
#else

// @todo 盤の表現を工夫して高速化？
struct Board {
    inline void swapCell(int x, int y, int dir) {
        check(x, y);
        check(x + dx[dir], y + dy[dir]);
        const int c1 = board[y][x];
        const int c2 = board[y + dy[dir]][x + dx[dir]];
        h ^= hs[c1][y][x];
        h ^= hs[c2][y + dy[dir]][x + dx[dir]];
        swap(board[y][x], board[y + dy[dir]][x + dx[dir]]);
        h ^= hs[c2][y][x];
        h ^= hs[c1][y + dy[dir]][x + dx[dir]];
    }

    void init() {
        h = 0;
        memset(board, 0, sizeof(board));
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                h ^= hs[0][y][x];
            }
        }
    }

    inline void check(int x, int y) const {
#ifdef LOCAL
        if (x < 0 || y < 0 || x >= n || y >= n) {
			cerr << "Illegal position: " << x << " " << y << " " << n << endl;
			throw 0;
		}
#endif
    }

    inline void check(int x, int y, int c) const {
#ifdef LOCAL
        check(x, y);
		if (c < 0 || c >= colors) {
			cerr << "Illegal color: " << c << " " << colors << endl;
			throw 0;
		}
#endif
    }

    inline int func(int x, int y) const {
        check(x, y);
        check(x + 1, y + 1);
        return
        (board[y][x] ^ board[y][x + 1]) |
        (board[y + 1][x + 1] ^ board[y][x + 1]) |
        (board[y + 1][x + 1] ^ board[y + 1][x]);
    }

    inline void put(int x, int y, int c) {
        check(x, y, c);
        // cerr << "put: " << x << " " << y << " " << c << " " << (int)board[y][x] << endl;
        h ^= hs[board[y][x]][y][x];
        board[y][x] = c;
        h ^= hs[c][y][x];
    }

    inline int get(int x, int y) const {
        check(x, y, board[y][x]);
        return board[y][x];
    }

    inline void copyFrom(const Board &b) {
        h = b.h;
        memcpy(board, b.board, sizeof(board));
    }

    inline long long hash() { return h; }

    void dump() {
        cerr << "dump" << endl;
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                cerr << board[y][x];
            }
            cerr << endl;
        }
    }

private:
    long long h;
    unsigned char board[MAX_N][MAX_N];
};
#endif

struct State {
    inline int nextColor() {
        int res = (int)(seed % colors);
        seed = seed * 48271 % 2147483647;
        return res;
    }

    inline void put(int x, int y) {
        board.put(x + 0, y + 0, nextColor());
        board.put(x + 1, y + 0, nextColor());
        board.put(x + 0, y + 1, nextColor());
        board.put(x + 1, y + 1, nextColor());
    }

    inline void remove() { remove(0, 0, n - 1, n - 1); }
    inline void remove(int x1, int y1, int x2, int y2) {
        // cerr << x1 << " " << y1 << " " << x2 << " " << y2 << " " << endl;
        x1 = max(0, x1);
        y1 = max(0, y1);
        x2 = min(n - 1, x2);
        y2 = min(n - 1, y2);

        for (int y = y1; y < y2; y++) {
            for (int x = x1; x < x2; x++) {
                if (!board.func(x, y)) {
                    score2 -= updateHeuristicsScore(x, y, x + 1, y + 1);
                    put(x, y);
                    score2 += updateHeuristicsScore(x, y, x + 1, y + 1);
                    score++;
                    remove(min(x - 2, x1), min(y - 2, y1), max(x + 2, x2), max(y + 2, y2));
                    return;
                }
            }
        }

        // cerr << "finish" << endl;
    }

    inline int updateHeuristicsScore(int minX, int minY, int maxX, int maxY) {
        int res = 0;

        for (int y = max(0, minY - 1); y <= min(n - 2, maxY); y++) {
            for (int x = max(0, minX - 1); x <= min(n - 2, maxX); x++) {
                res += getHeuristicsScore(x, y);
            }
        }

        return res;
    }

    inline int updateHeuristicsScore(int x, int y, int dir) {
        int minX = min(x, x + dx[dir]);
        int maxX = x + x + dx[dir] - minX;
        int minY = min(y, y + dy[dir]);
        int maxY = y + y + dy[dir] - minY;
        return updateHeuristicsScore(minX, minY, maxX, maxY);
    }

    inline void swapCell(int x, int y, int dir) {
        if (turn < MAX_TURN) {
            // if (x < 0 || y < 0 || x >= n || y >= n) throw 0;
            // moves[turn++] = ((y - 1)*MAX_N + (x - 1)) * 4 + dir;
            turn++;
            prevMove = g.add(prevMove, (y*MAX_N + x) * 4 + dir);
            score2 -= updateHeuristicsScore(x, y, dir);
            board.swapCell(x, y, dir);
            score2 += updateHeuristicsScore(x, y, dir);
            remove(x - 2, y - 2, x + 2, y + 2);
        }
    }

    inline void swapCellTry(int x, int y, int dir) {
        if (turn < MAX_TURN) {
            turn++;
            prevMove2 = g2.add(prevMove2, (y*MAX_N + x) * 4 + dir);
            score2 -= updateHeuristicsScore(x, y, dir);
            board.swapCell(x, y, dir);
            score2 += updateHeuristicsScore(x, y, dir);
            remove(x - 2, y - 2, x + 2, y + 2);
        }
    }

    inline void commit() {
        static int tmp[MAX_TURN];
        int len = 0;
        for (; prevMove2 != -1; prevMove2 = g2.next[prevMove2]) {
            tmp[len++] = g2.move[prevMove2];
        }
        while (len > 0) {
            prevMove = g.add(prevMove, tmp[--len]);
        }
    }

    inline long long hash() { return board.hash(); }

    // @todo コピーを高速化
    inline void copyFrom(const State &s) {
        seed = s.seed;
        turn = s.turn;
        score = s.score;
        score2 = s.score2;
        board.copyFrom(s.board);
        // memcpy(moves, s.moves, sizeof(moves[0]) * turn);
    }

    inline void copyMovesFrom(const State &s) {
        // memcpy(moves, s.moves, sizeof(moves[0]) * turn);
        prevMove = s.prevMove;
        prevMove2 = s.prevMove2;
    }

    void init(int seed, vector<string> &board) {
        this->seed = seed;
        turn = 0;
        score = 0;
        score2 = 0;
        this->board.init();
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                this->board.put(x, y, board[y][x] - '0');
            }
        }
        prevMove = -1;
        prevMove2 = -1;
        remove();
        score2 = getHeuristicsScore();
    }

    vector<int> getAns() {
        vector<int> res;
        int toDir[] = { 1, 0, 3, 2, };
        for (int m = prevMove; m != -1; m = g.next[m]) {
            const int mv = g.move[m];
            res.push_back(toDir[mv % 4]);
            res.push_back(mv / 4 % MAX_N);
            res.push_back(mv / 4 / MAX_N);
            /*
            res.push_back(moves[i] / 4 / MAX_N);
            res.push_back(moves[i] / 4 % MAX_N);
            res.push_back(toDir[moves[i] % 4]);
            */
        }
        reverse(res.begin(), res.end());
        cerr << "size: " << turn << " " << res.size() << endl;
        cerr << "score: " << score << endl;
        return res;
    }

    bool operator<(const State &s) const {
        if (score * s.turn != s.score * turn) {
            return score * s.turn < s.score * turn;
        }
        return score2 < s.score2;
        // if(s.turn != turn) return turn > s.turn;
        // return score < s.score;
    }

    State() {}
    State(const State &st) {
        copyFrom(st);
        copyMovesFrom(st);
        // score2 = getHeuristicsScore();
    }

    inline int getHeuristicsScore(int x, int y) {
        int res = 0;
        unsigned char cnt[MAX_COLOR] = {};
        for (int dy = 0; dy <= 1; dy++) {
            for (int dx = 0; dx <= 1; dx++) {
                cnt[board.get(x + dx, y + dy)]++;
            }
        }

        for (int i = 0; i < MAX_COLOR; i++) {
            res += cnt[i] * (int)cnt[i] * cnt[i];
        }

        return res;
    }

    inline int getHeuristicsScore() {
        int res = 0;
        // if(false)
        for (int y = 0; y < n - 1; y++) {
            for (int x = 0; x < n - 1; x++) {
                int cnt[MAX_COLOR] = {};
                for (int dy = 0; dy <= 1; dy++) {
                    for (int dx = 0; dx <= 1; dx++) {
                        cnt[board.get(x + dx, y + dy)]++;
                    }
                }

                for (int i = 0; i < MAX_COLOR; i++) {
                    res += cnt[i] * cnt[i] * cnt[i];
                }
            }
        }

        return res;
    }

    long long seed;
    int turn;
    int score;
    int score2;
    // unsigned short moves[MAX_TURN];
    int prevMove;
    int prevMove2;
    Board board;
};

class Solver2 {
public:
    vector<int> solve(vector<string> &board, int startSeed);

private:
    int cost(int x, int y, int dx, int dy, int c, int limit);
    int move(int x, int y, int dx, int dy, int c, int limit);
    int calcScore(int x, int y, int c);
    void doTurn(State &st);

    set<long long> used;
    vector<State> states;
    vector<State> nextStates;
    State state;
};

int Solver2::cost(int x, int y, int dx, int dy, int c, int limit) {
    for (int i = 0; i <= limit; i++) {
        bool ok = false;
        for (int j = 0; j <= i; j++) {
            const int px = x + dx * j;
            const int py = y + dy * (i - j);
            if (px >= 0 && px < n && py >= 0 && py < n) {
                if (state.board.get(px, py) == c) {
                    return i;
                }
                ok = true;
            }
        }
        if (!ok) break;
    }
    return 1 << 20;
}

int Solver2::calcScore(int x, int y, int c) {
    int cnt = 0;
    for (int dy = 0; dy <= 1; dy++) {
        for (int dx = 0; dx <= 1; dx++) {
            if (state.board.get(x + dx, y + dy) == c) {
                cnt++;
            }
        }
    }
    return cnt;
}

int Solver2::move(int x, int y, int dx, int dy, int c, int lim) {
    // @todo ここよくする？高速化に比べて効果が未知数なので後回しかなあ。
    //       bfsにするとか？
    // cerr << "move: " << x << " " << y << " " << dx << " " << dy << " " << c << endl;
    // state.board.dump();
    if (lim == 0) return 0;
    for (int i = 0; i <= lim; i++) {
        for (int j = 0; j <= i; j++) {
            int px = x + dx * j;
            int py = y + dy * (i - j);
            if (!(px >= 0 && px < n && py >= 0 && py < n)) continue;
            if (state.board.get(px, py) != c) continue;
            // dp?
            while (px != x || py != y) {
#if 1
                if (px != x && py != y) {
                    // state.board.;
                    int ox = dx == -1 ? px : px - 1;
                    int oy = dy == -1 ? py : py - 1;
                    int c1 = state.board.get(px - dx, py);
                    int c2 = state.board.get(px, py - dy);
                    int v1 = calcScore(ox, oy, c1);
                    int v2 = calcScore(ox, oy, c2);
                    if (v1 > v2 || (v1 == v2 && (rndm.nextInt() & 1) == 0)) {
                        state.swapCellTry(px, py, (dx + 1) & 3);
                        px -= dx;
                    }
                    else {
                        state.swapCellTry(px, py, (dy + 4) & 3);
                        py -= dy;
                    }
                }
                else if (px != x) {
                    // cerr << "move1: " << x << " " << y << " " << px << " " << py << " " << ((dx + 3) & 3) << endl;
                    state.swapCellTry(px, py, (dx + 1) & 3);
                    px -= dx;
                    // state.board.dump();
                }
                else {
                    // cerr << "move2: " << x << " " << y << " " << px << " " << py << " " << ((dx + 3) & 3) << endl;
                    // state.board.dump();
                    state.swapCellTry(px, py, (dy + 4) & 3);
                    py -= dy;
                    // state.board.dump();
                }
#else
                if (px != x && (py == y || (rndm.nextInt() & 1) == 1)) {
					// cerr << "move1: " << x << " " << y << " " << px << " " << py << " " << ((dx + 3) & 3) << endl;
					state.swapCellTry(px, py, (dx + 1) & 3);
					px -= dx;
					// state.board.dump();
				}
				else {
					// cerr << "move2: " << x << " " << y << " " << px << " " << py << " " << ((dx + 3) & 3) << endl;
					// state.board.dump();
					state.swapCellTry(px, py, (dy + 4) & 3);
					py -= dy;
					// state.board.dump();
				}
#endif
            }
            return i;
        }
    }
    // state.board.dump();
    // throw 0;
    return 1 << 20;
}

void Solver2::doTurn(State &st) {
    if (st.turn >= MAX_TURN) {
        nextStates.push_back(st);
        used.insert(st.hash());
        return;
    }

    static Cookie3D<bool, false, MAX_N, MAX_N, MAX_COLOR> vis;

    vis.clear();
    state.copyFrom(st);
    state.copyMovesFrom(st);
    int cnt = 0;
    // for(int limit = 1; cnt == 0; limit++) {
    for (int limit = 1; cnt < MAX_BEAM_KOUHO_SIZE && limit < MAX_LIMIT; limit++) {
        for (int y = 0; y < n - 1; y++) {
            for (int x = 0; x < n - 1; x++) {
                for (int c = 0; c < colors; c++) if (!vis.get(x, y, c)) {
                        int same = 0;
                        if(state.board.get(x + 0, y + 0) == c) same++;
                        if(state.board.get(x + 1, y + 0) == c) same++;
                        if(state.board.get(x + 0, y + 1) == c) same++;
                        if(state.board.get(x + 1, y + 1) == c) same++;
                        if(same < 2) {
                            vis.set(x, y, c, true);
                            continue;
                        }

                        int lim = limit;
                        lim -= cost(x + 0, y + 0, -1, -1, c, lim);
                        lim -= cost(x + 1, y + 0, +1, -1, c, lim);
                        lim -= cost(x + 0, y + 1, -1, +1, c, lim);
                        lim -= cost(x + 1, y + 1, +1, +1, c, lim);

                        if (lim >= 0) {
                            // state.board.dump();
                            vis.set(x, y, c, true);

                            for (int dup = 0; dup < 1; dup++) {
                                int lv = limit;
                                // cerr << "doTurn: " << limit << " " << lim << " " << x << " " << y << " " << c <<  endl;
                                lv -= move(x + 0, y + 0, -1, -1, c, lv);
                                lv -= move(x + 1, y + 0, +1, -1, c, lv);
                                lv -= move(x + 0, y + 1, -1, +1, c, lv);
                                lv -= move(x + 1, y + 1, +1, +1, c, lv);

                                if (lv == 0 && !used.count(state.hash())) {
                                    // cerr << "add: " << x << " " << y << " " << c << " " << state.hash() << " " << st.hash() << " " << cnt << endl;
                                    used.insert(state.hash());
                                    nextStates.push_back(state);
                                    if (++cnt >= MAX_BEAM_KOUHO_SIZE) return;
                                    state.copyFrom(st);
                                    state.copyMovesFrom(st);
                                }
                                else {
                                    state.copyFrom(st);
                                    state.copyMovesFrom(st);
                                    break;
                                    // cerr << "dup: " << lv << " " << lim << " " << x << " " << y << " " << c << " " << state.hash() << " " << st.hash() << " " << cnt << endl;
                                }
                                // return;
                            }
                        }
                    }
            }
        }
    }
}

vector<int> Solver2::solve(vector<string> &board, int startSeed) {
    state.init(startSeed, board);
    states.push_back(state);

    cerr << "color: " << colors << endl;
    cerr << "MAX_BEAM_SIZE: " << MAX_BEAM_SIZE << endl;
    cerr << "MAX_BEAM_KOUHO_SIZE: " << MAX_BEAM_KOUHO_SIZE << endl;
    // state.board.dump();
    while (true) {
#ifdef LOCAL
        // if(state.turn == 736) state.board.dump();
		// if(state.turn >= 736) { if(state.turn == 736) state.board.dump(); state.swapCell(1, 1, 0); continue; }
		// for(State &st : states) { cerr << st.turn << " "; } cerr << endl;
		cerr << states[0].turn << endl;
#endif
        nextStates.clear();
        used.clear();
        for (int i = 0; i < MAX_BEAM_SIZE && i < (int)states.size(); i++) {
            states[i].commit();
        }
        g2.clear();
        for (int i = 0; i < MAX_BEAM_SIZE && i < (int)states.size(); i++) {
            doTurn(states[i]);
        }

        swap(nextStates, states);
        sort(states.rbegin(), states.rend());

        int cnt = 0;
        for (int i = 0; i < MAX_BEAM_SIZE && i < (int)states.size(); i++) {
            if (states[i].turn >= MAX_TURN) cnt++;
        }
        if (cnt >= MAX_BEAM_SIZE) break;
    }
    states[0].commit();
    return states[0].getAns();
}




class SquareRemover {
public:
    vector<int> playIt(int colors, vector<string> board, int startSeed);
};

vector<int> SquareRemover::playIt(int colors, vector<string> board, int startSeed) {
    // check time limit
#if 0
    colors = 6;
	startSeed = 94238747;
	board = {
		"0115355505011041",
		"3121314313412532",
		"0253553013441354",
		"4104555211231454",
		"5221455452134314",
		"3522002005032544",
		"1001152140204540",
		"2035250142033031",
		"0052333505514520",
		"3511030522115214",
		"4201320532535034",
		"0210252505225122",
		"0345252022244153",
		"1511351143112323",
		"1321110202424042",
		"0242233103010513",
	};
#endif

    ::colors = colors;
    ::n = board.size();

    // ::MAX_BEAM_SIZE = (int)((40 - 20) / (8 - 16.0) * (n - 8) + 40);
    // ::MAX_BEAM_KOUHO_SIZE = (int)((20 - 10) / (8 - 16.0) * (n - 8) + 20);
    ::MAX_BEAM_SIZE = (int)((70 - 50) / (8 - 16.0) * (n - 8) + 70);
    ::MAX_BEAM_KOUHO_SIZE = 7;
    ::init();
    // GreedySolver solver;
    Solver2 solver;
    return solver.solve(board, startSeed);
}





#ifdef LOCAL

int main() {
	cerr << "check output" << endl;

	int n;
	int colors;
	int startSeed;
	vector<string> board;

	cin >> colors >> n;

	cerr << colors << endl;
	cerr << n << endl;

	for (int i = 0; i < n; i++) {
		string s;
		cin >> s;
		board.push_back(s);
		cerr << s << endl;
	}

	cin >> startSeed;

	cerr << startSeed << endl;

	SquareRemover solver;
	vector<int> ans = solver.playIt(colors, board, startSeed);

	for (int i = 0; i < (int)ans.size(); i++) {
		cout << ans[i] << endl;
	}

	return 0;
}

#endif
