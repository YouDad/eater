// Copyright 2020 <luwh0708@thundersoft.com>
#include "modules/map_parser.h"
#include "utils/assert.h"

int main() {
    char map[] =
        "[MAP A@ "
        "12345"
        "asdwg"
        "99999"
        "54321"
        "12345"
        "][LOCATION A@ 5 6 7 8][SCORE A@ 0 1 2 3]";
    class server_data data(map, 5, 0);

    assert(data.get(0, 0) == '1');
    assert(data.get(0, 1) == '2');
    assert(data.get(0, 2) == '3');
    assert(data.get(0, 3) == '4');
    assert(data.get(0, 4) == '5');

    assert(data.get(1, 0) == 'a');
    assert(data.get(1, 1) == 's');
    assert(data.get(1, 2) == 'd');
    assert(data.get(1, 3) == 'w');
    assert(data.get(1, 4) == 'g');

    assert(data.get(5, 5) == 0);

    int row, col;
    data.get_my_pos(row, col);
    assert(row == 1);
    assert(col == 0);

    assert(data.get_my_score() == 0);

    assert(data.get_score_by_pos(5) == 0);
    assert(data.get_score_by_pos(6) == 1);
    assert(data.get_score_by_pos(7) == 2);
    assert(data.get_score_by_pos(8) == 3);

    return 0;
}
