#include "modules/strategy.h"
#include <stdio.h>

struct strategy _player_strategys[] = {
/*
       6
     7 3 a
   8 4 1 b c
 9 5 2 w d e f
   g h i j k
     l m n
       o
*/
// 1: v
// 2: w, wv
// 3: s(v), !s(-w, -wv)
// 4: -w, -wv, -a, -av
// 5: w, wv
// 6: -w, -wv
// 7: -a, -av
// 8: -w, -wv
// 9: -a, -av
// a: -d, -dv
// b: -d, -dv, -w, -wv
// c: -w, -wv
// d: w, wv
// e: w, wv
// f: -d, -dv
// g: -s, -sv
// h: -s, -sv, -a, -av
// i: sv
// j: -s, -sv, -d, -dv
// k: -s, -sv
// l: -a, -av
// m: w(w, wv), !w(-s, -sv)
// n: -d, -dv
// o: -s, -sv

// 1: v
    { -1,  0,  'w',    0, move_op_stay,  true,  +400, },
    { +1,  0,  's',    0, move_op_stay,  true,  +400, },
    {  0, -1,  'a',    0, move_op_stay,  true,  +400, },
    {  0, +1,  'd',    0, move_op_stay,  true,  +400, },

// 2: w, wv
    {  0, -1,  'w',    0, move_op_up,    false, +400, },
    {  0, +1,  's',    0, move_op_down,  false, +400, },
    { +1,  0,  'a',    0, move_op_left,  false, +400, },
    { -1,  0,  'd',    0, move_op_right, false, +400, },

    {  0, -1,  'w',    0, move_op_up,    true,  +400, },
    {  0, +1,  's',    0, move_op_down,  true,  +400, },
    { +1,  0,  'a',    0, move_op_left,  true,  +400, },
    { -1,  0,  'd',    0, move_op_right, true,  +400, },

// 3: s(v), !s(-w, -wv)
    { -2,  0,  'w',  's', move_op_stay,  true,  +400, },
    { +2,  0,  's',  'w', move_op_stay,  true,  +400, },
    {  0, -2,  'a',  'd', move_op_stay,  true,  +400, },
    {  0, +2,  'd',  'a', move_op_stay,  true,  +400, },

    { -2,  0,  'w', -'s', move_op_up,    false, -200, },
    { +2,  0,  's', -'w', move_op_down,  false, -200, },
    {  0, -2,  'a', -'d', move_op_left,  false, -200, },
    {  0, +2,  'd', -'a', move_op_right, false, -200, },

    { -2,  0,  'w', -'s', move_op_up,    true,  -200, },
    { +2,  0,  's', -'w', move_op_down,  true,  -200, },
    {  0, -2,  'a', -'d', move_op_left,  true,  -200, },
    {  0, +2,  'd', -'a', move_op_right, true,  -200, },

// 4: -w, -wv, -a, -av
    { -1, -1,  'w',    0, move_op_up,    false, -200, },
    { +1, +1,  's',    0, move_op_down,  false, -200, },
    { +1, -1,  'a',    0, move_op_left,  false, -200, },
    { -1, +1,  'd',    0, move_op_right, false, -200, },

    { -1, -1,  'w',    0, move_op_up,    true,  -200, },
    { +1, +1,  's',    0, move_op_down,  true,  -200, },
    { +1, -1,  'a',    0, move_op_left,  true,  -200, },
    { -1, +1,  'd',    0, move_op_right, true,  -200, },

    { -1, -1,  'w',    0, move_op_left,  false, -200, },
    { +1, +1,  's',    0, move_op_right, false, -200, },
    { +1, -1,  'a',    0, move_op_down,  false, -200, },
    { -1, +1,  'd',    0, move_op_up,    false, -200, },

