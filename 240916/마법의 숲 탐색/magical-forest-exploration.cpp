#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define MAX 70

int R, C, K;
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
int result = 0;
int map[MAX+3][MAX];
bool golam_exist[MAX+3][MAX];
bool fairy_visited[MAX+3][MAX];

struct Golam {
    int id;
    int y;
    int x;
    int d;

    Golam(int id, int y, int x, int d) : id(id), y(y), x(x), d(d) {
    }
};

bool InRange(int y, int x) {
    return y >= 3 && y <= R + 2 && x >= 0 && x <= C - 1;
}

void Init() {
    for (int i = 0; i < R + 3; i++) {
        for (int j = 0; j < C; j++) {
            golam_exist[i][j] = false;
            map[i][j] = 0;
        }
    }
}

bool CanGo(int y, int x) {
    if (y<2 || y>R + 1 || x<1 || x>C - 2)
        return false;

    if (map[y][x - 1] != 0 || map[y][x + 1] != 0 || map[y + 1][x] != 0) {
        return false;
    }
    if (map[y - 1][x - 1] != 0 || map[y-2][x] != 0) {
        return false;
    }
    if (map[y - 1][x + 1] != 0 || map[y - 2][x]!= 0) {
        return false;
    }
    return true;
}

bool MoveGolam(Golam& golam) {
    while (true) {
        if (CanGo(golam.y + 1, golam.x)) {
            golam.y += 1;
            //MoveGolam(golam);
        }

        else if (CanGo(golam.y + 1, golam.x - 1)) {
            golam.y += 1;
            golam.x -= 1;
            golam.d = (golam.d + 3) % 4;
            //MoveGolam(golam);
        }

        else if (CanGo(golam.y + 1, golam.x + 1)) {
            golam.y += 1;
            golam.x += 1;
            golam.d = (golam.d + 1) % 4;
            //MoveGolam(golam);
        }

        else {
            if (!InRange(golam.y - 1, golam.x) || !InRange(golam.y + 1, golam.x) || !InRange(golam.y, golam.x - 1) || !InRange(golam.y, golam.x + 1)) {
                Init();
                return false;
            }
            else {
                golam_exist[golam.y + dy[golam.d]][golam.x + dx[golam.d]] = true; // 출구 표시
                map[golam.y][golam.x] = golam.id;
                for (int i = 0; i < 4; i++) {
                    map[golam.y + dy[i]][golam.x + dx[i]] = golam.id;
                }
            }
            break;
        }
    }
    return true;
}

int MoveFairy(Golam& golam) {
    for (int i = 0; i < R + 3; i++) {
        for (int j = 0; j < C; j++) {
            fairy_visited[i][j] = false;
        }
    }
    int maximum_down = golam.y-2;
    queue<pair<int, int>> q;
    
    q.push({ golam.y, golam.x });
    fairy_visited[golam.y][golam.x] = true;

    while (!q.empty()) {
        int y = q.front().first;
        int x = q.front().second;

        q.pop();
        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (!InRange(ny, nx))
                continue;

            // 골렘 탐색
            if (!fairy_visited[ny][nx] && map[ny][nx]!=0) {
                // 골렘 자신일 경우
                if (map[ny][nx] == map[y][x]) {
                    q.push({ ny, nx });
                    fairy_visited[ny][nx] = true;
                    maximum_down = max(maximum_down, ny-2);
                }

                // 다른 골렘일 경우, 자신의 탈출구 위치일 때 추가
                else if (golam_exist[y][x]) {
                    q.push({ ny, nx });
                    fairy_visited[ny][nx] = true;
                    maximum_down = max(maximum_down, ny-2);
                }
            }
        }
    }
    return maximum_down;
}

int Game(Golam &golam) {
    int fairy_max_row = 0;
    if (MoveGolam(golam))
        fairy_max_row = MoveFairy(golam);
    return fairy_max_row;
}

 
int main() {
    cin >> R >> C >> K;
    
    Init();
    
    for (int id = 1; id <= K; id++) {
        int ci, di;
        cin >> ci >> di;
        Golam golam(id, 1, ci-1, di);
        result+=Game(golam);
    }
    
    cout << result;
    
    return 0;
}