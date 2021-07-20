/**
 * 生成RTP输出组件
 * @author aokihu
 * @copyright Reasonbox
 */

#include "../headers/player.h"

void qp_player_rtpbin_pad_added_handler(GstElement *src, GstPad *pad, gpointer *data)
{
  gchar *name = gst_pad_get_name(pad);
  g_message("Pad name: %s", name);
  g_free(name);
}

/**
 * 生成RTP输出组件
 * 
 *                                  -> queue -> rtpbin -> udp_sink
 * opusencode -> rtpopuspay -> tee |
 *                                  -> queue -> rtpbin -> udp_sink
 * 
 */
GstElement *qp_player_make_rtp_bin(QP_Player *player)
{
  /* 创建一个GstBin将网络输出封装起来 */
  GstElement *bin = gst_bin_new(QP_PLAYER_ELEMENT_RTPSINKBIN);

  GstElement *tee = gst_element_factory_make("tee", QP_PLAYER_ELEMENT_TEE);
  GstElement *queue = gst_element_factory_make("queue", QP_PLYAER_ELEMENT_QUEUE);
  GstElement *queue6 = gst_element_factory_make("queue", QP_PLAYER_ELEMENT_QUEUE6);
  GstElement *udpsink = gst_element_factory_make("udpsink", QP_PLAYER_ELEMENT_UDPSINK);
  GstElement *udpsink6 = gst_element_factory_make("udpsink", QP_PLAYER_ELEMENT_UDPSINK6);
  GstElement *opusencode = gst_element_factory_make("opusenc", QP_PLAYER_ELEMENT_OUPSENCODE);
  GstElement *rtpopuspay = gst_element_factory_make("rtpopuspay", QP_PLAYER_ELEMENT_RTPOPUSPAY);
  GstElement *rtpbin = gst_element_factory_make("rtpbin", "rtpbin");
  GstElement *rtpbin6 = gst_element_factory_make("rtpbin", "rtpbin6");

  gst_bin_add_many(GST_BIN(bin), tee, queue, queue6, opusencode, rtpopuspay, rtpbin, rtpbin6, udpsink, udpsink6, NULL);

  /* 给Bin添加一个输入的Pad */
  GstPad *pad = gst_element_get_static_pad(opusencode, "sink");
  gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));

  /* -------------------------------------------- */
  /*             以下部分是连接各个组件               */
  /* -------------------------------------------- */

  /* opusencode ->rtpopuspay */
  gst_element_link(opusencode, rtpopuspay);

  /* rtpopuspay -> tee */
  gst_element_link_pads(rtpopuspay, "src", tee, "sink");

  /* tee -> queue */
  GstPad *teeRequestPad = gst_element_get_request_pad(tee, "src_%u");
  gchar *padName = gst_pad_get_name(teeRequestPad);
  gst_element_link_pads(tee, padName, queue, "sink");

  /* tee -> queue6 */
  GstPad *teeRequestPad6 = gst_element_get_request_pad(tee, "src_%u");
  gchar *padName6 = gst_pad_get_name(teeRequestPad6);
  gst_element_link_pads(tee, padName6, queue6, "sink");

  /* queue -> rtpbin */
  GstPad *rtpbinSinkPad = gst_element_get_request_pad(rtpbin, "send_rtp_sink_0");
  gchar *rtpbinSinkPadName = gst_pad_get_name(rtpbinSinkPad);
  gst_element_link_pads(queue, "src", rtpbin, rtpbinSinkPadName);

  /* queue6 -> rtpbin6 */
  GstPad *rtpbin6SinkPad = gst_element_get_request_pad(rtpbin6, "send_rtp_sink_0");
  gchar *rtpbin6SinkPadName = gst_pad_get_name(rtpbin6SinkPad);
  gst_element_link_pads(queue6, "src", rtpbin6, rtpbin6SinkPadName);

  /* rtpbin -> udpsink */
  gst_element_link_pads(rtpbin, "send_rtp_src_0", udpsink, "sink");

  /* rtpbin6 -> udpsink6 */
  gst_element_link_pads(rtpbin6, "send_rtp_src_0", udpsink6, "sink");

  /* 设置UDP广播参数 */
  g_object_set(udpsink,
               "host", player->opt_address->str,
               "port", player->opt_port,
               NULL);

  g_object_set(udpsink6,
               "host", player->opt_address6->str,
               "port", player->opt_port,
               NULL);

  /* 释放资源 */
  gst_object_unref(teeRequestPad);
  gst_object_unref(teeRequestPad6);
  gst_object_unref(rtpbinSinkPad);
  gst_object_unref(rtpbin6SinkPad);

  gst_debug_bin_to_dot_file(bin, GST_DEBUG_GRAPH_SHOW_ALL, "kankan");
  return bin;
}