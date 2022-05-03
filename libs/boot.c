#include "../headers/boot.h"
#include "../headers/version.h"

/* 定义静态变量用于解析命令行参数 */
static GString *qp_cmdopt_uri;
static GString *qp_cmdopt_address;
static GString *qp_cmdopt_address6;
static guint qp_cmdopt_port;                                         // 端口号
static guint qp_cmdopt_src_port;                                     // 输入源是UDP模式时，输入的UDP端口号
static GString *qp_cmdopt_src_address;                               // 输入源是UDP模式时，驶入的UDP地址
static guint qp_cmdopt_card = 0;                                     // 输出是local模式时的声卡编号
static guint qp_cmdopt_card_sub = 0;                                 // 输出是local模式时声卡子设备编号
static GString *qp_cmdopt_alsa_device;                               // ALSA设备名称,当输出模式是'local'模式的时候
static guint qp_cmdopt_volume = 60;                                  // 启动时播放器的音量
static gboolean qp_cmdopt_silent = FALSE;                            // 静默模式
static QP_SET_INPUT_TYPE qp_cmdopt_input = QP_SET_INPUT_TYPE_URI;    // 输入源类型
static QP_SET_OUTPUT_TYPE qp_cmdopt_output = QP_SET_OUTPUT_TYPE_NET; // 输出源类型
static QP_SET_QUALITY qp_cmdopt_quality = QP_SET_QUALITY_NORMAL;     // 音质质量

/**
 * 检查输入参数
 * @private
 * @param option_name 参数名称，前面有'--'或者'-'
 * @param value 参数输入的值
 * @param data
 * @param error 返回错误信息的数据对象
 */
static gboolean qp_boot_cmdopt_check(
    const gchar *option_name,
    const gchar *value,
    gpointer data,
    GError *error)
{

  /*----- 通用参数配置 -----*/

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

  /*----- 输出组参数配置 -----*/

  // 检查<output>参数
  if (!g_ascii_strcasecmp("--output", option_name) || !g_ascii_strcasecmp("-o", option_name))
  {
    if (!g_ascii_strcasecmp("local", value))
    {
      qp_cmdopt_output = QP_SET_OUTPUT_TYPE_LOCAL;
      return TRUE;
    }
  }

  // 检查<output-address>参数
  if (!g_ascii_strcasecmp("--output-address", option_name))
  {
    qp_cmdopt_address = g_string_new(value);
    return TRUE;
  }
  // 检查<output-address6>参数
  if (!g_ascii_strcasecmp("--output-address6", option_name))
  {
    qp_cmdopt_address6 = g_string_new(value);
    return TRUE;
  }

  // 检查<alsa-device参数
  if (!g_ascii_strcasecmp("--alsa-device", option_name))
  {
    qp_cmdopt_alsa_device = g_string_new(value);
    return TRUE;
  }

  /*----- 输入组参数配置 -----*/

  // 检查<input>参数
  if (!g_ascii_strcasecmp("--input", option_name) || !g_ascii_strcasecmp("-i", option_name))
  {
    if (!g_ascii_strcasecmp("udp", value))
    {
      qp_cmdopt_input = QP_SET_INPUT_TYPE_UDP;
      return TRUE;
    }

    if (!g_ascii_strcasecmp("fd", value))
    {
      qp_cmdopt_input = QP_SET_INPUT_TYPE_FD;
      return TRUE;
    }
  }

  // 检查<src-address>参数
  if (!g_ascii_strcasecmp("--input-address", option_name))
  {
    qp_cmdopt_src_address = g_string_new(value);
    return TRUE;
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
        "input",
        'i',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Select input mode, default is [uri]",
        "['uri'|'udp'|'fd']",
    },
    {
        "output",
        'o',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Choose output mode, default is [net]",
        "['net'|'local']",
    },
    {
        "quality",
        'q',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Set output quality, when output type is [net]",
        "['low'|'normal'|'high']",
    },
    {
        "volume",
        'v',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_volume,
        "Set volume",
        "[0~100]",
    },
    {NULL},
};

/* 输入源设置参数 */
static GOptionEntry QP_OPTION_SRC_ENTIRES[] = {
    {
        "input-address",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Set source address when input mode is 'udp', default is 127.0.0.1",
        NULL,
    },
    {
        "input-port",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_src_port,
        "Set source port when input mode is 'udp', default is 55100",
        NULL,
    },
    {NULL},
};

