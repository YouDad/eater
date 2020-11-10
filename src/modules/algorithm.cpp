#include "modules/algorithm.h"
#include <queue>
#include <cmath>
#include <string.h>

int evaluate(class server_data &m, int &score)
{
	// 周围围一圈墙
	int line_size = m.get_map_size();
	int map_size = line_size * line_size + 4 * line_size + 4;

	// 定义位置函数
	auto get_pos = [&](int row, int col) -> int {
		return (row + 1) * (line_size + 2) + col + 1;
	};

	// 定义堆
	struct qnode {
		int row;    // 行
		int col;    // 列
		char dir;   // 方向
		int dist;   // 距离
		qnode(int r, int c, char _dir, int _dist) {
			row = r;
			col = c;
			dir = _dir;
			dist = _dist;
		}
		bool operator < (const struct qnode r) const {
			return dist > r.dist;
		}
	};
	std::priority_queue<qnode> pq;
	int origin_row, origin_col;
	m.get_my_pos(origin_row, origin_col);

	// 计算距离图
	int *dist_graph = new int[map_size];
	memset(dist_graph, 0x7f, map_size * sizeof(int));
	auto get_dist = [&](int row, int col) -> int {
		return dist_graph[get_pos(row, col)];
	};
	auto set_dist = [&](int row, int col, int dist) -> void {
		dist_graph[get_pos(row, col)] = dist;
	};

	char dir = m.get(origin_row, origin_col);
	pq.push(qnode(origin_row, origin_col, dir, 0));
	while (pq.size()) {
		struct qnode node = pq.top();
		set_dist(node.row, node.col, node.dist);
		pq.pop();

		auto check_move = [&](char dir) -> void {
			// 目标行列
			int tr = node.row + (dir == 's') - (dir == 'w');
			int tc = node.col + (dir == 'd') - (dir == 'a');
			// 不能是墙
			bool have_value = m.get(tr, tc) != '9';
			// 距离要更优
			int move_dist = node.dist + 1 + (node.dir == dir);
			have_value = have_value && move_dist <= get_dist(tr, tc);
			if (have_value) {
				pq.push(qnode(tr, tc, dir, move_dist));
			}
		};

		check_move('w');
		check_move('a');
		check_move('s');
		check_move('d');
	}

	// 计算移动价值图
	double *move_val = new double[map_size];
	memset(move_val, 0, map_size * sizeof(double));
	auto get_move_val = [&](int row, int col) -> double {
		return move_val[get_pos(row, col)];
	};
	auto add_move_val = [&](int row, int col, double val) -> void {
		int pos = get_pos(row, col);
		if (pos < 0 || pos >= map_size) {
			return;
		}
		move_val[get_pos(row, col)] += val;
	};
	auto get_manhattan_dist = [](int i, int j, int r, int c) -> int {
		return abs(i - r) + abs(j - c);
	};

	for (int i = 0; i < line_size; i++) {
		for (int j = 0; j < line_size; j++) {
			char ch = m.get(i, j);
			switch (ch) {
				case '1': case '2': case '3': case '4': case '5':
					add_move_val(i, j, ch - '0');
					break;

				case 'w': case 'a': case 's': case 'd':
					if (i != origin_row || j != origin_col) {
						for (int di = i - 2; di <= i + 2; di++) {
							for (int dj = j - 2; dj <= j + 2; dj++) {
								add_move_val(di, dj, -move_val[get_pos(di, dj)]);
							}
						}
					}
					break;

				case '<':
					for (int k = j - 1; k >= 0; k--) {
						if (m.get(i, k) != '9') {
							add_move_val(i, k, -25.0 / (j - k));
						}
					}
					break;

				case '^':
					for (int k = i - 1; k >= 0; k--) {
						if (m.get(k, j) != '9') {
							add_move_val(k, j, -25.0 / (i - k));
						}
					}
					break;

				case '>':
					for (int k = j + 1; k < line_size; k++) {
						if (m.get(i, k) != '9') {
							add_move_val(i, k, -25.0 / (k - j));
						}
					}
					break;

				case 'v':
					for (int k = i + 1; k < line_size; k++) {
						if (m.get(k, j) != '9') {
							add_move_val(k, j, -25.0 / (k - i));
						}
					}
					break;

				case 'g':
					for (int di = i - 3; di <= i + 3; di++) {
						for (int dj = j - 3; dj <= j + 3; dj++) {
							int dist = get_manhattan_dist(i, j, di, dj);
							if (dist <= 3) {
								add_move_val(di, dj, -25.0 / dist);
							}
						}
					}
					break;
			}
		}
	}

	score = 0;
	for (int i = 0; i < line_size; i++) {
		for (int j = 0; j < line_size; j++) {
			if (get_dist(i, j)) {
				score += get_move_val(i, j) / get_dist(i, j);
			}
		}
	}

	return 0;
}

std::pair<enum move_operating, bool> algorithm(class server_data &m)
{
	class server_data cloned;
	int score;
	int max_score = -0x7f7f7f7f;
	enum move_operating move;
	bool fire;

	auto normal_evaluate = [&](enum move_operating _move) -> void {
		cloned = m.clone();
		cloned.move(_move);
		evaluate(cloned, score);
		if (max_score < score) {
			max_score = score;
			move = _move;
			fire = false;
		}
		cloned.fire();
		evaluate(cloned, score);
		if (max_score < score) {
			max_score = score;
			move = _move;
			fire = true;
		}
	};

	normal_evaluate(move_op_up);
	normal_evaluate(move_op_down);
	normal_evaluate(move_op_left);
	normal_evaluate(move_op_right);
	normal_evaluate(move_op_stay);

	return std::make_pair(move, fire);
}

/* 炮弹移动 > 玩家移动 > 玩家开火 > 鬼移动
 * */
