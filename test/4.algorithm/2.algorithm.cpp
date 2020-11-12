#include "modules/algorithm.h"
#include "utils/assert.h"

int main()
{
	char map[] =
		"[MAP A@ "
		"12345"
		"0sdwg"
		"99999"
		"54321"
		"w2345"
		"][LOCATION A@ 20 6 7 8][SCORE 10 1 2 3]";

	class server_data data(map, 5, 0);

	auto result = algorithm(data);
	assert(result.first == move_op_up);
	assert(result.second == false);

	return 0;
}
