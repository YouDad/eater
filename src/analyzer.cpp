// Copyright 2020 <luwh0708@thundersoft.com>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <string>

#include "modules/draw.h"

const int map_size = 25;
int main(int argc, char **argv) {
    int ptr = 0;
    auto fp = fopen(".record", "r");

    std::vector<std::string> strings;
    while (!feof(fp)) {
        char buf[map_size * map_size + 200];
        fgets(buf, sizeof(buf), fp);
        strings.push_back(buf);
    }

    fclose(fp);

    for (int i = 0; i < map_size; i++) {
        printf("\n");
    }

    int player = 0;
    if (argc > 1) {
        sscanf(argv[1], "%d", &player);
    }

    // strings size base 0
    int str_size = strings.size() - 1;

    while (!feof(stdin)) {
        char buf[16];
        fgets(buf, 16, stdin);
        printf("\033[2J");
        if (buf[0] != '\n') {
            if (buf[0] == 'p') {
                printf("%s\n", strings[ptr - 1].c_str());
                continue;
            } else {
                ptr = atoi(buf);
            }
        }

        draw((char *)strings[ptr].c_str(), map_size, player);
        printf("round (%d/%d)\n\033[1A", ptr, str_size);
        if (ptr < str_size) {
            ptr++;
        }
    }

    return 0;
}
