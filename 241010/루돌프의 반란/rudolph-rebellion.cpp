#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

#define MAX 51

struct Santa {
	int id;
	int r;
	int c;
	int score = 0;
	int status = 0;
	bool die = false;
};

struct Rudolf {
	int r;
	int c;
};

int N, M, P, C, D;
int r_r, r_c;
int map[MAX][MAX];

int dr[8] = {-1, 0, 1, 0, -1, -1, 1, 1};
int dc[8] = {0, 1, 0, -1, -1, 1, -1, 1};

Rudolf rudolf;
std::vector<Santa> santas;
std::queue<int> removed_santa_idx;

void UpdateSantaStatus() {
	for (auto& santa : santas) {
		if (santa.status < 0 && santa.die == false) {
			santa.status += 1;
		}
	}
}

bool InMap(int y, int x) {
	if (y < 1 || x < 1 || y > N || x > N)
		return false;
	return true;
}

void Interaction(Santa& santa, const int d) {
	int r = santa.r;
	int c = santa.c;
	int id = santa.id;

	while (map[r][c] > 1 && InMap(r, c)) {
		for (auto& move_santa : santas) {
			if (move_santa.id != id && move_santa.die == false && r == move_santa.r && c == move_santa.c) { // 밀려날 산타
				map[move_santa.r][move_santa.c] -= 1;   // map에서 --

				move_santa.r += dr[d];
				move_santa.c += dc[d];

				r = move_santa.r;
				c = move_santa.c;
				id = move_santa.id;

				if (!InMap(move_santa.r, move_santa.c)) {
					move_santa.die = true;
					break;
				}

				map[move_santa.r][move_santa.c] += 1;
				break;
			}
		}
	}
}

void CheckCollisionMoveSanta(Santa &santa, const int d) {
	if (santa.r == rudolf.r && santa.c == rudolf.c) {

		// 점수 추가
		santa.score += D;

		int santa_nr = santa.r + dr[(d + 2) % 4] * D;
		int santa_nc = santa.c + dc[(d + 2) % 4] * D;

		// 산타 탈락
		if (!InMap(santa_nr, santa_nc)) {
			for (int i = 0; i < santas.size(); ++i) {
				if (santas[i].id == santa.id) {
					map[santa.r][santa.c] -= 1;   // map 에서 지우기
					santa.die = true;
					return;
				}
			}
		}

		// 산타 이동
		map[santa.r][santa.c] -= 1;
		santa.r = santa_nr;
		santa.c = santa_nc;
		map[santa.r][santa.c] += 1;

		// 상호작용
		if (map[santa.r][santa.c] > 1) {  // 해당 칸에 다른 산타가 있을 경우
			Interaction(santa, (d + 2) % 4);
		}

		// 산타 기절
		santa.status = -2;
	}
}

void CheckCollisionMoveRudolf(Santa& santa, const int d) {
	if (santa.r == rudolf.r && santa.c == rudolf.c) {
		// 점수 추가
		santa.score += C;

		int santa_nr = santa.r + dr[d] * C;
		int santa_nc = santa.c + dc[d] * C;

		// 산타 탈락
		if (!InMap(santa_nr, santa_nc)) {
			map[santa.r][santa.c] -= 1;   // map 에서 지우기
			santa.die = true;
			return;
		}

		// 산타 이동
		map[santa.r][santa.c] -= 1;
		santa.r = santa_nr;
		santa.c = santa_nc;
		map[santa.r][santa.c] += 1;

		// 상호작용
		if (map[santa.r][santa.c] > 1) {
			Interaction(santa, d);
		}

		// 산타 기절
		santa.status = -2;
	}
}

bool cmp(std::vector<int>& a, std::vector<int>& b) {
	if (a[0] != b[0])
		return a[0] < b[0];
	else if (a[1] != b[1])
		return a[1] > b[1];
	else if (a[2] != b[2])
		return a[2] > b[2];
}

