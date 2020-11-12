#include "modules/algorithm.h"
#include "utils/assert.h"

#define test(map, move, is_fire) \
{\
	class server_data data((char *)map, 5, 0);\
	auto result = algorithm(data);\
	/*printf("%d, %d\n", result.first, result.second);*/ \
	assert(result.first == move);\
	assert(result.second == is_fire);\
}

int main()
{
	test("[MAP A@ "
		"12345"
		"0sdwG"
		"99999"
		"54321"
		"w2345"
		"][LOCATION A@ 20 6 7 8][SCORE 10 1 2 3]" , move_op_up, false);
	test("[MAP A@ "
		"w3000"
		"20000"
		"00000"
		"00000"
		"0000G"
		"][LOCATION A@ 0][SCORE 10]" , move_op_right, false);
	test("[MAP A@ "
		"19441"
		"3s214"
		"40312"
		"10213"
		"40325"
		"][LOCATION A@ 6][SCORE 10]" , move_op_right, false);
	test("[MAP A@ "
		"G4440"
		"09590"
		"02G40"
		"00040"
		"1322s"
		"][LOCATION A@ 24][SCORE 10]" , move_op_left, false);

	return 0;
}
