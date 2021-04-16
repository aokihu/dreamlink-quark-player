#include "player.h"

/**
 * 生成管道
 * @private
 * @param player 播放器对象
 */
void qp_player_make_pipeline(QP_Player *player)
{
  //
  // 定义各个部件对象
  //
  GstElement *obj_src;
  GstElement *obj_sink;
  GstElement *obj_volume = gst_element_factory_make("volume", QP_PLAYER_ELEMENT_VOLUME);
  GstElement *obj_audioconvert = gst_element_factory_make("audioconvert", QP_PLAYER_ELEMENT_AUDIOCONVERT);
  GstElement *obj_resample = gst_element_factory_make("audioresample", QP_PLAYER_ELEMENT_RESAMPLE);
  GstElement *obj_pipeline = gst_pipeline_new(QP_PLAYER_ELEMENT_PIPELINE);

  /* 播放源是URI资源的情况 */

  if (player->opt_input == QP_SET_INPUT_TYPE_URI)
  {
    obj_src = gst_element_factory_make("uridecodebin", QP_PLAYER_ELEMENT_SRC);
    g_object_set(obj_src, "uri", player->opt_uri->str, NULL);
  }

  /* 播放源是UDP Src的情况 */

  if (player->opt_input == QP_SET_INPUT_TYPE_UDP)
  {
    obj_src = gst_element_factory_make("udpsrc", QP_PLAYER_ELEMENT_SRC);
  }

  /* 输出源是ALSA设备的情况 */

  if (player->opt_output == QP_SET_OUTPUT_TYPE_LOCAL)
  {
#if __APPLE__
    obj_sink = gst_element_factory_make("autoaudiosink", QP_PLAYER_ELEMENT_SINK);
#elif __linux__
    obj_sink = gst_element_factory_make("alsaink", QP_PLAYER_ELEMENT_SINK);
#endif
  }

  /* 输出源是网络广播 */

  if (player->opt_output == QP_SET_OUTPUT_TYPE_NET)
  {
    // obj_sink = gst_element_factory_make("udpsink", QP_PLAYER_ELEMENT_SINK);
    // g_object_set(obj_sink, "host", player->opt_address->str,
    //              "port", player->opt_port,
    //              NULL);
    obj_sink = qp_player_make_rtp_bin(player);
  }

  /* 将所有的元件组合起来 */

  gst_bin_add_many(GST_BIN(obj_pipeline), obj_src, obj_audioconvert, obj_volume, obj_resample, obj_sink, NULL);
  player->gst_pipeline = GST_PIPELINE(obj_pipeline);

  /* 将中间组件连接 */
  if (!gst_element_link_many(obj_audioconvert, obj_volume, obj_resample, obj_sink, NULL))
  {
    g_debug("Element can not be linked\n");
    exit(-1);
  }

  /* 设置消息总线监听 */
  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(obj_pipeline));
  gst_bus_add_watch(bus, qp_player_bus_handler, player);
  gst_object_unref(bus);

  /* 当输入源是URI资源的时候监听PAD完成事件 */
  g_signal_connect(obj_src, "pad-added", G_CALLBACK(qp_player_pad_added_handler), obj_audioconvert);
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