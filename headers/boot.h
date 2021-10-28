/**
 * 执行步骤定义
 * 
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.0
 * @date Mon Apr  5 16:43:03 CST 2021
 * 
 */

#include "application.h"
#include "player.h"
#include "setting.h"
#include "slave.h"

#include <glib.h>

#ifndef QP_H_FLOW
#define QP_H_FLOW

// 命令行解析回调方法

static gboolean qp_boot_cmdopt_check(
    const gchar *option_name,
    const gchar *value,
    gpointer data,
    GError *error);

// 程序启动准备方法
extern void qp_boot(gint argc, gchar **argv, QP_Application *applicayion);

#endif