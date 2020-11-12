#include "modules/draw.h"
#include <vector>
#include <map>
#include <stdlib.h>
#define fg_rgb(r, g, b) printf("\033[38;2;%d;%d;%dm", (r), (g), (b))
#define bg_rgb(r, g, b) printf("\033[48;2;%d;%d;%dm", (r), (g), (b))

struct colorscheme {
	char match_char;
	int fg;
	int bg;
};

struct colorscheme colorschemes[] = {
	{ '9', 0xffffff, 0xffffff, },
	{ '1', 0x0000aa, 0x0000aa, },
	{ '2', 0x2a00aa, 0x2a00aa, },
	{ '3', 0x5500aa, 0x5500aa, },
	{ '4', 0x9000aa, 0x9000aa, },
	{ '5', 0xaa00aa, 0xaa00aa, },
	{ 'o', 0x0000ff, 0xffff00, },
	{ 'm', 0xff00ff, 0x00ff00, },
	{ '^', 0xffffff, 0xff0000, },
	{ '<', 0xffffff, 0xff0000, },
	{ 'v', 0xffffff, 0xff0000, },
	{ '>', 0xffffff, 0xff0000, },
	{ 'G', 0x000000, 0xff0000, },
};

static void use_color(char c)
{
	for (int i = 0; i < sizeof(colorschemes) / sizeof(*colorschemes); i++) {
		if (colorschemes[i].match_char == c) {
			int fg = colorschemes[i].fg;
			int bg = colorschemes[i].bg;
			printf("\033[38;2;%d;%d;%dm",
					(fg >> 16) & 0xff, (fg >> 8) & 0xff, fg & 0xff);
			printf("\033[48;2;%d;%d;%dm",
					(bg >> 16) & 0xff, (bg >> 8) & 0xff, bg & 0xff);
			return;
		}
	}
}

static void reset_color()
{
	printf("\033[0m");
}

static void cursor_up(int cnt)
{
	printf("\033[%dA", cnt);
}

static void cursor_down(int cnt)
{
	printf("\033[%dB", cnt);
}

static void cursor_right(int cnt)
{
	printf("\033[%dC", cnt);
}

static void cursor_left(int cnt)
{
	printf("\033[%dD", cnt);
}

static void cursor_save()
{
	printf("\033[s");
}

static void cursor_recover()
{
	printf("\033[u");
}

void draw(char *buf, int map_size, int player_id)
{
	char *map = new char[map_size * map_size + 1];
	char location[100], score[100];
	int n = 0, m = 0;
	sscanf(buf + n, "[MAP %*s %[^]]]%n", map, &m);
	n += m;
	sscanf(buf + n, "[LOCATION %*s%[^]]]%n", location, &m);
	n += m;
	sscanf(buf + n, "[SCORE %*s%[^]]]%n", score, &m);

	std::vector<int> locations;
	for (int i = 0; location[i]; i++) {
		if (location[i] == ' ') {
			locations.push_back(atoi(&location[i]));
		}
	}

	std::vector<int> scores;
	std::map<int, bool> visited;
	for (int i = 0; score[i]; i++) {
		if (score[i] == ' ') {
			scores.push_back(atoi(&score[i]));
		}
	}

	cursor_up(map_size);
	for (int i = 0; i < map_size; i++) {
		printf("  ");
		for (int j = 0; j < map_size; j++) {
			char str[4];
			char ch = map[i * map_size + j];

			switch (ch) {
				case '0':
					sprintf(str, "  ");
					break;

				case '9':
					sprintf(str, "田");
					break;

				case 'w':
				case 'a':
				case 's':
				case 'd':
					use_color('o');
					for (int k = 0; k < locations.size(); k++) {
						if (i * map_size + j == locations[k]) {
							visited[k] = true;
							sprintf(str, "%c%d", ch, k);
							if (player_id == k) {
								use_color('m');
							}
						}
					}
					break;

				default:
					use_color(ch);
					sprintf(str, "%c ", ch);
					break;
			}
			printf("%s", str);
			reset_color();
		}
		printf("\n");
	}
	cursor_up(map_size);

	cursor_right(4 + map_size * 2);

	cursor_save();
	printf("score:");
	cursor_recover();

	for (int i = 0; i < scores.size(); i++) {
		char status_char = ' ';
		if (visited.find(i) == visited.end()) {
			status_char = 'x';
		}
		if (scores[i] <= 0) {
			status_char = 'x';
		}

		char us_char = '=';
		if (player_id != i) {
			us_char = ' ';
		}

		cursor_down(1);

		cursor_save();
		printf("[%c]%cplayer %d: %d   ", status_char, us_char, i, scores[i]);
		cursor_recover();
	}

	cursor_left(4 + map_size * 2);
	cursor_down(map_size - scores.size());
	delete []map;
}
