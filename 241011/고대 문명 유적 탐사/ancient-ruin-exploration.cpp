#include <iostream>
#include <queue>
#include <vector>

#define N_large 5 
#define N_small 3

using namespace std;

class Board {
public:
    int grid_[N_large][N_large];

    Board() {
        for (int i = 0; i < N_large; i++) {
            for (int j = 0; j < N_large; j++) {
                grid_[i][j] = 0;
            }
        }
    }

    bool InRange(int y, int x) const {
        return 0 <= y && y < N_large && 0 <= x && x < N_large;
    }

    void Rotate(int sy, int sx, int cnt) {
        for (int i = 0; i < cnt; i++) {
            vector<vector<int>> temp(N_small, vector<int>(N_small));
            for (int y = 0; y < N_small; y++) {
                for (int x = 0; x < N_small; x++) {
                    temp[x][N_small - 1 - y] = grid_[sy + y][sx + x];
                }
            }
            for (int y = 0; y < N_small; y++) {
                for (int x = 0; x < N_small; x++) {
                    grid_[sy + y][sx + x] = temp[y][x];
                }
            }
        }
    }

    int CalScore() {
        bool visit[N_large][N_large] = { false };
        int score = 0, dy[4] = { 0, 1, 0, -1 }, dx[4] = { 1, 0, -1, 0 };

        for (int i = 0; i < N_large; i++) {
            for (int j = 0; j < N_large; j++) {
                if (visit[i][j]) continue;

                queue<pair<int, int>> q, trace;
                q.push({ i, j });
                visit[i][j] = true;

                while (!q.empty()) {
                    int y = q.front().first;
                    int x = q.front().second;
                    q.pop();
                    trace.push({ y, x });

                    for (int k = 0; k < 4; k++) {
                        int ny = y + dy[k], nx = x + dx[k];
                        if (InRange(ny, nx) && grid_[ny][nx] == grid_[y][x] && !visit[ny][nx]) {
                            q.push({ ny, nx });
                            visit[ny][nx] = true;
                        }
                    }
                }

                // 3개 이상 모인 조각 제거
                if (trace.size() >= 3) {
                    score += trace.size();
                    while (!trace.empty()) {
                        int y = trace.front().first;
                        int x = trace.front().second;
                        trace.pop();
                        grid_[y][x] = 0;
                    }
                }
            }
        }
        return score;
    }

    // 빈 곳에 새로운 조각을 채움
    void Fill(queue<int>& que) {
        for (int j = 0; j < N_large; j++) {
            for (int i = N_large - 1; i >= 0; i--) {
                if (grid_[i][j] == 0 && !que.empty()) {
                    grid_[i][j] = que.front();
                    que.pop();
                }
            }
        }
    }
};

int main() {
    int M, K;
    queue<int> artifacts;
    Board board;

    cin >> K >> M;
    for (int i = 0; i < N_large; i++) {
        for (int j = 0; j < N_large; j++) {
            cin >> board.grid_[i][j];
        }
    }
    for (int i = 0; i < M; i++) {
        int artifact_id;
        cin >> artifact_id;
        artifacts.push(artifact_id);
    }

    while (K--) {
        int max_score = 0;
        Board best_board;

        for (int cnt = 1; cnt <= 3; cnt++) {
            for (int sx = 0; sx <= N_large - N_small; sx++) {
                for (int sy = 0; sy <= N_large - N_small; sy++) {
                    Board temp_board = board;
                    temp_board.Rotate(sy, sx, cnt);
                    
                    int score = temp_board.CalScore();
                    if (score > max_score) {
                        max_score = score;
                        best_board = temp_board;
                    }
                }
            }
        }

        if (max_score == 0) 
            break;

        board = best_board;

        while (true) {
            board.Fill(artifacts);
            int new_score = board.CalScore();
            if (new_score == 0) 
                break;
            max_score += new_score;
        }

        cout << max_score << " ";
    }

    return 0;
}