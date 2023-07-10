#include <bits/stdc++.h>
// #include <atcoder/all>
using namespace std;
typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pll;
#define rep(i, N) for(ll i = 0; i < (ll)N; i++)
#define ALL(v) (v).begin(), (v).end()

ll dx[5] = {-1, 1, 0, 0, 0};
ll dy[5] = {0, 0, -1, 1, 0};
string dir = "LRUDN";
const ll INF = 1LL << 60;
const ll MOD = 1000000007;

template<class T> inline bool chmin(T& a, T b) {
    if (a > b) {
        a = b;
        return true;
    }
    return false;
}

template<class T> inline bool chmax(T& a, T b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}

class TimeKeeper {
    private:
        chrono::high_resolution_clock::time_point start_time_;
        ll time_threshold_;
    public:
        TimeKeeper(const ll &time_threshold) : start_time_(chrono::high_resolution_clock::now()), time_threshold_(time_threshold) {}

        bool isTimeOver() {
            auto diff = chrono::high_resolution_clock::now() - this->start_time_;
            return chrono::duration_cast<chrono::milliseconds>(diff).count() >= time_threshold_;
        }
};

const ll N = 50;
const ll time_threshold = 1900;

ll sx, sy;
ll old_score = 0;
string best_ans = "";
vector<vector<ll>> tile_direction(N, vector<ll>(N));
vector<vector<ll>> tile(N, vector<ll>(N));
vector<vector<ll>> point(N, vector<ll>(N));
vector<vector<bool>> reached(N, vector<bool>(N, false));

bool is_range(ll x, ll y) {
    if(x < 0 || x >= N || y < 0 || y >= N) return false;
    return true;
}


bool dfs(ll x, ll y, ll cur_score, string ans, TimeKeeper time_keeper) {
    if(time_keeper.isTimeOver()) {
        return true;
    }
    
    reached[y][x] = true;
    reached[y + dy[tile_direction[y][x]]][x + dx[tile_direction[y][x]]] = true;

    rep(i, 4) {
        ll nx = x + dx[i];
        ll ny = y + dy[i];
        if(!is_range(nx, ny)) continue;
        if(reached[ny][nx]) continue;
        bool time_limit = dfs(nx, ny, cur_score + point[ny][nx], ans + dir[i], time_keeper);
        if(time_limit) return true;
    }

    if(old_score < cur_score) {
        old_score = cur_score;
        best_ans = ans;
    }

    reached[y][x] = false;
    reached[y + dy[tile_direction[y][x]]][x + dx[tile_direction[y][x]]] = false;
    return false;
}

int main() {
    TimeKeeper time_keeper = TimeKeeper(time_threshold);
    cin >> sy >> sx;
    rep(i, N) {
        rep(j, N) {
            cin >> tile[i][j];
        }
    }
    rep(i, N) {
        rep(j, N) {
            cin >> point[i][j];
        }
    }
    rep(i, N) {
        rep(j, N) {
            rep(k, 5) {
                ll nx = j + dx[k];
                ll ny = i + dy[k];
                if(!is_range(nx, ny)) continue;
                if(tile[i][j] == tile[ny][nx]) {
                    tile_direction[i][j] = k;
                    break;
                }
            }
        }
    }

    reached[sy][sx] = true;
    reached[sy + dy[tile_direction[sy][sx]]][sx + dx[tile_direction[sy][sx]]] = true;
    dfs(sx, sy, 0, "", time_keeper);
    cout << best_ans << endl;
}