#include "../headers/player.h"

/**
 * @brief 生成ALSA Sink字符串
 *
 * @param QP_Player 播放器对象
 * @return GString* 返回alsaink字符串
 */
GString *qp_player_alsa_sink_generate(QP_Player *player)
{
    GPtrArray *alsa_devices = player->opt_alsa_devices;

    // 当参数中没有设置ALSA设备的时候, 直接返回alsasink
    if (alsa_devices->len == 0)
    {
        GString *sink = g_string_new("alsasink");
        return sink;
    }

    // 当设置了至少一个ALSA设备的时候
    GString *sink = g_string_new(" ! tee name=t ");

    guint idx = 0;
    for (; idx < alsa_devices->len; idx += 1)
    {
        g_string_append_printf(sink, " t. ! queue ! alsasink device=%s", (gchar *)alsa_devices->pdata[idx]);
    }

    return sink;
}