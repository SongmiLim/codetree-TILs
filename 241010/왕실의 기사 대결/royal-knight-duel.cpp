#include <iostream>
#include <queue>
#include <vector>

using namespace std;

#define MAX_L 41
#define MAX_N 31

struct Knight {
    int r, c, h, w, k;
    int nr, nc;
    int dmg;
    bool is_moved;

    Knight(int r, int c, int h, int w, int k)
        : r(r), c(c), h(h), w(w), k(k), nr(r), nc(c), dmg(0), is_moved(false) {}
};

int l, n, q;
int info[MAX_L][MAX_L];
vector<Knight> knights;
int bef_k[MAX_N];
int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, 1, 0, -1 };


bool TryMovement(int idx, int dir) {
    for (auto& knight : knights) {
        knight.dmg = 0;
        knight.is_moved = false;
        knight.nr = knight.r;
        knight.nc = knight.c;
    }

    queue<int> q;
    q.push(idx);
    knights[idx].is_moved = true;

    while (!q.empty()) {
        int x = q.front(); 
        q.pop();

        knights[x].nr += dx[dir];
        knights[x].nc += dy[dir];

        if (knights[x].nr < 1 || knights[x].nc < 1 ||
            knights[x].nr + knights[x].h - 1 > l || knights[x].nc + knights[x].w - 1 > l) {
            return false;
        }

        for (int i = knights[x].nr; i <= knights[x].nr + knights[x].h - 1; i++) {
            for (int j = knights[x].nc; j <= knights[x].nc + knights[x].w - 1; j++) {
                if (info[i][j] == 1)
                    knights[x].dmg++;
                if (info[i][j] == 2)
                    return false;
            }
        }

        // 다른 기사와의 충돌 시 함께 이동
        for (int i = 0; i < n; i++) {
            if (knights[i].is_moved || knights[i].k <= 0)
                continue;
            if (knights[i].r > knights[x].nr + knights[x].h - 1 || knights[x].nr > knights[i].r + knights[i].h - 1)
                continue;
            if (knights[i].c > knights[x].nc + knights[x].w - 1 || knights[x].nc > knights[i].c + knights[i].w - 1)
                continue;

            knights[i].is_moved = true;
            q.push(i);
        }
    }

    knights[idx].dmg = 0;
    return true;
}

void MovePiece(int idx, int dir) {
    if (knights[idx].k <= 0)
        return;

    if (TryMovement(idx, dir)) {
        for (auto& knight : knights) {
            knight.r = knight.nr;
            knight.c = knight.nc;
            knight.k -= knight.dmg;
        }
    }
}

int main() {
    cin >> l >> n >> q;
    for (int i = 1; i <= l; i++)
        for (int j = 1; j <= l; j++)
            cin >> info[i][j];

    for (int i = 1; i <= n; i++) {
        int r, c, h, w, k;
        cin >> r >> c >> h >> w >> k;
        knights.emplace_back(r, c, h, w, k);
        bef_k[i] = k;
    }

    for (int i = 1; i <= q; i++) {
        int idx, dir;
        cin >> idx >> dir;
        MovePiece(idx - 1, dir);
    }

    long long total_damage = 0;
    for (int i = 0; i < n; i++) {
        if (knights[i].k > 0) {
            total_damage += bef_k[i + 1] - knights[i].k;
        }
    }

    cout << total_damage << endl;
    return 0;
}