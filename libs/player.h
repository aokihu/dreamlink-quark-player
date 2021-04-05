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
 * @opt_uri     媒体资源URI
 * @opt_volume  音量
 * @address UDP广播地址
 * @port    UDP广播端口
 * @output_type 输出模式
 * @status_ready 播放器准备状态
 * @status_playing 正在播放状态
 * @gst_pipeline Gstreamter管道对象
 * @gst_bus Gstreamer总线对象
 */
typedef struct _QP_PLAYER_STRUCT
{
  // 参数
  gchar *opt_uri;
  guint opt_volume;
  gchar *opt_address;
  gulong opt_port;
  QP_OPTION_OUTPUT_TYPE opt_output_type;

  // 状态
  gboolean status_ready;
  gboolean status_playing;

  // Gstreamer
  GstPipeline *gst_pipeline;
  GstBus *gst_bus;
  GstElement *gst_src;
  GstElement *gst_sink;
} QP_PLAYER;

//

/**
 * 播放器初始化
 * @public
 * @param player 播放器数据对象
 */
extern void qp_player_init(QP_PLAYER *player);

/**
 * 提交设备设置数据,并且创建管道
 * @public
 * @param player 播放器数据对象
 */
extern void qp_player_commit(QP_PLAYER *player);

/**
 * 设置播放器的输出模式
 * @public
 * @param player 播放器数据对象
 * @param output_type QP_OPTION_OUTPUT_TYPE 输出模式枚举
 */
extern void qp_player_set_output(QP_PLAYER *player, QP_OPTION_OUTPUT_TYPE optput_type);

/**
 * 设置播放器音量
 * @public
 * @param player 播放器数据对象
 * @param volume guint 设置的音量,范围从1到100,内部转化成浮点数格式
 */
extern void qp_plauer_set_volume(QP_PLAYER *player, guint volume);

/**
 * 设置ALSA设备ID
 * @public
 * @param player 播放器数据对象
 * @param id guint ALSA设备ID
 */
extern void qp_player_set_alsa_device_id(QP_PLAYER *player, guint id);

/**
 * 设置ALSA子设备ID
 * @public
 * @param player 播放器数据对象
 * @param id guint ALSA子设备ID
 */
extern void qp_player_set_alsa_subdevice_id(QP_PLAYER *player, guint id);

/**
 * 设置UDP端口号
 * @public
 * @param player 播放器数据对象
 * @param port gulong 端口号
 */
extern void qp_player_set_udp_port(QP_PLAYER *player, gulong port);

/**
 * 设置UDP广播地址
 * @public
 * @param player 播放器数据对象
 * @param address gchar* 广播地址 
 */
extern void qp_player_set_udp_address(QP_PLAYER *player, gchar *address);

/**
 * 设置播放资源URI
 * @param player 播放器数据对象
 * @public
 */
extern void qp_player_play_uri(QP_PLAYER *player, gchar *);

/**
 * 播放器开始播放
 * @public
 * @param player 播放器数据对象
 */
extern void qp_player_play(QP_PLAYER *player);

/**
 * 播放器停止播放
 * @public
 * @param player 播放器数据对象
 */
extern void qp_player_stop(QP_PLAYER *player);

/**
 * 播放器暂停播放
 * @public
 * @param player 播放器数据对象
 */
extern void qp_player_pause(QP_PLAYER *player);

/**
 * 恢复播放器播放
 * @public
 * @param player 播放器数据对象
 */
extern void qp_player_resume(QP_PLAYER *player);



#endif