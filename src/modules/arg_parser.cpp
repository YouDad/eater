// Copyright 2020 <luwh0708@thundersoft.com>
#include <stdio.h>
#include <string.h>

#include "modules/arg_parser.h"

static char key[40];
static char ip[32];
static uint32_t port_number;
static bool have_port = false;
static bool have_ip = false;
static bool have_key = false;
static bool have_record = false;

int arg_parse(int argc, const char **argv) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--ip") == 0 && i + 1 < argc) {
            if (strlen(argv[i + 1]) <= 16) {
                have_ip = true;
                strncpy(ip, argv[i + 1], sizeof(ip));
            }
        }

        if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            if (strlen(argv[i + 1]) <= 6) {
                int ret = sscanf(argv[i + 1], "%d", &port_number);
                if (ret == 1) {
                    have_port = true;
                }
            }
        }

        if (strcmp(argv[i], "--key") == 0 && i + 1 < argc) {
            if (strlen(argv[i + 1]) <= 40) {
                int ret = sscanf(argv[i + 1], "%[0-9a-f]", key);
                if (ret == 1) {
                    have_key = true;
                }
            }
        }

        if (strcmp(argv[i], "--record") == 0) {
            have_record = true;
        }
    }

    return have_ip || have_port ? 0 : 1;
}

int get_ip(uint32_t &addr) {
    if (!have_ip) {
        return 1;
    }

    for (int i = 0; ip[i]; i++) {
        if ('0' <= ip[i] && ip[i] <= '9') {
            continue;
        }
        if (ip[i] == '.') {
            continue;
        }
        return 1;
    }

    int num[4];
    int ret = sscanf(ip, "%d.%d.%d.%d", num, num + 1, num + 2, num + 3);
    if (ret != 4) {
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        if (0 > num[i] || num[i] > 255) {
            return 1;
        }
    }

    addr = (num[0] << 24) | (num[1] << 16) | (num[2] << 8) | (num[3] << 0);

    return 0;
}

int get_port(uint32_t &port) {
    if (have_port && 0 <= port_number && port_number < 65536) {
        port = port_number;
        return 0;
    }
    return 1;
}

int get_key(char *k, int k_len) {
    if (have_key) {
        strncpy(k, key, k_len);
        return 0;
    }
    return 1;
}

bool is_recording() {
    return have_record;
}
