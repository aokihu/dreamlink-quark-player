/**
 * @file player_utils.c
 */
#include "../headers/player.h"

/**
 * @brief 生成本地输出设备的字符串
 * @param player { QP_Player* } 播放器对象
 * @return { GString* } 本地输出设备的字符串
 */
GString *qp_player_fusion_local_output_generate(QP_Player *player)
{
  // 遍历输出设备列表 player->opt_output_list
  // 找到dev://开头的字符串,提取出设备名[DEVICE_NAME]
  // 生成本地输出设备字符串,格式" t. ! queue ! alsasink name=[DEVICE_NAME]"

  // 本地输出设备字符串
  GString *local_output = g_string_new(NULL);

  // 遍历输出设备列表
  guint idx = 0;
  for (; idx < player->opt_output_list->len; idx += 1)
  {
    // 获取输出设备字符串
    gchar *output = (gchar *)player->opt_output_list->pdata[idx];

    // 判断是否是本地输出设备
    if (g_str_has_prefix(output, "dev://"))
    {
      // 提取设备名
      gchar *device_name = g_strrstr(output, "/") + 1;

      // 生成本地输出设备字符串
      g_string_append_printf(local_output, " t. ! queue ! alsasink name=%s", device_name);
    }
  }

  // 返回本地输出设备字符串
  return local_output;
}