#ifndef STRATEGY_H_
#define STRATEGY_H_

#include <string.h>
#include <assert.h>

#include <vector>
#include <algorithm>

#include "types.h"
#include "modules/algorithm.h"

typedef enum move_operating mop_t;
typedef std::pair<mop_t, bool> op_t;
typedef std::vector<op_t> ops_t;

typedef struct special_judge {
    mop_t move_op;
    bool is_fire;
    double trend;
} spjdg_t;

typedef std::vector<spjdg_t> spjdgs_t;

struct strategy {
    int dr;
    int dc;
    char we;      // 0 for any
    char player;  // 0 for any
    mop_t move;
    bool is_fire;
    double trend;
};

void exec_strategys(struct strategy *strategys, int len,
        spjdgs_t &ops, class server_data &m, int dr, int dc);

int new_and_load_strategys(const char *filename, struct strategy *&strategys);

#endif
