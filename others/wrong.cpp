#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <numeric>
#include <algorithm>
#include <bitset>
#include <complex>
#include <iomanip>
#include <unordered_set>

using namespace std;

typedef unsigned uint;
typedef long long Int;

const int INF = 1001001001;
const Int INFLL = 1001001001001001001LL;

template<typename T> void pv(T a, T b) { for (T i = a; i != b; ++i) cout << *i << " "; cout << endl; }
template<typename T> void chmin(T& a, T b) { if (a > b) a = b; }
template<typename T> void chmax(T& a, T b) { if (a < b) a = b; }

uint xor128() {
    static uint x = 123456789;
    static uint y = 362436069;
    static uint z = 521288629;
    static uint w = 88675123;
    uint t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

const int DR[4] = {-1, 0, 1, 0}, DC[4] = {0, 1, 0, -1};
int N, COLORS, TileQueue[1<<18], HashValue[18][18][6];

int8_t Cost[1<<21];
int BIT[5][5] = {
{-1, 0, 1, 2, -1},
{3, 4, 5, 6, 7},
{8, 9, 10, 11, 12},
{13, 14, 15, 16, 17},
{-1, 18, 19, 20, -1},
};
int BITR0[21] = {-2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2};
int BITC0[21] = {-1, 0, 1, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -1, 0, 1};
int BITR[18][18][21], BITC[18][18][21], BITN[18][18], BITI[18][18][21];

inline bool test(int s, int r, int c) {
    return s & (1 << BIT[r][c]);
}

inline int swapbit(int s, int p1, int p2) {
    int ss = s & ~((1<<p1) | (1<<p2));
    ss |= !!(s & (1<<p1)) << p2;
    ss |= !!(s & (1<<p2)) << p1;
    return ss;
}

void PrecalcCost() {
    for (int i = 0; i < 18; ++i) {
        for (int j = 0; j < 18; ++j) {
            for (int k = 0; k < 6; ++k) {
                HashValue[i][j][k] = xor128();
            }
        }
    }

    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= N; ++c) {
            BITN[r][c] = 0;
            for (int i = 0; i < 21; ++i) {
                int rr = r - BITR0[i];
                int cc = c - BITC0[i];
                if (1 <= rr && rr <= N && 1 <= cc && cc <= N) {
                    BITR[r][c][BITN[r][c]] = rr;//BITR0[i];
                    BITC[r][c][BITN[r][c]] = cc;//BITC0[i];
                    BITI[r][c][BITN[r][c]] = i;
                    ++BITN[r][c];
                }
            }
        }
    }

    int adj[1<<5], popcount[1<<5];
    for (int i = 0; i < (1<<5); ++i) {
        adj[i] = 0;
        popcount[i] = __builtin_popcount(i);
        for (int j = 0; j < 4; ++j) {
            if ((i & (1<<j)) && (i & (1<<(j+1)))) {
                adj[i] |= 1<<j;
            }
        }
    }

    queue<int> que;
    for (int i = 0; i < (1<<21); ++i) {
        Cost[i] = 5;
        int x[5];
        x[0] = adj[i % (1<<3)] << 1;
        x[1] = adj[(i>>3) % (1<<5)];
        x[2] = adj[(i>>8) % (1<<5)];
        x[3] = adj[(i>>13) % (1<<5)];
        x[4] = adj[i>>18] << 1;

        int sq = 0;
        sq += popcount[x[0] & x[1]];
        sq += popcount[x[1] & x[2]];
        sq += popcount[x[2] & x[3]];
        sq += popcount[x[3] & x[4]];

        if (sq >= 1) {
            Cost[i] = 0;
            que.push(i);
        }
    }

    while (!que.empty()) {
        int s = que.front(); que.pop();
        int cost = Cost[s] < 0 ? 0 : Cost[s];

        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                if (BIT[r][c] == -1) {
                    continue;
                }
                if (c < 4 && BIT[r][c + 1] >= 0 && (test(s, r, c) ^ test(s, r, c + 1))) {
                    int ss = swapbit(s, BIT[r][c], BIT[r][c + 1]);
                    if (Cost[ss] > cost + 1) {
                        Cost[ss] = cost + 1;
                        que.push(ss);
                    }
                }
                if (r < 4 && BIT[r + 1][c] >= 0 && (test(s, r, c) ^ test(s, r + 1, c))) {
                    int ss = swapbit(s, BIT[r][c], BIT[r + 1][c]);
                    if (Cost[ss] > cost + 1) {
                        Cost[ss] = cost + 1;
                        que.push(ss);
                    }
                }
            }
        }
    }

    for (int i = 0; i < (1<<21); ++i) {
        if (Cost[i] >= 4) Cost[i] = 0;
        else if (Cost[i] == 3) Cost[i] = 1;
        else if (Cost[i] == 2) Cost[i] = 2;
        else if (Cost[i] == 1) Cost[i] = 4;
        else Cost[i] = 8;
    }
}

