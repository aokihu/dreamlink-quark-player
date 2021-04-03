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
  qp_application.mainLoop = g_main_loop_new(NULL, FALSE);
  qp_application.mainContext = g_main_context_default();

  // 启动主循环
  g_main_loop_run(qp_application.mainLoop);
  g_main_loop_unref(qp_application.mainLoop);

  return 0;
}
