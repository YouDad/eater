// #define MAP_PARSER_DEBUG
#include "modules/map_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

server_data::server_data()
{
}

server_data::server_data(char *data, int size, int id)
{
	_map = new char[size * size + 1];
	char pos_str[64];
	char score_str[64];
	sscanf(data, "[MAP %*s %[^]]]"
			"[LOCATION %*s %[^]]]"
			"[SCORE %*s %[^]]]",
			_map, pos_str, score_str);

	int len = strlen(pos_str);
	pos_str[len] = ' ';
	pos_str[len + 1] = 0;

	len = strlen(score_str);
	score_str[len] = ' ';
	score_str[len + 1] = 0;

	_id = id;
	_size = size;

	char *num_ptr;
	_positions.clear();
	num_ptr = pos_str;
	for (char *i = pos_str; *i; i++) {
		if (*i == ' ') {
			*i = 0;
			_positions.push_back(atoi(num_ptr));
			num_ptr = i+1;
		}
	}
	
	_scores.clear();
	num_ptr = score_str;
	for (char *i = score_str; *i; i++) {
		if (*i == ' ') {
			*i = 0;
			_scores.push_back(atoi(num_ptr));
			num_ptr = i+1;
		}
	}
}

server_data::~server_data()
{
	delete []_map;
}

class server_data server_data::clone()
{
	class server_data ret;
	ret._map = new char[_size * _size + 1];
	memcpy(ret._map, _map, _size * _size + 1);

	ret._id = _id;
	ret._size = _size;

	ret._positions.clear();
	for (int i = 0; i < _positions.size(); i++) {
		ret._positions.push_back(_positions[i]);
	}

	ret._scores.clear();
	for (int i = 0; i < _scores.size(); i++) {
		ret._scores.push_back(_scores[i]);
	}
	return ret;
}

int server_data::get_map_size()
{
	return _size;
}

char server_data::get(int row, int col)
{
	if (row < 0 || row >= _size) {
		return 0;
	}
	if (col < 0 || col >= _size) {
		return 0;
	}
	return _map[row * _size + col];
}

int server_data::get_score_by_pos(int pos)
{
	for (int i = 0; i < _positions.size(); i++) {
		if (_positions[i] == pos) {
			return _scores[i];
		}
	}
	return -1;
}

void server_data::get_my_pos(int &row, int &col)
{
	int pos = _positions[_id];
	row = pos / _size;
	col = pos % _size;
}

int server_data::get_my_score()
{
	return _scores[_id];
}

void server_data::calc_ball()
{
}

int server_data::move(enum move_operating move_op)
{
	int r, c;
	get_my_pos(r, c);

	char direction = ' ';
	// delta row, delta col
	int dr = 0, dc = 0;
	switch (move_op) {
		case move_op_up:
			direction = 'w';
			dr = -1;
			break;
		case move_op_down:
			direction = 's';
			dr = 1;
			break;
		case move_op_left:
			direction = 'a';
			dc = -1;
			break;
		case move_op_right:
			direction = 'd';
			dc = 1;
			break;
		case move_op_stay:
			direction = ' ';
			break;
	}

	if (get(r, c) == direction) {
		// 如果面朝上
		char ch = get(r + dr, c + dc);
#ifdef MAP_PARSER_DEBUG
		printf("move: '%c', %d, %d\n", ch, r+dr, c+dc);
#endif

		if ('0' <= ch && ch <= '5') {
			// 空地果子
			set(r + dr, c + dc, direction);
			set(r, c, '0');
			_scores[_id] += ch - '0';
		} else {
			// 其他情况
			return 1;
		}
	} else {
		// 如果面朝其他方向
		if (direction != ' ') {
			set(r, c, direction);
		}
	}

	return 0;
}

int server_data::fire()
{
	int r, c;
	get_my_pos(r, c);

	char direction = get(r, c);
	char ball;
	// delta row, delta col
	int dr = 0, dc = 0;
	switch (direction) {
		case 'w':
			ball = '^';
			dr = -1;
			break;
		case 's':
			ball = 'v';
			dr = 1;
			break;
		case 'a':
			ball = '<';
			dc = -1;
			break;
		case 'd':
			ball = '>';
			dc = 1;
			break;
		case ' ':
			ball = '?';
			break;
	}

	char ch = get(r + dr, c + dc);
#ifdef MAP_PARSER_DEBUG
	printf("fire: '%c', %d, %d\n", ch, r+dr, c+dc);
#endif
	switch (ch) {
		case '0': case '1':
		case '2': case '3':
		case '4': case '5':
			set(r + dr, c + dc, ball);
			_scores[_id] -= 3;
			break;

		case '9':
		case 0:
			return 1;

		case 'w': case 'a':
		case 's': case 'd':
			set(r + dr, c + dc, '0');
			_scores[_id] += 7;
			break;

		case '<': case '>':
		case '^': case 'v':
			set(r + dr, c + dc, '0');
			_scores[_id] -= 3;
			break;

		case 'G':
			set(r + dr, c + dc, '0');
			_scores[_id] += 3;
			break;
	}

	return 0;
}

int server_data::set(int row, int col, char ch)
{
	if (row >= _size || col >= _size || row < 0 || col < 0) {
		return 1;
	}

	_map[row * _size + col] = ch;
	return 0;
}

bool server_data::over()
{
	return _positions[_id] == -1;
}