int FindCloseSanta() {
	std::vector<std::vector<int>>rudolf_santa_infos;

	for (auto& santa: santas) {
		if (santa.die == false) {
			int distance = pow(rudolf.r - santa.r, 2) + pow(rudolf.c - santa.c, 2);
			rudolf_santa_infos.push_back({ distance, santa.r, santa.c, santa.id });
		}
	}
	sort(rudolf_santa_infos.begin(), rudolf_santa_infos.end(), cmp);

	return rudolf_santa_infos[0][3];
}

void MoveRudolf() {
	int close_santa_id = FindCloseSanta();
	int d = -1;
	int close_santa_idx = -1;

	for (int i = 0; i < santas.size(); ++i) {
		if (santas[i].id == close_santa_id) {
			close_santa_idx = i;
			break;
		}
	}

	int close_distance = pow(santas[close_santa_idx].r - rudolf.r, 2) + pow(santas[close_santa_idx].c - rudolf.c, 2);

	// 산타와 가장 가까워지는 방향 찾기
	for (int i = 0; i < 8; ++i) {
		int nr = rudolf.r + dr[i];
		int nc = rudolf.c + dc[i];

		if (!InMap(nr, nc)) {
			continue;
		}

		int distance = pow(santas[close_santa_idx].r - nr, 2) + pow(santas[close_santa_idx].c - nc, 2);
		if (distance < close_distance) {
			close_distance = distance;
			d = i;
		}
	}

	// 가까운 산타 방향으로 움직이기
	if (d != -1) {
		rudolf.r += dr[d];
		rudolf.c += dc[d];

		CheckCollisionMoveRudolf(santas[close_santa_idx], d);
	}

}

void MoveSanta() {
	// 모든 산타 움직임
	for (int i = 0; i < santas.size(); ++i) {
		int d = -1;
		auto& santa = santas[i];
		int close_distance = pow(rudolf.r - santa.r, 2) + pow(rudolf.c - santa.c, 2);

		if (santa.status == 0 && santa.die == false) {
			// 루돌프와 가장 가까워지는 방향 찾기
			for (int i = 0; i < 4; ++i) {
				int nr = santa.r + dr[i];
				int nc = santa.c + dc[i];
				
				if (!InMap(nr, nc)) {
					continue;
				}
				
				if (map[nr][nc] > 0) {  // 다른 산타 있음
					continue;
				}
				
				int distance = pow(rudolf.r - nr, 2) + pow(rudolf.c - nc, 2);
				if (distance < close_distance) {
					close_distance = distance;
					d = i;
				}
			}

			// 루돌프와 가장 가까운 방향으로 움직이기
			if (d != -1) {
				map[santa.r][santa.c] -= 1;
				santa.r += dr[d];
				santa.c += dc[d];
				map[santa.r][santa.c] += 1; 
				CheckCollisionMoveSanta(santa, d);
			}
		}
	}
}

void GiveScoreToSanta() {
	for (auto& santa : santas) {
		if (santa.die == false) {
			santa.score += 1;
		}
	}
}

bool CheckAllSantaDead() {
	for (auto& santa : santas) {
		if (santa.die == false) {
			return false;
		}
	}
	return true;
}

bool Game() {
	int flag = true;

	UpdateSantaStatus();
	MoveRudolf();
	MoveSanta();
	if (CheckAllSantaDead())
		flag = false;
	GiveScoreToSanta();

	return flag;
}

bool santa_cmp(Santa& a, Santa& b) {
	return a.id < b.id;
}

int main() {
	std::cin >> N >> M >> P >> C >> D;
	std::cin >> r_r >> r_c;

	rudolf.r = r_r;
	rudolf.c = r_c;

	for (int i = 0; i < P; ++i) {
		Santa santa;
		std::cin >> santa.id >> santa.r >> santa.c;
		santas.push_back(santa);
		map[santa.r][santa.c] = 1;
	}

	for (int i = 0; i < M; ++i) {
		if(!Game())
			break;
	}
	
	sort(santas.begin(), santas.end(), santa_cmp);
	for (auto& santa : santas) {
		std::cout << santa.score << " ";
	}
}