#include "stdmessage.h"

/* 输出错误信息 */
extern void
qp_std_error_output(QP_ERROR_CODE code, const gchar *message)
{
  QP_STD_FMT_OUT("E", "%d:%s", code, message);
}

/* 输出媒体当前播放时间 */
extern void
qp_std_current_time_output(guint64 time)
{
  QP_STD_FMT_OUT("C", "%lld", GST_TIME_AS_MSECONDS(time));
}

/* 输出媒体总播放时间 */
extern void
qp_std_total_time_output(guint64 time)
{
  QP_STD_FMT_OUT("T", "%lld", GST_TIME_AS_MSECONDS(time));
}

/**
 * 输出状态信息
 */
extern void
qp_std_status_output(QP_PLAYER_STATUS status)
{
  QP_STD_FMT_OUT("S", "%d", status);
}

/**
 * 输出播放器信息
 * @param 播放器的数据
 */
extern void
qp_std_info_output(gchar *name, gchar *value)
{
  QP_STD_FMT_OUT("I", "%s=%s", name, value);
}