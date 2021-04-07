/**
 * Main
 * 
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.0
 * @date Mon Apr  5 16:43:03 CST 2021
 * 
 */

#include "libs/common.h"

int main(gint argc, gchar **argv)
{
  // Gstreamer初始化
  gboolean gst_ok = gst_init_check(&argc, &argv, NULL);
  if (!gst_ok)
  {
    g_printerr("Gstreamer is not OK\n");
  }

  // 初始化全局应用对象
  QP_Application *application;
  application = g_new(QP_Application, 1);

  QP_Player *player = g_new(QP_Player, 1);
  application->player = player;

  application->mainLoop = g_main_loop_new(NULL, FALSE);
  application->mainContext = g_main_loop_get_context(application->mainLoop);

  // Boot
  qp_boot(argc, argv, application);

  // 播放
  qp_player_play(application->player);

  // 主循环运行
  g_main_loop_run(application->mainLoop);

  // 释放资源
  g_main_loop_unref(application->mainLoop);

  g_object_unref(application->player->gst_pipeline);
  g_main_loop_unref(application->mainLoop);
  g_main_context_unref(application->mainContext);
  g_free(application);

  exit(0);
}