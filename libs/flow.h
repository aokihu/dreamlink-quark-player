/**
 * 执行步骤定义
 * 
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.0
 * @date Mon Apr  5 16:43:03 CST 2021
 * 
 */

#include "player.h"
#include "setting.h"

#ifndef QP_H_FLOW
#define QP_H_FLOW

extern QP_CmdParam *qp_flow_parse_cmdline(gint argc, gchar **argv);
extern void qp_flow_player_init(QP_Player *player);

#endif