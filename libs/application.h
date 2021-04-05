/**
 * 应用程序定义 
 * 
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.0
 * @date Mon Apr  5 16:43:03 CST 2021
 * 
 */

#include "player.h"
#include <glib.h>

#ifndef QP_H_APPLICATION
#define QP_H_APPLICATION

/**
 * 应用程序结构体
 * @struct QP_Application
 * @property mainContext 主循环上下文对象
 * @property mainLoop 主循环体对象
 * @property player 播放器对象
 */
typedef struct _QP_APPLICATION
{
  GMainContext *mainContext;
  GMainLoop *mainLoop;
  QP_Player *player;
} QP_Application;

#endif