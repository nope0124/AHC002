#include <bits/stdc++.h>
// #include <atcoder/all>
using namespace std;
typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pll;
#define rep(i, N) for(ll i = 0; i < (ll)N; i++)
#define ALL(v) (v).begin(), (v).end()

ll dx[8] = {-1, 1, 0, 0, 0};
ll dy[8] = {0, 0, -1, 1, 0};
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


const ll H = 50;
const ll W = 50;

struct Point {
    ll x_, y_;
    Point(ll x = 0, ll y = 0) : x_(x), y_(y) {};
};


ll score[H][W];
ll tile[H][W];
ll tile_direction[H][W];

struct Board {
    bool reached_[H][W];
    Point character_;
};

class State {
    private:
    public:
        ll evaluated_score_ = 0;
        vector<ll> actions_;
        Board board_;

        State() {}

        void advance(ll action) {
            ll nx = board_.character_.x_ + dx[action];
            ll ny = board_.character_.y_ + dy[action];
            evaluated_score_ += score[ny][nx];
            board_.reached_[ny][nx] = true;
            board_.reached_[ny + dy[tile_direction[ny][nx]]][nx + dx[tile_direction[ny][nx]]] = true;
            board_.character_.x_ = nx;
            board_.character_.y_ = ny;
            return;
        }

        vector<ll> legalActions() {
            vector<ll> actions;
            for(ll action = 0; action < 4; action++) {
                ll nx = board_.character_.x_ + dx[action];
                ll ny = board_.character_.y_ + dy[action];
                if(nx < 0 || nx >= 50 || ny < 0 || ny >= 50) continue;
                if(board_.reached_[ny][nx] == true) continue;
                actions.push_back(action);
            }
            return actions;
        }
};

bool operator<(const State state_1, const State state_2) {
    return state_1.evaluated_score_ < state_2.evaluated_score_;
}

vector<ll> beamSearchActionWithTimeThreshold(State state, ll beam_width, ll time_threshold) {
    auto time_keeper = TimeKeeper(time_threshold);
    auto legal_actions = state.legalActions();
    priority_queue<State> now_beam;
    State best_state;

    now_beam.push(state);
    while(1) {
        priority_queue<State> next_beam;
        rep(i, beam_width) {
            if(time_keeper.isTimeOver()) return best_state.actions_;
            if(now_beam.empty()) break;
            State now_state = now_beam.top();
            now_beam.pop();
            vector<ll> legal_actions = now_state.legalActions();
            for(ll action: legal_actions) {
                State next_state = now_state;
                next_state.advance(action);
                next_state.actions_.emplace_back(action);
                next_beam.push(next_state);
            }
        }

        now_beam = next_beam;
        State now_best_state = now_beam.top();
        if(now_best_state.evaluated_score_ > best_state.evaluated_score_) {
            best_state = now_best_state;
        }
    }
}

int main() {
    ll player_x, player_y;
    cin >> player_y >> player_x;

    State state;
    Board board;
    board.character_ = Point(player_x, player_y);
    rep(i, H) {
        rep(j, W) {
            cin >> tile[i][j];
        }
    }
    rep(i, H) {
        rep(j, W) {
            cin >> score[i][j];
            score[i][j] += abs(i - H/2) + abs(j - W/2);
        }
    }
    rep(i, H) {
        rep(j, W) {
            tile_direction[i][j] = 4;
            rep(k, 4) {
                ll nx = j + dx[k];
                ll ny = i + dy[k];
                if(nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
                if(tile[i][j] == tile[ny][nx]) {
                    tile_direction[i][j] = k;
                }
            }
        }
    }
    
    // board.reached_[player_y][player_x] = true;
    // board.reached_[player_y + dy[tile_direction[player_y][player_x]]][player_x + dx[tile_direction[player_y][player_x]]] = true;
    // state.board_ = board;


    // vector<ll> actions = beamSearchActionWithTimeThreshold(state, 32, 1000);
    // string S = "LRUD";
    // rep(i, actions.size()) {
    //     cout << S[actions[i]];
    // }
    // cout << endl;
    dfs();
    
}