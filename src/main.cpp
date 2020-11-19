// Copyright 2020 <luwh0708@thundersoft.com>
#include <stdio.h>
#include <string.h>

#include "modules/arg_parser.h"
#include "modules/network.h"
#include "modules/map_parser.h"
#include "modules/algorithm.h"
#include "modules/draw.h"

int main(int argc, const char **argv) {
    int ret;

    ret = arg_parse(argc, argv);
    if (ret) {
        perror("arg_parse");
        return 1;
    }

    uint32_t addr;
    ret = get_ip(addr);
    if (ret) {
        perror("get_ip");
        return 1;
    }

    uint32_t port;
    ret = get_port(port);
    if (ret) {
        perror("get_port");
        return 1;
    }

    char key[40];
    ret = get_key(key, sizeof(key));
    if (ret) {
        strncpy(key, "c96f4d7661c94cbb9706469649a7cbbc", sizeof(key));
    }

    ret = connect(addr, port, key);
    if (ret) {
        perror("connect");
        return 1;
    }

    ret = wait_for_start();
    if (ret) {
        perror("wait_for_start");
        return 1;
    }

    ret = start_read_thread();
    if (ret) {
        perror("start_read_thread");
        return 1;
    }

    int map_line_size = get_map_size();
    char *buf;
    if (map_line_size > 0) {
        buf = new char [map_line_size * map_line_size + 200];
    }

    int player_id = get_player_id();
    printf("round:\n");
    for (int i = 0; i < map_line_size; i++) {
        printf("\n");
    }

    if (is_recording()) {
        auto fp = fopen(".record", "w");
        fclose(fp);
    }
    printf("\033[2J");

    init_algorithm();
    while (true) {
        ret = get_server_data(buf);
        if (ret == 2) {
            break;
        }

        if (is_recording()) {
            auto fp = fopen(".record", "a");
            fprintf(fp, "%s\n", buf);
            fclose(fp);
        }

        draw(buf, map_line_size, player_id);

        class server_data data(buf, map_line_size, player_id);
        if (data.over()) {
            continue;
        }

        auto op = algorithm(data);
        send_operating(op.first, op.second);
    }

    deinit_algorithm();
    delete []buf;
    finish_read_thread();
    disconnect();
    return 0;
}
