/**
 * 程序执行流程
 */

#include "../libs.h"

/**
 * @flow Parse_Commandline
 * 解析命令行参数
 *
 * @param gint argc 命令行参数数量
 * @param gchar** argv 命令行参数实际参数字符串
 * @return void
 *
 */

extern void qp_flow_parse_commandline(gint argc, gchar **argv)
{
  GError_autoptr error = NULL;
  GString_autoptr version = qp_get_software_version();
  GString_autoptr infoMessage = g_string_new(NULL);
  GString_autoptr summaryMessage = g_string_new(NULL);
  g_string_append_printf(infoMessage, "- set option for quark player");
  g_string_append_printf(summaryMessage, "Quark player is used for dreamlink service\nVersion %s", version->str);
  GOptionContext_autoptr context = g_option_context_new(infoMessage->str);

  g_option_context_set_summary(context, summaryMessage->str);
  g_option_context_add_main_entries(context, qp_g_option_entries, NULL);

  // 执行解析操作
  if (!g_option_context_parse(context, &argc, &argv, &error))
  {
    g_print("%s\n", error->message);
    exit(0);
  }

  // 释放资源
  g_option_context_free(context);
  g_string_free(infoMessage, TRUE);
  g_string_free(summaryMessage, TRUE);
  g_string_free(version, TRUE);
}

/**
 * @flow Display Summary
 * 打印运行时参数数据
 *
 * @return void
 *
 */

extern void qp_flow_display_summary()
{
  GString_autoptr version = qp_get_software_version();
  GString_autoptr summary = g_string_new(NULL);
  GString_autoptr output = g_string_new(NULL);

  // 设置输出模式显示
  if (qp_g_program_option.output == QP_OUTPUT_TYPE_NET)
  {
    g_string_append_printf(output, "net");
  }
  else if (qp_g_program_option.output == QP_OUTPUT_TYPE_LOCAL)
  {
    g_string_append_printf(output, "local");
  }

  // 整合所有打印数据
  g_string_append_printf(summary,
                         "Quark player - %s\n"
                         "Copyright for Reasonbox, only be used on Dreamlink devices\n"
                         "Initialization volume: %d\n"
                         "Output mode: %s",
                         version->str,
                         qp_g_program_option.volume,
                         output->str);
  // 打印数据
  g_print("%s", summary->str);

  // 释放资源
  g_string_free(output, TRUE);
  g_string_free(version, TRUE);
  g_string_free(summary, TRUE);
}