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

#include "modules/strategy.h"

using std::vector;
using std::pair;
using std::make_pair;

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

static op_t normal_algorithm(class server_data &m, spjdgs_t &spjdg_ops) {
    const int old_score = m.get_my_score();
#ifdef ALGORITHM_DEBUG_ALGORITHM
    for (int i = 0; i < spjdg_ops.size(); i++) {
        printf("special judge: %d %d %lf\n", spjdg_ops[i].move_op,
                spjdg_ops[i].is_fire, spjdg_ops[i].trend);
    }
#endif

    mop_t optimal_move_op;
    bool optimal_is_fire;
    double max_sp = -1e9;
    // sp: situation point 局势分
    // s: score
    auto update = [&](int s, double sp, mop_t move_op, bool is_fire) -> void {
        for (int i = 0; i < spjdg_ops.size(); i++) {
            bool is_same_action = true;
            is_same_action = is_same_action && spjdg_ops[i].move_op == move_op;
            is_same_action = is_same_action && spjdg_ops[i].is_fire == is_fire;

            if (is_same_action) {
#ifdef ALGORITHM_DEBUG_ALGORITHM
                printf("special judge! %lf\n", spjdg_ops[i].trend);
#endif

                sp += spjdg_ops[i].trend;
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

struct strategy ghost_strategys[] = {
// 相邻
    // 不动射击
    { -1,  0,  'w', 'G', move_op_stay,  true,  +300, },
    { +1,  0,  's', 'G', move_op_stay,  true,  +300, },
    {  0, -1,  'a', 'G', move_op_stay,  true,  +300, },
    {  0, +1,  'd', 'G', move_op_stay,  true,  +300, },
    // 转向射击
    { -1,  0, -'w', 'G', move_op_up,    true,  +300, },
    { +1,  0, -'s', 'G', move_op_down,  true,  +300, },
    {  0, -1, -'a', 'G', move_op_left,  true,  +300, },
    {  0, +1, -'d', 'G', move_op_right, true,  +300, },
    // 惩罚：移动到幽灵上
    { -1,  0,  'w', 'G', move_op_up,    false, -999, },
    { +1,  0,  's', 'G', move_op_down,  false, -999, },
    { 0,  -1,  'a', 'G', move_op_left,  false, -999, },
    { 0,  +1,  'd', 'G', move_op_right, false, -999, },
    { -1,  0,  'w', 'G', move_op_up,    true,  -999, },
    { +1,  0,  's', 'G', move_op_down,  true,  -999, },
    { 0,  -1,  'a', 'G', move_op_left,  true,  -999, },
    { 0,  +1,  'd', 'G', move_op_right, true,  -999, },
// 隔一格
    // 不动射击
    { -2,  0,  'w', 'G', move_op_stay,  true,  +100, },
    { +2,  0,  's', 'G', move_op_stay,  true,  +100, },
    {  0, -2,  'a', 'G', move_op_stay,  true,  +100, },
    {  0, +2,  'd', 'G', move_op_stay,  true,  +100, },
    // 移动射击
    { -2,  0,  'w', 'G', move_op_up,    true,  +100, },
    { +2,  0,  's', 'G', move_op_down,  true,  +100, },
    {  0, -2,  'a', 'G', move_op_left,  true,  +100, },
    {  0, +2,  'd', 'G', move_op_right, true,  +100, },
    // 转向射击
    { -2,  0, -'w', 'G', move_op_up,    true,  +100, },
    { +2,  0, -'s', 'G', move_op_down,  true,  +100, },
    {  0, -2, -'a', 'G', move_op_left,  true,  +100, },
    {  0, +2, -'d', 'G', move_op_right, true,  +100, },
// 对角线
    // 转向水平射击
    { -1, -1, -'a', 'G', move_op_left,  true,  +50,  },
    { +1, -1, -'a', 'G', move_op_left,  true,  +50,  },
    { -1, +1, -'d', 'G', move_op_right, true,  +50,  },
    { +1, +1, -'d', 'G', move_op_right, true,  +50,  },
    // 不动射击
    { -1, -1,  'a', 'G', move_op_stay,  true,  +50,  },
    { +1, -1,  'a', 'G', move_op_stay,  true,  +50,  },
    { -1, +1,  'd', 'G', move_op_stay,  true,  +50,  },
    { +1, +1,  'd', 'G', move_op_stay,  true,  +50,  },
    // 惩罚：移动到幽灵攻击范围里
    { -1, -1,  'a', 'G', move_op_left,  false, -999,  },
    { -1, -1,  'w', 'G', move_op_up,    false, -999,  },
    { +1, -1,  'a', 'G', move_op_left,  false, -999,  },
    { +1, -1,  's', 'G', move_op_down,  false, -999,  },
    { -1, +1,  'd', 'G', move_op_right, false, -999,  },
    { -1, +1,  'w', 'G', move_op_up,    false, -999,  },
    { +1, +1,  'd', 'G', move_op_right, false, -999,  },
    { +1, +1,  's', 'G', move_op_down,  false, -999,  },
    { -1, -1,  'a', 'G', move_op_left,  true,  -999,  },
    { -1, -1,  'w', 'G', move_op_up,    true,  -999,  },
    { +1, -1,  'a', 'G', move_op_left,  true,  -999,  },
    { +1, -1,  's', 'G', move_op_down,  true,  -999,  },
    { -1, +1,  'd', 'G', move_op_right, true,  -999,  },
    { -1, +1,  'w', 'G', move_op_up,    true,  -999,  },
    { +1, +1,  'd', 'G', move_op_right, true,  -999,  },
    { +1, +1,  's', 'G', move_op_down,  true,  -999,  },
};

struct strategy bullet_strategys[] = {
/*
     1
   2 3 4
 5 6 w 7 8
   9 a b
     c
*/
// 1: v(---w)
    { -2,  0,  'w', 'v', move_op_up,    false, -300, },
    { +2,  0,  's', '^', move_op_down,  false, -300, },
    {  0, -2,  'a', '>', move_op_left,  false, -300, },
    {  0, +2,  'd', '<', move_op_right, false, -300, },
// 2: >(---w, ---wv)
    { -1, -1,  'w', '>', move_op_up,    false, -300, },
    { +1, +1,  's', '<', move_op_down,  false, -300, },
    { +1, -1,  'a', '^', move_op_left,  false, -300, },
    { -1, +1,  'd', 'v', move_op_right, false, -300, },

    { -1, -1,  'w', '>', move_op_up,    true,  -300, },
    { +1, +1,  's', '<', move_op_down,  true,  -300, },
    { +1, -1,  'a', '^', move_op_left,  true,  -300, },
    { -1, +1,  'd', 'v', move_op_right, true,  -300, },
// 3: v(++v), (<, >)(---w, ---wv)
    { -1,  0,  'w', 'v', move_op_stay,  true,  +200, },
    { +1,  0,  's', '^', move_op_stay,  true,  +200, },
    {  0, -1,  'a', '>', move_op_stay,  true,  +200, },
    {  0, +1,  'd', '<', move_op_stay,  true,  +200, },

    { -1,  0,  'w', '<', move_op_up,    false, -300, },
    { +1,  0,  's', '>', move_op_down,  false, -300, },
    {  0, -1,  'a', 'v', move_op_left,  false, -300, },
    {  0, +1,  'd', '^', move_op_right, false, -300, },

    { -1,  0,  'w', '<', move_op_up,    true,  -300, },
    { +1,  0,  's', '>', move_op_down,  true,  -300, },
    {  0, -1,  'a', 'v', move_op_left,  true,  -300, },
    {  0, +1,  'd', '^', move_op_right, true,  -300, },

    { -1,  0,  'w', '>', move_op_up,    false, -300, },
    { +1,  0,  's', '<', move_op_down,  false, -300, },
    {  0, -1,  'a', '^', move_op_left,  false, -300, },
    {  0, +1,  'd', 'v', move_op_right, false, -300, },

    { -1,  0,  'w', '>', move_op_up,    true,  -300, },
    { +1,  0,  's', '<', move_op_down,  true,  -300, },
    {  0, -1,  'a', '^', move_op_left,  true,  -300, },
    {  0, +1,  'd', 'v', move_op_right, true,  -300, },
// 4: <(---w, ---wv)
    { -1, +1,  'w', '<', move_op_up,    false, -300, },
    { +1, -1,  's', '>', move_op_down,  false, -300, },
    { -1, -1,  'a', 'v', move_op_left,  false, -300, },
    { +1, +1,  'd', '^', move_op_right, false, -300, },

    { -1, +1,  'w', '<', move_op_up,    true,  -300, },
    { +1, -1,  's', '>', move_op_down,  true,  -300, },
    { -1, -1,  'a', 'v', move_op_left,  true,  -300, },
    { +1, +1,  'd', '^', move_op_right, true,  -300, },
// 5: >(+av)
    {  0, -2,  'w', '>', move_op_left,  true,  +100, },
    {  0, +2,  's', '<', move_op_right, true,  +100, },
    { +2,  0,  'a', '^', move_op_down,  true,  +100, },
    { -2,  0,  'd', 'v', move_op_up,    true,  +100, },
// 6: >(+++av)
    {  0, -1,  'w', '>', move_op_left,  true,  +300, },
    {  0, +1,  's', '<', move_op_right, true,  +300, },
    { +1,  0,  'a', '^', move_op_down,  true,  +300, },
    { -1,  0,  'd', 'v', move_op_up,    true,  +300, },
// 7: <(+++dv)
    {  0, +1,  'w', '<', move_op_right, true,  +300, },
    {  0, -1,  's', '>', move_op_left,  true,  +300, },
    { -1,  0,  'a', 'v', move_op_up,    true,  +300, },
    { +1,  0,  'd', '^', move_op_down,  true,  +300, },
// 8: <(+dv)
    {  0, +2,  'w', '<', move_op_right, true,  +100, },
    {  0, -2,  's', '>', move_op_left,  true,  +100, },
    { -2,  0,  'a', 'v', move_op_up,    true,  +100, },
    { +2,  0,  'd', '^', move_op_down,  true,  +100, },
// 9:
// a: ^(---v, ---, +sv)
    { +1,  0,  'w', '^', move_op_stay,  false, -300, },
    { -1,  0,  's', 'v', move_op_stay,  false, -300, },
    { 0,  +1,  'a', '<', move_op_stay,  false, -300, },
    { 0,  -1,  'd', '>', move_op_stay,  false, -300, },

    { +1,  0,  'w', '^', move_op_stay,  true,  -300, },
    { -1,  0,  's', 'v', move_op_stay,  true,  -300, },
    { 0,  +1,  'a', '<', move_op_stay,  true,  -300, },
    { 0,  -1,  'd', '>', move_op_stay,  true,  -300, },

    { +1,  0,  'w', '^', move_op_down,  true,  +100, },
    { -1,  0,  's', 'v', move_op_up,    true,  +100, },
    { 0,  +1,  'a', '<', move_op_right, true,  +100, },
    { 0,  -1,  'd', '>', move_op_left,  true,  +100, },
// b:
// c: ^(+sv)
    { +2,  0,  'w', '^', move_op_down,  true,  +100, },
    { -2,  0,  's', 'v', move_op_up,    true,  +100, },
    { 0,  +2,  'a', '<', move_op_right, true,  +100, },
    { 0,  -2,  'd', '>', move_op_left,  true,  +100, },
};

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
static spjdgs_t special_algorithm(class server_data &m, int dmin, int dmax) {
    spjdgs_t special_ops;

    int r, c;
    m.get_my_pos(r, c);

    for (int dr = -dmax; dr <= dmax; dr++) {
        for (int dc = -dmax; dc <= dmax; dc++) {
            bool is_pass = dist_bound_check_pass(m, r, c, dr, dc, dmin, dmax);
            if (!is_pass) {
                continue;
            }

            char ch = m.get(r + dr, c + dc);

            if (is_ghost(ch)) {
                auto& s = ghost_strategys;
                int len = sizeof(s) / sizeof(*s);
                exec_strategys(s, len, special_ops, m, dr, dc);
            }

            if (is_player(ch)) {
                struct strategy *s;
                int len  = new_and_load_strategys("config/player.json", s);
                exec_strategys(s, len, special_ops, m, dr, dc);
                delete []s;
            }

            if (is_bullet(ch)) {
                auto&s = bullet_strategys;
                int len = sizeof(s) / sizeof(*s);
                exec_strategys(s, len, special_ops, m, dr, dc);
            }
        }
    }

    return special_ops;
}

op_t algorithm(class server_data &m) {
    auto spjdg_ops = special_algorithm(m, 1, 3);

    auto ret = normal_algorithm(m, spjdg_ops);

    return ret;
}


/* 炮弹移动 > 玩家移动 > 玩家开火 > 鬼移动 */
