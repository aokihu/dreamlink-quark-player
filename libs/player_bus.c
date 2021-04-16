/**
 * Player消息总线处理
 * @author aokihu
 * @copyright Reasonbox
 */

#include "player.h"
#include "stdmessage.h"
#include "error.h"

/* 获取显示播放时间 */
gboolean qp_player_position_handler(gpointer userdata)
{
  QP_Player *player = (QP_Player *)userdata;
  gint64 pos, len;

  /* 当播放器处于非播放状态的时候，取消定时器监听 */
  if (player->status != QP_PLAYER_STATUS_PLAYING)
  {
    return FALSE;
  }

  if (gst_element_query_position(GST_ELEMENT(player->gst_pipeline), GST_FORMAT_TIME, &pos) && gst_element_query_duration(GST_ELEMENT(player->gst_pipeline), GST_FORMAT_TIME, &len))
  {
    qp_std_time_output(len, pos);
  }
  return TRUE;
}

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

  /* 释放资源 */
  g_free(debug);
  g_error_free(error);
}

/**
 * 处理管道状态改变
 */
void qp_player_status_changed_handler(GstMessage *message, gpointer userdata)
{
  GstState oldState;
  GstState newState;
  GstState pending;

  QP_Player *player = (QP_Player *)userdata;
  gst_message_parse_state_changed(message, &oldState, &newState, &pending);

  if (GST_MESSAGE_SRC(message) == GST_OBJECT(player->gst_pipeline))
  {
    // PAUSED => PLAYING
    if (oldState == GST_STATE_PAUSED && newState == GST_STATE_PLAYING)
    {
      player->timer_flag = g_timeout_add(300, qp_player_position_handler, player);
      player->status = QP_PLAYER_STATUS_PLAYING;
    }
  }
}

/* 接收Bus处理消息 */
gboolean qp_player_bus_handler(GstBus *bus, GstMessage *message, gpointer userdata)
{
  QP_Player *player = (QP_Player *)userdata;

  switch (GST_MESSAGE_TYPE(message))
  {
    /* 播放停止消息 */
  case GST_MESSAGE_EOS:
  {
    player->status = QP_PLAYER_STATUS_EOS;
    qp_std_status_output(QP_PLAYER_STATUS_EOS);
    break;
  }
  /* 发生错误 */
  case GST_MESSAGE_ERROR:
  {
    qp_player_error_handler(message, userdata);
    return FALSE;
    break;
  }
  /* 状态改变 */
  case GST_MESSAGE_STATE_CHANGED:
  {
    qp_player_status_changed_handler(message, userdata);
    break;
  }
  }

  return TRUE;
}