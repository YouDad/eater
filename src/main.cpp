#include <stdio.h>

#include "modules/arg_parser.h"
#include "modules/network.h"
#include "modules/map_parser.h"
#include "modules/algorithm.h"
#include "modules/draw.h"

int main(int argc, const char **argv)
{
	int ret;

	ret = arg_parse(argc, argv);
	if (ret) {
		perror("arg_parse");
		return 1;
	}

	uint32_t addr;
	ret = get_ip(addr);
	if (ret) {
		perror("get_ip");
		return 1;
	}

	uint32_t port;
	ret = get_port(port);
	if (ret) {
		perror("get_port");
		return 1;
	}

	char key[40];
	ret = get_key(key);
	if (ret) {
		perror("get_key");
		return 1;
	}

	ret = connect(addr, port, key);
	if (ret) {
		perror("connect");
		return 1;
	}

	ret = start_read_thread();
	if (ret) {
		perror("start_read_thread");
		return 1;
	}

	ret = start_heartbeat_thread();
	if (ret) {
		perror("start_heartbeat_thread");
		return 1;
	}

	int map_line_size = get_map_size();
	char *buf;
	if (map_line_size > 0) {
		buf = new char [map_line_size * map_line_size + 100];
	}

	int player_id = get_player_id();
	printf("data:\n");
	for (int i = 0; i < map_line_size; i++) {
		printf("\n");
	}

	while (true) {
		ret = get_server_data(buf);
		if (ret == 2) {
			break;
		}

		draw(buf, map_line_size, player_id);

		class server_data data(buf, map_line_size, player_id);
		if (data.over()) {
			continue;
		}

		auto op = algorithm(data);
		send_operating(op.first, op.second);
	}

	delete []buf;
	finish_heartbeat_thread();
	finish_read_thread();
	disconnect();
	return 0;
}
