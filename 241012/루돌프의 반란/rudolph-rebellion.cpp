#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_N 51
#define MAX_P 31

struct Position {
	int x, y;
};

struct Santa {
	Position pos;
	int points;
	bool is_alive;
	int stun_until;
};

int n, m, p, c, d;
Position rudolf;
vector<Santa> santas(MAX_P);

int board[MAX_N][MAX_N];

const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

bool is_inrange(int x, int y) {
	return 1 <= x && x <= n && 1 <= y && y <= n;
}

pair<int, pair<int, int>> find_closest_santa() {
	int closestX = 10000, closestY = 10000, closestIdx = 0;

	for (int i = 1; i <= p; i++) {
		if (!santas[i].is_alive)
			continue;

		pair<int, pair<int, int>> currentBest = { (closestX - rudolf.x) * (closestX - rudolf.x) + (closestY - rudolf.y) * (closestY - rudolf.y), {-closestX, -closestY} };
		pair<int, pair<int, int>> currentValue = { (santas[i].pos.x - rudolf.x) * (santas[i].pos.x - rudolf.x) + (santas[i].pos.y - rudolf.y) * (santas[i].pos.y - rudolf.y), {-santas[i].pos.x, -santas[i].pos.y} };

		if (currentValue < currentBest) {
			closestX = santas[i].pos.x;
			closestY = santas[i].pos.y;
			closestIdx = i;
		}
	}

	return make_pair(closestIdx, make_pair(closestX, closestY));
}

void move_rudolf_towards_closest_santa(int closestIdx, int closestX, int closestY, int t) {
	if (closestIdx == 0)
		return;

	Position prevRudolf = rudolf;
	int moveX = (closestX > rudolf.x) - (closestX < rudolf.x);
	int moveY = (closestY > rudolf.y) - (closestY < rudolf.y);

	rudolf.x += moveX;
	rudolf.y += moveY;
	board[prevRudolf.x][prevRudolf.y] = 0;

	if (rudolf.x == closestX && rudolf.y == closestY) {
		int firstX = closestX + moveX * c;
		int firstY = closestY + moveY * c;
		int lastX = firstX;
		int lastY = firstY;

		santas[closestIdx].stun_until = t + 1;

		while (is_inrange(lastX, lastY) && board[lastX][lastY] > 0) {
			lastX += moveX;
			lastY += moveY;
		}

		while (!(lastX == firstX && lastY == firstY)) {
			int beforeX = lastX - moveX;
			int beforeY = lastY - moveY;

			if (!is_inrange(beforeX, beforeY))
				break;

			int idx = board[beforeX][beforeY];

			if (!is_inrange(lastX, lastY)) {
				santas[idx].is_alive = false;
			}
			else {
				board[lastX][lastY] = board[beforeX][beforeY];
				santas[idx].pos = { lastX, lastY };
			}

			lastX = beforeX;
			lastY = beforeY;
		}

		santas[closestIdx].points += c;
		santas[closestIdx].pos = { firstX, firstY };
		if (is_inrange(firstX, firstY)) {
			board[firstX][firstY] = closestIdx;
		}
		else {
			santas[closestIdx].is_alive = false;
		}
	}

	board[rudolf.x][rudolf.y] = -1;
}

void move_santas(int t) {
	for (int i = 1; i <= p; i++) {
		if (!santas[i].is_alive || santas[i].stun_until >= t)
			continue;

		int minDist = (santas[i].pos.x - rudolf.x) * (santas[i].pos.x - rudolf.x) + (santas[i].pos.y - rudolf.y) * (santas[i].pos.y - rudolf.y);
		int moveDir = -1;

		for (int dir = 0; dir < 4; dir++) {
			int nx = santas[i].pos.x + dx[dir];
			int ny = santas[i].pos.y + dy[dir];

			if (!is_inrange(nx, ny) || board[nx][ny] > 0)
				continue;

			int dist = (nx - rudolf.x) * (nx - rudolf.x) + (ny - rudolf.y) * (ny - rudolf.y);
			if (dist < minDist) {
				minDist = dist;
				moveDir = dir;
			}
		}

		if (moveDir != -1) {
			int nx = santas[i].pos.x + dx[moveDir];
			int ny = santas[i].pos.y + dy[moveDir];

			if (nx == rudolf.x && ny == rudolf.y) {
				santas[i].stun_until = t + 1;

				int moveX = -dx[moveDir];
				int moveY = -dy[moveDir];

				int firstX = nx + moveX * d;
				int firstY = ny + moveY * d;
				int lastX = firstX;
				int lastY = firstY;

				if (d == 1) {
					santas[i].points += d;
				}
				else {
					while (is_inrange(lastX, lastY) && board[lastX][lastY] > 0) {
						lastX += moveX;
						lastY += moveY;
					}

					while (!(lastX == firstX && lastY == firstY)) {
						int beforeX = lastX - moveX;
						int beforeY = lastY - moveY;

						if (!is_inrange(beforeX, beforeY))
							break;

						int idx = board[beforeX][beforeY];

						if (!is_inrange(lastX, lastY)) {
							santas[idx].is_alive = false;
						}
						else {
							board[lastX][lastY] = board[beforeX][beforeY];
							santas[idx].pos = { lastX, lastY };
						}

						lastX = beforeX;
						lastY = beforeY;
					}

					santas[i].points += d;
					board[santas[i].pos.x][santas[i].pos.y] = 0;
					santas[i].pos = { firstX, firstY };
					if (is_inrange(firstX, firstY)) {
						board[firstX][firstY] = i;
					}
					else {
						santas[i].is_alive = false;
					}
				}
			}
			else {
				board[santas[i].pos.x][santas[i].pos.y] = 0;
				santas[i].pos = { nx, ny };
				board[nx][ny] = i;
			}
		}
	}
}

int main() {
	cin >> n >> m >> p >> c >> d;
	cin >> rudolf.x >> rudolf.y;
	board[rudolf.x][rudolf.y] = -1;

	for (int i = 1; i <= p; i++) {
		int id = -1;
		cin >> id;
		cin >> santas[id].pos.x >> santas[id].pos.y;
		board[santas[id].pos.x][santas[id].pos.y] = id;
		santas[id].is_alive = true;
		santas[id].points = 0;
		santas[id].stun_until = 0;
	}

	for (int t = 1; t <= m; t++) {
		auto closestIdx = find_closest_santa().first;
		auto closestPos = find_closest_santa().second;
		move_rudolf_towards_closest_santa(closestIdx, closestPos.first, closestPos.second, t);
		move_santas(t);

		for (int i = 1; i <= p; i++) {
			if (santas[i].is_alive)
				santas[i].points++;
		}
	}

	for (int i = 1; i <= p; i++) {
		cout << santas[i].points << " ";
	}

	return 0;
}