// #define TIMERS
// #define RELEASE

const bool REMOVE_REPETITIONS = true;
const bool FINAL_STATS = false;
const bool BEAM_STATS = false;
const bool ADD_TRIPLE = true;
const bool USE_TIER2 = true;
const double TRIPLE_BONUS_COL[] = {0.0026, 0.0026, 0.0026};
const double FORCE_TIER2_COL[] = {0.025, 0.07, 0.13};
const double MIN_BEAMS_COL[] = {1.25, 1.25, 1.25};
const int FIXED_BEAMS = 0;
const int TIME_MULT = 1;
const int TRIPLE_VALUE = 90;
const int DOUBLE_VALUE = 17;

#ifdef LOCAL
const double TIME_LIMIT = 29.0 * TIME_MULT * 0.31;
#else
const double TIME_LIMIT = 29.0 * TIME_MULT;
#endif

#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <sys/time.h>
#include <unistd.h>
#include <emmintrin.h>

using namespace std;

#define INLINE   inline __attribute__ ((always_inline))
#define NOINLINE __attribute__ ((noinline))

#define ALIGNED __attribute__ ((aligned(16)))

#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

#define SSELOAD(a)      _mm_load_si128((__m128i*)&a)
#define SSESTORE(a, b)  _mm_store_si128((__m128i*)&a, b)
#define SSESTOREU(a, b) _mm_storeu_si128((__m128i*)&a, b)

#define FOR(i,a,b)  for(int i=(a);i<(b);++i)
#define REP(i,a)    FOR(i,0,a)
#define ZERO(m)     memset(m,0,sizeof(m))
#define MINUS(m)    memset(m,-1,sizeof(m))
#define ALL(x)      x.begin(),x.end()
#define PB          push_back
#define S           size()
#define LL          long long
#define ULL         unsigned long long
#define LD          long double
#define MP          make_pair
#define X           first
#define Y           second
#define VC          vector
#define PII         pair <int, int>
#define VI          VC < int >
#define VVI         VC < VI >
#define VVVI        VC < VVI >
#define VPII        VC < PII >
#define VD          VC < double >
#define VVD         VC < VD >
#define VS          VC < string >
#define VVS         VC < VS >
#define DB(a)       cerr << #a << ": " << (a) << endl;

template<class T> void print(VC < T > v) {cerr << "[";if (v.S) cerr << v[0];FOR(i, 1, v.S) cerr << ", " << v[i];cerr << "]" << endl;}
template<class T> string i2s(T x) {ostringstream o; o << x; return o.str();}
VS splt(string s, char c = ' ') {VS all; int p = 0, np; while (np = s.find(c, p), np >= 0) {if (np != p) all.PB(s.substr(p, np - p)); p = np + 1;} if (p < s.S) all.PB(s.substr(p)); return all;}

double getTime() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

ULL getTicks() {
#ifdef __i386
    ULL time;
    __asm__ volatile ("rdtsc" : "=A" (time));
    return time;
#else
    ULL timelo, timehi;
    __asm__ volatile ("rdtsc" : "=a" (timelo), "=d" (timehi));
    return (timehi << 32) + timelo;
#endif
}

double convertTicks(ULL time) {
#ifdef LOCAL
    return time / 3.3e9;
#else
    return time / 1.8e9;
#endif
}

struct Timer {
#ifdef TIMERS
    ULL ticks;
    string name;
    bool running;
    int count;
    int count2;

    Timer(string _name) : name(_name) {
        ticks = 0;
        count = 0;
        count2 = 0;
        running = false;
    }

    INLINE void clear() {
        ticks = 0;
        count = 0;
        count2 = 0;
        running = false;
    }

    INLINE void start() {
        if (running) return;
        ticks = getTicks() - ticks;
        running = true;
    }

    INLINE void stop() {
        if (!running) return;
        ticks = getTicks() - ticks;
        running = false;
        count++;
    }

    INLINE void stop(int add) {
        if (!running) return;
        ticks = getTicks() - ticks;
        running = false;
        count++;
        count2 += add;
    }

    INLINE double elapsed() {
        return convertTicks(running ? getTicks() - ticks : ticks);
    }

    void show() {
        if (count == 0 && count2 == 0) return;
        // cerr << name << ": " << elapsed() << " count: " << count;
		while (name.S < 20) name += " ";
        cerr << name << ": " << (elapsed() / TIME_LIMIT) << endl;
    }
#else
    Timer(string) { }
    double elapsed() { }
    void start() { }
    void stop() { }
    void stop(int add) { }
    void show() { }
#endif
};

struct RNG {
    unsigned int MT[624];
    int index;

    void init(int seed = 1) {
        MT[0] = seed;
        FOR(i, 1, 624) MT[i] = (1812433253UL * (MT[i-1] ^ (MT[i-1] >> 30)) + i);
        index = 0;
    }

    void generate() {
        const unsigned int MULT[] = {0, 2567483615UL};
        REP(i, 227) {
            unsigned int y = (MT[i] & 0x8000000UL) + (MT[i+1] & 0x7FFFFFFFUL);
            MT[i] = MT[i+397] ^ (y >> 1);
            MT[i] ^= MULT[y&1];
        }
        FOR(i, 227, 623) {
            unsigned int y = (MT[i] & 0x8000000UL) + (MT[i+1] & 0x7FFFFFFFUL);
            MT[i] = MT[i-227] ^ (y >> 1);
            MT[i] ^= MULT[y&1];
        }
        unsigned int y = (MT[623] & 0x8000000UL) + (MT[0] & 0x7FFFFFFFUL);
        MT[623] = MT[623-227] ^ (y >> 1);
        MT[623] ^= MULT[y&1];
    }

    unsigned int rand() {
        if (index == 0) {
            generate();
        }

        unsigned int y = MT[index];
        y ^= y >> 11;
        y ^= y << 7  & 2636928640UL;
        y ^= y << 15 & 4022730752UL;
        y ^= y >> 18;
        index = index == 623 ? 0 : index + 1;
        return y;
    }

    INLINE int next(int x) {
        return rand() % x;
    }

    INLINE int next(int a, int b) {
        return a + (rand() % (b - a));
    }

