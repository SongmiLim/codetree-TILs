#include <iostream>
#include <cmath>

using namespace std;

#define MAX_N 51
#define MAX_P 31

int n, m, p, c, d;
int points[MAX_P];
pair<int, int> santa[MAX_P];
pair<int, int> rudolf;

int board[MAX_N][MAX_N];
bool is_live[MAX_P];
int stun[MAX_P];

const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

bool is_inrange(int x, int y) {
	return 1 <= x && x <= n && 1 <= y && y <= n;
}

int FindClosestSanta() {
	int closestX = 10000, closestY = 10000, closestIdx = 0;

	for (int i = 1; i <= p; i++) {
		if (!is_live[i])
			continue;

		pair<int, pair<int, int>> currentBest
			= { static_cast<int>(pow(closestX - rudolf.first,2) + pow(closestY - rudolf.second,2)), {-closestX, -closestY} };
		pair<int, pair<int, int>> currentValue
			= { static_cast<int>(pow(santa[i].first - rudolf.first, 2) + pow(santa[i].second - rudolf.second, 2)), {-santa[i].first, -santa[i].second} };

		if (currentValue < currentBest) {
			closestX = santa[i].first;
			closestY = santa[i].second;
			closestIdx = i;
		}
	}
	return closestIdx;
}

void MoveRudolf(int t) {
	int closestIdx = FindClosestSanta();
	int closestX = santa[closestIdx].first;
	int closestY = santa[closestIdx].second;

	if (closestIdx) {
		pair<int, int> prevRudolf = rudolf;
		int moveX = 0;
		if (closestX > rudolf.first)
			moveX = 1;
		else if (closestX < rudolf.first)
			moveX = -1;

		int moveY = 0;
		if (closestY > rudolf.second)
			moveY = 1;
		else if (closestY < rudolf.second)
			moveY = -1;

		rudolf.first += moveX;
		rudolf.second += moveY;
		board[prevRudolf.first][prevRudolf.second] = 0;

		// 루돌프의 이동으로 충돌한 경우, 산타를 이동시키고 처리를 합니다.
		if (rudolf.first == closestX && rudolf.second == closestY) {
			int firstX = closestX + moveX * c;
			int firstY = closestY + moveY * c;
			int lastX = firstX;
			int lastY = firstY;

			stun[closestIdx] = t + 1;

			// 만약 이동한 위치에 산타가 있을 경우, 연쇄적으로 이동이 일어납니다.
			while (is_inrange(lastX, lastY) && board[lastX][lastY] > 0) {
				lastX += moveX;
				lastY += moveY;
			}

			// 연쇄적으로 충돌이 일어난 가장 마지막 위치에서 시작해,
			// 순차적으로 보드판에 있는 산타를 한칸씩 이동시킵니다.
			while (!(lastX == firstX && lastY == firstY)) {
				int beforeX = lastX - moveX;
				int beforeY = lastY - moveY;

				if (!is_inrange(beforeX, beforeY))
					break;

				int idx = board[beforeX][beforeY];

				if (!is_inrange(lastX, lastY)) {
					is_live[idx] = false;
				}
				else {
					board[lastX][lastY] = board[beforeX][beforeY];
					santa[idx] = { lastX, lastY };
				}

				lastX = beforeX;
				lastY = beforeY;
			}

			points[closestIdx] += c;
			santa[closestIdx] = { firstX, firstY };
			if (is_inrange(firstX, firstY)) {
				board[firstX][firstY] = closestIdx;
			}
			else {
				is_live[closestIdx] = false;
			}
		}
	}

	board[rudolf.first][rudolf.second] = -1;
}
void MoveSantas(int t) {
	// 각 산타들은 루돌프와 가장 가까운 방향으로 한칸 이동합니다.
	for (int i = 1; i <= p; i++) {
		if (!is_live[i] || stun[i] >= t)
			continue;

		int minDist = (santa[i].first - rudolf.first) * (santa[i].first - rudolf.first) + (santa[i].second - rudolf.second) * (santa[i].second - rudolf.second);
		int moveDir = -1;

		for (int dir = 0; dir < 4; dir++) {
			int nx = santa[i].first + dx[dir];
			int ny = santa[i].second + dy[dir];

			if (!is_inrange(nx, ny) || board[nx][ny] > 0)
				continue;

			int dist = (nx - rudolf.first) * (nx - rudolf.first) + (ny - rudolf.second) * (ny - rudolf.second);
			if (dist < minDist) {
				minDist = dist;
				moveDir = dir;
			}
		}

		if (moveDir != -1) {
			int nx = santa[i].first + dx[moveDir];
			int ny = santa[i].second + dy[moveDir];

			// 산타의 이동으로 충돌한 경우, 산타를 이동시키고 처리를 합니다.
			if (nx == rudolf.first && ny == rudolf.second) {
				stun[i] = t + 1;

				int moveX = -dx[moveDir];
				int moveY = -dy[moveDir];

				int firstX = nx + moveX * d;
				int firstY = ny + moveY * d;
				int lastX = firstX;
				int lastY = firstY;

				if (d == 1) {
					points[i] += d;
				}

				else {
					// 만약 이동한 위치에 산타가 있을 경우, 연쇄적으로 이동이 일어납니다.
					while (is_inrange(lastX, lastY) && board[lastX][lastY] > 0) {
						lastX += moveX;
						lastY += moveY;
					}

					// 연쇄적으로 충돌이 일어난 가장 마지막 위치에서 시작해,
					// 순차적으로 보드판에 있는 산타를 한칸씩 이동시킵니다.
					while (!(lastX == firstX && lastY == firstY)) {
						int beforeX = lastX - moveX;
						int beforeY = lastY - moveY;

						if (!is_inrange(beforeX, beforeY))
							break;

						int idx = board[beforeX][beforeY];

						if (!is_inrange(lastX, lastY)) {
							is_live[idx] = false;
						}
						else {
							board[lastX][lastY] = board[beforeX][beforeY];
							santa[idx] = { lastX, lastY };
						}

						lastX = beforeX;
						lastY = beforeY;
					}

					points[i] += d;
					board[santa[i].first][santa[i].second] = 0;
					santa[i] = { firstX, firstY };

					if (is_inrange(firstX, firstY)) {
						board[firstX][firstY] = i;
					}
					else {
						is_live[i] = false;
					}
				}
			}
			else {
				board[santa[i].first][santa[i].second] = 0;
				santa[i] = { nx, ny };
				board[nx][ny] = i;
			}
		}
	}
}

int main() {
	cin >> n >> m >> p >> c >> d;
	cin >> rudolf.first >> rudolf.second;
	board[rudolf.first][rudolf.second] = -1;

	for (int i = 1; i <= p; i++) {
		int id;
		cin >> id;
		cin >> santa[id].first >> santa[id].second;
		board[santa[id].first][santa[id].second] = id;
		is_live[id] = true;
	}

	for (int t = 1; t <= m; t++) {
		MoveRudolf(t);
		MoveSantas(t);

		// 라운드가 끝나고 탈락하지 않은 산타들의 점수를 1 증가시킵니다.
		for (int i = 1; i <= p; i++)
			if (is_live[i])
				points[i]++;
	}

	for (int i = 1; i <= p; i++)
		cout << points[i] << " ";
}