    { -1, -1,  'w',    0, move_op_left,  true,  -200, },
    { +1, +1,  's',    0, move_op_right, true,  -200, },
    { +1, -1,  'a',    0, move_op_down,  true,  -200, },
    { -1, +1,  'd',    0, move_op_up,    true,  -200, },

// 5: w, wv
    {  0, -2,  'w',    0, move_op_up,    false, +400, },
    {  0, +2,  's',    0, move_op_down,  false, +400, },
    { +2,  0,  'a',    0, move_op_left,  false, +400, },
    { -2,  0,  'd',    0, move_op_right, false, +400, },

    {  0, -2,  'w',    0, move_op_up,    true,  +400, },
    {  0, +2,  's',    0, move_op_down,  true,  +400, },
    { +2,  0,  'a',    0, move_op_left,  true,  +400, },
    { -2,  0,  'd',    0, move_op_right, true,  +400, },

// 6: -w, -wv
    { -3,  0,  'w',    0, move_op_up,    false, -200, },
    { +3,  0,  's',    0, move_op_down,  false, -200, },
    {  0, -3,  'a',    0, move_op_left,  false, -200, },
    {  0, +3,  'd',    0, move_op_right, false, -200, },

    { -3,  0,  'w',    0, move_op_up,    true,  -200, },
    { +3,  0,  's',    0, move_op_down,  true,  -200, },
    {  0, -3,  'a',    0, move_op_left,  true,  -200, },
    {  0, +3,  'd',    0, move_op_right, true,  -200, },

// 7: -a, -av
    { -2, -1,  'w',    0, move_op_left,  false, -200, },
    { +2, +1,  's',    0, move_op_right, false, -200, },
    { +1, -2,  'a',    0, move_op_down,  false, -200, },
    { -1, +2,  'd',    0, move_op_up,    false, -200, },

    { -2, -1,  'w',    0, move_op_left,  true,  -200, },
    { +2, +1,  's',    0, move_op_right, true,  -200, },
    { +1, -2,  'a',    0, move_op_down,  true,  -200, },
    { -1, +2,  'd',    0, move_op_up,    true,  -200, },

// 8: -w, -wv
    { -1, -2,  'w',    0, move_op_up,    false, -200, },
    { +1, +2,  's',    0, move_op_down,  false, -200, },
    { +2, -1,  'a',    0, move_op_left,  false, -200, },
    { -2, +1,  'd',    0, move_op_right, false, -200, },

    { -1, -2,  'w',    0, move_op_up,    true,  -200, },
    { +1, +2,  's',    0, move_op_down,  true,  -200, },
    { +2, -1,  'a',    0, move_op_left,  true,  -200, },
    { -2, +1,  'd',    0, move_op_right, true,  -200, },

// 9: -a, -av
    {  0, -3,  'w',    0, move_op_left,  false, -200, },
    {  0, +3,  's',    0, move_op_right, false, -200, },
    { +3,  0,  'a',    0, move_op_down,  false, -200, },
    { -3,  0,  'd',    0, move_op_up,    false, -200, },

    {  0, -3,  'w',    0, move_op_left,  true,  -200, },
    {  0, +3,  's',    0, move_op_right, true,  -200, },
    { +3,  0,  'a',    0, move_op_down,  true,  -200, },
    { -3,  0,  'd',    0, move_op_up,    true,  -200, },

// a: -d, -dv
    { -2, +1,  'w',    0, move_op_right, false, -200, },
    { +2, -1,  's',    0, move_op_left,  false, -200, },
    { -1, -2,  'a',    0, move_op_up,    false, -200, },
    { +1, +2,  'd',    0, move_op_down,  false, -200, },

    { -2, +1,  'w',    0, move_op_right, true,  -200, },
    { +2, -1,  's',    0, move_op_left,  true,  -200, },
    { -1, -2,  'a',    0, move_op_up,    true,  -200, },
    { +1, +2,  'd',    0, move_op_down,  true,  -200, },

// b: -d, -dv, -w, -wv
    { -1, +1,  'w',    0, move_op_right, false, -200, },
    { +1, -1,  's',    0, move_op_left,  false, -200, },
    { -1, -1,  'a',    0, move_op_up,    false, -200, },
    { +1, +1,  'd',    0, move_op_down,  false, -200, },

