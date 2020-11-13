// #define ALGORITHM_DEBUG_MEMSET
// #define ALGORITHM_DEBUG_DIST
// #define ALGORITHM_DEBUG_DIST_GRAPH
// #define ALGORITHM_DEBUG_MOVE_VAL
// #define ALGORITHM_DEBUG_ADD_VAL
// #define ALGORITHM_DEBUG_CHAR_GRAPH
// #define ALGORITHM_DEBUG_ALGORITHM
#include "modules/algorithm.h"
#include <stdio.h>
#include <queue>
#include <cmath>
#include <string.h>

int evaluate(class server_data &m, double &score)
{
	int line_size = m.get_map_size();
	int map_size = line_size * line_size;

	// 定义位置函数
	auto get_pos = [&](int row, int col) -> int {
		return row * line_size + col;
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
#ifdef ALGORITHM_DEBUG_MEMSET
	for (int i = 0; i < map_size; i++) dist_graph[i] = 999;
#else
	memset(dist_graph, 0x7f, map_size * sizeof(int));
#endif
	auto get_dist = [&](int row, int col) -> int {
		if (get_pos(row, col) >= map_size) printf("get dist %d\n", get_pos(row, col));
		if (get_pos(row, col) < 0) printf("get dist %d\n", get_pos(row, col));
		return dist_graph[get_pos(row, col)];
	};
	auto set_dist = [&](int row, int col, int dist) -> void {
		int pos = get_pos(row, col);
		if (pos < 0 || pos >= map_size) {
			printf("set_dist %d, %d = %d\n", row, col, dist);
			printf("set dist %d\n", get_pos(row, col));
			return;
		}
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
#ifdef ALGORITHM_DEBUG_DIST
			printf("target: %d, %d\n", tr, tc);
#endif
			// 不能是墙
			bool have_value = m.get(tr, tc) != '9';
			// 距离要更优
			int move_dist = node.dist + 1 + (node.dir != dir);
			if (tr < 0 || tr >= line_size) {
				return;
			}
			if (tc < 0 || tc >= line_size) {
				return;
			}
			have_value = have_value && move_dist <= get_dist(tr, tc);
#ifdef ALGORITHM_DEBUG_DIST
			printf("have: %d\n", have_value);
#endif
			if (have_value) {
				pq.push(qnode(tr, tc, dir, move_dist));
			}
		};

		check_move('w');
		check_move('a');
		check_move('s');
		check_move('d');
	}

#ifdef ALGORITHM_DEBUG_DIST_GRAPH
	printf("dist_graph:\n");
	for (int i = 0; i < line_size; i++) {
		for (int j = 0; j < line_size; j++) {
			printf("%3d, ", dist_graph[i * line_size + j]);
		}
		printf("\n");
	}
#endif

	// 计算移动价值图
	double *move_val = new double[map_size];
	memset(move_val, 0, map_size * sizeof(double));
	auto get_move_val = [&](int row, int col) -> double {
		if (get_pos(row, col) >= map_size) printf("get move %d\n", get_pos(row, col));
		if (get_pos(row, col) < 0) printf("get move %d\n", get_pos(row, col));
		return move_val[get_pos(row, col)];
	};
	auto add_move_val = [&](int row, int col, double val) -> void {
#ifdef ALGORITHM_DEBUG_ADD_VAL
		printf("set move (%d, %d) += %lf\n", row, col, val);
#endif
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
					add_move_val(i, j, (ch - '0') * 1.0 / get_dist(i, j));
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

				case 'G':
					for (int di = i - 3; di <= i + 3; di++) {
						for (int dj = j - 3; dj <= j + 3; dj++) {
							int dist = get_manhattan_dist(i, j, di, dj);
							if (dist <= 3 && dist > 0) {
								add_move_val(di, dj, -25.0 / dist);
							}
						}
					}
					break;
			}
		}
	}

#ifdef ALGORITHM_DEBUG_MOVE_VAL
	printf("move_val:\n");
	for (int i = 0; i < line_size; i++) {
		for (int j = 0; j < line_size; j++) {
			printf("%.2lf\t", move_val[i * line_size + j]);
		}
		printf("\n");
	}
#endif

#ifdef ALGORITHM_DEBUG_CHAR_GRAPH
	printf("char_graph:\n");
	for (int i = 0; i < line_size; i++) {
		for (int j = 0; j < line_size; j++) {
			printf("%c ", m.get(i, j));
		}
		printf("\n");
	}
#endif

	score = 0;
	for (int i = 0; i < line_size; i++) {
		for (int j = 0; j < line_size; j++) {
			if (get_dist(i, j)) {
				score += get_move_val(i, j);
			}
		}
	}

	delete []dist_graph;
	delete []move_val;

	return 0;
}

std::pair<enum move_operating, bool> algorithm(class server_data &m)
{
	const int old_score = m.get_my_score();

	enum move_operating optimal_move_op;
	bool optimal_is_fire;
	int max_score = 0;
	// sp: situation point 局势分
	double max_sp = -1e9;
	auto update = [&](int score, double sp, enum move_operating move_op, bool is_fire) -> void {
		if (max_score < score) {
			max_score = score;
			max_sp = sp;
			optimal_move_op = move_op;
			optimal_is_fire = is_fire;
		} else if (max_score == score && max_sp < sp) {
			max_sp = sp;
			optimal_move_op = move_op;
			optimal_is_fire = is_fire;
		}
	};

	auto normal_evaluate = [&](enum move_operating move_op) -> void {
		double sp;
		class server_data cloned = m.clone();
		int ret = cloned.move(move_op);
		if (ret) {
			return;
		}
#ifdef ALGORITHM_DEBUG_ALGORITHM
		printf("%d, 0\n", move_op);
#endif

		evaluate(cloned, sp);

#ifdef ALGORITHM_DEBUG_ALGORITHM
		printf("score: %d, point: %lf\n\n", cloned.get_my_score() - old_score, sp);
#endif

		update(cloned.get_my_score() - old_score, sp, move_op, false);

		ret = cloned.fire();
		if (ret) {
			return;
		}
#ifdef ALGORITHM_DEBUG_ALGORITHM
		printf("%d, 1\n", move_op);
#endif

		evaluate(cloned, sp);

#ifdef ALGORITHM_DEBUG_ALGORITHM
		printf("score: %d, point: %lf\n\n", cloned.get_my_score() - old_score, sp);
#endif

		update(cloned.get_my_score() - old_score, sp, move_op, true);
	};

	normal_evaluate(move_op_up);
	normal_evaluate(move_op_down);
	normal_evaluate(move_op_left);
	normal_evaluate(move_op_right);
	normal_evaluate(move_op_stay);

	return std::make_pair(optimal_move_op, optimal_is_fire);
}

/* 炮弹移动 > 玩家移动 > 玩家开火 > 鬼移动 */
