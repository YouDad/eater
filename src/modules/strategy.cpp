#include <vector>
#include <algorithm>

#include "modules/strategy.h"
#include "modules/json.h"

using std::vector;
using std::pair;
using std::make_pair;

void exec_strategys(struct strategy *strategys, int len,
        spjdgs_t &ops, class server_data &m, int dr, int dc) {
    int r, c;
    m.get_my_pos(r, c);
    char we = m.get(r, c);
    char player = m.get(r + dr, c + dc);

    for (int i = 0; i < len; i++) {
        auto& s = strategys[i];

        if (s.we > 0 && we != s.we) {
            continue;
        }
        if (s.we < 0 && we == -s.we) {
            continue;
        }

        if (s.player > 0 && player != s.player) {
            continue;
        }
        if (s.player < 0 && player == -s.player) {
            continue;
        }

        if (s.dr != dr || s.dc != dc) {
            continue;
        }

        ops.push_back(spjdg_t{s.move, s.is_fire, s.trend});
    }
}

static void rotate_position_clockwise(int &x, int &y) {
    int t = x;
    x = y;
    y = t;
    y = -y;
}

static char rotate_char_clockwise(char ch) {
    char chars[8] = "wdsaw";
    for (int i = 0; i < sizeof(chars); i++) {
        if (ch == chars[i]) {
            return chars[i + 1];
        }
    }

    strncpy(chars, "^>v<^", sizeof(chars));
    for (int i = 0; i < sizeof(chars); i++) {
        if (ch == chars[i]) {
            return chars[i + 1];
        }
    }

    chars[0] = -'w'; chars[1] = -'d';
    chars[2] = -'s'; chars[3] = -'a';
    chars[4] = -'w';
    for (int i = 0; i < sizeof(chars); i++) {
        if (ch == chars[i]) {
            return chars[i + 1];
        }
    }

    chars[0] = -'^'; chars[1] = -'>';
    chars[2] = -'v'; chars[3] = -'<';
    chars[4] = -'^';
    for (int i = 0; i < sizeof(chars); i++) {
        if (ch == chars[i]) {
            return chars[i + 1];
        }
    }

    return ch;
}

static mop_t rotate_move_op_clockwise(mop_t move_op) {
    mop_t ops[] = {move_op_up, move_op_right,
        move_op_down, move_op_left, move_op_up};

    for (int i = 0; i < sizeof(ops) / sizeof(*ops); i++) {
        if (move_op == ops[i]) {
            return ops[i + 1];
        }
    }

    return move_op;
}

static mop_t char_to_move_op(char move) {
    switch (move) {
        case ' ':
            return move_op_stay;

        case 'w':
            return move_op_up;

        case 's':
            return move_op_down;

        case 'a':
            return move_op_left;

        case 'd':
            return move_op_right;

        default:
            printf("'%c': %d\n", move, move);
            assert(0);
    }

    return move_op_stay;
}

int new_and_load_strategys(const char *filename, struct strategy *&strategys) {
    neb::CJsonObject json;

    bool ret = load_json(filename, json);
    if (!ret) {
        perror("load_json");
        return 0;
    }

    neb::CJsonObject array;
    if (!json.KeyExist("strategys")) {
        return 0;
    }
    if (!json.Get("strategys", array)) {
        return 0;
    }
    if (!array.IsArray()) {
        return 0;
    }

    strategys = new struct strategy[array.GetArraySize() * 4];
    int cnt = 0;

    std::string op;
    for (int i = 0; i < array.GetArraySize(); i++) {
        array.Get(i, op);

        int dr, dc;
        double trend;
        char player[4];
        char move, fire;
        char single = 0;
        mop_t move_op;
        int ret = sscanf(op.c_str(), "<%d, %d>{%[^}]}(%c%c)[%lf]%c",
                &dr, &dc, player, &move, &fire, &trend, &single);
        if (ret < 6) {
            continue;
        }

        if (player[0] == ' ') {
            player[0] = 0;
        }
        if (player[0] == '-') {
            player[0] = -player[1];
        }

        move_op = char_to_move_op(move);
        char we = 'w';
        for (int j = 0; j < 4; j++) {
            strategys[cnt].dr = dr;
            strategys[cnt].dc = dc;
            strategys[cnt].trend = trend;
            strategys[cnt].is_fire = fire == 'v';
            strategys[cnt].move = move_op;
            strategys[cnt].player = player[0];
            if (single) {
                strategys[cnt].we = single;
            } else {
                strategys[cnt].we = we;
            }

            rotate_position_clockwise(dr, dc);
            we = rotate_char_clockwise(we);
            player[0] = rotate_char_clockwise(player[0]);
            move_op = rotate_move_op_clockwise(move_op);
            cnt++;
            if (single) {
                break;
            }
        }
    }

    return cnt;
}
