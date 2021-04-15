/**
 * Player消息总线处理
 * @author aokihu
 * @copyright Reasonbox
 */

#include "player.h"
#include "stdmessage.h"
#include "error.h"

/* 处理Bus总线错误 */
void qp_player_error_handler(GstMessage *message, gpointer userdata)
{
  GError *error;
  gchar *debug;
  gst_message_parse_error(message, &error, &debug);

  /* domain是Gst Resource的时候 */
  if (GST_RESOURCE_ERROR == error->domain)
  {
    switch (error->code)
    {
      /* URI资源没有找到 */
    case GST_RESOURCE_ERROR_NOT_FOUND:
      qp_std_error_output(QP_ERROR_RESOURCE_NOT_FOUND, "resource not found");
      break;
    }
  }
}

/* 接收Bus处理消息 */
gboolean qp_player_bus_handler(GstBus *bus, GstMessage *message, gpointer userdata)
{
  switch (GST_MESSAGE_TYPE(message))
  {
    /* 播放停止消息 */
  case GST_MESSAGE_EOS:
  {
    break;
  }
  case GST_MESSAGE_ERROR:
  {
    qp_player_error_handler(message, userdata);
    return FALSE;
    break;
  }
  }

  return TRUE;
}