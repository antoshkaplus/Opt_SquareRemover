#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<cstdio>
#include<cstring>
#include<map>
#include<cstdlib>
using namespace std;
#define F0(i,n) for (int i=0; i<n; i++)
#define F1(i,n) for (int i=1; i<=n; i++)
#define SZ(x) ((int)x.size())
const int inf = 1000009;
typedef pair<int,int> pii;
typedef pair<pii,int> Move;
typedef long long ll;

int cf1 = 12;
int cf2 = 10;
#define SUBMIT
const double DEST = 29.3;
//#define TRACK_M
const int MOD = 2147483647;
const ll MULT = 48271;
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};
const int N = 10000;
int BW = 100;

const int MIN_COLORS = 4;
const int MAX_COLORS = 6;
const int ALL_COLORS = MAX_COLORS - MIN_COLORS + 1;
const int MIN_N = 8;
const int MAX_N = 16;
const int ALL_N = MAX_N - MIN_N + 1;

int S5[ALL_N] =
{999, 999, 999, 999, 999, 999, 999, 999, 999};

int S6[ALL_N] =
{999, 999, 999, 999, 999, 999, 999, 999, 999};

int CFS[ALL_COLORS][ALL_N] = {
{10,10,10,10, 9, 9,10,10,10},
{10,10,10,10, 9, 9,10,11,10},
{ 9, 9, 9,10, 9, 9, 9, 9, 9} };

const int BWS[ALL_COLORS][ALL_N] = {
// 8,   9,  10, 11,   12,  13,  14,  15,  16
{500, 400, 300, 250, 200, 200, 150, 150, 120}, //4
{350, 154, 124,  96, 190,  65, 130,  48, 120}, //5
{175, 145, 125, 150,  85,  62,  55,  48, 44}, //6
};

const double DIV6[9] = {2.2, 3.5, 4.2, 4.5, 6.0, 6.0, 6.0, 6.1, 6.7};

int track[1<<26], goback[1<<26], pN;

inline int GetIntByMove(const Move& m) {
    return m.second * 256 + m.first.second * 16 + m.first.first;
}

inline Move GetMoveByInt(int x) {
    return Move( pii(x % 16, x / 16 % 16), x / 256 );
}

int A[10*N]; // should be enough
int n, C;

#include<sys/time.h>

double GetTime() {
    timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}
double saveTime;
double t_o1, t_o2, t_o3;
int c_o1, c_o2, c_o3, c_o4, c_o5, c_o6;
void Init() {
    saveTime = GetTime();
    t_o1 = 0.0;
    t_o2 = 0.0;
    t_o3 = 0.0;
    c_o1 = c_o2 = c_o3 = c_o4 = c_o5 = c_o6 = 0;
}
double Elapsed() {
    return GetTime() - saveTime;
}
void Report() {
    cerr << "Elapsed time: " << Elapsed() << " sec" << endl;
    if (t_o1) cerr << "o1 time: " << t_o1 << " sec" << endl;
    if (t_o2) cerr << "o2 time: " << t_o2 << " sec" << endl;
    if (t_o3) cerr << "o3 time: " << t_o3 << " sec" << endl;
    if (c_o1) cerr << "o1 count: " << c_o1 << endl;
    if (c_o2) cerr << "o2 count: " << c_o2 << endl;
    if (c_o3) cerr << "o3 count: " << c_o3 << endl;
    if (c_o4) cerr << "o4 count: " << c_o4 << endl;
    if (c_o5) cerr << "o5 count: " << c_o5 << endl;
    if (c_o6) cerr << "o6 count: " << c_o6 << endl;
}

struct VVI {
    char u[16][16];
    VVI() {}
    VVI(const vector<string> board) {
        F0(i,n) F0(j,n) u[i][j] = board[i][j]-'0';
    }

    char* operator [] (int x) {
        return u[x];
    }
    const char* operator [] (int x) const {
        return u[x];
    }
    bool operator == (const VVI& other) {
        F0(i,n) if (memcmp(u[i], other.u[i], n) != 0) return false;
        return true;
    }
    void print() const {
        F0(i,n) { F0(j,n) cerr << (int)u[i][j]; cerr << endl; } cerr << endl;
    }
};

int costEq(const VVI& v)
{
    int ret = 0;
    F0(i,n-1) F0(j,n-1) {
            ret += (v[i][j] == v[i+1][j+1]) + (v[i][j+1] == v[i+1][j]) + (v[i][j] == v[i][j+1]) + (v[j][i] == v[j+1][i]);
        }
    F0(j,n-1) ret += (v[n-1][j] == v[n-1][j+1]) + (v[j][n-1] == v[j+1][n-1]);
    return ret;
}

struct State
{
    VVI v;
    State() {}
#ifdef TRACK_M
    map<int,int> M;
    int or11, or2;
#endif

    int at;
    int tindex;
    int cost;

    void Initialize(const VVI& _v) {
#ifdef TRACK_M
        or2 = or11 = 0;
#endif
        v = _v;
        at = 0; tindex = -1;
        bool found = true;
        while (found) {
            found = false;
            F0(i,n-1) F0(j,n-1) if (!found)
                        if (v[i][j] == v[i][j+1] && v[i][j] == v[i+1][j] && v[i][j] == v[i+1][j+1]) {
                            v[i][j] = A[at++];
                            v[i][j+1] = A[at++];
                            v[i+1][j] = A[at++];
                            v[i+1][j+1] = A[at++];
                            found = true;
                            break;
                        }
        }
        cost = costEq(v);
    }

