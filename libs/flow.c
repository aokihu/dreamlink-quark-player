#include "flow.h"

//
// 定义静态变量用于解析命令行参数
//
static gchar qp_cmdopt_uri[10240];
static gint qp_cmdopt_port;
static gchar qp_cmdopt_address[1024];

//
// 定义命令行解析实体
//
static GOptionEntry QP_OPTION_ENTIRES[] = {
    {
        "uri",
        0,
        G_OPTION_FLAG_FILENAME,
        G_OPTION_ARG_STRING,
        qp_cmdopt_uri,
        "Media resource uri",
        "S",
    },
    {
        "port",
        'p',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &qp_cmdopt_port,
        "UDP broadcast port, when output type is [net]",
        "N",
    },
    {
        "address",
        'a',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_STRING,
        &qp_cmdopt_address,
        "UDP broadcast address, when output type is [net]",
        "S",
    },
    {NULL},
};

QP_CmdParam *qp_flow_parse_cmdline(gint argc, gchar **argv)
{
  QP_CmdParam *_params = g_new(QP_CmdParam, 1);

  GError *error = NULL;
  GOptionContext *context;
  gchar **args;

#ifdef G_OS_WIN32
  args = g_win32_get_command_line();
#else
  args = g_strdupv(argv);
#endif

  context = g_option_context_new("- version: 1.1.0");
  g_option_context_add_main_entries(context, QP_OPTION_ENTIRES, NULL);
  if (!g_option_context_parse(context, &argc, &args, &error))
  {
    exit(1);
  }

  g_strfreev(args);
  return _params;
}