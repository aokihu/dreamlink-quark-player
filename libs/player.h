/**
 * 
 * 播放器相关定义
 * @author aokihu
 * @copyright Reasonbox
 * @date Sat Apr  3 16:52:54 CST 2021
 * 
 */
#include "option.h"
#include <gst/gst.h>

#ifndef QP_H_PLAYER
#define QP_H_PLAYER

/**
 * 播放器结构数据定义
 * @uri     媒体资源URI
 * @volume  音量
 * @address UDP广播地址
 * @port    UDP广播端口
 * @output_type 输出模式
 */
typedef struct _QP_PLAYER_STRUCT
{
  gchar *uri;
  guint volume;
  gchar *address;
  gulong port;
  QP_OPTION_OUTPUT_TYPE output_type;

  // Gstreamer
  GstPipeline *gst_pipeline;
  GstBus *gst_bus;
} QP_PLAYER;

/**
 * 播放器初始化
 */
extern void qp_player_init();

/**
 * 提交设备设置数据,并且创建管道
 */
extern void qp_player_commit();

/**
 * 设置播放器的输出模式
 * @param output_type QP_OPTION_OUTPUT_TYPE 输出模式枚举
 */
extern void qp_player_set_output(QP_OPTION_OUTPUT_TYPE optput_type);

/**
 * 设置播放器音量
 * @param volume guint 设置的音量,范围从1到100,内部转化成浮点数格式
 */
extern void qp_plauer_set_volume(guint volume);

/**
 * 设置ALSA设备ID
 * @param id guint ALSA设备ID
 */
extern void qp_player_set_alsa_device_id(guint id);
/**
 * 设置ALSA子设备ID
 * @param id guint ALSA子设备ID
 */
extern void qp_player_set_alsa_subdevice_id(guint id);

/**
 * 设置UDP端口号
 * @param port gulong 端口号
 */
extern void qp_player_set_udp_port(gulong port);
extern void qp_player_set_udp_address(gchar *);
/**
 * 设置播放资源URI
 */
extern void qp_player_play_uri(gchar *);

extern void qp_player_play();
extern void qp_player_stop();
extern void qp_player_pause();
#endif