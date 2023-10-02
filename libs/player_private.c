#include "../headers/player.h"

/**
 * 生成管道
 * @private
 * @since 2.1.0
 * @param player 播放器对象
 * @brief 使用字符串方式构建管道
 */
void qp_player_make_pipeline(QP_Player *player)
{
  //
  // 管道命令字符串
  //
  GString *pipeline_string = g_string_new(NULL);

  //
  // 1. 构建输入源指令字符串
  //

  switch (player->opt_input)
  {
  /* 输入源是FD */
  case QP_SET_INPUT_TYPE_FD:
    // 使用标准输入fd=0
    g_string_append_printf(pipeline_string,
                           "fdsrc !"
                           "queue max-size-bytes=102400 min-threshold-bytes=2048 !"
                           "decodebin");
    break;
  /* 输入源是UDP */
  case QP_SET_INPUT_TYPE_UDP:
    g_string_append_printf(pipeline_string, "udpsrc uri=\"%s\" ! queue ! decodebin", player->opt_uri->str);
    break;
  /* 输入源是URI */
  case QP_SET_INPUT_TYPE_URI:
  default:
    g_string_append_printf(pipeline_string, "uridecodebin uri=\"%s\"", player->opt_uri->str);
    break;
  }

  //
  // 2. 构建音效组件字符串
  //
  g_string_append_printf(
      pipeline_string,
      " ! audioconvert"
      " ! volume name=" QP_PLAYER_ELEMENT_VOLUME
      " ! equalizer-3bands name=" QP_PLAYER_ELEMENT_EQ
      " ! audioresample");

  //
  // 3. 构建输出字符串
  //
  // GString *alsa_sink = NULL;
  switch (player->opt_output)
  {
  /* 本地音频设备输出播放 */
  case QP_SET_OUTPUT_TYPE_LOCAL:
  default:
#ifdef __APPLE__
    g_string_append_printf(pipeline_string, " ! autoaudiosink name=sink");
#else
    alsa_sink = qp_player_alsa_sink_generate(player);
    g_string_append(pipeline_string, alsa_sink->str);
    g_string_free(alsa_sink, TRUE);
#endif
    break;
  /* 网络广播输出播放 */
  case QP_SET_OUTPUT_TYPE_NET:
    g_string_append_printf(pipeline_string,
                           " ! opusenc"
                           " ! rtpopuspay"
                           " ! .send_rtp_sink_0 rtpbin"
                           " ! udpsink name=udpsink auto-multicast=true ttl-mc=30"); /* 增加Multicast TTL参数 */
    break;

  /* 融合输出播放 */
  //
  // @since 3.0 增加融合输出模式
  //            该模式下，播放器将同时输出到本地音频设备和网络广播
  //            构建的字符串将使用tee组件实现 "分流" 功能
  //            从player->output_list中获取输出列表
  //            列表中每一项都是一个字符串,
  //            本地设备字符串格式: dev://[DEVICE_NAME]
  //            网络RTP字符串格式: udp://[IP]:[PORT]
  //
  case QP_SET_OUTPUT_TYPE_FUSION:

    // 添加tee组件
    g_string_append_printf(pipeline_string, " ! tee name=t");

    break;
  }

  /* 解析命令字符串,生成管道对象 */

#ifdef DEBUG
  g_print("Launch string: %s\n", pipeline_string->str);
#endif

  GError *error = NULL;
  GstElement *obj_pipeline = gst_parse_launch(pipeline_string->str, &error);

  if (error != NULL)
  {
    g_printerr("Parse launch error: %s\n", error->message);
    g_error_free(error);
    exit(1);
  }

  player->gst_pipeline = GST_PIPELINE(obj_pipeline);

  /* 设置消息总线监听 */
  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(obj_pipeline));
  gst_bus_add_watch(bus, qp_player_bus_handler, player);

  /* 释放内存资源 */
  g_string_free(pipeline_string, TRUE);
  gst_object_unref(bus);
}