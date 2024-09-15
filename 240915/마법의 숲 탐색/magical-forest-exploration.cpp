#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define MAX 73

int R, C, K;
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
int result = 0;
int map[MAX][MAX];
int golam_exist[MAX][MAX];
bool fairy_visited[MAX][MAX];

struct Golam {
    int id;
    int y;
    int x;
    int d;

    Golam(int id, int y, int x, int d) : id(id), y(y), x(x), d(d) {
    }
};

void Init() {
    for (int i = 0; i < R + 3; i++) {
        for (int j = 0; j < C; j++) {
            golam_exist[i][j] = -1;
            map[i][j] = -1;
        }
    }
}

bool CanGo(int y, int x) {
    if (y >= R + 2 || y < 2 || x >= C - 1 || x < 1) {
        return false;
    }
    if (golam_exist[y][x - 1] != -1 || golam_exist[y][x + 1] != -1 || golam_exist[y + 1][x] != -1) {
        return false;
    }
    if (golam_exist[y-1][x - 1] != -1 || golam_exist[y-2][x] != -1) {
        return false;
    }
    if (golam_exist[y - 1][x + 1] != -1 || golam_exist[y - 2][x]!= -1) {
        return false;
    }
    return true;
}

bool MoveGolam(Golam& golam) {
    if (CanGo(golam.y + 1, golam.x)) {
        golam.y += 1;
        MoveGolam(golam);
    }

    else if (CanGo(golam.y+1, golam.x - 1)) {
        golam.x -= 1;
        golam.d = (golam.d + 3) % 4;
        MoveGolam(golam);
    }

    else if (CanGo(golam.y+1, golam.x + 1)) {
        golam.x += 1;
        golam.d = (golam.d + 1) % 4;
        MoveGolam(golam);
    }

    else {
        if (golam.y <= 3 || golam.x < 1 ||golam.y>=R+2 || golam.x>=C-1) {
            Init();
            return false;
        }
        else {
            map[golam.y + dy[golam.d]][golam.x + dx[golam.d]] = golam.id; // 출구 표시
            golam_exist[golam.y][golam.x] = golam.id;
            for (int i = 0; i < 4; i++) {
                golam_exist[golam.y + dy[i]][golam.x + dx[i]] = golam.id;
            }
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
    int maximum_down = 0;
    queue<pair<int, int>> q;
    
    q.push({ golam.y, golam.x });
    fairy_visited[golam.y][golam.x] = true;

    while (!q.empty()) {
        int y = q.front().first;
        int x = q.front().second;
        maximum_down < y-2 ? maximum_down = y-2 : maximum_down = maximum_down;

        q.pop();
        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (ny <0 || ny >R + 2 || nx<0 || nx>C - 1)
                continue;

            // 골렘 탐색
            if (!fairy_visited[ny][nx] && golam_exist[ny][nx]!=-1) {
                // 골렘 자신일 경우
                if (golam_exist[ny][nx] == golam_exist[y][x]) {
                    q.push({ ny, nx });
                    fairy_visited[ny][nx] = true;
                }

                // 다른 골렘일 경우, 자신의 탈출구 위치일 때 추가
                else if (map[y][x] != -1) {
                    q.push({ ny, nx });
                    fairy_visited[ny][nx] = true;
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
    
    for (int id = 0; id < K; id++) {
        int ci, di;
        cin >> ci >> di;
        Golam golam(id, 1, ci-1, di);
        result+=Game(golam);
    }
    
    cout << result;
    
    return 0;
}