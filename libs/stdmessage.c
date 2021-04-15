#include "stdmessage.h"

/* 输出错误信息 */
extern void
qp_std_error_output(QP_ERROR_CODE code, const gchar *message)
{
  QP_STD_FMT_OUT("E", "%d:%s\n", code, message);
}

/* 输出时间信息 */
extern void
qp_std_time_output(guint64 total, guint time)
{
  QP_STD_FMT_OUT("T", "%lld/%lld", total, time);
}