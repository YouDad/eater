#include "modules/arg_parser.h"
#include "utils/assert.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

int main() {
	int ret;
	uint32_t addr, port;

	const char *argv1[] = {"a"};
	ret = arg_parse(ARRAY_SIZE(argv1), argv1);
	assert(ret == 1);
	ret = get_ip(addr);
	assert(ret == 1);
	ret = get_port(port);
	assert(ret == 1);

	const char *argv2[] = {"--ip", "345"};
	ret = arg_parse(ARRAY_SIZE(argv2), argv2);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 1);
	ret = get_port(port);
	assert(ret == 1);

	const char *argv3[] = {"--ip", "3.3.3."};
	ret = arg_parse(ARRAY_SIZE(argv3), argv3);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 1);
	ret = get_port(port);
	assert(ret == 1);

	const char *argv4[] = {"--ip", "3.3.3.3"};
	ret = arg_parse(ARRAY_SIZE(argv4), argv4);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 0);
	assert(addr == 0x03030303);
	ret = get_port(port);
	assert(ret == 1);

	const char *argv5[] = {"--ip", "192.168.103.238"};
	ret = arg_parse(ARRAY_SIZE(argv5), argv5);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 0);
	assert(addr == ((192 << 24) + (168 << 16) + (103 << 8) + (238 << 0)));
	ret = get_port(port);
	assert(ret == 1);

	const char *argv6[] = {"--ip", "255.255.255.255", "--port"};
	ret = arg_parse(ARRAY_SIZE(argv6), argv6);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 0);
	assert(addr == 0xffffffff);
	ret = get_port(port);
	assert(ret == 1);

	const char *argv7[] = {"--ip", "1.2.256.3", "--port", "2", "-c", "3"};
	ret = arg_parse(ARRAY_SIZE(argv7), argv7);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 1);
	ret = get_port(port);
	assert(ret == 0);
	assert(port == 2);

	const char *argv8[] = {"--ip", "1.1.1.1", "--port", "9527"};
	ret = arg_parse(ARRAY_SIZE(argv8), argv8);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 0);
	assert(addr == 0x01010101);
	ret = get_port(port);
	assert(ret == 0);
	assert(port == 9527);

	const char *argv9[] = {"--ip", "0.0.-3.0", "--port", "65539"};
	ret = arg_parse(ARRAY_SIZE(argv9), argv9);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 1);
	ret = get_port(port);
	assert(ret == 1);

	const char *argv10[] = {"--ip", "10.0.28.187", "--port", "9527"};
	ret = arg_parse(ARRAY_SIZE(argv10), argv10);
	assert(ret == 0);
	ret = get_ip(addr);
	assert(ret == 0);
	assert(addr == ((10 << 24) + (28 << 8) + (187 << 0)));
	ret = get_port(port);
	assert(ret == 0);
	assert(port == 9527);

	return 0;
}
