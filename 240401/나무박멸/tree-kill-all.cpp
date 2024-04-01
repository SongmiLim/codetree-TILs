#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define MAX 20

int n, m, k, c;
int map[MAX][MAX] = { 0, };
int remove_tree_cnt = 0;
int dy[4] = {-1, 1, 0, 0}; 
int dx[4] = {0, 0, -1, 1};
// 행 오름차순, 열 오름차순
int cross_dy[4] = { -1, -1, 1, 1 };
int cross_dx[4] = { -1, 1, -1, 1 };
int trace_spray[MAX][MAX] = { 0, };

void grow_tree(int y, int x) {
	int cnt = 0;

	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i];
		int nx = x + dx[i];

		if (ny < 0 || nx < 0 || ny >= n || nx >= n)
			continue;

		if (map[ny][nx] > 0) {
			cnt++;
		}
	}
	map[y][x] += cnt;
}



void copy_tree(int y, int x, int copy_map[][MAX]) {
	vector <pair<int, int>> copy_list;

	int cnt = 0;

	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i];
		int nx = x + dx[i];

		if (ny < 0 || nx < 0 || ny >= n || nx >= n)
			continue;

		if (map[ny][nx] == 0 && !trace_spray[ny][nx]) {
			cnt++;
			copy_list.push_back({ ny, nx });
		}
	}

	for (auto& a : copy_list) {
		copy_map[a.first][a.second] += map[y][x] / cnt;
	}
}

int check_spray_pos(int y, int x, int k) {
	int sum = map[y][x];  // 제초제가 뿌려진 칸

	for (int i = 1; i <= k; i++) {
		for (int j = 0; j < 4; j++) {
			int ny = y + cross_dy[j] * i;
			int nx = x + cross_dx[j] * i;

			if (ny < 0 || nx < 0 || ny >= n || nx >= n)
				continue;

			if (map[ny][nx] > 0) {
				sum += map[ny][nx];
			}
		}
	}
	return sum;
}

void spray_tree(int y, int x) {
	remove_tree_cnt += map[y][x];
	map[y][x] = 0;
	trace_spray[y][x] = -k - 1;

	for (int i = 1; i <= k; i++) {
		for (int j = 0; j < 4; j++) {
			int ny = y + cross_dy[j] * i;
			int nx = x + cross_dx[j] * i;

			if (ny < 0 || nx < 0 || ny >= n || nx >= n)
				continue;

			if (map[ny][nx] >= 0) {
				remove_tree_cnt += map[ny][nx];
				map[ny][nx] = 0;
				trace_spray[ny][nx] = -k - 1;
			}
		}
	}
}

void game() {

	// 나무 성장
	vector<vector<bool>> visited_grow(n, vector<bool>(n, false));

	for (int y = 0; y < n; y++) {
		for (int x = 0; x < n; x++) {
			if (!visited_grow[y][x] && map[y][x] > 0) {
				grow_tree(y, x);
				visited_grow[y][x] = true;
			}
		}
	}

	// 나무 번식
	vector<vector<bool>> visited_copy(n, vector<bool>(n, false));
	int copy_map[MAX][MAX] = { 0, };
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			copy_map[i][j] = map[i][j];
		}
	}
	for (int y = 0; y < n; y++) {
		for (int x = 0; x < n; x++) {
			if (!visited_copy[y][x] && map[y][x] > 0) {
				copy_tree(y, x, copy_map);
				visited_copy[y][x] = true;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			map[i][j] = copy_map[i][j];
		}
	}

	// 제초제 뿌릴 위치 파악하기
	vector<vector<bool>> visited_spray(n, vector<bool>(n, false));
	int max = 0;
	int spray_pos_y = 0;
	int spray_pos_x = 0;

	for (int y = 0; y < n; y++) {
		for (int x = 0; x < n; x++) {
			if (!visited_spray[y][x] && map[y][x] > 0) {
				int cnt_tree = check_spray_pos(y, x, k);
				visited_spray[y][x] = true;
				if (max < cnt_tree) {
					spray_pos_y = y;
					spray_pos_x = x;
					max = cnt_tree;
				}
			}
		}
	}

	// 제초제 뿌리기
	spray_tree(spray_pos_y, spray_pos_x);

	// 제초제 흔적 하나 지우기
	for (int y = 0; y < n; y++) {
		for (int x = 0; x < n; x++) {
			if (trace_spray[y][x] < 0) {
				trace_spray[y][x]++;
			}
		}
	}
}

int main() {
	cin >> n >> m >> k >> c;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> map[i][j];
		}
	}
	
	for (int i = 0; i < m; i++) {
		game();
	}
	
	cout << remove_tree_cnt;
	
	return 0;
}