// https://atcoder.jp/contests/ahc002/submissions/22068837
// 2786822

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
// #define NDEBUG
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>
#include <limits>
#include <list>
#include <queue>
#include <tuple>
#include <map>
#include <stack>
#include <set>
#include <bitset>
#include <functional>
#include <chrono>
#include <cassert>
using namespace std;
#define fast_io ios_base::sync_with_stdio (false) ; cin.tie(0) ; cout.tie(0) ;
#define ll long long int
#define rep(i,n) for(int i=0; i<(int)(n); i++)
#define reps(i,n) for(int i=1; i<=(int)(n); i++)
#define REP(i,n) for(int i=n-1; i>=0; i--)
#define REPS(i,n) for(int i=n; i>0; i--)
#define MOD (long long int)(1e9+7)
#define INF (int)(1e9)
#define LINF (long long int)(1e18)
#define chmax(a, b) a = (((a)<(b)) ? (b) : (a))
#define chmin(a, b) a = (((a)>(b)) ? (b) : (a))
#define all(v) v.begin(), v.end()
#define endl "\x0d\x0a"
typedef pair<int, int> Pii;
typedef pair<ll, ll> Pll;

enum Dir{
    U, D, L, R, N
};

Dir DIR_SET[24][4] = {
    {U, D, L, R},
    {U, D, R, L},
    {U, L, D, R},
    {U, L, R, D},
    {U, R, D, L},
    {U, R, L, D},
    {D, U, L, R},
    {D, U, R, L},
    {D, L, U, R},
    {D, L, R, U},
    {D, R, U, L},
    {D, R, L, U},
    {L, U, D, R},
    {L, U, R, D},
    {L, R, U, D},
    {L, R, D, U},
    {L, D, U, R},
    {L, D, R, U},
    {R, U, D, L},
    {R, U, L, D},
    {R, L, U, D},
    {R, L, D, U},
    {R, D, U, L},
    {R, D, L, U}
};

constexpr int H = 50, W = 50;
constexpr int TIME_LIMIT = 1900;
int sy, sx;
vector<Dir> T;
vector<uint8_t> P;

class Clock{
    chrono::system_clock::time_point _start, _end;
    ll _sum = 0, _count = 0;

public:
    void start(){
        _start = chrono::system_clock::now();
    }

    void stop(){
        _end = chrono::system_clock::now();
    }

    void add(){
        const chrono::system_clock::time_point now = chrono::system_clock::now();
        _sum += static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(now - _start).count());
        _count++;
    }

    ll sum(){
        return _sum / 1000;
    }

    string average(){
        if(_count == 0){
            return "NaN";
        }
        return to_string(_sum / 1000 / _count);
    }

    void reset(){
        _start = chrono::system_clock::now();
        _sum = 0;
        _count = 0;
    }

    inline int ms() const{
        const chrono::system_clock::time_point now = chrono::system_clock::now();
        return static_cast<double>(chrono::duration_cast<chrono::microseconds>(now - _start).count() / 1000);
    }

    inline int ns() const{
        const chrono::system_clock::time_point now = chrono::system_clock::now();
        return static_cast<double>(chrono::duration_cast<chrono::microseconds>(now - _start).count());
    }

    void print(){
        cerr << to_string(ms()) << endl;
    }
};

Clock timer;

uint32_t xorshift(){
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    static uint32_t w = 88675123;
    uint32_t t;
    t = x ^ (x<<11);
    x = y; y = z; z = w;
    w ^= t ^ (t>>8) ^ (w>>19);
    return w;
}

Dir operator~(const Dir& d){
    if(d == U){
        return D;
    }else if(d == D){
        return U;
    }else if(d == L){
        return R;
    }else if(d == R){
        return L;
    }else{
        return N;
    }
}

ostream& operator<<(ostream& os, const Dir& d){
    if(d == U){
        os<<"U";
    }else if(d == D){
        os<<"D";
    }else if(d == L){
        os<<"L";
    }else if(d == R){
        os<<"R";
    }else{
        os<<"-";
    }
    return os;
}

constexpr Dir DIRS[5] = {U, D, L, R, N};
constexpr Dir DIRS4[4] = {U, D, L, R};

struct Pos{
    int y,x;
    Pos(){
        y = -1;
        x = -1;
    }
    Pos(int inp_y, int inp_x){
        y = inp_y;
        x = inp_x;
    }

    int idx() const{
        return y * W + x;      
    }

    int manhattan(const Pos& p) const{
        return abs(p.y - y) + abs(p.x - x);
    }

    int chebyshev(const Pos& p) const{
        return max(abs(p.x - x), abs(p.y - y));
    }

    vector<Pos> arround4() const{
        vector<Pos> ret;
        ret[0] = {y - 1, x};
        ret[1] = {y + 1, x};
        ret[2] = {y, x - 1};
        ret[3] = {y, x + 1};
        return ret;
    }

