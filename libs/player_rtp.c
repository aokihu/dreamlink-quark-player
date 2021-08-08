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
 *                                 
 * opusencode -> rtpopuspay -> rtpbin -> udp_sink
 * 
 */
GstElement *qp_player_make_rtp_bin(QP_Player *player)
{
  /* 创建一个GstBin将网络输出封装起来 */
  GstElement *bin = gst_bin_new(QP_PLAYER_ELEMENT_RTPSINKBIN);

  GstElement *opusencode = gst_element_factory_make("opusenc", QP_PLAYER_ELEMENT_OUPSENCODE);
  GstElement *rtpopuspay = gst_element_factory_make("rtpopuspay", QP_PLAYER_ELEMENT_RTPOPUSPAY);
  GstElement *rtpbin = gst_element_factory_make("rtpbin", "rtpbin");
  GstElement *udpsink = gst_element_factory_make("udpsink", QP_PLAYER_ELEMENT_UDPSINK);

  gst_bin_add_many(GST_BIN(bin), opusencode, rtpopuspay, rtpbin, udpsink, NULL);

  /* 给Bin添加一个输入的Pad */
  GstPad *pad = gst_element_get_static_pad(opusencode, "sink");
  gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));

  /* -------------------------------------------- */
  /*             以下部分是连接各个组件               */
  /* -------------------------------------------- */

  /* opusencode ->rtpopuspay */
  gst_element_link(opusencode, rtpopuspay);

  /* queue -> rtpbin */
  GstPad *rtpbinSinkPad = gst_element_get_request_pad(rtpbin, "send_rtp_sink_%u");
  gchar *rtpbinSinkPadName = gst_pad_get_name(rtpbinSinkPad);
  gst_element_link_pads(rtpopuspay, "src", rtpbin, rtpbinSinkPadName);

  /* rtpbin -> udpsink */
  gst_element_link_pads(rtpbin, "send_rtp_src_0", udpsink, "sink");

  /* 设置UDP广播参数 */
  g_signal_emit_by_name(udpsink, "add", player->opt_address->str, player->opt_port);
  g_signal_emit_by_name(udpsink, "add", player->opt_address6->str, player->opt_port);

  // gst_debug_bin_to_dot_file(bin, GST_DEBUG_GRAPH_SHOW_ALL, "kankan");
  return bin;
}