int CHANGE_COUNT, MOVE_COUNT;

class Game {
private:
    char board[18][18];
    int colormap[6][18][18];
    int eval, hash;
    int change_buf[64], prev_score, changes;
    int tile_next;
    int score;
    int turn_taken;

    inline void flip(const int col, const int r, const int c, const int b) {
        // if (r >= 1 && c >= 1 && r <= N && c <= N) {
        eval -= Cost[colormap[col][r][c]];
        colormap[col][r][c] ^= 1 << b;
        eval += Cost[colormap[col][r][c]];
        // }
    }

    void remove(const int r, const int c) {
        for (int i = 0; i < BITN[r][c]; ++i) {
            flip(board[r][c], BITR[r][c][i], BITC[r][c][i], BITI[r][c][i]);
        }
        // for (int i = 0; i < 21; ++i) {
        // flip(board[r][c], r-BITR0[i], c-BITC0[i], i);
        // }
        hash ^= HashValue[r][c][(int)board[r][c]];
        board[r][c] = 10;
    }

    void put(const int r, const int c, const int col) {
        for (int i = 0; i < BITN[r][c]; ++i) {
            flip(col, BITR[r][c][i], BITC[r][c][i], BITI[r][c][i]);
        }
        // for (int i = 0; i < 21; ++i) {
        // flip(col,r-BITR0[i],c-BITC0[i],i);
        // }
        board[r][c] = col;
        hash ^= HashValue[r][c][col];
    }

    void change(int r, int c, int col) {
        ++CHANGE_COUNT;
        if (board[r][c] != col) {
            change_buf[changes++] = (r - 1) * 16 * 6 + (c - 1) * 6 + board[r][c];
            remove(r, c);
            put(r, c, col);
        }
    }

    void change_undo(int r, int c, int col) {
        ++CHANGE_COUNT;
        if (board[r][c] != col) {
            remove(r, c);
            put(r, c, col);
        }
    }

    void adjust() {
        while (true) {
            bool cont = false;
            for (int r = 1; r < N; ++r) {
                for (int c = 1; c < N; ++c) {
                    if (Square(r, c)) {
                        cont = true;
                        ++score;
                        change(r, c, TileQueue[tile_next++]);
                        change(r, c + 1, TileQueue[tile_next++]);
                        change(r + 1, c, TileQueue[tile_next++]);
                        change(r + 1, c + 1, TileQueue[tile_next++]);
                        break;
                    }
                }
                if (cont) {
                    break;
                }
            }
            if (!cont) {
                break;
            }
        }
    }

public:
    uint8_t ScoreDiff[18][18][2];
    int EvalDiff[18][18][2];

    Game() { }

    Game(vector<string> b) {
        memset(colormap, 0, sizeof(colormap));
        eval = 0;
        for (int r = 0; r <= N + 1; ++r) {
            for (int c = 0; c <= N + 1; ++c) {
                board[r][c] = 10;
            }
        }
        hash = 0;
        for (int r = 1; r <= N; ++r) {
            for (int c = 1; c <= N; ++c) {
                ScoreDiff[r][c][0] = ScoreDiff[r][c][1] = 0;
                EvalDiff[r][c][0] = EvalDiff[r][c][1] = 0;
                put(r, c, b[r - 1][c - 1] - '0');
            }
        }
        eval = 0;
        for (int r = 1; r <= N; ++r) {
            for (int c = 1; c <= N; ++c) {
                for (int col = 0; col < COLORS; ++col) {
                    eval += Cost[colormap[col][r][c]];
                }
            }
        }
        score = 0;
        tile_next = 0;
        turn_taken = 0;
        changes = 0;
        adjust();
    }