    void Sim(pii a, pii b) {
        bool found = true;
        int x1 = max(min(a.first - 1, b.first - 1), 0);
        int y1 = max(min(a.second - 1, b.second - 1), 0);
        int x2 = min(max(a.first, b.first), n-2);
        int y2 = min(max(a.second, b.second), n-2);
        while (found) {
            found = false;
            for (int i = x1; i <= x2 && !found; i++) for (int j = y1; j <= y2; j++) {
                    if (v[i][j] == v[i][j+1] && v[i][j] == v[i+1][j] && v[i][j] == v[i+1][j+1]) {
                        cost += (A[at] == A[at+1]) + (A[at] == A[at+2]) + (A[at] == A[at+3]) + (A[at+1] == A[at+2]) + (A[at+1] == A[at+3]) + (A[at+2] == A[at+3]);
                        cost -= 6;
                        if (i > 0) {
                            if (j > 0) {
                                cost += (A[at] == v[i-1][j-1]) - (v[i][j] == v[i-1][j-1]);
                            }
                            cost -= (v[i][j] == v[i-1][j])+(v[i][j] == v[i-1][j+1])+(v[i][j+1] == v[i-1][j])+(v[i][j+1] == v[i-1][j+1]);
                            cost += (A[at] == v[i-1][j])+(A[at] == v[i-1][j+1])+(A[at+1] == v[i-1][j])+(A[at+1] == v[i-1][j+1]);

                            if (j + 2 < n) {
                                cost += (A[at+1] == v[i-1][j+2]) - (v[i][j+1] == v[i-1][j+2]);
                            }
                        }
                        if (j > 0) {
                            cost -= (v[i][j] == v[i][j-1])+(v[i][j] == v[i+1][j-1])+(v[i+1][j] == v[i][j-1])+(v[i+1][j] == v[i+1][j-1]);
                            cost += (A[at] == v[i][j-1])+(A[at] == v[i+1][j-1])+(A[at+2] == v[i][j-1])+(A[at+2] == v[i+1][j-1]);
                        }
                        if (j + 2 < n) {
                            cost -= (v[i][j+1] == v[i][j+2])+(v[i][j+1] == v[i+1][j+2])+(v[i+1][j+1] == v[i][j+2])+(v[i+1][j+1] == v[i+1][j+2]);
                            cost += (A[at+1] == v[i][j+2])+(A[at+1] == v[i+1][j+2])+(A[at+3] == v[i][j+2])+(A[at+3] == v[i+1][j+2]);
                        }

                        if (i + 2 < n) {
                            if (j > 0) {
                                cost += (A[at+2] == v[i+2][j-1]) - (v[i+1][j] == v[i+2][j-1]);
                            }
                            cost -= (v[i+1][j] == v[i+2][j])+(v[i+1][j] == v[i+2][j+1])+(v[i+1][j+1] == v[i+2][j])+(v[i+1][j+1] == v[i+2][j+1]);
                            cost += (A[at+2] == v[i+2][j])+(A[at+2] == v[i+2][j+1])+(A[at+3] == v[i+2][j])+(A[at+3] == v[i+2][j+1]);

                            if (j + 2 < n) {
                                cost += (A[at+3] == v[i+2][j+2]) - (v[i+1][j+1] == v[i+2][j+2]);
                            }
                        }

                        v[i][j] = A[at++];
                        v[i][j+1] = A[at++];
                        v[i+1][j] = A[at++];
                        v[i+1][j+1] = A[at++];

                        // update x1,y1,x2,y2
                        //
                        x1 = max(i - 1, 0);
                        x2 = min(max(i + 1, x2), n-2);
                        y1 = max(min(y1, j-1), 0);
                        y2 = min(max(j + 1, y2), n-2);
                        found = true;
                        break;
                    }
                }
        }
    }

    bool Has(pii a, pii b) {
        char c = v[a.first][a.second];
        if (a.first+1 < n && c == v[a.first+1][a.second]) {
            if (a.second+1 < n && v[a.first][a.second+1] == c &&
                v[a.first+1][a.second+1] == c) return true;
            if (a.second>0 && v[a.first][a.second-1] == c &&
                v[a.first+1][a.second-1] == c) return true;
        }
        if (a.first > 0 && c == v[a.first-1][a.second]) {
            if (a.second+1 < n && v[a.first][a.second+1] == c &&
                v[a.first-1][a.second+1] == c) return true;
            if (a.second>0 && v[a.first][a.second-1] == c &&
                v[a.first-1][a.second-1] == c) return true;
        }
        c = v[b.first][b.second];
        if (b.first+1 < n && c == v[b.first+1][b.second]) {
            if (b.second+1 < n && v[b.first][b.second+1] == c &&
                v[b.first+1][b.second+1] == c) return true;
            if (b.second>0 && v[b.first][b.second-1] == c &&
                v[b.first+1][b.second-1] == c) return true;
        }
        if (b.first > 0 && c == v[b.first-1][b.second]) {
            if (b.second+1 < n && v[b.first][b.second+1] == c &&
                v[b.first-1][b.second+1] == c) return true;
            if (b.second>0 && v[b.first][b.second-1] == c &&
                v[b.first-1][b.second-1] == c) return true;
        }
        return false;
    }

