#include <stdio.h>

#include "modules/arg_parser.h"
#include "modules/network.h"
#include "modules/map_parser.h"
#include "modules/algorithm.h"

int main(int argc, const char **argv)
{
	int ret;

	ret = arg_parse(argc, argv);
	if (ret) {
		perror("arg_parse");
	}

	uint32_t addr;
	ret = get_ip(addr);
	if (ret) {
		perror("get_ip");
	}

	uint32_t port;
	ret = get_port(port);
	if (ret) {
		perror("get_port");
	}

	ret = connect(addr, port);
	if (ret) {
		perror("connect");
	}

	ret = start_read_thread();
	if (ret) {
		perror("start_read_thread");
	}

	ret = start_heartbeat_thread();
	if (ret) {
		perror("start_heartbeat_thread");
	}

	int map_line_size = get_map_size();
	char *buf;
	if (map_line_size > 0) {
		buf = new char [map_line_size * map_line_size + 100];
	}

	int player_id = get_player_id();

	while (true) {
		ret = get_server_data(buf);
		if (ret == 2) {
			break;
		}

		class server_data data(buf, map_line_size, player_id);

		auto op = algorithm(data);
		send_operating(op.first, op.second);
	}

	delete []buf;
	finish_heartbeat_thread();
	finish_read_thread();
	disconnect();
	return 0;
}
