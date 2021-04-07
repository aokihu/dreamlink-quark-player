/**
 * 
 * Slave模式交互
 * 
 * @author aokihu
 * @copyright Reasonbox
 * 
 */

#include "application.h"
#include <glib.h>
#include <gio/gio.h>

#ifndef QP_H_SLAVE
#define QP_H_SLAVE

/**
 * 定义指令字符串数组
 */
static char *QP_SLAVE_COMMANDS[] = {"quit", "stop", "vol"};

typedef enum _QP_SLAVE_ENUM_COMMAND
{
  QP_SLAVE_COMMAND_QUIT, // 结束程序
  QP_SLAVE_COMMAND_STOP, // 停止播放
  QP_SLAVE_COMMAND_VOL,  // 设置音量
} QP_SLAVE_COMMAND;

/*
 * 指令结构体
 */
typedef struct _QP_SLAVE_COMMAND
{
  QP_SLAVE_COMMAND command;
  union
  {
    gint intValue;
    gdouble doubleValue;
    gchar stringValue[2048];
  } param;

} QP_SlaveCommand;

void qp_slave_process_command(QP_SlaveCommand *cmd, QP_Application *app);
void qp_slave_parse_command(GString *message, QP_Application *app);
extern void qp_slave_prepare(QP_Application *app);

#endif