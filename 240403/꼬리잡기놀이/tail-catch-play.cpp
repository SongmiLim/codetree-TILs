#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
using namespace std;
#define MAX 20

struct Team {
	vector<pair<int, int>> head;
	vector<pair<int, int>> tail;
};

int n, m, k;
int map[MAX][MAX];
int team_index_map[MAX][MAX];
int total_score = 0;
int dy[4] = {-1, 1, 0, 0};
int dx[4] = {0, 0, -1, 1};
vector<Team> team_list;
int team_cnt = 1;

void move(Team &team) {
	int head_y = team.head.front().first;
	int head_x = team.head.front().second;
	bool head_flag = false;
	for (int i = 0; i < 4; i++) {
		int ny = head_y + dy[i];
		int nx = head_x + dx[i];

		if (ny < 0 || nx < 0 || ny >= n || nx >= n)
			continue;

		if (map[ny][nx] == 4) {
			team.head.front().first = ny;
			team.head.front().second = nx;
			map[ny][nx] = 1;
			map[head_y][head_x] = 2;
			head_flag = true;
			break;
		}
	}
	
	if (!head_flag) {    // 4가 없는 경우 => 1과 3이 붙어있음
		for (int i = 0; i < 4; i++) {
			int ny = head_y + dy[i];
			int nx = head_x + dx[i];

			if (ny < 0 || nx < 0 || ny >= n || nx >= n)
				continue;

			if (map[ny][nx] == 2) {
				team.head.front().first = ny;
				team.head.front().second = nx;
				map[ny][nx] = 1;
				map[head_y][head_x] = 3;
				return;
			}
		}
	}

	int tail_y = team.tail.front().first;
	int tail_x = team.tail.front().second;

	for (int i = 0; i < 4; i++) {
		int ny = tail_y + dy[i];
		int nx = tail_x + dx[i];

		if (ny < 0 || nx < 0 || ny >= n || nx >= n)
			continue;

		if (map[ny][nx] == 2) {
			team.tail.front().first = ny;
			team.tail.front().second = nx;
			map[ny][nx] = 3;
			map[tail_y][tail_x] = 4;
			break;
		}
	}
}

int dir_y[4] = { 0, -1,0, 1 };
int dir_x[4] = { 1, 0, -1, 0 };

void throw_ball(int round) {
	int dir = (round / k) % 4;
	int pos = round % k;

	if (dir == 0) {
		for (int i = 0; i < n; i++) {
			int ny = pos;
			int nx = i;
			if (map[ny][nx] !=4 && map[ny][nx] != 0) {
				int team_num = team_index_map[ny][nx];
				int head_y = team_list[team_num -1].head.front().first;
				int head_x = team_list[team_num-1].head.front().second;
				int tail_y = team_list[team_num - 1].tail.front().first;
				int tail_x = team_list[team_num - 1].tail.front().second;
				total_score += pow(abs(ny - head_y) + abs(nx - head_x) + 1, 2);
				swap(team_list[team_num - 1].head, team_list[team_num - 1].tail);
				swap(map[head_y][head_x], map[tail_y][tail_x]);
				break;
			}
		}
	}
	else if (dir == 1) {
		for (int i = n - 1; i >= 0; i--) {
			int ny = i;
			int nx = pos;
			if (map[ny][nx] != 4 && map[ny][nx] != 0) {
				int team_num = team_index_map[ny][nx];
				int head_y = team_list[team_num - 1].head.front().first;
				int head_x = team_list[team_num - 1].head.front().second;
				total_score += pow(abs(ny - head_y) + abs(nx - head_x) + 1, 2);
				swap(team_list[team_num - 1].head, team_list[team_num - 1].tail);
				break;
			}
		}
	}
	else if (dir == 2) {
		for (int i = n - 1; i >= 0; i--) {
			int ny = pos;
			int nx = i;
			if (map[ny][nx] != 4 && map[ny][nx] != 0) {
				int team_num = team_index_map[ny][nx];
				int head_y = team_list[team_num - 1].head.front().first;
				int head_x = team_list[team_num - 1].head.front().second;
				total_score += pow(abs(ny - head_y) + abs(nx - head_x) + 1, 2);
				swap(team_list[team_num - 1].head, team_list[team_num - 1].tail);
				break;
			}
		}
	}
	else if (dir == 3) {
		for (int i = 0; i < n; i++) {
			int ny = i;
			int nx = pos;
			if (map[ny][nx] != 4 && map[ny][nx] != 0) {
				int team_num = team_index_map[ny][nx];
				int head_y = team_list[team_num - 1].head.front().first;
				int head_x = team_list[team_num - 1].head.front().second;
				total_score += pow(abs(ny - head_y) + abs(nx - head_x) + 1, 2);
				swap(team_list[team_num - 1].head, team_list[team_num - 1].tail);
				break;
			}
		}
	}
}

void game(int k){
	
	for (int i = 0; i < k; i++) {
		// 한칸 움직이기
		for (auto& team : team_list) {
			move(team);
		}

		// 공 던지기
		throw_ball(i);
	}
}

void build_team(int y, int x, bool visited[MAX][MAX]) {
	queue <pair<int, int>> q;
	Team team;
	
	q.push({ y, x });
	visited[y][x] = true;
	while (!q.empty()) {
		int y = q.front().first;
		int x = q.front().second;
		q.pop();

		team_index_map[y][x] = team_cnt;
		if (map[y][x] == 1) {
			team.head.push_back({ y,x });
		}
		
		if (map[y][x] == 3) {
			team.tail.push_back({ y,x });
		}
		
		for (int i = 0; i < 4; i++) {
			int ny = y + dy[i];
			int nx = x + dx[i];

			if (ny < 0 || nx < 0 || ny >= n || nx >= n)
				continue;

			if (!visited[ny][nx] && map[ny][nx] > 0) {
				q.push({ ny, nx });
				visited[ny][nx] = true;
			}
		}
	}
	team_list.push_back(team);
}

int main() {
	cin >> n >> m >> k;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> map[i][j];
		}
	}
	
	// 팀 구성하기
	bool visited[MAX][MAX] = { false, };
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (!visited[i][j] && map[i][j]>0) {
				build_team(i, j, visited);
				team_cnt++;
			}
		}
	}

	game(k);
	cout << total_score;

	return 0;
}