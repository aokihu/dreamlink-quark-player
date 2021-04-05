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
  player->opt_port = 0;
  player->opt_address = g_string_new(NULL);
  player->status_playing = FALSE;

  // 设置URI
  gchar *uri = params->uri;
  g_assert_nonnull(uri);
  player->opt_uri = g_string_new(params->uri);

  // 设置输出模式
  if (g_ascii_strcasecmp("net", params->output) == 0)
  {
    player->opt_output = QP_SET_OUTPUT_TYPE_NET;
  }
  else if (g_ascii_strcasecmp("local", params->output) == 0)
  {
    player->opt_output = QP_SET_OUTPUT_TYPE_LOCAL;
  }
  else
  {
    player->opt_output = QP_SET_OUTPUT_TYPE_NET;
  }

  // 设置输出质量
  if (player->opt_output == QP_SET_OUTPUT_TYPE_NET)
  {
    if (g_ascii_strcasecmp("low", params->quality) == 0)
    {
      player->opt_quality = QP_SET_QUALITY_LOW;
    }
    else if (g_ascii_strcasecmp("normal", params->quality) == 0)
    {
      player->opt_quality = QP_SET_QUALITY_NORMAL;
    }
    else if (g_ascii_strcasecmp("high", params->quality) == 0)
    {
      player->opt_quality = QP_SET_QUALITY_HIGH;
    }
    else
    {
      player->opt_quality = QP_SET_QUALITY_NORMAL;
    }
  }
  else
  {
    player->opt_quality = QP_SET_QUALITY_NORMAL;
  }

  // 播放器出于准备状态
  player->status_ready = TRUE;
}
