#include "slave.h"
#include <unistd.h>

void qp_slave_process_command(QP_SlaveCommand *cmd, QP_Application *app)
{

  switch (cmd->command)
  {
  case QP_SLAVE_COMMAND_QUIT:
    g_main_loop_quit(app->mainLoop);
    break;
  }
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

  for (gint i = 0; i < sizeof(QP_SLAVE_COMMANDS); i++)
  {
    if (!g_strcmp0(_command[0], QP_SLAVE_COMMANDS[i]))
    {
      cmd->command = i;
      break;
    }
  }

  qp_slave_process_command(cmd, app);
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