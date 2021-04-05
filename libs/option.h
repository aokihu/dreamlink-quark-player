/**
 * 
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

/**
 * 全局范围参数配置
 */

static QP_PROGRAM_OPTION
    qp_g_program_option =
        {
            .silent = FALSE,
            .volume = 60,
            .card = 0,
            .card_sub = 0,
            .uri = NULL,
            .fadetime = 0,
            .output = QP_OPTION_OUTPUT_TYPE_NET,
            .quality = QP_OPTION_QUALITY_NORMAL,
};

//
// 私有方法定义
//

// 检查参数辅助方法
gboolean qp_option_helper(gpointer target,
                          const gchar *value,
                          const gchar **options_chars,
                          guint length);

// 检查输出模式
gboolean qp_option_check_output(
    const gchar *option_name,
    const gchar *value,
    gpointer data,
    GError *error);

// 检查品质设置参数
gboolean qp_option_check_quality(
    const gchar *option_name,
    const gchar *value,
    gpointer data,
    GError *error);

/**
 * 定义命令行解析参数
 */

static GOptionEntry qp_g_option_entries[] = {
    /** 静默设置 **/
    {
        "silent",
        's',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_NONE,
        &(qp_g_program_option.silent),
        "Disable print runtime info",
        NULL,
    },
    /** 音量设置 **/
    {
        "volume",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &(qp_g_program_option.volume),
        "Set volume, range from 0 to 100",
        "N",
    },
    /** 输出模式设置 **/
    {
        "output",
        'o',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)&qp_option_check_output,
        "Choose output mode",
        "[net|local]",
    },
    /** 声卡编号设置 **/
    {
        "card",
        'c',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &(qp_g_program_option.card),
        "Set card number when output mode is [local], default is 0",
        "N",
    },
    /** 声卡子设备编号设置 **/
    {
        "card-sub",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &(qp_g_program_option.card_sub),
        "Set card subdevice number when output mode is [local], default is 0",
        "N",
    },
    /** 网络输出品质设置 **/
    {
        "quality",
        'q',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_CALLBACK,
        (GOptionArgFunc *)&qp_option_check_quality,
        "Set quality of stream when mode is [net]",
        "[low|normal|high]",
    },
    /** URI **/
    {
        "uri",
        'u',
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_STRING,
        &(qp_g_program_option.uri),
        "The media resource URI",
        "[http://|file://]",
    },
    {
        "fade",
        0,
        G_OPTION_FLAG_NONE,
        G_OPTION_ARG_INT,
        &(qp_g_program_option.fadetime),
        "Set volume fadein time, default is 0 which mean no fadein, unit is <seconds>",
        "N",
    },
    {NULL}};

#endif