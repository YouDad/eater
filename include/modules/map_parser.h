// Copyright 2020 <luwh0708@thundersoft.com>
#ifndef MAP_PARSER_H_
#define MAP_PARSER_H_

#include <stdint.h>
#include <vector>
#include "types.h"

class server_data {
	public:
		// 构造函数
		server_data(char *map, int size, int id);
		// 临时空构造函数
		server_data();

		// 析构函数
		~server_data();

		// 克隆一个数据
		class server_data clone();

		// 获得地图的大小
		int get_map_size();

		// 获得地图上某点的字符
		// 越界返回0
		char get(int row, int col);

		// 获得pos位置的玩家分数
		int get_score_by_pos(int pos);

		// 获得自己的位置
		void get_my_pos(int &row, int &col);

		// 获得自己的分数，返回值是分数
		int get_my_score();

		/* 计算炮弹下一步 */
		void calc_ball();

		/* 移动
		 * 返回值:
		 * - 0: 成功
		 * - 1: 失败
		 * */
		int move(enum move_operating move_op);

		/* 开炮
		 * 返回值:
		 * - 0: 成功
		 * - 1: 失败
		 * */
		int fire();

		/* 是否结束 */
		bool over();
	protected:
		// 设置地图上某点的字符
		// 越界返回1
		// 正常返回0
		int set(int row, int col, char ch);
	private:
		// 位置数组
		std::vector<int> _positions;

		// 得分数组
		std::vector<int> _scores;

		// 己方编号
		int _id;

		// 地图大小
		int _size;

		// 地图字符指针
		char *_map;
};

#endif
