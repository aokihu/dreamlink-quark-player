#include "slave.h"

//
// 全局静态变量定义
//

static QP_SlaveCommandItem QP_SLAVE_COMMAND_LIST[] = {
    {"quit", qp_slave_cb_quit},
    {NULL},
};

/**
 * 处理退出程序的回掉方法
 * @private
 * @param cmd 解析后的命令数据包
 * @param data 附加的数据，一般就是application
 * @return void
 */
void qp_slave_cb_quit(QP_SlaveCommand *cmd, gpointer data)
{
  QP_Application *app = data;
  g_main_loop_quit(app->mainLoop);
}

/**
 * 解析指令字符串
 * @private
 */
void qp_slave_parse_command(GString *message, QP_Application *app)
{
  gchar *_message = g_strstrip(message->str);
  gchar **_command = g_strsplit_set(_message, ":", 16);

  QP_SlaveCommand *cmd = g_new(QP_SlaveCommand, 1);

  // 分析指令字符串
  for (gint i = 0; i < sizeof(QP_SLAVE_COMMANDS); i++)
  {
    if (!g_strcmp0(_command[0], QP_SLAVE_COMMANDS[i]))
    {
      cmd->raw_command = _command[0];
      cmd->command = i;
      break;
    }
  }

  // 遍历指令处理集合
  for (gint i = 0; TRUE; i++)
  {
    QP_SlaveCommandItem arg = QP_SLAVE_COMMAND_LIST[i];

    if (arg.cmd_name != NULL)
    {
      (arg.func)(cmd, app);
      continue;
    }

    break;
  }

  g_free(cmd);
}

/**
 * 处理标准输入的回掉方法
 * @private
 */
gboolean qp_slave_io_callback(GIOChannel *channel,
                              GIOCondition condition,
                              gpointer data)
{
  QP_Application *app = (QP_Application *)data;
  GError *error = NULL;
  GString *message = g_string_new(NULL);

  GIOStatus status = g_io_channel_read_line_string(channel,
                                                   message,
                                                   NULL,
                                                   &error);

  switch (status)
  {
  case G_IO_STATUS_NORMAL:
  {
    qp_slave_parse_command(message, app);
    g_string_free(message, TRUE);
    return TRUE;
  }
  case G_IO_STATUS_ERROR:
  {
    g_printerr("read stdin error\n");
  }
  }

  return TRUE;
}

/**
 * Slave模式准备
 * @public
 * @param app 应用程序上下文对象
 */

void qp_slave_prepare(QP_Application *app)
{
  GIOChannel *channel = g_io_channel_unix_new(STDIN_FILENO);

  g_io_add_watch(channel, G_IO_IN, qp_slave_io_callback, app);
  g_io_channel_unref(channel);
}