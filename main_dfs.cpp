#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pll;
#define rep(i, N) for(ll i = 0; i < (ll)N; i++)
#define ALL(v) (v).begin(), (v).end()

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


class Dir {
    public:
        int dx_;
        int dy_;
        char c_;
        int score_;

        Dir(int dx = 0, int dy = 0, char c = 'N', int score = 0) : dx_(dx), dy_(dy), c_(c), score_(score) { }

        bool operator<(const Dir& other) {
            return score_ < other.score_;
        }

        friend string operator+(const string& s, const Dir& d) {
            return s + string(1, d.c_);
        }

};

vector<Dir> dir = {Dir(-1, 0, 'L', 0), Dir(+1, 0, 'R', 1), Dir(0, -1, 'U', 2), Dir(0, +1, 'D', 3), Dir(0, 0, 'N', 4)};

class Pos {
    public:
        int x_;
        int y_;

        Pos(int x = 0, int y = 0) : x_(x), y_(y) { }

        bool inRange(int width, int height) {
            if(this->x_ < 0 || this->x_ >= width || this->y_ < 0 || this->y_ >= height) {
                return false;
            }
            return true;
        }

        Pos operator+(const Dir& dir) {
            return Pos(x_ + dir.dx_, y_ + dir.dy_);
        }

        friend istream& operator>>(istream &is, Pos &p) {
            return is >> p.y_ >> p.x_;
        }

        friend ostream& operator<<(ostream &os, const Pos &p) {
            return os << "Pos: (" << p.x_ << ", " << p.y_ << ')';
        }

};

class Ans {
    public:
        ll score_;
        string operation_;
        Ans(ll score = 0, string operation = "") : score_(score), operation_(operation) { }

        friend ostream& operator<<(ostream &os, const Ans &ans) {
            return os << ans.operation_;
        }

        bool operator<(const Ans& other) {
            if(score_ == other.score_) {
                return operation_.size() < other.operation_.size();
            }
            return score_ < other.score_;
        }
};

const ll N = 50;
const ll time_threshold = 100;

Pos start_pos;
Ans best_ans;
ll tile[N][N];
Dir tile_dir[N][N];
ll point[N][N];

class Board {
    public:
        vector<vector<bool>> reached_;

        Board(ll width, ll height) {
            reached_.resize(height, vector<bool>(width, false));
        }

        void fill(Pos p, bool flag = true) {
            reached_[p.y_][p.x_] = flag;
            reached_[p.y_ + tile_dir[p.y_][p.x_].dy_][p.x_ + tile_dir[p.y_][p.x_].dx_] = flag;
        }
};

Board board(N, N);

bool dfs(Pos p, Ans cur_ans, TimeKeeper time_keeper) {
    if(time_keeper.isTimeOver()) return true;
    
    board.fill(p, true);

    vector<Dir> cur_dir;
    rep(i, 4) {
        Pos np = p + dir[i];
        if(!np.inRange(N, N)) continue;
        if(board.reached_[np.y_][np.x_]) continue;
        Dir nd = dir[i];
        nd.score_ = (N/2-np.x_)*(N/2-np.x_) + (N/2-np.y_)*(N/2-np.y_);
        cur_dir.push_back(nd);
    }

    if(cur_dir.size() != 0) {
        sort(ALL(cur_dir));
        reverse(ALL(cur_dir));
        rep(i, cur_dir.size()) {
            Pos np = p + cur_dir[i];
            if(!np.inRange(N, N)) continue;
            if(board.reached_[np.y_][np.x_]) continue;
            Ans new_ans = cur_ans;
            new_ans.score_ += point[np.y_][np.x_];
            new_ans.operation_ = new_ans.operation_ + cur_dir[i];
            bool time_limit = dfs(np, new_ans, time_keeper);
            if(time_limit) return true;
        }
    }

    if(best_ans < cur_ans) best_ans = cur_ans;

    board.fill(p, false);

    return false;
}

int main() {
    TimeKeeper time_keeper = TimeKeeper(time_threshold);

    // input
    cin >> start_pos;
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
                Pos np = Pos(j, i) + dir[k];
                if(!np.inRange(N, N)) continue;
                if(tile[i][j] == tile[np.y_][np.x_]) {
                    tile_dir[i][j] = dir[k];
                    break;
                }
            }
        }
    }

    dfs(start_pos, Ans(0, ""), time_keeper);
    cout << best_ans << endl;
}