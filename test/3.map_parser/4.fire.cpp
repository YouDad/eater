#include "modules/map_parser.h"
#include "utils/assert.h"

int main()
{
	char map[] =
		"[MAP A@ "
		"12345"
		"asdwg"
		"99999"
		"54321"
		"12345"
		"][LOCATION A@ 5 6 7 8][SCORE A@ 10 1 2 3]";
	class server_data data(map, 5, 0);

	{
		class server_data cloned = data.clone();

		assert(cloned.move(move_op_left) == 1);

		assert(cloned.fire() == 1);
	}

	{
		class server_data cloned = data.clone();

		assert(cloned.move(move_op_down) == 0);
		assert(cloned.get(1, 0) == 's');

		assert(cloned.move(move_op_down) == 1);

		assert(cloned.fire() == 1);
	}

	{
		class server_data cloned = data.clone();

		assert(cloned.move(move_op_right) == 0);
		assert(cloned.get(1, 0) == 'd');

		assert(cloned.move(move_op_right) == 1);

	}

	{
		class server_data cloned = data.clone();

		assert(cloned.move(move_op_right) == 0);

		assert(cloned.fire() == 0);
		assert(cloned.get_score_by_pos(5) == 17);
	}

	{
		class server_data cloned = data.clone();

		assert(cloned.move(move_op_up) == 0);
		assert(cloned.get(1, 0) == 'w');

		assert(cloned.fire() == 0);
		assert(cloned.get_score_by_pos(5) == 7);
		assert(cloned.get(0, 0) == '^');
	}

	return 0;
}
