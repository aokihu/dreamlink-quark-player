/**
 * 
 * Slave模式交互
 * 
 * @author aokihu
 * @copyright Reasonbox
 * 
 */

#include "application.h"
#include "player.h"

#include <unistd.h>
#include <glib.h>
#include <gio/gio.h>

#ifndef QP_H_SLAVE
#define QP_H_SLAVE

/**
 * 定义指令字符串数组
 */
static char *QP_SLAVE_COMMANDS[] = {"quit", "stop", "vol"};

/**
 * @enum 控制指令枚举
 * @QP_SLAVE_COMMAND_QUIT 结束程序
 * @QP_SLAVE_COMMAND_STOP 定制播放音乐
 * @QP_SLAVE_COMMAND_VOL  设置音量
 */

// typedef enum _QP_SLAVE_ENUM_COMMAND
// {
//   QP_SLAVE_COMMAND_QUIT,
//   QP_SLAVE_COMMAND_STOP,
//   QP_SLAVE_COMMAND_VOL,
// } QP_SLAVE_COMMAND;

/*
 * 指令结构体
 * @raw_command 原始指令字符串
 * @command 解析后的指令枚举类型
 * @value 解析后获取的指令附带数据
 */
typedef struct _QP_SLAVE_COMMAND
{
  const gchar *raw_command;
  // QP_SLAVE_COMMAND command;
  gchar *value;
} QP_SlaveCommand;

/**
 * 
 * QP_SlaveFunc:
 * @QP_SlaveCommand 解析后的命令数据
 * @data 附加的数据
 * 
 */
typedef void (*QP_SlaveFunc)(QP_SlaveCommand *cmd,
                             gpointer data);

/**
 * 控制指令解析结构体
 * @cmd_name 命令名称,对应输入的指令字符串
 * @func 命令解析后的回掉方法
 */

typedef struct _QP_SLAVE_COMMAND_ITEM
{
  const gchar *cmd_name;
  GType type;
  QP_SlaveFunc func;
} QP_SlaveCommandItem;

//
// 私有方法定义
//
void qp_slave_process_command(QP_SlaveCommand *cmd, QP_Application *app);
void qp_slave_parse_command(GString *message, QP_Application *app);

//
// 回掉方法定义
//
void qp_slave_cb_quit(QP_SlaveCommand *cmd, gpointer data);
void qp_slave_cb_set_volume(QP_SlaveCommand *cmd, gpointer data);

//
// 公共方法定义
//
extern void qp_slave_prepare(QP_Application *app);

#endif