    bool Has() {
        F0(i,n-1) F0(j,n-1)
                if (v[i][j] == v[i][j+1] && v[i][j] == v[i+1][j] && v[i][j] == v[i+1][j+1]) {
                    return true;

                }
        return false;
    }

    inline void update(const Move& m) {
        switch (m.second)
        {
            case 0:
            {
                int x = m.first.first - 1;
                int y = m.first.second;
                char c1 = v[x][y];
                char c2 = v[x+1][y];
                if (x > 0)
                {
                    cost += (v[x-1][y] == c2) - (v[x-1][y] == c1);
                    if (y > 0) {
                        cost += (v[x-1][y-1] == c2) - (v[x-1][y-1] == c1);
                    }
                    if (y + 1 < n) {
                        cost += (v[x-1][y+1] == c2) - (v[x-1][y+1] == c1);
                    }
                }
                if (x + 2 < n)
                {
                    cost += (v[x+2][y] == c1) - (v[x+2][y] == c2);
                    if (y > 0) {
                        cost += (v[x+2][y-1] == c1) - (v[x+2][y-1] == c2);
                    }
                    if (y + 1 < n) {
                        cost += (v[x+2][y+1] == c1) - (v[x+2][y+1] == c2);
                    }
                }
                return;
            }
            case 1:
            {
                int x = m.first.first;
                int y = m.first.second;
                char c1 = v[x][y];
                char c2 = v[x][y+1];
                if (y > 0)
                {
                    cost += (v[x][y-1] == c2) - (v[x][y-1] == c1);
                    if (x > 0) {
                        cost += (v[x-1][y-1] == c2) - (v[x-1][y-1] == c1);
                    }
                    if (x + 1 < n) {
                        cost += (v[x+1][y-1] == c2) - (v[x+1][y-1] == c1);
                    }
                }
                if (y + 2 < n)
                {
                    cost += (v[x][y+2] == c1) - (v[x][y+2] == c2);
                    if (x > 0) {
                        cost += (v[x-1][y+2] == c1) - (v[x-1][y+2] == c2);
                    }
                    if (x + 1 < n) {
                        cost += (v[x+1][y+2] == c1) - (v[x+1][y+2] == c2);
                    }
                }
                return;
            }
            case 2:
            {
                int x = m.first.first;
                int y = m.first.second;
                char c1 = v[x][y];
                char c2 = v[x+1][y];
                if (x > 0)
                {
                    cost += (v[x-1][y] == c2) - (v[x-1][y] == c1);
                    if (y > 0) {
                        cost += (v[x-1][y-1] == c2) - (v[x-1][y-1] == c1);
                    }
                    if (y + 1 < n) {
                        cost += (v[x-1][y+1] == c2) - (v[x-1][y+1] == c1);
                    }
                }
                if (x + 2 < n)
                {
                    cost += (v[x+2][y] == c1) - (v[x+2][y] == c2);
                    if (y > 0) {
                        cost += (v[x+2][y-1] == c1) - (v[x+2][y-1] == c2);
                    }
                    if (y + 1 < n) {
                        cost += (v[x+2][y+1] == c1) - (v[x+2][y+1] == c2);
                    }
                }
                return;
            }
            case 3:
            {
                int x = m.first.first;
                int y = m.first.second - 1;
                char c1 = v[x][y];
                char c2 = v[x][y+1];
                if (y > 0)
                {
                    cost += (v[x][y-1] == c2) - (v[x][y-1] == c1);
                    if (x > 0) {
                        cost += (v[x-1][y-1] == c2) - (v[x-1][y-1] == c1);
                    }
                    if (x + 1 < n) {
                        cost += (v[x+1][y-1] == c2) - (v[x+1][y-1] == c1);
                    }
                }
                if (y + 2 < n)
                {
                    cost += (v[x][y+2] == c1) - (v[x][y+2] == c2);
                    if (x > 0) {
                        cost += (v[x-1][y+2] == c1) - (v[x-1][y+2] == c2);
                    }
                    if (x + 1 < n) {
                        cost += (v[x+1][y+2] == c1) - (v[x+1][y+2] == c2);
                    }
                }
                return;
            }
        }
    }

    void ApplySim(const Move& m) {
        int x1 = m.first.first, y1 = m.first.second;
        int x2 = x1 + dx[m.second], y2 = y1 + dy[m.second];
        update(m);
        swap(v[x1][y1], v[x2][y2]);
        goback[pN] = tindex;
        tindex = pN;
        track[pN++] = GetIntByMove(m);
        Sim(pii(x1, y1), pii(x2, y2));
    }

    void Apply(const Move& m) {
        int x1 = m.first.first, y1 = m.first.second;
        int x2 = x1 + dx[m.second], y2 = y1 + dy[m.second];
        update(m);
        swap(v[x1][y1], v[x2][y2]);
        goback[pN] = tindex;
        tindex = pN;
        track[pN++] = GetIntByMove(m);
    }

