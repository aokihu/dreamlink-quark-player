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

  gst_bin_add_many(GST_BIN(bin), tee, queue, queue6, opusencode, rtpopuspay, rtpbin, udpsink, udpsink6, NULL);

  /* 将固定pad的组件连接起来 */
  gst_element_link(queue, udpsink);
  gst_element_link(queue6, udpsink6);
  gst_element_link(opusencode, rtpopuspay);

  /* 给Bin添加一个输入的Pad */
  GstPad *pad = gst_element_get_static_pad(opusencode, "sink");
  gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));

  /* 获取rtpbin中接收数据的pad */
  GstPad *sendSinkPad = gst_element_get_request_pad(rtpbin, "send_rtp_sink_0");
  gchar *sendSinkPadName = gst_pad_get_name(sendSinkPad);

  /* 将rtpbin接收数据的pad与rtpopuspay的src连接 */
  gst_element_link_pads(rtpopuspay, "src", rtpbin, sendSinkPadName);

  /* 将rtpbin发送数据的pad与tee的sink连接 */
  /* 当请求send_rtp_sink_%u的时候会自动创建send_rtp_src_%u */
  gst_element_link_pads(rtpbin, "send_rtp_src_0", tee, "sink");
  gst_element_link_pads(tee, "src_%u", queue, "sink");
  gst_element_link_pads(tee, "src_%u", queue6, "sink");

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
  gst_object_unref(GST_OBJECT(pad));

  return bin;
}