    { -1, +1,  'w',    0, move_op_right, true,  -200, },
    { +1, -1,  's',    0, move_op_left,  true,  -200, },
    { -1, -1,  'a',    0, move_op_up,    true,  -200, },
    { +1, +1,  'd',    0, move_op_down,  true,  -200, },

    { -1, +1,  'w',    0, move_op_up,    false, -200, },
    { +1, -1,  's',    0, move_op_down,  false, -200, },
    { -1, -1,  'a',    0, move_op_left,  false, -200, },
    { +1, +1,  'd',    0, move_op_right, false, -200, },

    { -1, +1,  'w',    0, move_op_up,    true,  -200, },
    { +1, -1,  's',    0, move_op_down,  true,  -200, },
    { -1, -1,  'a',    0, move_op_left,  true,  -200, },
    { +1, +1,  'd',    0, move_op_right, true,  -200, },

// c: -w, -wv
    { -1, +2,  'w',    0, move_op_up,    false, -200, },
    { +1, -2,  's',    0, move_op_down,  false, -200, },
    { -2, -1,  'a',    0, move_op_left,  false, -200, },
    { +2, +1,  'd',    0, move_op_right, false, -200, },

    { -1, +2,  'w',    0, move_op_up,    true,  -200, },
    { +1, -2,  's',    0, move_op_down,  true,  -200, },
    { -2, -1,  'a',    0, move_op_left,  true,  -200, },
    { +2, +1,  'd',    0, move_op_right, true,  -200, },

// d: w, wv
    {  0, +1,  'w',    0, move_op_up,    false, +400, },
    {  0, -1,  's',    0, move_op_down,  false, +400, },
    {  -1, 0,  'a',    0, move_op_left,  false, +400, },
    {  +1, 0,  'd',    0, move_op_right, false, +400, },

    {  0, +1,  'w',    0, move_op_up,    true,  +400, },
    {  0, -1,  's',    0, move_op_down,  true,  +400, },
    {  -1, 0,  'a',    0, move_op_left,  true,  +400, },
    {  +1, 0,  'd',    0, move_op_right, true,  +400, },

// e: w, wv
    {  0, +2,  'w',    0, move_op_up,    false, +400, },
    {  0, -2,  's',    0, move_op_down,  false, +400, },
    {  -2, 0,  'a',    0, move_op_left,  false, +400, },
    {  +2, 0,  'd',    0, move_op_right, false, +400, },

    {  0, +2,  'w',    0, move_op_up,    true,  +400, },
    {  0, -2,  's',    0, move_op_down,  true,  +400, },
    {  -2, 0,  'a',    0, move_op_left,  true,  +400, },
    {  +2, 0,  'd',    0, move_op_right, true,  +400, },

// f: -d, -dv
    {  0, +3,  'w',    0, move_op_right, false, -200, },
    {  0, -3,  's',    0, move_op_left,  false, -200, },
    {  -3, 0,  'a',    0, move_op_up,    false, -200, },
    {  +3, 0,  'd',    0, move_op_down,  false, -200, },

    {  0, +3,  'w',    0, move_op_right, true,  -200, },
    {  0, -3,  's',    0, move_op_left,  true,  -200, },
    {  -3, 0,  'a',    0, move_op_up,    true,  -200, },
    {  +3, 0,  'd',    0, move_op_down,  true,  -200, },

// g: -s, -sv
    { +1, -2,  'w',    0, move_op_down,  false, -200, },
    { -1, +2,  's',    0, move_op_up,    false, -200, },
    { +2, +1,  'a',    0, move_op_right, false, -200, },
    { -2, -1,  'd',    0, move_op_left,  false, -200, },

    { +1, -2,  'w',    0, move_op_down,  true,  -200, },
    { -1, +2,  's',    0, move_op_up,    true,  -200, },
    { +2, +1,  'a',    0, move_op_right, true,  -200, },
    { -2, -1,  'd',    0, move_op_left,  true,  -200, },

// h: -s, -sv, -a, -av
    { +1, -1,  'w',    0, move_op_down,  false, -200, },
    { -1, +1,  's',    0, move_op_up,    false, -200, },
    { +1, +1,  'a',    0, move_op_right, false, -200, },
    { -1, -1,  'd',    0, move_op_left,  false, -200, },

