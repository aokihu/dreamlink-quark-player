/**
 * 配置数据定义 
 * 
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.0
 * @date Mon Apr  5 16:43:03 CST 2021
 * 
 */

#ifndef QP_H_SETTING
#define QP_H_SETTING

#include <glib.h>

/**
 * 启动参数结构体
 * @struct QP_CmdParam
 * 
 */
struct _QP_CMD_PARAM
{
  gint port;
  gchar *address;
  gchar *output;
  gchar *quality;
  guint volume;

} QP_CmdParam;

#endif