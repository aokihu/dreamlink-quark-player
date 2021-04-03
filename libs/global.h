/**
 * 
 * 应用程序全局数据结构定义
 * 
 */

#include <glib.h>
#include <gst/gst.h>

#pragma once
#ifndef QP_H_GLOBAL
#define QP_H_GLOBAL

/**
 * 应用程序数据结构体
 * 
 * @mainContext 主循环体的上下文对象
 * @mainLoop 主循环体对象
 */
typedef struct _QP_STRUCT_APPLICATION
{
  GMainContext *mainContext;
  GMainLoop *mainLoop;
  GstPipeline *gstPipline;
  GstBus *gstBus;
} QP_APPLICATION;

/**
 * 定义全局静态应用程序数据对象
 */
static QP_APPLICATION qp_application
{
  .mainContext = NULL,
  .mainLoop = NULL,
}

#endif