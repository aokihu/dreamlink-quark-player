#include "../headers/option.h"

/**
 * 全局范围参数配置
 */

static QP_PROGRAM_OPTION qp_g_program_option =
    {
        .silent = FALSE,
        .volume = 60,
        .card = 0,
        .card_sub = 0,
        .output = QP_OPTION_OUTPUT_TYPE_NET,
        .quality = QP_OPTION_QUALITY_NORMAL,
};

/**
 * 参数解析辅助方法
 * 
 * @param gpointer target 程序配置参数目标
 * @param gchar* value 输入的命令行参数字符串
 * @param gchar** options_chars 需要查询的对应配置的字符串数组
 * @param guin length 字符串数组的长度
 */
gboolean qp_option_helper(gpointer target,
                          const gchar *value,
                          const gchar **options_chars,
                          guint length)
{
  gboolean flag = FALSE;

  for (guint i = 0; i < length; i += 1)
  {
    if (g_ascii_strcasecmp(value, options_chars[i]) == 0)
    {
      *(guint *)target = i;
      flag = TRUE;
      break;
    }
  }

  return flag;
}

/**
 * 检查命令行输出设置
 */

gboolean qp_option_check_output(
    const gchar *option_name,
    const gchar *value,
    gpointer data,
    GError *error)
{
  if (!qp_option_helper(&(qp_g_program_option.output),
                        value,
                        QP_OPTION_STRING_OUTPUT_TYPE,
                        QP_OPTION_OUTPUT_TYPE_LENGTH))
  {
    error = g_error_new(G_OPTION_ERROR, G_OPTION_ERROR_FAILED, "output argument is wrong");
    return FALSE;
  }

  return TRUE;
}

/**
 * 检查品质设置参数
 */

gboolean qp_option_check_quality(
    const gchar *option_name,
    const gchar *value,
    gpointer data,
    GError *error)
{
  if (!qp_option_helper(
          &(qp_g_program_option.quality),
          value,
          QP_OPTION_STRING_QUALITY,
          QP_OPTION_QUALITY_LENGTH))
  {
    error = g_error_new(G_OPTION_ERROR, G_OPTION_ERROR_FAILED, "quality argument is wrong");
    return FALSE;
  }
  return TRUE;
}

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
    {NULL}};