    void PrintExtra() {
        cerr << C << " " << n;
#ifdef TRACK_M
        for (map<int,int>::iterator it= M.begin(); it!=M.end(); it++) {
			cerr << "   [" << it->first << "-" << it->second << "]";
		}
        cerr << "     2: " << or2 << "  11: " << or11 << endl;
#endif
        cerr << endl;
    }

    int Cost() const {
        // TODO: depending on step/C/n?
        return 1000 * (cf1 * at + cf2 * cost) + at;
    }
};

inline int dist(const pii& a, const pii& b) { return abs(a.first-b.first) + abs(a.second-b.second); }
bool good(int x, int y) { return 0 <= x && x < n && 0 <= y && y < n; }

Move FromTo(pii from, pii to) {
    //c_o1++;
    int dir;
    if (to.first > from.first) dir = 2;
    else if (to.first < from.first) dir = 0;
    else if (to.second > from.second) dir = 1;
    else if (to.second < from.second) dir = 3;
    else throw 1;
    return Move(from, dir);
}

typedef pair<pii, pii> OneSwap;
typedef vector<OneSwap> AllSwaps;

pii need[4], have[256];
AllSwaps poss[1<<16];
#ifdef TRACK_M
int origin[1<<16];
#endif
int needCount, haveCount, possCount;

int surrx[16][16][16][128];
int surry[16][16][16][128];
int surrCount[16][16][16];
int sx[16][16][2][2][2];
int sy[16][16][2][2][2];
int sc[16][16][2][2];

const int S8 = 8;
const int S7 = 7;
State NextStates[S8][1<<16];
int NextCount[S8];


int COSTS[1<<16], ids[1<<16];
int cc(int x, int y) {
    return COSTS[x] > COSTS[y];
}

void BuildAllPlans(int at, int limit, AllSwaps& swaps, const pii& pos)
{
    if (at == needCount) {
        poss[possCount++] = swaps;
        return;
    }

    F0(i,haveCount) {
        int d = dist(need[at], have[i]);
        if (limit >= d + needCount - at - 1) {
            swaps.push_back( OneSwap(have[i], need[at]) );
            pii old = have[i]; have[i] = pii(inf,inf);

            BuildAllPlans(at + 1, limit - d, swaps, pos);
            have[i] = old;
            swaps.pop_back();
        }
    }
}

int d1[8][8][8][8], d1dx[8][8][8][8], d1dy[8][8][8][8];
int d2[8][8][8][8], d2b[8][8][8][8];
void precompute_surround() {
    F0(i,6) F0(j,6) F0(k,6) F0(l,6) {
                    d1[i][j][k][l] = -1;
                    F0(c,6) {
                        int z = (i == c) + (j == c) + (k == c) + (l == c);
                        if (z == 3) {
                            int dx = 0, dy = 0;
                            if (c != j) { dx = 0; dy = 1; } else
                            if (c != k) { dx = 1; dy = 0; } else
                            if (c != l) { dx = 1; dy = 1; }
                            d1[i][j][k][l] = c;
                            d1dx[i][j][k][l] = dx;
                            d1dy[i][j][k][l] = dy;
                        }
                    }
                }
    F0(i,6) F0(j,6) F0(k,6) F0(l,6) {
                    d2[i][j][k][l] = -1;
                    d2b[i][j][k][l] = -1;
                    F0(c,6) {
                        int z = (i == c) + (j == c) + (k == c) + (l == c);
                        if (z >= 2) {
                            if (d2[i][j][k][l] == -1) {
                                d2[i][j][k][l] = c;
                            } else d2b[i][j][k][l] = c;
                        }
                    }
                }

    F0(i,n) F0(j,n) F0(di,2) F0(dj,2) {
                    int cnt = 0;
                    for (int x = i+di-1; x <= i+di+1; x++)
                        for (int y = j+dj-1; y <= j+dj+1; y++) if (abs(x-i-di)+abs(y-j-dj) == 1)
                                if (good(x,y) && !(x >= i && x < i + 2 && y >= j && y < j + 2)) {
                                    sx[i][j][di][dj][cnt] = x;
                                    sy[i][j][di][dj][cnt] = y;
                                    cnt++;
                                }
                    sc[i][j][di][dj] = cnt;
                }

    F0(i,n) F0(j,n) F0(D,6) {
                int cnt = 0;
                int lx = max(i-D,0), rx = min(i+1+D,n-1);
                int ly = max(j-D,0), ry = min(j+1+D,n-1);
                for (int x = lx; x <= rx; x++)
                    for (int y = ly; y <= ry; y++)
                        if (!(x >= i && x < i + 2 && y >= j && y < j + 2)) {
                            int d1 = min(abs(x-i), abs(x-i-1));
                            int d2 = min(abs(y-j), abs(y-j-1));
                            if (d1 + d2 > D) continue;
                            surrx[i][j][D][cnt] = x;
                            surry[i][j][D][cnt] = y;
                            cnt++;
                        }
                surrCount[i][j][D] = cnt;
            }
}

