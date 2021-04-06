#include "boot.h"
#include "version.h"

//
// 定义静态变量用于解析命令行参数
//

static GString *qp_cmdopt_uri;
static GString *qp_cmdopt_address;
static guint qp_cmdopt_port;
static guint qp_cmdopt_card = 0;
static guint qp_cmdopt_card_sub = 0;
static guint qp_cmdopt_volume = 60;
static gboolean qp_cmdopt_silent = FALSE;
static QP_SET_OUTPUT_TYPE qp_cmdopt_output = QP_SET_OUTPUT_TYPE_NET;
static QP_SET_QUALITY qp_cmdopt_quality = QP_SET_QUALITY_NORMAL;

/**
 * @private
 */
gboolean qp_boot_cmdopt_check(
    const gchar *option_name,
    const gchar *value,
    gpointer data,
    GError *error)
{

  // 检查<silent>参数
  if (!g_ascii_strcasecmp("--silent", option_name) || !g_ascii_strcasecmp("-s", option_name))
  {
    qp_cmdopt_silent = TRUE;
    return TRUE;
  }

  // 检查<uri>参数
  if (!g_ascii_strcasecmp("--uri", option_name))
  {
    qp_cmdopt_uri = g_string_new(value);
    return TRUE;
  }

  // 检查<address>参数
  if (!g_ascii_strcasecmp("--address", option_name))
  {
    qp_cmdopt_address = g_string_new(value);
    return TRUE;
  }

  // 检查<output>参数
  if (!g_ascii_strcasecmp("--output", option_name) || !g_ascii_strcasecmp("-o", option_name))
  {
    if (!g_ascii_strcasecmp("local", value))
    {
      qp_cmdopt_output = QP_SET_OUTPUT_TYPE_LOCAL;
      return TRUE;
    }
  }

  // 检查<quality>参数
  if (!g_ascii_strcasecmp("--quality", option_name) || !g_ascii_strcasecmp("-q", option_name))
  {
    if (!g_ascii_strcasecmp("low", value))
    {
      qp_cmdopt_quality = QP_SET_QUALITY_LOW;
      return TRUE;
    }
    else if (!g_ascii_strcasecmp("high", value))
    {
      qp_cmdopt_quality = QP_SET_QUALITY_HIGH;
      return TRUE;
    }
  }

  return TRUE;
}
//
// 定义命令行解析实体
//
static GOptionEntry QP_OPTION_ENTIRES[] = {
    {
        "silent",
        's',
        G_OPTION_FLAG_NO_ARG,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Silent mode, do not display info message",
        NULL,
    },
    {
        "uri",
        0,
        G_OPTION_FLAG_FILENAME,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Media resource uri",
        "[uri://...]",
    },
    {
        "output",
        'o',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Choose output mode",
        "'net'|'local'",
    },
    {
        "quality",
        'q',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Set output quality, when output type is [net]",
        "'low'|'normal'|'high'",
    },
    {
        "volume",
        'v',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_volume,
        "Set volume",
        "0~100",
    },
    {
        "port",
        'p',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_port,
        "UDP broadcast port, when output type is [net]",
        "[port number]",
    },
    {
        "address",
        'a',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "UDP broadcast address, when output type is [net]",
        "234.1.1.1",
    },
    {
        "card",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_card,
        "Select ALSA sound card number, when output is [local]",
        "[card number]",
    },
    {
        "card-sub",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_card_sub,
        "Select ALSA sound card subdevice number, when output is [local]",
        "[subdevice number]",
    },
    {NULL},
};

/**
 * @flow 解析命令行参数
 */
void qp_flow_parse_cmdline(gint argc, gchar **argv)
{
  GError *error = NULL;
  GOptionContext *context;
  gchar **args;

#ifdef G_OS_WIN32
  args = g_win32_get_command_line();
#else
  args = g_strdupv(argv);
#endif

  GString *version = g_string_new(NULL);
  g_string_printf(version, "- version: %d.%d.%d", QP_VERSION_MAJOR, QP_VERSION_MINOR, QP_VERSION_PATCH);

  context = g_option_context_new(version->str);
  g_option_context_add_main_entries(context, QP_OPTION_ENTIRES, NULL);
  if (!g_option_context_parse(context, &argc, &args, &error))
  {
    exit(1);
  }

  g_string_free(version, TRUE);
  g_strfreev(args);
}

/**
 * 打印参数
 * @private
 */
void qp_flow_print_env(QP_Application *application)
{
  QP_Player *player = application->player;

  if (!player->status_ready)
  {
    g_printerr("Player unready.\n");
    exit(1);
  }

  if (!qp_cmdopt_silent)
  {

    // 设置输出类型信息
    GString *output_message = g_string_new(NULL);
    switch (player->opt_output)
    {
    case QP_SET_OUTPUT_TYPE_NET:
      g_string_append_printf(output_message,
                             "Output: net\n"
                             "Address: %s\n"
                             "Port: %d\n",
                             player->opt_address->str,
                             player->opt_port);
      break;
    case QP_SET_OUTPUT_TYPE_LOCAL:
      g_string_append_printf(output_message,
                             "Output: local\n"
                             "Alsa Card: %d:%d",
                             player->card, player->card_sub);
      break;
    }

    GString *message = g_string_new("Quark Player\n");
    g_string_append_printf(message,
                           "Version: %d.%d.%d\n"
                           "URI: %s\n",
                           QP_VERSION_MAJOR,
                           QP_VERSION_MINOR,
                           QP_VERSION_PATCH,
                           player->opt_uri->str);

    g_string_append(message, output_message->str);

    // 设置输出质量
    GString *quality_message = g_string_new(NULL);
    switch (player->opt_quality)
    {
    case QP_SET_QUALITY_LOW:
      g_string_printf(quality_message, "Quality: low\n");
      break;
    case QP_SET_QUALITY_NORMAL:
      g_string_printf(quality_message, "Qulaity: normal\n");
      break;
    case QP_SET_QUALITY_HIGH:
      g_string_printf(quality_message, "Quality: high\n");
      break;
    }

    g_string_append(message, quality_message->str);
    g_print("%s", message->str);

    // 释放资源
    g_string_free(quality_message, TRUE);
    g_string_free(output_message, TRUE);
    g_string_free(message, TRUE);
  }
}

/**
 * 设置环境配置
 */
void qp_flow_set_env(QP_Application *app)
{
  QP_CmdParam *params = g_new(QP_CmdParam, 1);

  params->uri = qp_cmdopt_uri;
  params->address = qp_cmdopt_address != NULL ? qp_cmdopt_address : g_string_new("234.1.1.1");
  params->port = qp_cmdopt_port;
  params->output = qp_cmdopt_output;
  params->quality = qp_cmdopt_quality;
  params->volume = qp_cmdopt_volume;
  params->card = qp_cmdopt_card;
  params->card_sub = qp_cmdopt_card_sub;

  // 播放器初始化
  QP_Player *player = app->player;
  qp_player_init(player, params);

  // 释放资源
  g_string_free(params->uri, TRUE);
  g_string_free(params->address, TRUE);
  g_free(params);
}

/**
 * 程序启动准备方法
 * @public
 */
extern void qp_boot(gint argc, gchar **argv, QP_Application *app)
{
  qp_flow_parse_cmdline(argc, argv);

  if (qp_cmdopt_uri == NULL)
  {
    g_printerr("Please input uri. type 'qplayer -h' show more help\n");
    exit(1);
  }

  qp_flow_set_env(app);
  qp_flow_print_env(app);
}