/* 输出参数设置组 */
static GOptionEntry QP_OPTION_OUTPUT_ENTIRES[] = {
    {
        "output-address",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Set UDP address when output mode is 'net', default is 234.1.1.1",
        NULL,
    },
    {
        "output-address6",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Set UDP address for IPv6 when output mode is 'net', default is FF15::D:EA",
        NULL,
    },
    {
        "output-port",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_port,
        "Set UDP port when output mode is 'net', default is  55100",
        NULL,
    },
    {
        "outout-card",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_card,
        "Select ALSA sound card number, when output is [local]",
        "[card number]",
    },
    {
        "output-card-sub",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_card_sub,
        "Select ALSA sound card subdevice number, when output is [local]",
        "[subdevice number]",
    },
    {
        "alsa-device",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)qp_boot_cmdopt_check,
        "Set ALSA device, there may be multi device name, sperate with comma. e.g. Card1,Card2",
        "[Card1,Card2,...]",
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
  GOptionGroup *src_group;
  GOptionGroup *output_group;

  /* 处理宽字符集 */
  gchar **args;

#ifdef G_OS_WIN32
  args = g_win32_get_command_line();
#else
  args = g_strdupv(argv);
#endif

  /* 准备版本信息 */
  GString *version = g_string_new(NULL);
  g_string_printf(version, "- version: %d.%d.%d build:" QP_RELEASE_DATE, QP_VERSION_MAJOR, QP_VERSION_MINOR, QP_VERSION_PATCH);

  context = g_option_context_new(version->str);

  /* 添加输入源组src_group */
  src_group = g_option_group_new("src",
                                 "Set the params of UDP source, when the input mode is 'udp'",
                                 "Set the params of UDP source, when the input mode is 'udp'",
                                 NULL,
                                 NULL);
  g_option_group_add_entries(src_group, QP_OPTION_SRC_ENTIRES);
  g_option_context_add_group(context, src_group);

  /* 添加输出组 output_group */
  output_group = g_option_group_new("output",
                                    "Set the params of UDP output, when the output mode is 'net'",
                                    "Set the params of UDP output, when the output mode is 'net'",
                                    NULL,
                                    NULL);
  g_option_group_add_entries(output_group, QP_OPTION_OUTPUT_ENTIRES);
  g_option_context_add_group(context, output_group);

  /* 添加主要帮助信息 */
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
    g_warning("Player unready.\n");
    exit(1);
  }

  /* 判断是否处于静默模式 */
  if (!qp_cmdopt_silent)
  {

    // 设置输出类型信息
    GString *output_message = g_string_new(NULL);
    switch (player->opt_output)
    {
    case QP_SET_OUTPUT_TYPE_NET:
      g_string_append_printf(output_message,
                             "Output: net\n"
                             "IPv4 output address: %s\n"
                             "IPv6 output address: %s\n"
                             "Port: %d\n",
                             player->opt_address->str,
                             player->opt_address6->str,
                             player->opt_port);
      break;
    case QP_SET_OUTPUT_TYPE_LOCAL:
      g_string_append_printf(output_message,
                             "Output: local\n"
                             "Alsa device: %d:%d\n",
                             player->opt_card, player->opt_card_sub);
      break;
    }

    /* 输出版本信息 */
    GString *message = g_string_new("Quark Player\n");
    g_string_append_printf(message,
                           "Version: %d.%d.%d\n", QP_VERSION_MAJOR,
                           QP_VERSION_MINOR,
                           QP_VERSION_PATCH);

    /* 输出URI信息 */
    if (player->opt_input == QP_SET_INPUT_TYPE_URI)
    {
      g_string_append_printf(message,
                             "URI:%s\n",
                             player->opt_uri->str);
    }

    g_string_append(message, output_message->str);

    /* 设置输出质量 */
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

  /* 设置参数 */
  params->uri = qp_cmdopt_uri;
  params->src_address = qp_cmdopt_src_address != NULL ? qp_cmdopt_src_address : g_string_new("127.0.0.1");
  params->address = qp_cmdopt_address != NULL ? qp_cmdopt_address : g_string_new("234.1.1.1");
  params->address6 = qp_cmdopt_address6 != NULL ? qp_cmdopt_address6 : g_string_new("FF15::D:EA");
  params->port = qp_cmdopt_port;
  params->src_port = qp_cmdopt_src_port;
  params->input = qp_cmdopt_input;
  params->output = qp_cmdopt_output;
  params->quality = qp_cmdopt_quality;
  params->volume = qp_cmdopt_volume;
  params->card = qp_cmdopt_card;
  params->card_sub = qp_cmdopt_card_sub;

  // 播放器初始化
  QP_Player *player = app->player;
  qp_player_init(player, params);

  /* 设置播放器初始音量 */
  qp_player_set_volume(player, params->volume);
  qp_player_play(player);

  // 释放资源
  // g_string_free(params->src_address, TRUE);
  // g_string_free(params->uri, TRUE);
  // g_string_free(params->address, TRUE);
  // g_string_free(params->address6, TRUE);
  g_free(params);
}

/**
 * 程序启动准备方法
 * @public
 */
extern void qp_boot(gint argc, gchar **argv, QP_Application *app)
{
  qp_flow_parse_cmdline(argc, argv);

  if (qp_cmdopt_input == QP_SET_INPUT_TYPE_URI && qp_cmdopt_uri == NULL)
  {
    g_print("Please input uri. type 'qplayer -h' show more help\n");
    exit(1);
  }

  qp_flow_set_env(app);
  qp_flow_print_env(app);
  qp_slave_prepare(app);
}