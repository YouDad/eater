#define NETWORK_DEBUG
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <mutex>

#include "modules/network.h"

static int socket_fd;

static std::thread *read_thread_p;
static std::thread *heartbeat_thread_p;
static volatile std::atomic<bool> read_thread_over;
static std::mutex data_mutex;

static int map_size;
static int player_id;

static char token[4];
static char *map;
static bool game_over;

int my_send(const char *str)
{
#ifdef NETWORK_DEBUG
	printf("send[%d]: \"%s\"\n", socket_fd, str);
#endif
	static char buf[32];
	sprintf(buf, "(%s)", str);
	return send(socket_fd, buf, strlen(buf), 0);
}

int connect(uint32_t ip_addr, uint32_t port, const char *key)
{
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	char addr[32];
	sprintf(addr, "%u.%u.%u.%u",
			(ip_addr >> 24) & 0xff, (ip_addr >> 16) & 0xff,
			(ip_addr >> 8) & 0xff, (ip_addr >> 0) & 0xff);

	int ret = inet_pton(AF_INET, addr, &serv_addr.sin_addr);
	if (ret <= 0) {
		perror("inet_pton");
		return 1;
	}

	printf("connecting: %s:%d\n", addr, port);
	ret = connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret < 0) {
		perror("connect");
		return 1;
	}
	printf("connected: %s:%d\n", addr, port);

	ret = my_send(key);
	if (ret < 0) {
		perror("send");
		return 1;
	}

	char buf[16];
	ret = recv(socket_fd, buf, sizeof(buf), 0);
	if (ret < 0) {
		perror("recv");
		return 1;
	}
	buf[ret] = 0;
#ifdef NETWORK_DEBUG
	printf("recv [%d]: \"%s\"\n", ret, buf);
#endif

	if (strstr(buf, "[OK]") != buf) {
		perror("strstr");
		printf("recv string[%d]: \"%s\"\n", ret, buf);
		return 1;
	}

	return 0;
}

int start_read_thread()
{
	data_mutex.lock();
	read_thread_over = false;
	game_over = false;
	read_thread_p = new std::thread([&]() -> void {
		static int buf_size = map_size * map_size + 100;
		static char *buf = new char[buf_size];

		while (!read_thread_over) {
			int ret = recv(socket_fd, buf, buf_size, 0);
			if (ret < 0) {
				perror("recv");
				break;
			}

			buf[ret] = 0;
#ifdef NETWORK_DEBUG
			printf("recv[%d/%d]: \"%s\"\n", ret, buf_size, buf);
#endif

			if (ret == 0) {
				return;
			}

			if (strstr(buf, "[ERROR") == buf) {
				continue;
			}

			if (strstr(buf, "[OK") == buf) {
				continue;
			}

			if (strstr(buf, "[MAP") == buf) {
				strcpy(map, buf);
				sscanf(map, "[MAP %s", token);
				data_mutex.unlock();
				continue;
			}

			if (strstr(buf, "[ROUNDOVER") == buf) {
				data_mutex.unlock();
				continue;
			}

			if (strstr(buf, "[GAMEOVER") == buf) {
				game_over = true;
				read_thread_over = true;
				data_mutex.unlock();
				continue;
			}
		}

		delete []buf;
		delete []map;
	});
	return 0;
}

int finish_read_thread()
{
	read_thread_over = true;
	read_thread_p->join();
	delete read_thread_p;
	return 0;
}

int wait_for_start()
{
	char buf[16];
	while (true) {
		int ret = my_send("H");
		if (ret < 0) {
			perror("send");
			return 1;
		}

		sleep(1);

		ret = recv(socket_fd, buf, sizeof(buf), 0);

		if (strstr(buf, "[START") != buf) {
			continue;
		}

		ret = sscanf(buf, "[START %d %d]", &player_id, &map_size);
		if (ret != 2) {
			printf("start recv: %s\n", buf);
		}

		map = new char[map_size * map_size + 100];
		my_send("READY");
	}
	return 0;
}

int disconnect()
{
	close(socket_fd);
	return 0;
}

int get_map_size()
{
	return map_size;
}

int get_player_id()
{
	return player_id;
}

int get_server_data(char *buf)
{
	data_mutex.lock();
	if (game_over) {
		return 2;
	}
	strcpy(buf, map);
	return 0;
}

int send_operating(enum move_operating move_op, bool is_fire)
{
	char op[16];
	char move = ' ';
	char fire = is_fire ? 'v' : ' ';

	switch(move_op) {
		case move_op_up:
			move = 'w';
			break;
		case move_op_down:
			move = 's';
			break;
		case move_op_left:
			move = 'a';
			break;
		case move_op_right:
			move = 'd';
			break;
		case move_op_stay:
			move = ' ';
			break;
	}

	sprintf(op, "%s%c%c", token, move, fire);
	my_send(op);
#ifdef NETWORK_DEBUG
	printf("send_operating: %s\n", op);
#endif
	return 0;
}
