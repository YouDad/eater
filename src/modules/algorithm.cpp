// Copyright 2020 <luwh0708@thundersoft.com>
#include <stdio.h>
#include <string.h>

#include <queue>
#include <cmath>
#include <vector>
#include <algorithm>

#include "modules/algorithm.h"

// #define ALGORITHM_DEBUG_MEMSET
// #define ALGORITHM_DEBUG_DIST
// #define ALGORITHM_DEBUG_DIST_GRAPH
// #define ALGORITHM_DEBUG_MOVE_VAL_GRAPH
// #define ALGORITHM_DEBUG_ADD_VAL
// #define ALGORITHM_DEBUG_CHAR_GRAPH
// #define ALGORITHM_DEBUG_ALGORITHM

using std::vector;
using std::pair;
using std::make_pair;
typedef enum move_operating mop_t;
typedef pair<mop_t, bool> op_t;
typedef vector<op_t> ops_t;
typedef ops_t(* special_func_t)(class server_data &m, int dr, int dc, char ch);

static bool is_wall(char ch) {
	return ch == '9';
}

static bool is_fruit(char ch) {
	return ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5';
}

static bool is_bullet(char ch) {
	return ch == '<' || ch == '>' || ch == '^' || ch == 'v';
}

static bool is_player(char ch) {
	return ch == 'w' || ch == 'a' || ch == 's' || ch == 'd';
}

static bool is_ghost(char ch) {
	return ch == 'G';
}