    inline int Hash() const {
        return hash;
    }

    inline int Tile(int r, int c) const {
        return board[r][c];
    }

    inline int Score() const {
        return score;
    }

    inline int Turn() const {
        return turn_taken;
    }

    inline int NextTileId() const {
        return tile_next;
    }

    inline int Eval() const {
        return eval;
    }

    inline void Clear(int r, int c, int d) {
        ScoreDiff[r][c][d] = EvalDiff[r][c][d] = 0;
    }

    void UpdateChange() {
        for (int i = 0; i < changes; ++i) {
            int r = change_buf[i] / 16 / 6 + 1;
            int c = change_buf[i] / 6 % 16 + 1;
            for (int rr = max(1, r - 5); rr <= min(N, r + 5); ++rr) {
                for (int cc = max(1, c - 5); cc <= min(N, c + 5); ++cc) {
                    Clear(rr, cc, 0);
                    Clear(rr, cc, 1);
                }
            }
            // for (int j = 0; j < BITN[r][c]; ++j) {
            //   Clear(r - BITR[r][c][j], c - BITC[r][c][j], 0);
            //   Clear(r - BITR[r][c][j], c - BITC[r][c][j], 1);
            //   Clear(r - BITR[r][c][j] - 1, c - BITC[r][c][j], 0);
            //   Clear(r - BITR[r][c][j], c - BITC[r][c][j] - 1, 1);
            // }
        }
    }

    inline bool Square(int r, int c) {
        return
        board[r][c] == board[r + 1][c] &&
        board[r][c] == board[r][c + 1] &&
        board[r][c] == board[r + 1][c + 1];
    }

    bool MoveTest(int r, int c, int rr, int cc) {
        if (board[r][c] == board[rr][cc]) {
            return false;
        }
        swap(board[r][c], board[rr][cc]);

        bool sq = false;
        sq = sq || Square(r, c);
        sq = sq || Square(r - 1, c);
        sq = sq || Square(r, c - 1);
        sq = sq || Square(r - 1, c - 1);

        sq = sq || Square(rr, cc);
        sq = sq || Square(rr - 1, cc);
        sq = sq || Square(rr, cc - 1);
        sq = sq || Square(rr - 1, cc - 1);

        swap(board[r][c], board[rr][cc]);
        return sq;
    }

    bool Move(int r, int c, int rr, int cc) {
        changes = 0;
        prev_score = score;
        if (board[r][c] == board[rr][cc]) {
            return false;
        }
        ++MOVE_COUNT;
        int from = board[r][c];
        change(r, c, board[rr][cc]);
        change(rr, cc, from);

        bool sq = false;
        sq = sq || Square(r, c);
        sq = sq || Square(r - 1, c);
        sq = sq || Square(r, c - 1);
        sq = sq || Square(r - 1, c - 1);

        sq = sq || Square(rr, cc);
        sq = sq || Square(rr - 1, cc);
        sq = sq || Square(rr, cc - 1);
        sq = sq || Square(rr - 1, cc - 1);

        if (!sq) {
            return false;
        } else {
            adjust();
            return true;
        }
    }

    bool Move(int r, int c, int dir) {
        int rr = r + DR[dir], cc = c + DC[dir];
        return Move(r, c, rr, cc);
    }

    void Undo() {
        for (int i = changes - 1; i >= 0; --i) {
            int r = change_buf[i] / 16 / 6 + 1;
            int c = change_buf[i] / 6 % 16 + 1;
            int col = change_buf[i] % 6;
            change_undo(r, c, col);
        }
        tile_next -= 4 * (score - prev_score);
        score = prev_score;
    }
};

int params[9][3][2] = {
{{40,30},{38,5},{38,5},},
{{33,7},{44,30},{44,10},},
{{38,10},{44,30},{44,10},},
{{38,15},{40,10},{44,20},},
{{33,15},{40,20},{44,20},},
{{33,15},{28,15},{38,15},},
{{30,20},{36,20},{40,15},},
{{32,20},{31,20},{34,20},},
{{32,30},{36,30},{40,20},}
};

