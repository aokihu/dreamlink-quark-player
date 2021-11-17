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
  GString* pipeline_string = g_string_new(NULL);

  //
  // 1. 构建输入源指令字符串
  //
  
  switch(player->opt_input) {
    /* 输入源是FD */
    case QP_SET_INPUT_TYPE_FD:
      // 使用标准输入fd=0
      g_string_append_printf(pipeline_string, 
      "fdsrc !"
      "queue max-size-bytes=102400 min-threshold-bytes=2048 !"
      "decodebin sink-caps=\"application/ogg\"");
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
  " ! audioconvert" \
  " ! volume name="QP_PLAYER_ELEMENT_VOLUME  \
  " ! equalizer-3bands name="QP_PLAYER_ELEMENT_EQ\
  " ! audioresample"
  );


  //
  // 3. 构建输出字符串
  //
  switch(player->opt_output) {
    /* 本地音频设备输出播放 */
    case QP_SET_OUTPUT_TYPE_LOCAL:
    default:
  #if __APPLE__
      g_string_append_printf(pipeline_string, " ! autoaudiosink name=sink");
  #elif
      // @TODO
      // 这里没有添加alsasink的参数
      // 之后开发调试的时候再添加
      g_string_append_printf(pipeline_string, " ! alsasink name=sink");
  #endif
    break;
    /* 网络广播输出播放 */
    case QP_SET_OUTPUT_TYPE_NET:
      g_string_append_printf(pipeline_string, 
      " ! opusenc" \
      " ! rtpopuspay" \
      " ! .send_rtp_sink_0 rtpbin" \
      " ! udpsink name=udpsink");
    break;
  }

  /* 解析命令字符串,生成管道对象 */

  g_print("Launch string: %s\n", pipeline_string->str);

  GstElement* obj_pipeline = gst_parse_launch(pipeline_string->str, NULL);
  player->gst_pipeline = GST_PIPELINE(obj_pipeline);

  /* 设置消息总线监听 */
  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(obj_pipeline));
  gst_bus_add_watch(bus, qp_player_bus_handler, player);
  
  /* 释放内存资源 */
  g_string_free(pipeline_string, TRUE);
  gst_object_unref(bus);
}