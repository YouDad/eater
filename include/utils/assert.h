// Copyright 2020 <luwh0708@thundersoft.com>
#ifndef ASSERT_H_
#define ASSERT_H_

#include <stdio.h>
#include <stdlib.h>

#define assert(cond) _assert(cond, NULL, __FILE__, __LINE__)
#define assert_info(cond, str) _assert(cond, str, __FILE__, __LINE__)
static inline void _assert(int cond, const char *str, const char *file, int line)
{
    if (!cond) {
        if (!str) {
            printf("\033[31massert fail at %s:%d\033[0m\n", file, line);
        } else {
            printf("\033[31m%s at %s:%d\033[0m\n", str, file, line);
        }
        exit(1);
    }
}

#endif
