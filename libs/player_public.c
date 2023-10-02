/**
 *
 * 播放器运行库
 *
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.1
 * @date Mon Apr  5 18:04:41 CST 2021
 *
 */

#include "../headers/player.h"
#include "../headers/stdmessage.h"

/**
 * 创建新播放器对象
 * @public
 * @return QP_Player* 播放器对象指针
 */
QP_Player *qp_player_new()
{
  QP_Player *_player = g_new(QP_Player, 1);
  _player->status_ready = FALSE;
  return _player;
}

/**
 * 初始化播放器
 * @public
 * @param player 播放器对象
 */
void qp_player_init(QP_Player *player, QP_CmdParam *params)
{
  // 设置播放器初始化状态
  player->status_ready = FALSE;
  player->status = QP_PLAYER_STATUS_NOT_READY;
  player->timer_flag = 0;

  /* ------------------------------- */
  /*             设置URI              */
  /* ------------------------------- */

  /* --------- 输入类型 = URI ------- */
  if (params->input == QP_SET_INPUT_TYPE_URI)
  {
    player->opt_uri = g_string_new(params->uri->str);
  }

  /* --------- 输入类型 = UDP ------ */
  if (params->input == QP_SET_INPUT_TYPE_UDP)
  {
    player->opt_uri = g_string_new("udp://");
    g_string_append_printf(player->opt_uri, "%s:%d", params->src_address->str, params->src_port);
  }

  /* 设置音量 */
  player->opt_volume = params->volume;

  /* 设置输出质量 */
  player->opt_quality = params->quality;

  /* 设置播放器输入模式 */
  player->opt_input = params->input;

  /* 设置播放器输出模式 */
  player->opt_output = params->output;

  switch (player->opt_output)
  {
  case QP_SET_OUTPUT_TYPE_NET:
    player->opt_address = g_string_new(params->address->str);
    player->opt_address6 = g_string_new(params->address6->str);
    player->opt_port = params->port;
    break;
  case QP_SET_OUTPUT_TYPE_LOCAL:
    player->opt_card = params->card;
    player->opt_card_sub = params->card_sub;
    player->opt_alsa_devices = params->alsa_devices;
    break;
  case QP_SET_OUTPUT_TYPE_FUSION;
    player->opt_output_list = params->output_list;
    break;
  }

  // 构造各个组件
  qp_player_make_pipeline(player);

  // 设置播放器为准备状态
  player->status_ready = TRUE;
  player->status = QP_PLAYER_STATUS_READY;
}

/**
 * 播放器播放
 * @public
 */
extern void qp_player_play(QP_Player *player)
{
  if (QP_PLAYER_IS_OK(player))
  {

    //
    // 如果是UDP输出，设置广播端口和地址
    //

    if (player->opt_output == QP_SET_OUTPUT_TYPE_NET)
    {
      GstElement *sink = gst_bin_get_by_name_recurse_up(GST_BIN(player->gst_pipeline), QP_PLAYER_ELEMENT_UDPSINK);
      g_signal_emit_by_name(sink, "add", player->opt_address->str, player->opt_port);
      g_signal_emit_by_name(sink, "add", player->opt_address6->str, player->opt_port);
      gst_object_unref(sink);
    }

    gst_element_set_state(GST_ELEMENT(player->gst_pipeline), GST_STATE_PLAYING);
    qp_std_status_output(QP_PLAYER_STATUS_PLAYING);
  }
}

/**
 * 设置音量
 * @public
 * @param volume 将要设置的音量，如果输入值超过了[0..100]的范围
 *                将会自动设置为最接近的数值
 */
extern void qp_player_set_volume(QP_Player *player, gint64 volume)
{
  if (QP_PLAYER_IS_OK(player))
  {
    gdouble vol = volume / 100.0;
    GstElement *obj_volume = gst_bin_get_by_name_recurse_up(GST_BIN(player->gst_pipeline), QP_PLAYER_ELEMENT_VOLUME);
    g_object_set(obj_volume, "volume", vol, NULL);
    gchar *volStr = g_strdup_printf("%lld", volume);
    qp_std_info_output("volume", volStr);

    /* 释放资源 */
    g_free(volStr);
    gst_object_unref(obj_volume);
  }
}