    Pos operator+ (Dir d) const{
        if(d == U){
            return {y-1, x};
        }else if(d == D){
            return {y+1, x};
        }else if(d == L){
            return {y, x-1};
        }else if(d == R){
            return {y, x+1};
        }else if(d == N){
            return {y, x};
        }

        throw invalid_argument("Posに加えるDirが適切ではありません");
    }

    Pos operator+ (const Pos& p) const{
        return {y + p.y, x + p.x};
    }

    void operator+= (Dir d){
        if(d == U){
            y -= 1;
        }else if(d == D){
            y += 1;
        }else if(d == L){
            x -= 1;
        }else if(d == R){
            x += 1;
        }else if(d == N){

        }else{
            throw invalid_argument("Posに+=するDirが適切ではありません");
        }
    }

    void operator-= (Dir d){
        if(d == D){
            y -= 1;
        }else if(d == U){
            y += 1;
        }else if(d == R){
            x -= 1;
        }else if(d == L){
            x += 1;
        }else if(d == N){

        }else{
            throw invalid_argument("Posに+=するDirが適切ではありません");
        }
    }

    bool operator==(const Pos& p) const{
        return x == p.x && y == p.y;
    }

    bool operator!=(const Pos& p) const{
        return x != p.x || y != p.y;
    }

    //mapに突っ込めるようにするために定義
    bool operator<(const Pos& p) const{
        if(y != p.y){
            return y < p.y;
        }
        return x < p.x;
    }

    Pos operator~() const{
        return {y, W - 1 - x};
    }

    bool in_range() const{
        return x >= 0 && x < W && y >= 0 && y < H;
    }

    string to_string() const{
        return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
};

Pos S;

inline int idx(int y, int x){
    return y * W + x;
}

inline int idx(const Pos& p){
    return p.y * W + p.x;
}

void input(){
    cin>>sy>>sx;
    S = {sy,sx};
    timer.start();
    vector<int> _T;
    rep(y,H){
        rep(x,W){
            int _t;
            cin>>_t;
            _T.push_back(_t);
        }
    }
    rep(y,H){
        rep(x,W){
            Pos p = {y,x};
            for(auto dir : DIRS){
                Pos ap = p + dir;
                if(!ap.in_range()) continue;
                if(_T[idx(p)] == _T[idx(ap)]){
                    T.push_back(dir);
                    break;
                }
            }
            assert(T.size() == idx(p) + 1);
        }
    }
    rep(y,H){
        rep(x,W){
            int p;cin>>p;
            P.push_back(p);
        }
    }
}

void solve(){
    vector<Dir> best_sol; // UDLRの文字列
    ll best_eval = 0;
    int g = 0;
    int update_count = 0;
    vector<Dir> sol; // UDLRの文字列
    while(timer.ms() < TIME_LIMIT) {
        g++;
        int si = 0;
        vector<bool> cantMove(H*W, false);
        Pos nowP = S;
        cantMove[idx(nowP)] = true; // 最初の点をtrue
        // 同じ値の場所をtrue
        {
            const auto adjP = nowP + T[idx(nowP)];
            cantMove[idx(adjP)] = true;
        }
        ll sum = 0; // score
        while(true){
            sum += P[idx(nowP)];
            int ri = xorshift()%24;
            Dir next_dir = N;
            if(si < sol.size()){
                next_dir = sol[si];
            }else{
                // DIR_SETの順に探索して進める方向に進む、進めなかったらbreak
                rep(rj,4){
                    const Dir dir = DIR_SET[ri][rj];
                    Pos kohoP = nowP + dir;
                    if(!kohoP.in_range()) continue; // 範囲外なら除外
                    if(cantMove[idx(kohoP)]) continue; // 既に訪れていたら除外
                    next_dir = dir;
                    break;
                }
                if(next_dir == N) break; // ここで抜ける
                sol.push_back(next_dir);
            }
            const Pos nextP = nowP + next_dir;
            cantMove[idx(nextP)] = true;
            Pos adjP = nextP + T[idx(nextP)];
            cantMove[idx(adjP)] = true;
            nowP = nextP;
            si++;
        }

        if(best_eval < sum){
            best_sol = sol;
            best_eval = sum;
            update_count++;
        }

        if((xorshift()&0b111111) == 0){
            sol.clear();
        }else{
            sol.resize(max(0, (int)sol.size()-5));
        }
    }
    rep(i,best_sol.size()){
        cout<<best_sol[i];
    }
    cout<<endl;
    cerr<<"eval:"<<best_eval<<endl;
    cerr<<"g:"<<g<<endl;
    cerr<<"update:"<<update_count<<endl;
}

void saiki(Pos& nowP, ll score, vector<bool> cant_move){
    
}

int main(void){
    fast_io;

    input();
    solve();

    return 0;
}

// やっていること
// 1. 方向の集合(DIR_SET)を用意する(UDLRの順列24個)
// 2. 始点からランダムに抜き出したDIR_SETの方向に進める
// 3. 進めなくなるまで繰り返し行い、best_scoreのものを残しておく
// 4. 1~3を繰り返す