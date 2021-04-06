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
 * 输出类型枚举
 * @enum QP_SET_OUTPUT_TYPE
 */
typedef enum _QP_SET_OUTPUT_TYPE
{
  QP_SET_OUTPUT_TYPE_NET,
  QP_SET_OUTPUT_TYPE_LOCAL,
} QP_SET_OUTPUT_TYPE;

/**
 * 网络传输质量类型枚举
 * @enum QP_SET_QUALITY
 */
typedef enum _QP_SET_QUALITY
{
  QP_SET_QUALITY_LOW,
  QP_SET_QUALITY_NORMAL,
  QP_SET_QUALITY_HIGH,
} QP_SET_QUALITY;

/**
 * 启动参数结构体
 * @struct QP_CmdParam
 * 
 */
typedef struct _QP_CMD_PARAM
{
  guint port;
  guint volume;
  guint card;
  guint card_sub;
  GString *uri;
  GString *address;
  QP_SET_OUTPUT_TYPE output;
  QP_SET_QUALITY quality;

} QP_CmdParam;

#endif