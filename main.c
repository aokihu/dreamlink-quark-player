/**
 * Dreamlink Quark Player
 * 
 * @author aokihu
 * @copyright Reasonbox
 * @version 1.0.0
 * 
 */
#include "libs.h"

int main(gint argc, gchar **argv)
{

  //
  // 处理命令行输入的参数
  //
  qp_flow_parse_commandline(argc, argv);

  //
  // 显示程序运行参数
  //
  if (!qp_g_program_option.silent)
  {
    qp_flow_display_summary();
  }

  //
  // 全局事件循环
  //
  GMainLoop *loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);

  return 0;
}
