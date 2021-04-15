/**
 * std_message标准输入输出处理方法
 * @author aokihu
 * @copyright Reasonbox
 */

#include <glib.h>

#ifndef QP_H_STD_MESSAGE
#define QP_H_STD_MESSAGE

#include "player.h"
#include "error.h"

/**
 * 统一格式输出方法 
 * 格式 qp_stdout(前缀, 数据格式, 数据...)
 * 输出 => g_printerr("前缀:数据格式", 数据...)
 * 
 * @前缀 用一个字符串表示，通常是E,T,I等等
 * @数据格式 数据格式是表示数据的数据，为了后续程序能够理解使用特殊的分隔符标识
 * @数据 可变长度的数据
 * 
 */
#define QP_STD_FMT_OUT(prefix, format, args...) g_printerr(prefix           \
                                                           ":" format "\n", \
                                                           ##args)

/**
 * 输出状态信息
 */
extern void
qp_std_status_output(QP_PLAYER_STATUS status);

/** 
 * 输出错误消息
 * @param code 错误消息代码
 * @param message 错误小的具体文字内容
 */

extern void qp_std_error_output(QP_ERROR_CODE code, const gchar *message);

/**
 * 输出tag信息
 * @param tag tag数据
 */
extern void
qp_std_tag_output(gchar *tag);

/**
 * 输出事件信息
 * @param total 总时间
 * @param time 当前时间
 */
extern void
qp_std_time_output(guint64 total, guint64 time);

/**
 * 输出播放器信息
 * @param 信息名
 * @param 信息值
 */
extern void
qp_std_info_output(gchar *name, gchar *value);

#endif