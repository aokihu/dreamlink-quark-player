#include "stdmessage.h"

extern void qp_std_output(QP_STD_MESSAGE_PREFIX prefix, const gchar *message)
{
  g_printerr("%s:%s", QP_STD_MESSAGE_PREFIX_STRING(prefix), message);
}