    INLINE double nextDouble() {
        return (rand() + 0.5) * (1.0 / 4294967296.0);
    }
};

static RNG rng;

Timer tGlobal("Global Time");
Timer tTier1("Tier1 Time");
Timer tTier2("Tier2 Time");
Timer tNth("Nth Element Time");
Timer tCopy("Copy Time");
Timer tRep("Remove Rep Time");
Timer tCreateBeam("Create Beam Time");
Timer tSim("Sim Time");
Timer tTriple("Triple Time");
Timer tTmp("Tmp Time");

int N;
int C;

const int MAX_TILES = 100000;
const int MAX_TURNS = 10000;
const int MAX_BEAMS = 2000 * TIME_MULT;
const int LOW_BEAMS = 20;
const int MAX_N = 20;
const int MAX_MOVES = MAX_N * MAX_N * 2;

const int XMOVE = 2;
const int YMOVE = 1;

const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};

typedef unsigned char BYTE;

int tiles[MAX_TILES] ALIGNED;

short boardBuffer[24][32] ALIGNED;
BYTE (*board)[32] = (BYTE(*)[32])&boardBuffer[4][0];

int score;

BYTE tboard[16][16] ALIGNED;
int tscore;

double timeStamp[MAX_TURNS];


VI moves;

struct State {
    BYTE b[16][16];
    short lineEv[16];
    int v;
    int fill0;
    int fill1;
    int fill2;
};

struct Move {
    short x;
    short y;
    short t;
    short x2;
    short y2;
    short t2;
    short l;
    Move() { }
    Move(short _x, short _y, short _t, short _l) : x(_x), y(_y), t(_t), t2(0), l(_l) { }
    Move(short _x, short _y, short _t, short _x2, short _y2, short _t2, short _l) : x(_x), y(_y), t(_t), x2(_x2), y2(_y2), t2(_t2), l(_l) { }
    bool operator < (const Move &m) const {
        return false;
    }
};


Move beamMove[MAX_TURNS + 10][MAX_BEAMS];
State prevBeam[MAX_BEAMS] ALIGNED;
State curBeam[MAX_BEAMS] ALIGNED;
State newBeam[MAX_BEAMS] ALIGNED;
pair < double, Move > nextBeam[MAX_BEAMS * MAX_MOVES];


INLINE void FASTCOPY(BYTE p0[16][32], BYTE p1[16][16]) {
    for (int i = 0; i < N; i += 2) {
        __m128i m0 = SSELOAD(p1[i+0][0]);
        __m128i m1 = SSELOAD(p1[i+1][0]);
        SSESTORE(p0[i+0][0], m0);
        SSESTORE(p0[i+1][0], m1);
    }
}

INLINE void FASTCOPY(BYTE p0[16][16], BYTE p1[16][16]) {
    for (int i = 0; i < N; i += 2) {
        __m128i m0 = SSELOAD(p1[i+0][0]);
        __m128i m1 = SSELOAD(p1[i+1][0]);
        SSESTORE(p0[i+0][0], m0);
        SSESTORE(p0[i+1][0], m1);
    }
}

INLINE void FASTCOPY(BYTE p0[16][16], BYTE p1[16][32]) {
    for (int i = 0; i < N; i += 2) {
        __m128i m0 = SSELOAD(p1[i+0][0]);
        __m128i m1 = SSELOAD(p1[i+1][0]);
        SSESTORE(p0[i+0][0], m0);
        SSESTORE(p0[i+1][0], m1);
    }
}

INLINE void FASTCOPY(BYTE p0[16][32], BYTE p1[16][32]) {
    for (int i = 0; i < N; i += 2) {
        __m128i m0 = SSELOAD(p1[i+0][0]);
        __m128i m1 = SSELOAD(p1[i+1][0]);
        SSESTORE(p0[i+0][0], m0);
        SSESTORE(p0[i+1][0], m1);
    }
}

INLINE void FASTCOPY(short p0[16], short p1[16]) {
    __m128i m0 = SSELOAD(p1[0]);
    __m128i m1 = SSELOAD(p1[8]);
    SSESTORE(p0[0], m0);
    SSESTORE(p0[8], m1);
}

INLINE void saveBoard() {
    FASTCOPY(tboard, board);
    tscore = score;
}

INLINE void loadBoard() {
    FASTCOPY(board, tboard);
    score = tscore;
}

int simX1, simX2;

int gSimX1, gSimX2;
INLINE void saveSimData() {
    gSimX1 = simX1;
    gSimX2 = simX2;
}

INLINE void addSimData() {
    gSimX1 = min(gSimX1, simX1);
    gSimX2 = max(gSimX2, simX2);
}

INLINE void loadBoardRel() {
    FOR(i, simX1, simX2+1) {
        __m128i m0 = SSELOAD(tboard[i][0]);
        SSESTORE(board[i][0], m0);
    }
    score = tscore;
}


INLINE void loadBoardGRel() {
    FOR(i, gSimX1, gSimX2+1) {
        __m128i m0 = SSELOAD(tboard[i][0]);
        SSESTORE(board[i][0], m0);
    }
    score = tscore;
}

VI simScore(20);

template <int t> void simulate(int x, int y) {
    tSim.start();
    if (t == YMOVE) {
        swap(board[x][y], board[x][y+1]);
        simX1 = x; simX2 = x;
    } else if (t == XMOVE) {
        swap(board[x][y], board[x+1][y]);
        simX1 = x; simX2 = x+1;
    } else {
        simX1 = 0; simX2 = N;
    }

    int matches = 0;
    while (true) {
        FOR(tx, max(simX1 - 1, 0), simX2 + 1) {
            __m128i mRow0 = SSELOAD(board[tx+0][0]);
            __m128i mRow1 = SSELOAD(board[tx+1][0]);
            __m128i mAnd0 = _mm_and_si128(mRow0, _mm_srli_si128(mRow0, 1));
            __m128i mAnd1 = _mm_and_si128(mRow1, _mm_srli_si128(mRow1, 1));
            __m128i mValid = _mm_and_si128(mAnd0, mAnd1);

            __m128i mCmp = _mm_cmpgt_epi8(mValid, _mm_setzero_si128());

            int mask = _mm_movemask_epi8(mCmp);

            if (mask == 0) continue;

            int ty = __builtin_ffs(mask) - 1;

            board[tx+0][ty+0] = tiles[4*score+0];
            board[tx+0][ty+1] = tiles[4*score+1];
            board[tx+1][ty+0] = tiles[4*score+2];
            board[tx+1][ty+1] = tiles[4*score+3];

            score++;
            simX1 = min(simX1, tx);
            simX2 = max(simX2, tx + 1);
            if (BEAM_STATS) matches++;
            tx -= 2;

        }

        break;
    }
    if (BEAM_STATS) simScore[matches]++;
    tSim.stop();
}

