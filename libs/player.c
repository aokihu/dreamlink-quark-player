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

#include "player.h"

/**
 * 创建新播放器对象
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
 * @param player 播放器对象
 */
void qp_player_init(QP_Player *player, QP_CmdParam *params)
{
  player->status_playing = FALSE;

  // 设置URI
  player->opt_uri = g_string_new(params->uri->str);

  // 设置输出质量
  player->opt_quality = params->quality;

  // 设置播放器输出模式
  player->opt_output = params->output;

  switch (player->opt_output)
  {
  case QP_SET_OUTPUT_TYPE_NET:
    player->opt_address = g_string_new(params->address->str);
    player->opt_port = params->port;
    break;
  case QP_SET_OUTPUT_TYPE_LOCAL:
    player->card = params->card;
    player->card_sub = params->card_sub;
    break;
  }

  // 播放器出于准备状态
  player->status_ready = TRUE;
}

/**
 * 播放器播放
 */
extern void qp_player_play(QP_Player *player)
{
  g_print("playing...\n");
  player->gst_source = gst_element_factory_make("playbin", "play");
  g_assert_nonnull(player->gst_source);
  // player->gst_sink = gst_element_factory_make("autodetect", "autoaudiosink");
  // g_assert_nonnull(player->gst_sink);

  g_object_set(G_OBJECT(player->gst_source), "uri", player->opt_uri->str, NULL);

  gst_element_set_state(GST_ELEMENT(player->gst_source), GST_STATE_PLAYING);
}
