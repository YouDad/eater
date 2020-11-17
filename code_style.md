# 代码风格文档

## 一个例子

```cpp
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char buf[4096];

int main(int argc, char **argv) {
	int socket_fd;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9527);

	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		perror("inet_pton");
		return 1;
	}

	if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		return 1;
	}

	printf("send msg to server: \n");
	if (send(socket_fd, argv[2], strlen(argv[2]), 0) < 0) {
		perror("send");
		return 1;
	}

	int n = recv(socket_fd, buf, sizeof(buf), 0);
	printf("recv[%d]: %s\n", n, buf);

	close(socket_fd);

	return 0;
}
```

## 代码风格规则

1. #include和文件名中间留一个空格
2. 变量名遵守小写+下划线的规则，如`socket_fd`
3. 变量可以在任何地方定义，如变量`n`作为recv的返回值接收时定义
4. 一行不能超过100个字符
5. 不同逻辑段需要加一个空行表示分隔
6. 其余代码结构参照例子，比如函数/if的大括号怎么放，逗号后面加空格，关键字后面加空格
7. 其他问题我会在提交时指出