void simulate(int x, int y, int t) {
    if (t == XMOVE) {
        simulate<XMOVE>(x, y);
    } else if (t == YMOVE) {
        simulate<YMOVE>(x, y);
    } else {
        simulate<0>(x, y);
    }
}

INLINE int calc8BitSum(__m128i &m0) {
    __m128i m0X = _mm_srli_si128(m0, 1);
    __m128i m1  = _mm_add_epi8(m0, m0X);
    __m128i m1X = _mm_srli_si128(m1, 2);
    __m128i m2  = _mm_add_epi8(m1, m1X);
    __m128i m2X = _mm_srli_si128(m2, 4);
    __m128i m3  = _mm_add_epi8(m2, m2X);
    __m128i m3X = _mm_srli_si128(m2, 8);
    __m128i m4  = _mm_add_epi8(m3, m3X);
    return _mm_extract_epi16(m4, 0) & 255;
}

INLINE int calcEvScore(int x) {
    __m128i mRow00 = SSELOAD(board[x+0][0]);
    __m128i mRow10 = SSELOAD(board[x+1][0]);

    __m128i mRow01 = _mm_srli_si128(mRow00, 1);
    __m128i mRow11 = _mm_srli_si128(mRow10, 1);

    __m128i mAnd0 = _mm_and_si128(mRow00, mRow01);
    __m128i mAnd1 = _mm_and_si128(mRow10, mRow11);

    __m128i mDouble0 = mAnd0;
    __m128i mDouble1 = _mm_and_si128(mRow00, mRow10);

    __m128i mTriple00 = _mm_and_si128(mAnd1, mRow01);
    __m128i mTriple01 = _mm_and_si128(mAnd1, mRow00);
    __m128i mTriple10 = _mm_and_si128(mAnd0, mRow11);
    __m128i mTriple11 = _mm_and_si128(mAnd0, mRow10);

    __m128i mDouble = _mm_or_si128(mDouble0, mDouble1);

    __m128i mTriple0 = _mm_or_si128(mTriple00, mTriple01);
    __m128i mTriple1 = _mm_or_si128(mTriple10, mTriple11);
    __m128i mTriple  = _mm_or_si128(mTriple0, mTriple1);

    const __m128i mOne = _mm_set1_epi8(1);

    __m128i mDoubleSum = _mm_add_epi8(_mm_min_epu8(mDouble0, mOne), _mm_min_epu8(mDouble1, mOne));
    __m128i mTripleSum = _mm_min_epu8(mTriple, mOne);

    return calc8BitSum(mDoubleSum) * DOUBLE_VALUE + calc8BitSum(mTripleSum) * TRIPLE_VALUE;
}

INLINE int calcEvScore(int x0, int x1) {
    __m128i mDoubleTotal = _mm_setzero_si128();
    __m128i mTripleTotal = _mm_setzero_si128();

    FOR(x, x0, x1) {
        __m128i mRow00 = SSELOAD(board[x+0][0]);
        __m128i mRow10 = SSELOAD(board[x+1][0]);

        __m128i mRow01 = _mm_srli_si128(mRow00, 1);
        __m128i mRow11 = _mm_srli_si128(mRow10, 1);

        __m128i mAnd0 = _mm_and_si128(mRow00, mRow01);
        __m128i mAnd1 = _mm_and_si128(mRow10, mRow11);

        __m128i mDouble0 = mAnd0;
        __m128i mDouble1 = _mm_and_si128(mRow00, mRow10);

        __m128i mTriple00 = _mm_and_si128(mAnd1, mRow01);
        __m128i mTriple01 = _mm_and_si128(mAnd1, mRow00);
        __m128i mTriple10 = _mm_and_si128(mAnd0, mRow11);
        __m128i mTriple11 = _mm_and_si128(mAnd0, mRow10);

        __m128i mDouble = _mm_or_si128(mDouble0, mDouble1);

        __m128i mTriple0 = _mm_or_si128(mTriple00, mTriple01);
        __m128i mTriple1 = _mm_or_si128(mTriple10, mTriple11);
        __m128i mTriple  = _mm_or_si128(mTriple0, mTriple1);

        const __m128i mOne = _mm_set1_epi8(1);

        __m128i mDoubleSum = _mm_add_epi8(_mm_min_epu8(mDouble0, mOne), _mm_min_epu8(mDouble1, mOne));
        __m128i mTripleSum = _mm_min_epu8(mTriple, mOne);

        mDoubleTotal = _mm_add_epi8(mDoubleTotal, mDoubleSum);
        mTripleTotal = _mm_add_epi8(mTripleTotal, mTripleSum);
    }

    return calc8BitSum(mDoubleTotal) * DOUBLE_VALUE + calc8BitSum(mTripleTotal) * TRIPLE_VALUE;
}

void show(BYTE b[16][16]) {
    REP(i, N) {
        REP(j, N) {
            int x = 0;
            while ((1 << x) != b[i][j]) x++;
            cerr << x;
        }
        cerr << endl;
    }
    cerr << endl;
}

void show(BYTE b[16][32]) {
    REP(i, N) {
        REP(j, N) {
            int x = 0;
            while ((1 << x) != b[i][j]) x++;
            cerr << x;
        }
        cerr << endl;
    }
    cerr << endl;
}

class SquareRemover {
public:
    VI playIt(int colors, VS _board, int seed);
};