class SquareRemover_2 {
private:
    int colors;
    vector<string> initial_board;

    struct BeamInfo {
        int from, move, score, eval, hash;
        BeamInfo() { }
        BeamInfo(int fr, int mv, int sc, int ev, int h) : from(fr), move(mv), score(sc), eval(ev), hash(h) { }
    };

    vector<int> solve() {
        const int COEF_SCORE = params[N - 8][COLORS - 4][0];
        // const int COEF_EVAL = 1;
        const int SCORE_LIM = 3;//params[N - 8][COLORS - 4][1];
        clock_t start = clock();

        int BEAM_WIDTH = 20;
        Game cur_g[20];
        vector<BeamInfo> beam;
        vector<vector<pair<int, int>>> beam_res;

        unordered_set<int> appeared;
        appeared.insert(Game(initial_board).Hash());

        cur_g[0] = Game(initial_board);
        beam.push_back(BeamInfo(-1, -1, cur_g[0].Score(), cur_g[0].Eval(), cur_g[0].Hash()));
        beam_res.push_back(vector<pair<int, int>>());
        beam_res.back().push_back(make_pair(-1, -1));

        for (int i = 0; i < 10000; ++i) {
            if ((i % 100) == 0) {
                double elapsed = (clock() - start) * 1.0 / CLOCKS_PER_SEC;
                if (elapsed > 25.0) {
                    BEAM_WIDTH = 1;
                }// else if (elapsed > 22.5) {
                //   BEAM_WIDTH = 5;
                // } else if (elapsed > 20.0){
                //   BEAM_WIDTH = 10;
                // }
            }
            vector<BeamInfo> next_beam(BEAM_WIDTH * (N-1) * (N-1) * 2);
            int range = min(BEAM_WIDTH, (int)beam.size()), num_cands = 0;
            for (int j = 0; j < range; ++j) {
                Game& g = cur_g[j];
                int cur_score = g.Score(), cur_eval = g.Eval();

                g.UpdateChange();

                int scoreup = 0;
                bool score[18][18][2] = {};
                for (int r = 1; r <= N; ++r) {
                    for (int c = 1; c <= N; ++c) {
                        if (r + 1 <= N) scoreup += (score[r][c][0] = g.MoveTest(r, c, r + 1, c));
                        if (c + 1 <= N) scoreup += (score[r][c][1] = g.MoveTest(r, c, r, c + 1));
                    }
                }

                for (int r = 1; r <= N; ++r) {
                    for (int c = 1; c <= N; ++c) {
                        if (r + 1 <= N && (scoreup < SCORE_LIM || score[r][c][0])) {
                            int h = g.Hash();
                            h ^= HashValue[r][c][g.Tile(r, c)];
                            h ^= HashValue[r][c][g.Tile(r + 1, c)];
                            h ^= HashValue[r + 1][c][g.Tile(r + 1, c)];
                            h ^= HashValue[r + 1][c][g.Tile(r, c)];
                            if (appeared.find(h) != appeared.end()) {
                                continue;
                            }
                            int ns, nd;
                            bool moved = false;
                            if (g.ScoreDiff[r][c][0] > 0 || g.EvalDiff[r][c][0] != 0) {
                                ns = cur_score + g.ScoreDiff[r][c][0];
                                nd = cur_eval + g.EvalDiff[r][c][0];
                            } else {
                                g.Move(r, c, r + 1, c);
                                ns = g.Score();
                                nd = g.Eval();
                                moved = true;
                            }
                            if (cur_score < ns || cur_eval < nd) {
                                next_beam[num_cands++] = BeamInfo(j, (r - 1) * 16 * 2 + (c - 1) * 2 + 0, ns, nd, h);
                            }
                            if (moved) {
                                g.ScoreDiff[r][c][0] = ns - cur_score;
                                g.EvalDiff[r][c][0] = nd - cur_eval;
                                g.Undo();
                            }
                        }
                        if (c + 1 <= N && (scoreup < SCORE_LIM || score[r][c][1])) {
                            int h = g.Hash();
                            h ^= HashValue[r][c][g.Tile(r, c)];
                            h ^= HashValue[r][c][g.Tile(r, c + 1)];
                            h ^= HashValue[r][c + 1][g.Tile(r, c + 1)];
                            h ^= HashValue[r][c + 1][g.Tile(r, c)];
                            if (appeared.find(h) != appeared.end()) {
                                continue;
                            }
                            int ns, nd;
                            bool moved = false;
                            if (g.ScoreDiff[r][c][1] > 0 || g.EvalDiff[r][c][1] != 0) {
                                ns = cur_score + g.ScoreDiff[r][c][1];
                                nd = cur_eval + g.EvalDiff[r][c][1];
                            } else {
                                g.Move(r, c, r, c + 1);
                                ns = g.Score();
                                nd = g.Eval();
                                moved = true;
                            }
                            if (cur_score < ns || cur_eval < nd) {
                                next_beam[num_cands++] = BeamInfo(j, (r - 1) * 16 * 2 + (c - 1) * 2 + 1, ns, nd, h);
                            }
                            if (moved) {
                                g.ScoreDiff[r][c][1] = ns - cur_score;
                                g.EvalDiff[r][c][1] = nd - cur_eval;
                                g.Undo();
                            }
                        }
                    }
                }
            }

            if (beam.size() < BEAM_WIDTH) {
                beam.resize(BEAM_WIDTH);
            }
            int filled = 0;
            Game next_g[20];
            for (int j = 0; j < num_cands && filled < BEAM_WIDTH; ++j) {
                if (j % (2 * BEAM_WIDTH) == 0) {
                    partial_sort(next_beam.begin() + j, next_beam.begin() + min((int)num_cands, j + BEAM_WIDTH * 2), next_beam.begin() + num_cands,
                                 [COEF_SCORE] (const BeamInfo& a, const BeamInfo& b) {
                                     return a.score * COEF_SCORE + a.eval > b.score * COEF_SCORE + b.eval;
                                 });
                }
                next_g[filled] = cur_g[next_beam[j].from];
                next_g[filled].Move(next_beam[j].move / 32 + 1, next_beam[j].move / 2 % 16 + 1, next_beam[j].move % 2 == 0 ? 2 : 1);
                if (appeared.find(next_g[filled].Hash()) != appeared.end()) {
                    continue;
                }
                appeared.insert(next_g[filled].Hash());
                beam[filled++] = next_beam[j];
            }

            vector<pair<int, int>> mvs(BEAM_WIDTH);
            for (int j = 0; j < BEAM_WIDTH; ++j) {
                mvs[j] = make_pair(beam[j].from, beam[j].move);
            }
            beam_res.push_back(std::move(mvs));
            // if ((i + 1) % 100 == 0) {
            //   cerr << i+1 << ' ' <<beam[0].score << endl;
            // }

            memcpy(cur_g, next_g, sizeof(cur_g));
        }

        vector<int> res;
        for (int i = 10000, p = 0; i > 0; --i) {
            pair<int, int> m = beam_res[i][p];
            res.push_back(m.second % 2 == 0 ? 2 : 1);
            res.push_back(m.second / 2 % 16);
            res.push_back(m.second / 32);
            p = m.first;
        }

        reverse(res.begin(), res.end());
        return res;
    }

public:
    vector<int> playIt(int colors_, vector<string> board, int startSeed) {
        N = board.size();
        COLORS = colors = colors_;
        initial_board = board;

        PrecalcCost();

        TileQueue[0] = startSeed;
        for (int i = 1; i < (1<<18); ++i) {
            TileQueue[i] = (TileQueue[i - 1] * 48271LL) % 2147483647;
        }
        for (int i = 0; i < (1<<18); ++i) {
            TileQueue[i] %= colors;
        }

        vector<int> ret = solve();
        // cerr<<CHANGE_COUNT<<' '<<MOVE_COUNT<<endl;
        // cerr << ret.size() << endl;
        return ret;
    }
};

class SquareRemover {
public:
    vector<int> playIt(int colors_, vector<string> board, int startSeed);
};

vector<int> SquareRemover::playIt(int colors, vector<string> board, int startSeed) {
    SquareRemover_2 sr;
    return sr.playIt(colors, board, startSeed);
}