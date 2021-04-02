/**
 * 命令行参数解析头文件定义
 * @author aokihu
 * @copyright Reasonbox
 * 
 */

#include <glib.h>

#ifndef QP_H_COMMANDLINE
#define QP_H_COMMANDLINE

/**
 * QP_CMD_OPTION_OUTPUT_TYPE
 *
 * 输出类型
 * @NET - 网络输出模式
 * @LOCAL - 本地声卡输出模式
 *
 */
typedef enum _QP_CMD_OPTION_OUTPUT_TYPE
{
  QP_OUTPUT_TYPE_NET = 0,
  QP_OUTPUT_TYPE_LOCAL = 1,
} QP_CMD_OPTION_OUTPUT_TYPE;

// 定义输出模式的字符串形式
static const gchar *QP_CMD_OPTION_OUTPUT_TYPE_NET = "net";
static const gchar *QP_CMD_OPTION_OUTPUT_TYPE_LOCAL = "local";

/**
 * 网络播放质量配置
 * 
 * @enum QP_OPTION_QUALITY_LOW 低品质输出，但是带宽和延时最低
 * @enum QP_OPTION_QUALITY_NORMAL 一般品质输出，带宽和延迟一般
 * @enum QP_OPTION_QUALITY_HIGH 最高品质输出，带宽最高，延迟可能加长
 */

typedef enum _QP_CMD_OPTION_QUALITY
{
  QP_OPTION_QUALITY_LOW,
  QP_OPTION_QUALITY_NORNAL,
  QP_OPTION_QUALITY_HIGH,
} QP_CMD_OPTION_QUALITY;

static const gchar *QP_CMD_OPTION_QUALITY_LOW = "low";
static const gchar *QP_CMD_OPTION_QUALITY_NORMAL = "normal";
static const gchar *QP_CMD_OPTION_QUALITY_HIGH = "high";

/**
 * 全局配置数据结构
 * 
 * @silent 是否显示信息输出,默认为TRUE
 * @volume 初始播放音量,整数类型
 * @output 输出模式选择,默认为网络输出[net]
 * @card 当输出模式是[local]的时候,设置声卡硬件编号,默认为0
 * @card_sub 当输出模式是[local]的时候，设置声卡硬件子设备编号,默认为0
 * @quality 设置网络输出的品质 
 */
typedef struct _QP_PROGRAM_OPTION
{
  gboolean silent;
  guint volume;
  guint card;
  guint card_sub;
  QP_CMD_OPTION_OUTPUT_TYPE output;
  QP_CMD_OPTION_QUALITY quality;

} QP_PROGRAM_OPTION;

#endif