/**
 * std_message标准输入输出处理方法
 * @author aokihu
 * @copyright Reasonbox
 */

#include <glib.h>

#ifndef QP_H_STD_MESSAGE
#define QP_H_STD_MESSAGE

/* 通过枚举类型获取前缀字符串 */
#define QP_STD_MESSAGE_PREFIX_STRING(prefix) _QP_STD_MESSAGE_PREFIX_STRINGS[(prefix)]

/**
 * 消息前缀枚举
 * @QP_STD_MESSAGE_PREFIX_STATUS 播放器状态
 */
typedef enum _QP_STD_MESSAGE_PREFIX
{
  QP_STD_MESSAGE_PREFIX_STATUS,
} QP_STD_MESSAGE_PREFIX;

/* 前缀字符串数组 */
static const gchar *_QP_STD_MESSAGE_PREFIX_STRINGS[] = {"status"};

/**
 * 向标准错误输出显示消息
 * @param message 显示的消息信息
 */
extern void
qp_std_output(QP_STD_MESSAGE_PREFIX prefix, const gchar *message);

#endif