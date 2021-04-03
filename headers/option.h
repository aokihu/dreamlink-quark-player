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
  QP_OPTION_OUTPUT_TYPE_NET,
  QP_OPTION_OUTPUT_TYPE_LOCAL,
} QP_OPTION_OUTPUT_TYPE;

// 定义输出模式的字符串形式
#define QP_OPTION_OUTPUT_TYPE_LENGTH 2
static const gchar *QP_OPTION_STRING_OUTPUT_TYPE[] = {"net", "local"};

/**
 * 网络播放质量配置
 * 
 * @enum QP_OPTION_QUALITY_LOW 低品质输出，但是带宽和延时最低
 * @enum QP_OPTION_QUALITY_NORMAL 一般品质输出，带宽和延迟一般
 * @enum QP_OPTION_QUALITY_HIGH 最高品质输出，带宽最高，延迟可能加长
 */

typedef enum _QP_OPTION_QUALITY
{
  QP_OPTION_QUALITY_LOW,
  QP_OPTION_QUALITY_NORMAL,
  QP_OPTION_QUALITY_HIGH,
} QP_OPTION_QUALITY;

#define QP_OPTION_QUALITY_LENGTH 3
static const gchar *QP_OPTION_STRING_QUALITY[] = {"low", "normal", "high"};

/**
 * 全局配置数据结构
 * 
 * @silent 是否显示信息输出,默认为TRUE
 * @volume 初始播放音量,整数类型
 * @output 输出模式选择,默认为网络输出[net]
 * @card 当输出模式是[local]的时候,设置声卡硬件编号,默认为0
 * @card_sub 当输出模式是[local]的时候,设置声卡硬件子设备编号,默认为0
 * @uri 媒体资源URI地址,本地文件需要绝对地址
 * @fadetime 声音渐响的时间,单位是<秒>,默认是0表示没有渐响
 * @quality 设置网络输出的品质 
 */
typedef struct _QP_PROGRAM_OPTION
{
  gboolean silent;
  guint volume;
  guint card;
  guint card_sub;
  gchar *uri;
  gint fadetime;
  QP_OPTION_OUTPUT_TYPE output;
  QP_OPTION_QUALITY quality;

} QP_PROGRAM_OPTION;

#endif