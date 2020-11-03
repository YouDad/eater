#include "modules/map_parser.h"
#include <utility>

// 评估一个局势，返回分数
int evaluate(class server_data &m, int &score);

// 给定一个局势，返回算法解
// 返回值是一个pair，第一个位置是移动，第二个位置是是否开炮
std::pair<enum move_operating, bool> algorithm(class server_data &m);
