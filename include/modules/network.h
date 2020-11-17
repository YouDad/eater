// Copyright 2020 <luwh0708@thundersoft.com>
#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdint.h>
#include "types.h"

/* 连接服务器，发送key
 * 参数:
 * - ip_addr: ip地址，0xAABBCCDD为AA.BB.CC.DD
 * - port: 端口号
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int connect(uint32_t ip_addr, uint32_t port, const char *key);

/* 开启读线程，响应START/MAP/GAMEOVER数据包
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int start_read_thread();

/* 关闭读线程
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int finish_read_thread();

/* 发心跳等待游戏开始
 * 返回值:
 * - 0: 成功
 *   1: 失败
 * */
int wait_for_start();

/* 与服务器断开连接
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int disconnect();

/* 获得地图大小
 * 可阻塞
 * 返回值: 地图边长
 * */
int get_map_size();

/* 获得玩家编号
 * 可阻塞
 * 返回值: 玩家编号
 * */
int get_player_id();

/* 获得一次服务器数据
 * 可堵塞
 * 参数:
 * - buf: 字符串返回值，由调用者初始化
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int get_server_data(char *buf);

/* 发送一次操作
 * 参数:
 * - move_op: 移动操作
 * - is_fire: 是否开炮
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int send_operating(enum move_operating move_op, bool is_fire);

#endif
