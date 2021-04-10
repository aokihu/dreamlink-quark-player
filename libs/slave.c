#include "slave.h"

//
// 全局静态变量定义
//

static QP_SlaveCommandItem QP_SLAVE_COMMAND_LIST[] = {
    {"quit", G_TYPE_NONE, qp_slave_cb_quit},
    {"vol", G_TYPE_INT64, qp_slave_cb_set_volume},
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
 * 处理设定音量的回掉方法
 * @private
 * @param cmd 解析后的命令数据包
 * @param data 附加的数据，一般就是application
 * @return void
 */
void qp_slave_cb_set_volume(QP_SlaveCommand *cmd, gpointer data)
{
  if (NULL != cmd->value)
  {
    QP_Application *app = (QP_Application *)data;
    gint64 volume = g_ascii_strtoll(cmd->value, NULL, 10);
    qp_player_set_volume(app->player, volume);
  }
}

/**
 * 解析指令字符串
 * @private
 */
void qp_slave_parse_command(GString *message, QP_Application *app)
{
  gchar *_message = g_strstrip(message->str);
  gchar **args = g_strsplit(_message, ":", -1);
  gsize size_args = g_strv_length(args);

  QP_SlaveCommand *cmd = g_new(QP_SlaveCommand, 1);
  guint idx = 0;

  while (idx < 256)
  {
    QP_SlaveCommandItem item = QP_SLAVE_COMMAND_LIST[idx];

    // 遇到NULL集合就退出循环
    if (item.cmd_name == NULL)
      break;

    // 匹配指令字符串
    if (!g_ascii_strcasecmp(args[0], item.cmd_name))
    {
      QP_SlaveFunc func = item.func;
      cmd->raw_command = args[0];
      cmd->value = size_args > 1 ? args[1] : NULL;

      func(cmd, app);
    }

    // 序号自增
    idx += 1;
  }

  g_strfreev(args);
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