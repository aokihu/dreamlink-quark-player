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
 * 构造中间输出组件
 * @private
 * 
 * 构造Source和Sink之间的处理组件
 * 中间组件用来处理通用的音效处理
 * 
 * - volume 音量控制
 * - audioconvert 音乐转换
 * - audiorate 
 * 
 */

void qp_player_construct_middle_component(QP_Player *player)
{
  // GstElement *volume = gst_element_factory_make("volume", QP_PLAYER_ELEMENT_VOLUME);
}

/**
 * 构造音乐播放器组件
 * @private
 */
void qp_player_constrcut_components(QP_Player *player)
{
}

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
  player->status_playing = FALSE;

  // 设置URI
  player->opt_uri = g_string_new(params->uri->str);

  // 设置输出质量
  player->opt_quality = params->quality;

  // 设置播放器输入模式
  player->opt_input = params->input;

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

  // 构造各个组件
  qp_player_constrcut_components(player);

  // 播放器出于准备状态
  player->status_ready = TRUE;
}

/**
 * 播放器播放
 * @public
 */
extern void qp_player_play(QP_Player *player)
{
}