void NextD2(const State& state, int D, int mod8) {
    int next8 = (mod8 + D) & S7;
    F0(o,possCount) {
        NextStates[next8][NextCount[next8]++] = state;
        State& tmp = NextStates[next8][NextCount[next8]-1];

        bool good = true;
        // first apply simple moves
        int x = 0;
        F0(si,SZ(poss[o])) if (dist(poss[o][si].first, poss[o][si].second) == 1) {
                if (++x < D) {
                    tmp.Apply(FromTo(poss[o][si].first, poss[o][si].second));
                    if (tmp.Has(poss[o][si].first, poss[o][si].second))
                    {
                        good = false;
                        break;
                    }
                }
                else {
                    tmp.ApplySim(FromTo(poss[o][si].first, poss[o][si].second));
                }
            }
        // next apply complex moves one by one
        F0(si,SZ(poss[o])) if (good && dist(poss[o][si].first, poss[o][si].second) > 1) {
                int k = dist(poss[o][si].first, poss[o][si].second);
                pii from = poss[o][si].first, to = poss[o][si].second;
                vector<Move> later;

                while (k--) {
                    // find first move
                    Move res = FromTo(from, to);
                    pii q = pii(from.first + dx[res.second], from.second + dy[res.second]);
                    if (tmp.v[from.first][from.second] != tmp.v[q.first][q.second]) {
                        if (++x < D) {
                            tmp.Apply(res);
                            if (tmp.Has(from, q))
                            {
                                good = false;
                                break;
                            }
                        } else {
                            tmp.ApplySim(res);
                        }
                    } else {
                        later.push_back(res);
                    }
                    from = q;
                }
                while (good && !later.empty()) {
                    if (++x < D) {
                        tmp.Apply(later.back());
                        if (tmp.Has())
                        {
                            good = false;
                            break;
                        }
                    } else {
                        tmp.ApplySim(later.back());
                    }
                    later.pop_back();
                }
                if (!good) break;
            }
        if (!good) {
            NextCount[next8]--;
            continue;
        }
#ifdef TRACK_M
        tmp.M[D]++;
        if (origin[o] == 11) tmp.or11++; else tmp.or2++;
#endif
    }
}