    { +1, -1,  'w',    0, move_op_down,  true,  -200, },
    { -1, +1,  's',    0, move_op_up,    true,  -200, },
    { +1, +1,  'a',    0, move_op_right, true,  -200, },
    { -1, -1,  'd',    0, move_op_left,  true,  -200, },

    { +1, -1,  'w',    0, move_op_left,  false, -200, },
    { -1, +1,  's',    0, move_op_right, false, -200, },
    { +1, +1,  'a',    0, move_op_down,  false, -200, },
    { -1, -1,  'd',    0, move_op_up,    false, -200, },

    { +1, -1,  'w',    0, move_op_left,  true,  -200, },
    { -1, +1,  's',    0, move_op_right, true,  -200, },
    { +1, +1,  'a',    0, move_op_down,  true,  -200, },
    { -1, -1,  'd',    0, move_op_up,    true,  -200, },

// i: sv
    { +1,  0,  'w',    0, move_op_down,  true,  +400, },
    { -1,  0,  's',    0, move_op_up,    true,  +400, },
    {  0, +1,  'a',    0, move_op_right, true,  +400, },
    {  0, -1,  'd',    0, move_op_left,  true,  +400, },

// j: -s, -sv, -d, -dv
    { +1, +1,  'w',    0, move_op_down,  false, -200, },
    { -1, -1,  's',    0, move_op_up,    false, -200, },
    { -1, +1,  'a',    0, move_op_right, false, -200, },
    { +1, -1,  'd',    0, move_op_left,  false, -200, },

    { +1, +1,  'w',    0, move_op_down,  true,  -200, },
    { -1, -1,  's',    0, move_op_up,    true,  -200, },
    { -1, +1,  'a',    0, move_op_right, true,  -200, },
    { +1, -1,  'd',    0, move_op_left,  true,  -200, },

    { +1, +1,  'w',    0, move_op_right, false, -200, },
    { -1, -1,  's',    0, move_op_left,  false, -200, },
    { -1, +1,  'a',    0, move_op_up,    false, -200, },
    { +1, -1,  'd',    0, move_op_down,  false, -200, },

    { +1, +1,  'w',    0, move_op_right, true,  -200, },
    { -1, -1,  's',    0, move_op_left,  true,  -200, },
    { -1, +1,  'a',    0, move_op_up,    true,  -200, },
    { +1, -1,  'd',    0, move_op_down,  true,  -200, },

// k: -s, -sv
    { +1, +2,  'w',    0, move_op_down,  false, -200, },
    { -1, -2,  's',    0, move_op_up,    false, -200, },
    { -2, +1,  'a',    0, move_op_right, false, -200, },
    { +2, -1,  'd',    0, move_op_left,  false, -200, },

    { +1, +2,  'w',    0, move_op_down,  true,  -200, },
    { -1, -2,  's',    0, move_op_up,    true,  -200, },
    { -2, +1,  'a',    0, move_op_right, true,  -200, },
    { +2, -1,  'd',    0, move_op_left,  true,  -200, },

// l: -a, -av
    { +2, -1,  'w',    0, move_op_left,  false, -200, },
    { -2, +1,  's',    0, move_op_right, false, -200, },
    { +1, +2,  'a',    0, move_op_down,  false, -200, },
    { -1, -2,  'd',    0, move_op_up,    false, -200, },

    { +2, -1,  'w',    0, move_op_left,  true,  -200, },
    { -2, +1,  's',    0, move_op_right, true,  -200, },
    { +1, +2,  'a',    0, move_op_down,  true,  -200, },
    { -1, -2,  'd',    0, move_op_up,    true,  -200, },

// m: w(w, wv), !w(-s, -sv)
    { +2,  0,  'w',  'w', move_op_up,    false, +400, },
    { -2,  0,  's',  's', move_op_down,  false, +400, },
    {  0, +2,  'a',  'a', move_op_left,  false, +400, },
    {  0, -2,  'd',  'd', move_op_right, false, +400, },