int evaluate(class server_data &m, double &score) {
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
		if (get_pos(row, col) >= map_size) {
			printf("get dist %d\n", get_pos(row, col));
		}
		if (get_pos(row, col) < 0) {
			printf("get dist %d\n", get_pos(row, col));
		}
		return dist_graph[get_pos(row, col)];
	};

	auto set_dist = [&](int row, int col, int dist) -> void {
		int pos = get_pos(row, col);
		if (pos < 0 || pos >= map_size) {
			printf("set_dist %d, %d = %d\n", row, col, dist);
			printf("set dist %d\n", get_pos(row, col));
			return;
		}
		if (dist_graph[get_pos(row, col)] > dist) {
			dist_graph[get_pos(row, col)] = dist;
		}
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
			bool have_value = !is_wall(m.get(tr, tc));

			// 距离要更优
			int move_dist = node.dist + 1 + (node.dir != dir);

			// 边界检查
			if (tr < 0 || tr >= line_size) {
				return;
			}
			if (tc < 0 || tc >= line_size) {
				return;
			}

			have_value = have_value && (move_dist <= get_dist(tr, tc));

#ifdef ALGORITHM_DEBUG_DIST
			printf("have: %d, %d <= %d\n",
					have_value, move_dist, get_dist(tr, tc));
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
		if (get_pos(row, col) >= map_size) {
			printf("get move %d\n", get_pos(row, col));
		}
		if (get_pos(row, col) < 0) {
			printf("get move %d\n", get_pos(row, col));
		}
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

	// 计算曼哈顿距离
	auto get_m_dist = [](int i, int j, int r, int c) -> int {
		return abs(i - r) + abs(j - c);
	};
	auto get_fruit_val = [&](int i, int j) -> double {
		char ch = m.get(i, j);
		if ('1' <= ch && ch <= '5') {
			return (ch - '0') / (get_dist(i, j) + 0.45);
		}
		return 0;
	};

	for (int r = 0; r < line_size; r++) {
		for (int c = 0; c < line_size; c++) {
			char ch = m.get(r, c);
			if (is_fruit(ch)) {
				add_move_val(r, c, get_fruit_val(r, c));
			}

			if (is_player(ch)) {
				if (!(r == origin_row && c == origin_col)) {
					for (int dr = r - 2; dr <= r + 2; dr++) {
						for (int dc = c - 2; dc <= c + 2; dc++) {
							add_move_val(dr, dc, -move_val[get_pos(dr, dc)]);
							add_move_val(dr, dc, -get_fruit_val(dr, dc));
						}
					}
				}
			}

			switch (ch) {
				case '<':
					for (int i = c - 1; i >= 0; i--) {
						if (m.get(r, i) != '9') {
							add_move_val(r, i, -25.0 / (c - i) / (c - i));
						} else {
							break;
						}
					}
					break;

				case '^':
					for (int i = r - 1; i >= 0; i--) {
						if (m.get(i, c) != '9') {
							add_move_val(i, c, -25.0 / (r - i) / (r - i));
						} else {
							break;
						}
					}
					break;

				case '>':
					for (int i = c + 1; i < line_size; i++) {
						if (m.get(r, i) != '9') {
							add_move_val(r, i, -25.0 / (i - c) / (i - c));
						} else {
							break;
						}
					}
					break;

				case 'v':
					for (int i = r + 1; i < line_size; i++) {
						if (m.get(i, c) != '9') {
							add_move_val(i, c, -25.0 / (i - r) / (i - r));
						} else {
							break;
						}
					}
					break;
			}

			if (is_ghost(ch)) {
				for (int dr = r - 3; dr <= r + 3; dr++) {
					for (int dc = c - 3; dc <= c + 3; dc++) {
						int dist = get_m_dist(r, c, dr, dc);
						if (0 < dist && dist <= 3) {
							add_move_val(dr, dc, -get_fruit_val(dr, dc));
						}
					}
				}
				{
					int dist = get_m_dist(r, c, origin_row, origin_col);
					if (dist <= 3) {
						add_move_val(r, c, -25.0 / dist);
					}
				}
			}
		}
	}

#ifdef ALGORITHM_DEBUG_MOVE_VAL_GRAPH
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
			if (i == origin_row && j == origin_col) {
				printf("\033[31m%c \033[0m", m.get(i, j));
			} else {
				printf("%c ", m.get(i, j));
			}
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

static op_t normal_algorithm(class server_data &m, ops_t &dops, ops_t &rops) {
	const int old_score = m.get_my_score();
#ifdef ALGORITHM_DEBUG_ALGORITHM
	for (int i = 0; i < dops.size(); i++) {
		printf("dangerous: %d %d\n", dops[i].first, dops[i].second);
	}
#endif

	mop_t optimal_move_op;
	bool optimal_is_fire;
	double max_sp = -1e9;
	// sp: situation point 局势分
	// s: score
	auto update = [&](int s, double sp, mop_t move_op, bool is_fire) -> void {
		// skip dangerous operating
		for (int i = 0; i < dops.size(); i++) {
			if (dops[i].first == move_op && dops[i].second == is_fire) {
#ifdef ALGORITHM_DEBUG_ALGORITHM
				printf("skip!\n");
#endif
				return;
			}
		}

		for (int i = 0; i < rops.size(); i++) {
			if (rops[i].first == move_op && rops[i].second == is_fire) {
#ifdef ALGORITHM_DEBUG_ALGORITHM
				printf("recommand!\n");
#endif
				sp += 100;
			}
		}

		if (s > 0) {
			sp += s * 2.5;
		} else {
			sp += s * 1.0;
		}
		if (max_sp < sp) {
#ifdef ALGORITHM_DEBUG_ALGORITHM
			printf("update!\n");
#endif
			max_sp = sp;
			optimal_move_op = move_op;
			optimal_is_fire = is_fire;
		}
	};

	auto normal_evaluate = [&](mop_t move_op, double punishment) -> void {
		double sp;
		class server_data cloned = m.clone();
		int ret = cloned.move(move_op);
		if (ret) {
			return;
		}
#ifdef ALGORITHM_DEBUG_ALGORITHM
		printf("\n%d, 0\n", move_op);
#endif

		evaluate(cloned, sp);
		sp -= punishment;

#ifdef ALGORITHM_DEBUG_ALGORITHM
		int delta_score = cloned.get_my_score() - old_score;
		printf("score: %d, point: %lf\n", delta_score, sp);
#endif

		update(cloned.get_my_score() - old_score, sp, move_op, false);

		ret = cloned.fire();
		if (ret) {
			return;
		}
#ifdef ALGORITHM_DEBUG_ALGORITHM
		printf("\n%d, 1\n", move_op);
#endif

		evaluate(cloned, sp);
		sp -= punishment;

#ifdef ALGORITHM_DEBUG_ALGORITHM
		delta_score = cloned.get_my_score() - old_score;
		printf("score: %d, point: %lf\n", delta_score, sp);
#endif

		update(cloned.get_my_score() - old_score, sp, move_op, true);
	};

	normal_evaluate(move_op_stay, 0.5);
	normal_evaluate(move_op_up, 0);
	normal_evaluate(move_op_down, 0);
	normal_evaluate(move_op_left, 0);
	normal_evaluate(move_op_right, 0);

	return make_pair(optimal_move_op, optimal_is_fire);
}

static ops_t process_player_danger(class server_data &m, int dr, int dc) {
	ops_t dangerous_ops;
	int r, c;
	m.get_my_pos(r, c);

	char player = m.get(r + dr, c + dc);
	char we = m.get(r, c);

	// 相邻
	if (abs(dr) + abs(dc) == 1) {
		if (abs(dr)) {
			dangerous_ops.push_back(make_pair(move_op_up, false));
			dangerous_ops.push_back(make_pair(move_op_down, false));
		}

		if (abs(dc)) {
			dangerous_ops.push_back(make_pair(move_op_left, false));
			dangerous_ops.push_back(make_pair(move_op_right, false));
		}
	}

	// 隔一格
	if (abs(dr) == 2 || abs(dc) == 2) {
		bool facing = false;
		facing |= we == 'w' && dr == -2;
		facing |= we == 's' && dr == 2;
		facing |= we == 'a' && dc == -2;
		facing |= we == 'd' && dc == 2;

		facing |= we == 'w' && dr == 2  && player == we;
		facing |= we == 's' && dr == -2 && player == we;
		facing |= we == 'a' && dc == 2  && player == we;
		facing |= we == 'd' && dc == -2 && player == we;

		if (facing) {
			dangerous_ops.push_back(make_pair(move_op_stay, false));
			dangerous_ops.push_back(make_pair(move_op_up, false));
			dangerous_ops.push_back(make_pair(move_op_down, false));
			dangerous_ops.push_back(make_pair(move_op_left, false));
			dangerous_ops.push_back(make_pair(move_op_right, false));
		}

		bool facing_each_other = false;
		if (we == 'w' && dr == -2 && player == 's') {
			dangerous_ops.push_back(make_pair(move_op_up, true));
		}
		if (we == 's' && dr == 2 && player == 'w') {
			dangerous_ops.push_back(make_pair(move_op_down, true));
		}
		if (we == 'a' && dc == -2 && player == 'd') {
			dangerous_ops.push_back(make_pair(move_op_left, true));
		}
		if (we == 'd' && dc == 2 && player == 'a') {
			dangerous_ops.push_back(make_pair(move_op_right, true));
		}
	}

	// 对角线
	if (abs(dr) == 1 && abs(dc) == 1) {
		auto vertical_op = make_pair(move_op_down, false);
		if (dr < 0) {
			vertical_op.first = move_op_up;
		}
		dangerous_ops.push_back(vertical_op);

		auto horizontal_op = make_pair(move_op_right, false);
		if (dc < 0) {
			horizontal_op.first = move_op_left;
		}
		dangerous_ops.push_back(horizontal_op);
	}

	return dangerous_ops;
}

static ops_t process_player_recommand(class server_data &m, int dr, int dc) {
	ops_t recommand_ops;
	return recommand_ops;
}

static ops_t process_ghost_danger(class server_data &m, int dr, int dc) {
	ops_t dangerous_ops;

	if (dr != 0) {
		auto op = make_pair(move_op_down, false);

		if (dr < 0) {
			op.first = move_op_up;
		}

		dangerous_ops.push_back(op);
	}

	if (dc != 0) {
		auto op = make_pair(move_op_right, false);

		if (dc < 0) {
			op.first = move_op_left;
		}

		dangerous_ops.push_back(op);
	}

	return dangerous_ops;
}

static ops_t process_ghost_recommand(class server_data &m, int dr, int dc) {
	ops_t recommand_ops;
	int r, c;
	m.get_my_pos(r, c);

	if (dr == 2 && dc == 0) {
		auto op = make_pair(move_op_down, true);

		if (dr < 0) {
			op.first = move_op_up;
		}

		recommand_ops.push_back(op);
	}

	if (0 < dc && dc <= 2) {
		auto op = make_pair(move_op_right, true);

		if (m.get(r, c) == 'd') {
			op.first = move_op_stay;
		}

		recommand_ops.push_back(op);
	}

	if (0 > dc && dc >= -2) {
		auto op = make_pair(move_op_left, true);

		if (m.get(r, c) == 'a') {
			op.first = move_op_stay;
		}

		recommand_ops.push_back(op);
	}

	return recommand_ops;
}

static bool dist_bound_check_pass(class server_data &m,
		int r, int c, int dr, int dc, int min_dist, int max_dist) {
	// 曼哈顿距离[min_dist, max_dist]
	int dist = abs(dr) + abs(dc);
	if (min_dist > dist || dist > max_dist) {
		return false;
	}

	// 边界检查
	if (0 > r + dr || r + dr >= m.get_map_size()) {
		return false;
	}

	if (0 > c + dc || c + dc >= m.get_map_size()) {
		return false;
	}
	return true;
}

// dmin: min dist
// dmax: max dist
static ops_t special_algorithm(class server_data &m,
		int dmin, int dmax, special_func_t func) {
	ops_t special_ops;

	int r, c;
	m.get_my_pos(r, c);

	for (int dr = -dmax; dr <= dmax; dr++) {
		for (int dc = -dmax; dc <= dmax; dc++) {
			bool is_pass = dist_bound_check_pass(m, r, c, dr, dc, dmin, dmax);
			if (!is_pass) {
				continue;
			}

			char ch = m.get(r + dr, c + dc);
			auto ret = func(m, dr, dc, ch);
			for (int i = 0; i < ret.size(); i++) {
				special_ops.push_back(ret[i]);
			}
		}
	}

	return special_ops;
}

op_t algorithm(class server_data &m) {
	auto dangerous_ops = special_algorithm(m, 1, 2, [](class server_data &m,
				int dr, int dc, char ch) -> ops_t {
		ops_t ops, ret;

		if (is_ghost(ch)) {
			ret = process_ghost_danger(m, dr, dc);
			for (int i = 0; i < ret.size(); i++) {
				ops.push_back(ret[i]);
			}
		}

		if (is_player(ch)) {
			ret = process_player_danger(m, dr, dc);
			for (int i = 0; i < ret.size(); i++) {
				ops.push_back(ret[i]);
			}
		}
		return ops;
	});

	auto recommand_ops = special_algorithm(m, 1, 2, [](class server_data &m,
				int dr, int dc, char ch) -> ops_t {
		ops_t ops, ret;

		if (is_ghost(ch)) {
			ret = process_ghost_recommand(m, dr, dc);
			for (int i = 0; i < ret.size(); i++) {
				ops.push_back(ret[i]);
			}
		}

		if (is_player(ch)) {
			ret = process_player_recommand(m, dr, dc);
			for (int i = 0; i < ret.size(); i++) {
				ops.push_back(ret[i]);
			}
		}
		return ops;
	});

	auto ret = normal_algorithm(m, dangerous_ops, recommand_ops);

	return ret;
}


/* 炮弹移动 > 玩家移动 > 玩家开火 > 鬼移动 */