bool x[16][16];
void GenNextStates5(const State& state, int mod8, bool keep)
{
    const VVI& v = state.v;
    memset(x, false, sizeof(x));
    possCount = 0;
    {
        //int D = 1;
        F0(i,n-1) F0(j,n-1) if (d1[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]] != -1) {
                    int c = d1[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    int di = d1dx[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    int dj = d1dy[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    F0(p,sc[i][j][di][dj]) {
                        if (v[sx[i][j][di][dj][p]][sy[i][j][di][dj][p]] == c) {
                            x[i][j] = x[i+1][j] = x[i][j+1] = x[i+1][j+1] = true;
                            if (i > 0) x[i-1][j] = x[i-1][j+1] = true;
                            if (j > 0) x[i][j-1] = x[i+1][j-1] = true;
                            //c_o1++;
                            poss[possCount++].assign(1, OneSwap(pii(sx[i][j][di][dj][p],sy[i][j][di][dj][p]), pii(i+di, j+dj)));
                        }
                    }
                }

        if (possCount) {
            int next8 = (mod8+1)&S7;
            F0(o,possCount) {
                NextStates[next8][NextCount[next8]++] = state;
                State& tmp = NextStates[next8][NextCount[next8]-1];
                pii from = poss[o][0].first;
                pii to = poss[o][0].second;
                Move res = FromTo(from, to);
                tmp.ApplySim(res);

#ifdef TRACK_M
                tmp.M[1]++;
#endif
            }
            if (possCount > S5[n-8] || !keep)
                return; else { possCount = 0; }
        }
    }

    for (int D = 2; D <= 3; D++) {
        F0(i,n-1) F0(j,n-1) if (!x[i][j] && d2[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]] != -1) {
                    int c = d2[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    needCount = 0;
                    F0(di,2) F0(dj,2) {
                            if (v[i+di][j+dj] != c) need[needCount++] = pii(i+di,j+dj);
                        }


                    if (D == 2 && needCount == 2) {
                        int di0 = need[0].first - i, dj0 = need[0].second - j, di1 = need[1].first - i, dj1 = need[1].second - j;
                        F0(p0,sc[i][j][di0][dj0]) if (v[sx[i][j][di0][dj0][p0]][sy[i][j][di0][dj0][p0]] == c) {
                                F0(p1,sc[i][j][di1][dj1]) if (v[sx[i][j][di1][dj1][p1]][sy[i][j][di1][dj1][p1]] == c) {
                                        poss[possCount].resize(2);
#ifdef TRACK_M
                                        origin[possCount] = 11;
#endif
                                        poss[possCount][0] = OneSwap(pii(sx[i][j][di0][dj0][p0],sy[i][j][di0][dj0][p0]), pii(i+di0, j+dj0));
                                        poss[possCount][1] = OneSwap(pii(sx[i][j][di1][dj1][p1],sy[i][j][di1][dj1][p1]), pii(i+di1, j+dj1));
                                        //c_o3++;
                                        possCount++;
                                    }
                            }
                        continue;
                    }


                    if (possCount > 10) continue;

                    if (needCount == 1) {
                        F0(p,surrCount[i][j][D]) {
                            if (v[surrx[i][j][D][p]][surry[i][j][D][p]] == c) {
                                if (dist(need[0], pii(surrx[i][j][D][p],surry[i][j][D][p])) == D) {
                                    //c_o2++;
#ifdef TRACK_M
                                    origin[possCount] = 2;
#endif
                                    poss[possCount++].assign(1, OneSwap(pii(surrx[i][j][D][p],surry[i][j][D][p]), need[0]));
                                    break;
                                }
                            }
                        }
                        continue;
                    }

                    haveCount = 0;
                    int useD = D - needCount + 1;
                    F0(p,surrCount[i][j][useD]) {
                        if (v[surrx[i][j][useD][p]][surry[i][j][useD][p]] == c)
                            have[haveCount++] = pii(surrx[i][j][useD][p],surry[i][j][useD][p]);
                    }

                    if (needCount > haveCount) { continue; }

                    AllSwaps allswaps;
                    BuildAllPlans(0, D, allswaps, pii(i, j));
                }

        if (possCount) {
            NextD2(state, D, mod8);
            return;
        }
    }
}


void GenNextStates6(const State& state, int mod8, bool keep)
{
    const VVI& v = state.v;
    memset(x, false, sizeof(x));
    possCount = 0;
    {
        //int D = 1;
        F0(i,n-1) F0(j,n-1) if (d1[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]] != -1) {
                    int c = d1[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    int di = d1dx[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    int dj = d1dy[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    F0(p,sc[i][j][di][dj]) {
                        if (v[sx[i][j][di][dj][p]][sy[i][j][di][dj][p]] == c) {
                            x[i][j] = x[i+1][j] = x[i][j+1] = x[i+1][j+1] = true;
                            if (i > 0) x[i-1][j] = x[i-1][j+1] = true;
                            if (j > 0) x[i][j-1] = x[i+1][j-1] = true;
                            //c_o1++;
                            poss[possCount++].assign(1, OneSwap(pii(sx[i][j][di][dj][p],sy[i][j][di][dj][p]), pii(i+di, j+dj)));
                        }
                    }
                }

        if (possCount) {
            int next8 = (mod8+1)&S7;
            F0(o,possCount) {
                NextStates[next8][NextCount[next8]++] = state;
                State& tmp = NextStates[next8][NextCount[next8]-1];
                pii from = poss[o][0].first;
                pii to = poss[o][0].second;
                Move res = FromTo(from, to);
                tmp.ApplySim(res);

#ifdef TRACK_M
                tmp.M[1]++;
#endif
            }
            if (possCount > S6[n-8] || !keep)
                return; else { possCount = 0; }
        }
    }

    for (int D = 2; D <= 3; D++) {
        F0(i,n-1) F0(j,n-1) if (!x[i][j] && d2[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]] != -1) {
                    int c = d2[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    needCount = 0;
                    F0(di,2) F0(dj,2) {
                            if (v[i+di][j+dj] != c) need[needCount++] = pii(i+di,j+dj);
                        }
                    //c_o6++;
                    if (needCount == 1) {
                        F0(p,surrCount[i][j][D]) {
                            if (v[surrx[i][j][D][p]][surry[i][j][D][p]] == c) {
                                if (dist(need[0], pii(surrx[i][j][D][p],surry[i][j][D][p])) == D) {
                                    //c_o2++;
#ifdef TRACK_M
                                    origin[possCount] = 2;
#endif
                                    poss[possCount++].assign(1, OneSwap(pii(surrx[i][j][D][p],surry[i][j][D][p]), need[0]));
                                }
                            }
                        }
                        continue;
                    }

                    if (D == 2) {
                        int di0 = need[0].first - i, dj0 = need[0].second - j, di1 = need[1].first - i, dj1 = need[1].second - j;
                        F0(p0,sc[i][j][di0][dj0]) if (v[sx[i][j][di0][dj0][p0]][sy[i][j][di0][dj0][p0]] == c) {
                                F0(p1,sc[i][j][di1][dj1]) if (v[sx[i][j][di1][dj1][p1]][sy[i][j][di1][dj1][p1]] == c) {
                                        poss[possCount].resize(2);
                                        // c_o3++;
#ifdef TRACK_M
                                        origin[possCount] = 11;
#endif
                                        poss[possCount][0] = OneSwap(pii(sx[i][j][di0][dj0][p0],sy[i][j][di0][dj0][p0]), pii(i+di0, j+dj0));
                                        poss[possCount][1] = OneSwap(pii(sx[i][j][di1][dj1][p1],sy[i][j][di1][dj1][p1]), pii(i+di1, j+dj1));
                                        possCount++;
                                    }
                            }
                        continue;
                    }

                    haveCount = 0;
                    int useD = D - needCount + 1;
                    F0(p,surrCount[i][j][useD]) {
                        if (v[surrx[i][j][useD][p]][surry[i][j][useD][p]] == c)
                            have[haveCount++] = pii(surrx[i][j][useD][p],surry[i][j][useD][p]);
                    }

                    if (needCount > haveCount) { continue; }

                    AllSwaps allswaps;
                    BuildAllPlans(0, D, allswaps, pii(i, j));
                }
        if (possCount) {
            NextD2(state, D, mod8);
            return;
        }
    }
}


void GenNextStates(const State& state, int mod8)
{
    const VVI& v = state.v;
    possCount = 0;
    {
        //int D = 1;
        F0(i,n-1) F0(j,n-1) if (d1[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]] != -1) {
                    int c = d1[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    int di = d1dx[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    int dj = d1dy[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    F0(p,sc[i][j][di][dj]) {
                        if (v[sx[i][j][di][dj][p]][sy[i][j][di][dj][p]] == c) {
                            poss[possCount++].assign(1, OneSwap(pii(sx[i][j][di][dj][p],sy[i][j][di][dj][p]), pii(i+di, j+dj)));
                        }
                    }
                }
        if (possCount) {
            int next8 = (mod8+1)&S7;
            F0(o,possCount) {
                NextStates[next8][NextCount[next8]++] = state;
                State& tmp = NextStates[next8][NextCount[next8]-1];
                pii from = poss[o][0].first;
                pii to = poss[o][0].second;
                Move res = FromTo(from, to);
                tmp.ApplySim(res);

#ifdef TRACK_M
                tmp.M[1]++;
#endif
            }
            return;
        }
    }

    if (NextCount[(mod8+1)&S7] && NextCount[(mod8+2)&S7]) return;

    for (int D = 2; D <= 3; D++)
    {
        F0(i,n-1) F0(j,n-1) if (d2[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]] != -1) {
                    int c = d2[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    needCount = 0;
                    F0(di,2) F0(dj,2) {
                            if (v[i+di][j+dj] != c) need[needCount++] = pii(i+di,j+dj);
                        }

                    if (needCount == 1) {
                        F0(p,surrCount[i][j][D]) {
                            if (v[surrx[i][j][D][p]][surry[i][j][D][p]] == c) {
                                if (dist(need[0], pii(surrx[i][j][D][p],surry[i][j][D][p])) == D) {
                                    poss[possCount++].assign(1, OneSwap(pii(surrx[i][j][D][p],surry[i][j][D][p]), need[0]));

                                    NextD2(state, D, mod8);
                                    return;
                                }
                            }
                        }
                    } else {
                        int di0 = need[0].first - i, dj0 = need[0].second - j, di1 = need[1].first - i, dj1 = need[1].second - j;
                        F0(p0,sc[i][j][di0][dj0]) if (v[sx[i][j][di0][dj0][p0]][sy[i][j][di0][dj0][p0]] == c) {
                                F0(p1,sc[i][j][di1][dj1]) if (v[sx[i][j][di1][dj1][p1]][sy[i][j][di1][dj1][p1]] == c) {
                                        poss[possCount].resize(2);
                                        poss[possCount][0] = OneSwap(pii(sx[i][j][di0][dj0][p0],sy[i][j][di0][dj0][p0]), pii(i+di0, j+dj0));
                                        poss[possCount][1] = OneSwap(pii(sx[i][j][di1][dj1][p1],sy[i][j][di1][dj1][p1]), pii(i+di1, j+dj1));
                                        possCount++;

                                        NextD2(state, D, mod8);
                                        return;
                                    }
                            }
                    }
                    if (d2b[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]] == -1) continue;

                    c = d2[v[i][j]][v[i][j+1]][v[i+1][j]][v[i+1][j+1]];
                    needCount = 0;
                    F0(di,2) F0(dj,2) {
                            if (v[i+di][j+dj] != c) need[needCount++] = pii(i+di,j+dj);
                        }

                    if (needCount == 1) {
                        F0(p,surrCount[i][j][D]) {
                            if (v[surrx[i][j][D][p]][surry[i][j][D][p]] == c) {
                                if (dist(need[0], pii(surrx[i][j][D][p],surry[i][j][D][p])) == D) {
                                    poss[possCount++].assign(1, OneSwap(pii(surrx[i][j][D][p],surry[i][j][D][p]), need[0]));

                                    NextD2(state, D, mod8);
                                    return;
                                }
                            }
                        }
                    } else {
                        int di0 = need[0].first - i, dj0 = need[0].second - j, di1 = need[1].first - i, dj1 = need[1].second - j;
                        F0(p0,sc[i][j][di0][dj0]) if (v[sx[i][j][di0][dj0][p0]][sy[i][j][di0][dj0][p0]] == c) {
                                F0(p1,sc[i][j][di1][dj1]) if (v[sx[i][j][di1][dj1][p1]][sy[i][j][di1][dj1][p1]] == c) {
                                        poss[possCount].resize(2);
                                        poss[possCount][0] = OneSwap(pii(sx[i][j][di0][dj0][p0],sy[i][j][di0][dj0][p0]), pii(i+di0, j+dj0));
                                        poss[possCount][1] = OneSwap(pii(sx[i][j][di1][dj1][p1],sy[i][j][di1][dj1][p1]), pii(i+di1, j+dj1));
                                        possCount++;

                                        NextD2(state, D, mod8);
                                        return;
                                    }
                            }
                    }
                }
    }
}


class SquareRemover {
public:
    vector<int> playIt(int colors, vector<string> board, int startSeed);
};

vector<int> SquareRemover::playIt(int colors, vector<string> board, int startSeed)
{
#ifdef EXAMPLE
        colors = 6;
        if (startSeed == 857377961) n = 8;
        if (startSeed == 55004692) n = 9;
        if (startSeed == 257017653) n = 10;
        if (startSeed == 373353050) n = 11;
        if (startSeed == 104364742) n = 12;
        if (startSeed == 1329682683) n = 13;
        if (startSeed == 656773882) n = 14;
        if (startSeed == 1086656358) n = 15;
        if (startSeed == 1786703902) n = 16;
        if (startSeed == 45966541) { colors = 6; n = 16; }
        board = vector<string>(n, string(n, '0'));
        ll xt = startSeed;
        F0(i,n) F0(j,n) { board[i][j] = '0' + xt % colors; xt = xt * 1231 % MOD; }
#endif
        pN = 0;
        Init();
        n = board.size();
        C = colors;
        precompute_surround();

        VVI b(board);
        A[0] = startSeed;
        for (int i = 1; i < 10*N; i++) {
            A[i] = A[i-1] * MULT % MOD;
        }
        F0(i,10*N) A[i] %= C;

        State state0;
        state0.Initialize(b);

        //cf2 = CFS[C-MIN_COLORS][n-MIN_N];

#ifdef SUBMIT
        BW = BWS[C-MIN_COLORS][n-MIN_N];
        int each = 10;
        double tmp = 0;
#endif
        F0(i,S8) NextCount[i] = 0;
        NextCount[0] = 1;
        NextStates[0][0] = state0;
        F0(steps,N) {
#ifdef SUBMIT
            if (steps % each == 0) tmp = GetTime();
#endif
            NextCount[(steps+S7)&S7] = 0;
            int mod8 = steps & S7;
            State* C = NextStates[mod8];
            int cnt = NextCount[mod8];
            if (cnt == 0) continue;
            // Choose only best ones
            //cerr << BW << " " << cnt << " " << NextCount1[mod8] << " " << NextCount2[mod8] << endl;
            F0(i,cnt) { COSTS[i] = C[i].Cost(); ids[i] = i; }
            sort(ids, ids + cnt, cc); // nth element
            vector<int> usedIds;
            if (colors == 6)
            {
                for (int i = 0; i < cnt && SZ(usedIds) < BW; i++) {
                    bool good = true;
                    for (int j = SZ(usedIds)-1; j >= 0 && COSTS[usedIds[j]] == COSTS[ids[i]]; j--)
                    {
                        if (C[usedIds[j]].at == C[ids[i]].at && C[usedIds[j]].v == C[ids[i]].v) {
                            good = false; break;
                        }
                    }
                    if (good) {
                        usedIds.push_back(ids[i]);
                        GenNextStates6(C[ids[i]], steps & S7, SZ(usedIds) < BW / DIV6[n-8]);
                    }
                }
            }
            else if (colors == 5) {
                for (int i = 0; i < cnt && SZ(usedIds) < BW; i++) {
                    bool good = true;
                    for (int j = SZ(usedIds)-1; j >= 0 && COSTS[usedIds[j]] == COSTS[ids[i]]; j--)
                    {
                        if (C[usedIds[j]].at == C[ids[i]].at && C[usedIds[j]].v == C[ids[i]].v) {
                            good = false; break;
                        }
                    }
                    if (good) {
                        usedIds.push_back(ids[i]);
                        GenNextStates5(C[ids[i]], steps & S7, SZ(usedIds) < BW / 5);
                    }
                }
            } else
                for (int i = 0; i < cnt && SZ(usedIds) < BW; i++) {
                    bool good = true;
                    for (int j = SZ(usedIds)-1; j >= 0 && COSTS[usedIds[j]] == COSTS[ids[i]]; j--)
                    {
                        if (C[usedIds[j]].at == C[ids[i]].at && C[usedIds[j]].v == C[ids[i]].v) {
                            good = false; break;
                        }
                    }
                    if (good) {
                        usedIds.push_back(ids[i]);
                        GenNextStates(C[ids[i]], steps & S7);
                    }
                }

#ifdef SUBMIT
            // if (steps % 2000 == 0) cerr << steps << " " << BW << " " << pN << endl;
            if (steps % each == each - 1) {
                double th = GetTime() - tmp;
                // Start slow?
                //cerr << NextStates[steps%S8][0].at / 4 << " " << BW << endl;
                if (Elapsed() + th * (N-steps-1) / each > DEST && BW > 1) BW--;
                if (Elapsed() + th * (N-steps-1) / each < DEST) BW++;
                //cerr << steps+1 << "/" << N << "    " << GetTime() - tmp << endl;
            }
#endif
        }
        vector<int> res;
        int bi = 0;
        State* B = NextStates[0];
        F0(i,NextCount[0]) if (B[i].at > B[bi].at) bi = i;
        int at = B[bi].tindex;
        while (at != -1) {
            Move m = GetMoveByInt(track[at]);
            res.push_back(m.second);
            res.push_back(m.first.second);
            res.push_back(m.first.first);
            at = goback[at];
        }
        reverse(res.begin(), res.end());
        cerr << pN << endl;
        B[bi].PrintExtra();

        Report();
        cerr << "I think score is:               " << B[bi].at / 4 << endl;
#ifdef EXAMPLE
        return vector<int>(30000, 1);
#endif
        return res;
}
