#include "modules/network.h"
#include "utils/assert.h"
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <thread>
#include <atomic>
#include <mutex>

int main()
{
	int ret;

	ret = connect(0x7f000001, 9000);
	assert(ret == 1);

	std::thread server_thread([]() ->void {
		int listen_fd, ret;
		struct sockaddr_in server_addr;
		char buf[128];

		listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_fd < 0) {
			perror("socket");
			return;
		}

		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		server_addr.sin_port = htons(9000);

		ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if (ret < 0) {
			perror("bind");
			return;
		}

		ret = listen(listen_fd, 10);
		if (ret < 0) {
			perror("listen");
			return;
		}

		int conn_fd = accept(listen_fd, NULL, NULL);
		if (conn_fd < 0) {
			perror("accept");
			return;
		}
		int n = recv(conn_fd, buf, 128, 0);
		assert(n > 0);
		buf[n] = 0;
		assert(strcmp(buf, "(c96f4d7661c94cbb9706469649a7cbbc)") == 0);
		send(conn_fd, "[OK]", 4, 0);

		sleep(1);
		send(conn_fd, "[START 1 5]", sizeof("[START 1 5]"), 0);

		n = recv(conn_fd, buf, 128, 0);
		assert(n > 0);
		buf[n] = 0;
		assert(strcmp(buf, "(READY)") == 0);
		send(conn_fd, "[OK]", 4, 0);

		sleep(1);
		const char *data =
			"[MAP C! wwwwwwww00000000000000000]"
			"[LOCATION C! 0 1 2 3 4 5 6 7]"
			"[SCORE 0 0 0 0 0 0 0 0]";
		send(conn_fd, data, strlen(data), 0);

		auto should_recv = [&](const char *should_recv_str) -> void {
			n = recv(conn_fd, buf, 128, 0);
			assert(n > 0);
			buf[n] = 0;
			assert(strcmp(buf, should_recv_str) == 0);
		};

		should_recv("C![  ]");
		should_recv("C![w ]");
		should_recv("C![s ]");
		should_recv("C![a ]");
		should_recv("C![d ]");
		should_recv("C![ v]");
		should_recv("C![wv]");
		should_recv("C![sv]");
		should_recv("C![av]");
		should_recv("C![dv]");

		close(conn_fd);
	});

	sleep(1);
	ret = connect(0x7f000001, 9000);
	assert(ret == 0);

	ret = start_read_thread();
	assert(ret == 0);
	sleep(3);
	int us = 10;

	ret = send_operating(move_op_stay, false);
	assert(ret == 0);
	usleep(us);
	ret = send_operating(move_op_up, false);
	assert(ret == 0);
	usleep(us);
	ret = send_operating(move_op_down, false);
	assert(ret == 0);
	usleep(us);
	ret = send_operating(move_op_left, false);
	assert(ret == 0);
	usleep(us);
	ret = send_operating(move_op_right, false);
	assert(ret == 0);
	usleep(us);

	ret = send_operating(move_op_stay, true);
	assert(ret == 0);
	usleep(us);
	ret = send_operating(move_op_up, true);
	assert(ret == 0);
	usleep(us);
	ret = send_operating(move_op_down, true);
	assert(ret == 0);
	usleep(us);
	ret = send_operating(move_op_left, true);
	assert(ret == 0);
	usleep(us);
	ret = send_operating(move_op_right, true);
	assert(ret == 0);
	usleep(us);

	ret = finish_read_thread();
	assert(ret == 0);
	ret = disconnect();
	assert(ret == 0);

	server_thread.join();
	return 0;
}
