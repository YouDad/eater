// Copyright 2020 <luwh0708@thundersoft.com>
#include <string.h>

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
        "][LOCATION A@ 5 6 7 8][SCORE A@ 0 0 0 0]";
    class server_data data(map, 5, 0);

    class server_data cloned = data.clone();
    struct _server_data {
        std::vector<int> _positions;
        std::vector<int> _scores;
        int _id;
        int _size;
        char *_map;
    };

    struct _server_data d1 = *(struct _server_data *)&data;
    struct _server_data d2 = *(struct _server_data *)&cloned;

    assert(d1._id == d2._id);
    assert(d1._id == 0);

    assert(d1._size == d2._size);
    assert(d1._size == 5);

    assert(d1._map != d2._map);
    assert(strcmp(d1._map, d2._map) == 0);

    assert(&d1._positions != &d2._positions);
    assert(d1._positions.size() == d2._positions.size());
    for (int i = 0; i < d1._positions.size(); i++) {
        assert(d1._positions[i] == d2._positions[i]);
    }

    assert(&d1._scores != &d2._scores);
    assert(d1._scores.size() == d2._scores.size());
    for (int i = 0; i < d1._scores.size(); i++) {
        assert(d1._scores[i] == d2._scores[i]);
    }

    return 0;
}
