#include "../headers/player.h"

/**
 * 生成管道
 * @private
 * @since 2.0.0
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
      g_string_append_printf(pipeline_string, "fdsrc fd=3 timeout=1000");
    break;
    /* 输入源是UDP */
    case QP_SET_INPUT_TYPE_UDP:
      g_string_append_printf(pipeline_string, "udpsrc");
    break;
    /* 输入源是URI */
    case QP_SET_INPUT_TYPE_URI:
    default:
      g_string_append_printf(pipeline_string, "uridecodebin uri=%s", player->opt_uri->str);
    break;
  }

  //
  // 2. 构建音效组件字符串
  //



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

/**
 * Pad添加后的回掉方法
 * @private
 * @param src 触发组件的源对象
 * @param pad 增加的pad
 * @param data 附带数据，这里是QP_Player
 */
void qp_player_pad_added_handler(GstElement *src, GstPad *new_pad, gpointer data)
{
  GstElement *convert = (GstElement *)data;
  GstPad *sink_pad = gst_element_get_static_pad(convert, "sink");
  GstPadLinkReturn ret;
  GstCaps *new_pad_caps = NULL;
  GstStructure *new_pad_struct = NULL;
  const gchar *new_pad_type = NULL;

  /* 检查Convert组件的Pad是否已经连接 */
  if (gst_pad_is_linked(sink_pad))
  {
    g_debug("We are already linked\n");
    goto exit;
  }

  /* 检查新的Pad的类型 */
  new_pad_caps = gst_pad_get_current_caps(new_pad);
  new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
  new_pad_type = gst_structure_get_name(new_pad_struct);
  if (!g_str_has_prefix(new_pad_type, "audio/x-raw"))
  {
    g_debug("Not raw audio. Ignoring.\n");
    goto exit;
  }

  /* 尝试连接Pad */
  ret = gst_pad_link(new_pad, sink_pad);
  if (GST_PAD_LINK_FAILED(ret))
  {
    g_debug("Type is '%s' but link failed.\n", new_pad_type);
  }
  else
  {
    g_debug("Link successed(type '%s').\n", new_pad_type);
  }

exit:
  if (new_pad_caps != NULL)
    gst_caps_unref(new_pad_caps);

  gst_object_unref(sink_pad);
}