    { +2,  0,  'w',  'w', move_op_up,    true,  +400, },
    { -2,  0,  's',  's', move_op_down,  true,  +400, },
    {  0, +2,  'a',  'a', move_op_left,  true,  +400, },
    {  0, -2,  'd',  'd', move_op_right, true,  +400, },

    { +2,  0,  'w', -'w', move_op_down,  false, -200, },
    { -2,  0,  's', -'s', move_op_up,    false, -200, },
    { 0,  +2,  'a', -'a', move_op_right, false, -200, },
    { 0,  -2,  'd', -'d', move_op_left,  false, -200, },

    { +2,  0,  'w', -'w', move_op_down,  true,  -200, },
    { -2,  0,  's', -'s', move_op_up,    true,  -200, },
    { 0,  +2,  'a', -'a', move_op_right, true,  -200, },
    { 0,  -2,  'd', -'d', move_op_left,  true,  -200, },

// n: -d, -dv
    { +2, +1,  'w',    0, move_op_right, false, -200, },
    { -2, -1,  's',    0, move_op_left,  false, -200, },
    { -1, +2,  'a',    0, move_op_up,    false, -200, },
    { +1, -2,  'd',    0, move_op_down,  false, -200, },

    { +2, +1,  'w',    0, move_op_right, true,  -200, },
    { -2, -1,  's',    0, move_op_left,  true,  -200, },
    { -1, +2,  'a',    0, move_op_up,    true,  -200, },
    { +1, -2,  'd',    0, move_op_down,  true,  -200, },

// o: -s, -sv
    { +3,  0,  'w',    0, move_op_down,  false, -200, },
    { -3,  0,  's',    0, move_op_up,    false, -200, },
    { 0,  +3,  'a',    0, move_op_right, false, -200, },
    { 0,  -3,  'd',    0, move_op_left,  false, -200, },

    { +3,  0,  'w',    0, move_op_down,  true,  -200, },
    { -3,  0,  's',    0, move_op_up,    true,  -200, },
    { 0,  +3,  'a',    0, move_op_right, true,  -200, },
    { 0,  -3,  'd',    0, move_op_left,  true,  -200, },
};

int main() {
    struct strategy *ss;
    int len = new_and_load_strategys("config/player.json", ss);

    for (int i = 0; i < len; i++) {
        auto& s = ss[i];
        char print_we[4] = "' '";
        char print_player[4] = "' '";
        print_we[1] = s.we;
        print_player[1] = s.player < 0 ? -s.player : s.player;

        printf("{ %+d, %+d, %c%s, %c%s, move_op_%s %s %+d, }, \n",
                s.dr, s.dc,
                s.we < 0 ? '-' : ' ', s.we ? print_we : "  0",
                s.player < 0 ? '-' : ' ', s.player ? print_player : "  0",
                s.move == move_op_stay ? "stay, ":
                s.move == move_op_up ?   "up,   ":
                s.move == move_op_down ? "down, ":
                s.move == move_op_left ? "left, ": "right,",
                s.is_fire ? "true, " : "false,",
                (int)s.trend
        );

        int slen = sizeof(_player_strategys) / sizeof(*_player_strategys);
        bool have = false;
        for (int j = 0; j < slen; j++) {
            auto &s2 = _player_strategys[j];
            if(s.dr != s2.dr)
                continue;
            if(s.dc != s2.dc)
                continue;
            if(s.is_fire != s2.is_fire)
                continue;
            if(s.move != s2.move)
                continue;
            if(s.player != s2.player)
                continue;
            if(s.trend != s2.trend)
                continue;
            if(s.we != s2.we)
                continue;
            have = true;
        }
        assert(have);
    }

    if (len) {
        delete[] ss;
    }
    return 0;
}
