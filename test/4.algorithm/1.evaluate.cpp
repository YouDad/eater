// Copyright 2020 <luwh0708@thundersoft.com>
#include "modules/algorithm.h"
#include "utils/assert.h"

int main() {
    char map1[] =
        "[MAP A@ "
        "12345"
        "asdwG"
        "99999"
        "54321"
        "12345"
        "][LOCATION A@ 5 6 7 8][SCORE 10 1 2 3]";
    char map2[] =
        "[MAP A@ "
        "12345"
        "0sdwG"
        "99999"
        "54321"
        "a2345"
        "][LOCATION A@ 20 6 7 8][SCORE 10 1 2 3]";

    class server_data data1(map1, 5, 0);
    class server_data data2(map2, 5, 0);

    double score1;
    double score2;
    assert(evaluate(data1, score1) == 0);
    assert(evaluate(data2, score2) == 0);
    assert(score1 < score2);

    return 0;
}