VI SquareRemover::playIt(int colors, VS _board, int seed) {
        tGlobal.start();

        rng.init(1);
        double startTime = getTime();

        C = colors;
        N = _board.S;

        const double TRIPLE_BONUS = TRIPLE_BONUS_COL[C - 4];
        const double FORCE_TIER2 = FORCE_TIER2_COL[C - 4];
        const double MIN_BEAMS = MIN_BEAMS_COL[C - 4];

        tiles[0] = seed;
        FOR(i, 1, MAX_TILES) tiles[i] = (tiles[i-1] * 48271LL) % 2147483647LL;
        REP(i, MAX_TILES) tiles[i] = 1 << (tiles[i] % C);

        REP(i, N) REP(j, N) board[i][j] = 1 << (_board[i][j] - '0');

        int turn = 0;
        score = 0;
        moves.clear();

        int zeros = 0;

        simulate(0, 0, 0);

        int startingBeams[] = {1500, 1260, 1100, 960, 830, 750, 680, 600, 530};
        int maxBeams = FIXED_BEAMS ? FIXED_BEAMS : (int)(startingBeams[N-8] * (C == 4 ? 1 : 1.15));
        int beamsNo = 1;
        curBeam[0].v = score;
        REP(i, N) REP(j, N) curBeam[0].b[i][j] = board[i][j];
        REP(i, N) curBeam[0].lineEv[i] = calcEvScore(i);

        int sumBeams = 0;
        int repBeams = 0;
        int totBeams = 0;
        int tot2Beams = 0;
        int beamShortage = 0;
        VI diffBeam(20);

        int oldBeamsNo = 0;
        while (true) {
            if (turn >= MAX_TURNS) break;

            if (beamsNo == 0 && oldBeamsNo == 0) {
                DB(turn);
                break;
                // exit(1);
            }

            int newBeamsNo = 0;
            int maxScore = 0;
            REP(beam, beamsNo) maxScore = max(maxScore, curBeam[beam].v);

            sumBeams += beamsNo;

            tTier1.start();
            REP(beam, beamsNo) {
                FASTCOPY(board, curBeam[beam].b);
                score = curBeam[beam].v;
                saveBoard();

                if (BEAM_STATS) diffBeam[maxScore - curBeam[beam].v]++;

                static short lineEvTop[16] ALIGNED = {0};
                static short lineEvBot[16] ALIGNED = {0};
                for (int i = 2; i < N; i++)
                    lineEvTop[i] = curBeam[beam].lineEv[i-2] + lineEvTop[i-1];
                for (int i = N-2; i >= 0; i--)
                    lineEvBot[i] = curBeam[beam].lineEv[i+1] + lineEvBot[i+1];

                __m128i mRow0;
                __m128i mRow1 = SSELOAD(board[-1][0]);
                __m128i mRow2 = SSELOAD(board[0][0]);
                __m128i mRow3 = SSELOAD(board[1][0]);

                REP(x, N - 1) {
                    mRow0 = mRow1;
                    mRow1 = mRow2;
                    mRow2 = mRow3;
                    mRow3 = SSELOAD(board[x+2][0]);

                    __m128i mAnd0 = _mm_and_si128(mRow0, _mm_srli_si128(mRow0, 1));
                    __m128i mAnd3 = _mm_and_si128(mRow3, _mm_srli_si128(mRow3, 1));

                    __m128i mValid0 = _mm_and_si128(_mm_slli_si128(mAnd0, 1), _mm_and_si128(_mm_slli_si128(mRow1, 1), mRow2));
                    __m128i mValid1 = _mm_and_si128(mAnd0, _mm_and_si128(_mm_srli_si128(mRow1, 1), mRow2));
                    __m128i mValid2 = _mm_and_si128(_mm_slli_si128(mAnd3, 1), _mm_and_si128(_mm_slli_si128(mRow2, 1), mRow1));
                    __m128i mValid3 = _mm_and_si128(mAnd3, _mm_and_si128(_mm_srli_si128(mRow2, 1), mRow1));

                    __m128i mValid4 = _mm_or_si128(mValid0, mValid1);
                    __m128i mValid5 = _mm_or_si128(mValid2, mValid3);
                    __m128i mValid  = _mm_or_si128(mValid4, mValid5);

                    __m128i mCmp = _mm_cmpgt_epi8(mValid, _mm_setzero_si128());

                    int mask = _mm_movemask_epi8(mCmp);

                    while (mask) {
                        int y = __builtin_ffs(mask) - 1;
                        mask &= mask - 1;

                        tTmp.start();
                        simulate<XMOVE>(x, y);
                        int bonus;
                        if (ADD_TRIPLE) {
                            tTriple.start();
                            bonus = lineEvTop[simX1] + lineEvBot[simX2];
                            bonus += calcEvScore(max(0, simX1 - 1), simX2 + 1);
                            tTriple.stop();
                        }
                        nextBeam[newBeamsNo++] = MP(score + bonus * TRIPLE_BONUS, Move(x, y, XMOVE, beam));
                        loadBoardRel();
                        tTmp.stop();
                    }
                }

                mRow1 = SSELOAD(board[-1][0]);
                mRow2 = SSELOAD(board[0][0]);

                REP(x, N) {
                    mRow0 = mRow1;
                    mRow1 = mRow2;
                    mRow2 = SSELOAD(board[x+1][0]);
                    __m128i mAnd0 = _mm_and_si128(mRow0, _mm_slli_si128(mRow0, 1));
                    __m128i mAnd1 = _mm_and_si128(mRow1, _mm_srli_si128(mRow1, 2));
                    __m128i mAnd2 = _mm_and_si128(mRow2, _mm_slli_si128(mRow2, 1));

                    __m128i mValid0 = _mm_and_si128(mAnd0, _mm_slli_si128(mAnd1, 1));
                    __m128i mValid1 = _mm_and_si128(mAnd2, _mm_slli_si128(mAnd1, 1));
                    __m128i mValid2 = _mm_and_si128(_mm_srli_si128(mAnd0, 2), mAnd1);
                    __m128i mValid3 = _mm_and_si128(_mm_srli_si128(mAnd2, 2), mAnd1);

                    __m128i mValid4 = _mm_or_si128(mValid0, mValid1);
                    __m128i mValid5 = _mm_or_si128(mValid2, mValid3);
                    __m128i mValid  = _mm_or_si128(mValid4, mValid5);

                    __m128i mCmp = _mm_cmpgt_epi8(mValid, _mm_setzero_si128());

                    int mask = _mm_movemask_epi8(mCmp);

                    while (mask) {
                        int y = __builtin_ffs(mask) - 1;
                        mask &= mask - 1;

                        tTmp.start();
                        simulate<YMOVE>(x, y);
                        int bonus = 0;
                        if (ADD_TRIPLE) {
                            tTriple.start();
                            bonus = lineEvTop[simX1] + lineEvBot[simX2];
                            bonus += calcEvScore(max(0, simX1 - 1), simX2 + 1);
                            tTriple.stop();
                        }
                        nextBeam[newBeamsNo++] = MP(score + bonus * TRIPLE_BONUS, Move(x, y, YMOVE, beam));
                        loadBoardRel();
                        tTmp.stop();
                    }

                }

            }
            tTier1.stop();

            tTier2.start();
            if (USE_TIER2) {
                int tier1Beams = newBeamsNo;

                REP(beam, oldBeamsNo) {

                    if (newBeamsNo > MIN_BEAMS * maxBeams && beam > oldBeamsNo * FORCE_TIER2) break;

                    FASTCOPY(board, prevBeam[beam].b);
                    score = prevBeam[beam].v;
                    saveBoard();

                    static short lineEvTopT2[16] ALIGNED = {0};
                    static short lineEvBotT2[16] ALIGNED = {0};
                    for (int i = 2; i < N; i++)
                        lineEvTopT2[i] = prevBeam[beam].lineEv[i-2] + lineEvTopT2[i-1];
                    for (int i = N-2; i >= 0; i--)
                        lineEvBotT2[i] = prevBeam[beam].lineEv[i+1] + lineEvBotT2[i+1];

#define ADDMOVE(x1, y1, t1, x2, y2, t2) \
	{ \
		simulate<t1>(x1, y1); \
		if (tscore == score) { \
			saveSimData(); \
			simulate<t2>(x2, y2); \
			addSimData(); \
			int bonus = 0; \
			if (ADD_TRIPLE) { \
				tTriple.start(); \
				bonus = lineEvTopT2[gSimX1] + lineEvBotT2[gSimX2]; \
				bonus += calcEvScore(max(0, gSimX1 - 1), gSimX2 + 1); \
				tTriple.stop(); \
			} \
			nextBeam[newBeamsNo++] = MP(score + bonus * TRIPLE_BONUS, Move(x1, y1, t1, x2, y2, t2, beam)); \
			loadBoardGRel(); \
		} else { \
			loadBoardRel(); \
		} \
	}

                    REP(x, N - 1) REP(y, N - 1) {

                            if (board[x][y] == board[x][y+1] && board[x][y] == board[x+1][y] && board[x][y] != board[x+1][y+2] && board[x][y] != board[x+2][y+1]) {
                                if (board[x][y] == board[x+2][y+2]) {
                                    ADDMOVE(x+2,y+1,YMOVE,x+1,y+1,XMOVE);
                                    // ADDMOVE(x+1,y+2,XMOVE,x+1,y+1,YMOVE);
                                }  if (board[x][y] == board[x+3][y+1]) {
                                    ADDMOVE(x+2,y+1,XMOVE,x+1,y+1,XMOVE);
                                }  if (board[x][y] == board[x+1][y+3]) {
                                    ADDMOVE(x+1,y+2,YMOVE,x+1,y+1,YMOVE);
                                }  if (board[x][y] == board[x+2][y]) {
                                    ADDMOVE(x+1,y,YMOVE,x+1,y,XMOVE);
                                    // ADDMOVE(x+2,y,YMOVE,x+1,y+1,XMOVE);
                                }  if (board[x][y] == board[x][y+2]) {
                                    ADDMOVE(x,y+1,XMOVE,x,y+1,YMOVE);
                                    // ADDMOVE(x,y+2,XMOVE,x+1,y+1,YMOVE);
                                }  if (board[x][y] == board[x-1][y+1]) {
                                    ADDMOVE(x,y+1,XMOVE,x-1,y+1,XMOVE);
                                }  if (board[x][y] == board[x+1][y-1]) {
                                    ADDMOVE(x+1,y,YMOVE,x+1,y-1,YMOVE);
                                }
                            }

                            if (board[x][y] == board[x][y+1] && board[x][y] == board[x+1][y+1] && board[x][y] != board[x+2][y] && board[x][y] != board[x+1][y-1]) {
                                if (board[x][y] == board[x-1][y]) {
                                    ADDMOVE(x,y,XMOVE,x-1,y,XMOVE);
                                }  if (board[x][y] == board[x][y-1]) {
                                    ADDMOVE(x,y-1,XMOVE,x+1,y-1,YMOVE);
                                    // ADDMOVE(x,y,XMOVE,x,y-1,YMOVE);
                                }  if (board[x][y] == board[x+1][y-2]) {
                                    ADDMOVE(x+1,y-2,YMOVE,x+1,y-1,YMOVE);
                                }  if (board[x][y] == board[x+2][y-1]) {
                                    ADDMOVE(x+1,y-1,XMOVE,x+1,y-1,YMOVE);
                                    // ADDMOVE(x+2,y-1,YMOVE,x+1,y,XMOVE);
                                }  if (board[x][y] == board[x+3][y]) {
                                    ADDMOVE(x+2,y,XMOVE,x+1,y,XMOVE);
                                }  if (board[x][y] == board[x+2][y+1]) {
                                    ADDMOVE(x+2,y,YMOVE,x+1,y,XMOVE);
                                    // ADDMOVE(x+1,y,YMOVE,x+1,y+1,XMOVE);
                                }  if (board[x][y] == board[x+1][y+2]) {
                                    ADDMOVE(x+1,y,YMOVE,x+1,y+1,YMOVE);
                                }
                            }

                            if (board[x+1][y] == board[x][y+1] && board[x+1][y] == board[x+1][y+1] && board[x+1][y] != board[x-1][y] && board[x+1][y] != board[x][y-1]) {
                                if (board[x+1][y] == board[x+2][y]) {
                                    ADDMOVE(x,y,XMOVE,x+1,y,XMOVE);
                                }  if (board[x+1][y] == board[x+1][y-1]) {
                                    ADDMOVE(x,y-1,XMOVE,x,y-1,YMOVE);
                                }  if (board[x+1][y] == board[x][y-2]) {
                                    ADDMOVE(x,y-2,YMOVE,x,y-1,YMOVE);
                                }  if (board[x+1][y] == board[x-1][y-1]) {
                                    ADDMOVE(x-1,y-1,XMOVE,x,y-1,YMOVE);
                                }  if (board[x+1][y] == board[x-2][y]) {
                                    ADDMOVE(x-2,y,XMOVE,x-1,y,XMOVE);
                                }  if (board[x+1][y] == board[x-1][y+1]) {
                                    ADDMOVE(x-1,y,YMOVE,x-1,y,XMOVE);
                                }  if (board[x+1][y] == board[x][y+2]) {
                                    ADDMOVE(x,y,YMOVE,x,y+1,YMOVE);
                                }
                            }

                            if (board[x][y] == board[x+1][y] && board[x][y] == board[x+1][y+1] && board[x][y] != board[x][y+2] && board[x][y] != board[x-1][y+1]) {
                                if (board[x][y] == board[x][y-1]) {
                                    ADDMOVE(x,y,YMOVE,x,y-1,YMOVE);
                                }  if (board[x][y] == board[x-1][y]) {
                                    ADDMOVE(x-1,y,YMOVE,x-1,y+1,XMOVE);
                                }  if (board[x][y] == board[x-2][y+1]) {
                                    ADDMOVE(x-2,y+1,XMOVE,x-1,y+1,XMOVE);
                                }  if (y < N - 2 && board[x][y] == board[x-1][y+2]) {
                                    ADDMOVE(x-1,y+1,YMOVE,x-1,y+1,XMOVE);
                                }  if (board[x][y] == board[x][y+3]) {
                                    ADDMOVE(x,y+2,YMOVE,x,y+1,YMOVE);
                                }  if (board[x][y] == board[x+1][y+2]) {
                                    ADDMOVE(x,y+2,XMOVE,x,y+1,YMOVE);
                                }  if (board[x][y] == board[x+2][y+1]) {
                                    ADDMOVE(x,y+1,XMOVE,x+1,y+1,XMOVE);
                                }
                            }

                            if (board[x][y] == board[x+1][y+1] && board[x][y] != board[x+1][x] && board[x][y] != board[x][y+1]) {
                                bool v0 = board[x][y] == board[x+2][y];
                                bool v1 = board[x][y] == board[x+1][y-1];
                                bool v2 = board[x][y] == board[x][y+2];
                                bool v3 = board[x][y] == board[x-1][y+1];
                                if (v0) {
                                    if (v2) ADDMOVE(x+1,y,XMOVE,x,y+1,YMOVE);
                                    if (v3) ADDMOVE(x+1,y,XMOVE,x-1,y+1,XMOVE);
                                }
                                if (v1) {
                                    if (v2) ADDMOVE(x+1,y-1,YMOVE,x,y+1,YMOVE);
                                    if (v3) ADDMOVE(x+1,y-1,YMOVE,x-1,y+1,XMOVE);
                                }
                            }

                            if (board[x+1][y] == board[x][y+1] && board[x+1][y] != board[x][y] && board[x+1][y] != board[x+1][y+1]) {
                                bool v0 = board[x+1][y] == board[x-1][y];
                                bool v1 = board[x+1][y] == board[x][y-1];
                                bool v2 = board[x+1][y] == board[x+1][y+2];
                                bool v3 = board[x+1][y] == board[x+2][y+1];
                                if (v0) {
                                    if (v2) ADDMOVE(x-1,y,XMOVE,x+1,y+1,YMOVE);
                                    if (v3) ADDMOVE(x-1,y,XMOVE,x+1,y+1,XMOVE);
                                }
                                if (v1) {
                                    if (v2) ADDMOVE(x,y-1,YMOVE,x+1,y+1,YMOVE);
                                    if (v3) ADDMOVE(x,y-1,YMOVE,x+1,y+1,XMOVE);
                                }
                            }

                            if (board[x][y] == board[x][y+1] && board[x][y] != board[x+1][y] && board[x][y] != board[x+1][y+1]) {
                                bool v0 = board[x][y] == board[x+1][y-1];
                                bool v1 = board[x][y] == board[x+2][y];
                                bool v2 = board[x][y] == board[x+1][y+2];
                                bool v3 = board[x][y] == board[x+2][y+1];
                                if (v0) {
                                    if (v2) ADDMOVE(x+1,y-1,YMOVE,x+1,y+1,YMOVE);
                                    if (v3) ADDMOVE(x+1,y-1,YMOVE,x+1,y+1,XMOVE);
                                }
                                if (v1) {
                                    if (v2) ADDMOVE(x+1,y,XMOVE,x+1,y+1,YMOVE);
                                    if (v3) ADDMOVE(x+1,y,XMOVE,x+1,y+1,XMOVE);
                                }
                            }

                            if (board[x+1][y] == board[x+1][y+1] && board[x+1][y] != board[x][y] && board[x+1][y] != board[x][y+1]) {
                                bool v0 = board[x+1][y] == board[x-1][y];
                                bool v1 = board[x+1][y] == board[x][y-1];
                                bool v2 = board[x+1][y] == board[x-1][y+1];
                                bool v3 = board[x+1][y] == board[x][y+2];
                                if (v0) {
                                    if (v2) ADDMOVE(x-1,y,XMOVE,x-1,y+1,XMOVE);
                                    if (v3) ADDMOVE(x-1,y,XMOVE,x,y+1,YMOVE);
                                }
                                if (v1) {
                                    if (v2) ADDMOVE(x,y-1,YMOVE,x-1,y+1,XMOVE);
                                    if (v3) ADDMOVE(x,y-1,YMOVE,x,y+1,YMOVE);
                                }
                            }

                            if (board[x][y+1] == board[x+1][y+1] && board[x][y+1] != board[x][y] && board[x][y+1] != board[x+1][y]) {
                                bool v0 = board[x][y+1] == board[x-1][y];
                                bool v1 = board[x][y+1] == board[x][y-1];
                                bool v2 = board[x][y+1] == board[x+2][y];
                                bool v3 = board[x][y+1] == board[x+1][y-1];
                                if (v0) {
                                    if (v2) ADDMOVE(x-1,y,XMOVE,x+1,y,XMOVE);
                                    if (v3) ADDMOVE(x-1,y,XMOVE,x+1,y-1,YMOVE);
                                }
                                if (v1) {
                                    if (v2) ADDMOVE(x,y-1,YMOVE,x+1,y,XMOVE);
                                    if (v3) ADDMOVE(x,y-1,YMOVE,x+1,y-1,YMOVE);
                                }
                            }

                            if (board[x][y] == board[x+1][y] && board[x][y] != board[x][y+1] && board[x][y] != board[x+1][y+1]) {
                                bool v0 = board[x][y] == board[x-1][y+1];
                                bool v1 = board[x][y] == board[x][y+2];
                                bool v2 = board[x][y] == board[x+2][y+1];
                                bool v3 = board[x][y] == board[x+1][y+2];
                                if (v0) {
                                    if (v2) ADDMOVE(x-1,y+1,XMOVE,x+1,y+1,XMOVE);
                                    if (v3) ADDMOVE(x-1,y+1,XMOVE,x+1,y+1,YMOVE);
                                }
                                if (v1) {
                                    if (v2) ADDMOVE(x,y+1,YMOVE,x+1,y+1,XMOVE);
                                    if (v3) ADDMOVE(x,y+1,YMOVE,x+1,y+1,YMOVE);
                                }
                            }
                        }

                }

                tot2Beams += newBeamsNo - tier1Beams;

            }
            tTier2.stop();

            while (newBeamsNo < LOW_BEAMS) {
                int beam = rng.next(beamsNo);

                FASTCOPY(board, curBeam[beam].b);
                score = curBeam[beam].v;
                saveBoard();

                while (true) {
                    int x = rng.next(N);
                    int y = rng.next(N);
                    int t = rng.next(1, 3);

                    if (x == N - 1 && t == XMOVE || y == N - 1 && t == YMOVE) continue;
                    if (board[x][y] == board[x+dx[t]][y+dy[t]]) continue;

                    simulate(t, x, y);
                    int bonus = 0;
                    if (ADD_TRIPLE) {
                        tTriple.start();
                        REP(x, max(0, simX1 - 1)) bonus += curBeam[beam].lineEv[x];
                        bonus += calcEvScore(max(0, simX1 - 1), simX2 + 1);
                        FOR(x, simX2 + 1, N) bonus += curBeam[beam].lineEv[x];
                        tTriple.stop();
                    }
                    nextBeam[newBeamsNo++] = MP(score + bonus * TRIPLE_BONUS, Move(x, y, t, beam));
                    break;
                }

            }

            timeStamp[turn] = (getTime() - startTime) / TIME_LIMIT;
            if (turn % 100 == 0 && turn) {
                maxBeams = FIXED_BEAMS ? FIXED_BEAMS : max(LOW_BEAMS, min(MAX_BEAMS, (int)(maxBeams * (1.0 - timeStamp[turn]) / (100 - turn / 100) / (timeStamp[turn] - timeStamp[turn - 100]))));
                // cerr << turn << ' ' << maxBeams << endl;
            }

            totBeams += newBeamsNo;
            beamShortage += max(0, maxBeams - beamsNo);

            oldBeamsNo = beamsNo;
            beamsNo = min(newBeamsNo, maxBeams);

            tNth.start();
            nth_element(nextBeam, nextBeam + beamsNo, nextBeam + newBeamsNo, greater<pair<double,Move>>());
            nth_element(nextBeam, nextBeam + (int)(FORCE_TIER2 * beamsNo), nextBeam + beamsNo, greater<pair<double,Move>>());
            tNth.stop();

            tCreateBeam.start();
            turn++;
            REP(i, beamsNo) {
                beamMove[turn][i] = nextBeam[i].Y;
                if (beamMove[turn][i].t2) {
                    FASTCOPY(board, prevBeam[beamMove[turn][i].l].b);
                    score = prevBeam[beamMove[turn][i].l].v;
                    simulate(beamMove[turn][i].x , beamMove[turn][i].y , beamMove[turn][i].t);
                    saveSimData();
                    simulate(beamMove[turn][i].x2, beamMove[turn][i].y2, beamMove[turn][i].t2);
                    addSimData();
                    FASTCOPY(newBeam[i].b, board);
                    FASTCOPY(newBeam[i].lineEv, prevBeam[beamMove[turn][i].l].lineEv);
                    FOR(x, max(0, gSimX1 - 1), gSimX2 + 1) newBeam[i].lineEv[x] = calcEvScore(x);
                    newBeam[i].v = score;
                } else {
                    FASTCOPY(board, curBeam[beamMove[turn][i].l].b);
                    score = curBeam[beamMove[turn][i].l].v;
                    simulate(beamMove[turn][i].x, beamMove[turn][i].y, beamMove[turn][i].t);
                    FASTCOPY(newBeam[i].b, board);
                    FASTCOPY(newBeam[i].lineEv, curBeam[beamMove[turn][i].l].lineEv);
                    FOR(x, max(0, simX1 - 1), simX2 + 1) newBeam[i].lineEv[x] = calcEvScore(x);
                    newBeam[i].v = score;
                }
            }
            tCreateBeam.stop();

            tCopy.start();
            if (USE_TIER2) {
                REP(i, oldBeamsNo) {
                    prevBeam[i].v = curBeam[i].v;
                    FASTCOPY(prevBeam[i].b, curBeam[i].b);
                    FASTCOPY(prevBeam[i].lineEv, curBeam[i].lineEv);
                }
            }

            REP(i, beamsNo) {
                curBeam[i].v = newBeam[i].v;
                FASTCOPY(curBeam[i].b, newBeam[i].b);
                FASTCOPY(curBeam[i].lineEv, newBeam[i].lineEv);
            }
            tCopy.stop();

            if (REMOVE_REPETITIONS && turn % 10 == 0) {
                tRep.start();
                unordered_set<unsigned int> ss;
                REP(i, beamsNo) {
                    unsigned int x = 0;
                    REP(j,N) REP(k,N) x = 7 * x + curBeam[i].b[j][k];
                    if (ss.count(x)) {
                        beamsNo--;
                        swap(curBeam[i], curBeam[beamsNo]);
                        swap(beamMove[turn][i], beamMove[turn][beamsNo]);
                        i--;
                        repBeams++;
                    } else {
                        ss.insert(x);
                    }
                }
                tRep.stop();
            }

        }

        cerr << "N: " << N << " C: " << C << endl;

        int bp = 0;
        int tot2 = 0;
        VI moves(30000);
        for (int turn = 10000; turn; turn--) {
            if (beamMove[turn][bp].t2) {
                tot2++;
                moves[turn * 3 - 6] = beamMove[turn][bp].x;
                moves[turn * 3 - 5] = beamMove[turn][bp].y;
                moves[turn * 3 - 4] = beamMove[turn][bp].t;
                moves[turn * 3 - 3] = beamMove[turn][bp].x2;
                moves[turn * 3 - 2] = beamMove[turn][bp].y2;
                moves[turn * 3 - 1] = beamMove[turn][bp].t2;
                bp = beamMove[turn][bp].l;
                turn--;
            } else {
                moves[turn * 3 - 3] = beamMove[turn][bp].x;
                moves[turn * 3 - 2] = beamMove[turn][bp].y;
                moves[turn * 3 - 1] = beamMove[turn][bp].t;
                bp = beamMove[turn][bp].l;
            }
        }

        REP(i, beamsNo) if (curBeam[i].v > curBeam[bp].v) bp = i;
        cerr << "Total Score: " << curBeam[bp].v << endl;
        cerr << "AvgBeams: " << (sumBeams * 1. / MAX_TURNS) << endl;
        cerr << "AvgMoves: " << (totBeams * 1. / MAX_TURNS) << ' ' << (tot2Beams * 1. / MAX_TURNS) << endl;
        if (BEAM_STATS) {
            cerr << "Shortage: " << (beamShortage * 1. / MAX_TURNS) << endl;
            cerr << "Tier2 Moves: " << tot2 << endl;
            while (diffBeam.S && diffBeam.back() == 0) diffBeam.pop_back();
            while (simScore.S && simScore.back() == 0) simScore.pop_back();
            cerr << "diffBeam: "; print(diffBeam);
            cerr << "simScore: "; print(simScore);
        }

        tGlobal.stop();

        if (FINAL_STATS) {
            score = 0;
            REP(i, N) REP(j, N) board[i][j] = 1 << (_board[i][j] - '0');
            simulate(0, 0, 0);
            VI moveScore(20);
            VI moveGap(20);
            VI bestMove(20);
            VD movesNo(20);
            int lastimp = -1;
            REP(turn, MAX_TURNS) {
                int lscore = score;

                VPII vp[4];
                int used[16*16];
                int pused[4];
                int minMove = 1 << 20;
                REP(x, N-1) REP(y, N-1) REP(c, C) {
                            int no = 0;
                            no += board[x][y] == c;
                            no += board[x+1][y] == c;
                            no += board[x][y+1] == c;
                            no += board[x+1][y+1] == c;
                            if (no < 2) continue;
                            assert(no < 4);

                            REP(i, 4) vp[i].clear();

                            const int BM_DIST = 3;
                            FOR(i, max(0, x - BM_DIST), min(N, x + BM_DIST + 2))
                                FOR(j, max(0, y - BM_DIST), min(N, y + BM_DIST + 2))
                                    if (board[i][j] == c)
                                        REP(d0, 2) REP(d1, 2) vp[d0*2+d1].PB(MP(abs(x+d0-i)+abs(y+d1-j),i*16+j));
                            if (vp[0].S < 4) continue;

                            // REP(i, 4) {
                            // nth_element(vp[i].begin(), vp[i].begin() + 4, vp[i].end());
                            // sort(vp[i].begin(), vp[i].begin() + 4);
                            // }
                            REP(i, 4) sort(ALL(vp[i]));

                            ZERO(used);
                            ZERO(pused);
                            int tv = 0;
                            REP(i, 4) {
                                int bv = 1 << 20;
                                int bpa = 0;
                                int bpb = 0;
                                REP(j, 4) if (!pused[j]) {
                                        int pos = 0;
                                        while (used[vp[j][pos].Y]) pos++;
                                        int av = vp[j][pos].X * 1000 + 999 - vp[j][pos+1].X;
                                        if (av < bv) {
                                            bv = av;
                                            bpa = j;
                                            bpb = vp[j][pos].Y;
                                        }
                                    }
                                tv += bv / 1000;
                                pused[bpa] = 1;
                                used[bpb] = 1;
                            }

                            if (tv == 0) {
                                cerr << x << ' ' << y << endl;
                                REP(i, N) {
                                    REP(j, N) cerr << board[i][j];
                                    cerr << endl;
                                }
                                REP(i, 4) {
                                    REP(j, 4) cerr << "(" << vp[i][j].X << ", " << vp[i][j].Y << ") : ";
                                    cerr << endl;
                                }
                                cerr << endl;
                            }

                            minMove = min(minMove, tv);
                            movesNo[tv]++;
                        }
                bestMove[minMove]++;

                simulate(moves[turn * 3 + 0], moves[turn * 3 + 1], moves[turn * 3 + 2]);
                moveScore[score - lscore]++;
                if (score > lscore) {
                    moveGap[turn - lastimp]++;
                    lastimp = turn;
                }

            }

            REP(i, movesNo.S) movesNo[i] /= MAX_TURNS;
            while (moveScore.back() == 0) moveScore.pop_back();
            while (moveGap.back() == 0) moveGap.pop_back();
            while (bestMove.back() == 0) bestMove.pop_back();
            while (movesNo.back() == 0) movesNo.pop_back();
            cerr << "moveScore: "; print(moveScore);
            cerr << "moveGap: "; print(moveGap);
            cerr << "bestMove: "; print(bestMove);
            cerr << "movesNo: "; print(movesNo);
        }

        tGlobal.show();
        tTier1.show();
        tTier2.show();
        tNth.show();
        tCopy.show();
        tRep.show();
        tCreateBeam.show();
        tSim.show();
        tTriple.show();
        tTmp.show();

        cerr << "TestTime = " << (getTime() - startTime) << endl;
        cerr << "Group = " << C << endl;

        return moves;
}