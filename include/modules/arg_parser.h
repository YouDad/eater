// Copyright 2020 <luwh0708@thundersoft.com>
#ifndef ARG_PARSER_H_
#define ARG_PARSER_H_

#include <stdint.h>

/* 解析命令行参数
 * 参数:
 * - argc: 参数个数
 * - argv: 参数字符串数组
 * 返回值:
 * - 0: 解析正常
 * - 1: 存在非法表达式
 * */
int arg_parse(int argc, const char **argv);

/* 从命令行中获得ip地址
 * 参数:
 * - addr: 返回式结果指针，结果是AA.BB.CC.DD，会编码成0xAABBCCDD
 *         由调用者初始化
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int get_ip(uint32_t &addr);

/* 从命令行中获得端口号
 * 参数:
 * - port: 返回式结果指针，结果是端口号
 *         由调用者初始化
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int get_port(uint32_t &port);

/* 从命令行中获得key
 * 参数:
 * - k: 缓冲区，结果是key
 *         由调用者初始化
 * - k_len: 缓冲区长度
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int get_key(char *k, int k_len);

/* 从命令行中获得是否记录 */
bool is_recording();

#endif
