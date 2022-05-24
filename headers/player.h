/**
 * 播放器对象定义
 *
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.1
 * @date Mon Apr  5 16:43:03 CST 2021
 *
 */

#ifndef QP_H_PLAYER
#define QP_H_PLAYER

#include "setting.h"
#include <glib.h>
#include <gst/gst.h>

#define QP_PLAYER_ELEMENT_PIPELINE "pipeline"
#define QP_PLAYER_ELEMENT_SRC "src"
#define QP_PLAYER_ELEMENT_SINK "sink"
#define QP_PLAYER_ELEMENT_VOLUME "volume"
#define QP_PLAYER_ELEMENT_EQ "eq"
#define QP_PLAYER_ELEMENT_RESAMPLE "resample"
#define QP_PLAYER_ELEMENT_UDPSINK "udpsink"
#define QP_PLAYER_ELEMENT_OUPSENCODE "opusencode"
#define QP_PLAYER_ELEMENT_RTPOPUSPAY "rtpopuspay"
#define QP_PLAYER_ELEMENT_RTPSINKBIN "rtpsinkbin"

/* 宏方法定义 */

#define QP_PLAYER_IS_OK(OBJ) (OBJ->status != QP_PLAYER_STATUS_NOT_READY)

/**
 * Player播放器状态定义 (从player_status.h文件中合并而来)
 */

typedef enum _QP_PLAYER_STATUS
{
  QP_PLAYER_STATUS_NOT_READY,      // 播放器未准备好
  QP_PLAYER_STATUS_READY,          // 播放器准备好
  QP_PLAYER_STATUS_PLAYING,        // 播放器正在播放
  QP_PLAYER_STATUS_EOS,            // 播放器播放结束
  QP_PLAYER_STATUS_PAUSE,          // 播放器暂停
  QP_PLAYER_STATUS_BUFFER_LOADING, // 播放器缓冲中
} QP_PLAYER_STATUS;

/**
 * 播放器结构体定义
 * @struct QP_Player
 * @property status_ready 播放器准备状态,初始化完成后状态为TRUE
 * @property status_playing 播放器正在播放状态
 * @property opt_port UDP广播端口
 * @property opt_address UDP广播地址
 */
typedef struct _QP_Player
{
  /* 状态定义 */
  gboolean status_ready;
  QP_PLAYER_STATUS status;
  guint timer_flag; // 定时器句柄

  /* 参数定义 */
  GString *opt_uri;              // 播放资源uri地址
  guint opt_port;                // 广播端口
  GString *opt_address;          // IPv4输出地址
  GString *opt_address6;         // IPv6输出地址
  GPtrArray *opt_alsa_devices;   // ALSA设备名称数组
  guint opt_card;                // 音频输出设备
  guint opt_card_sub;            // 音频输出子设备
  guint opt_volume;              // 音量
  QP_SET_QUALITY opt_quality;    // 播放音质质量
  QP_SET_INPUT_TYPE opt_input;   // 输入设备类型
  QP_SET_OUTPUT_TYPE opt_output; // 输出设备类型

  // GStreamer对象
  GstPipeline *gst_pipeline;
  GstElement *gst_source_bin;
  GstElement *gst_sink_bin;
} QP_Player;

//
// 私有方法
//
void qp_player_make_pipeline(QP_Player *player);

gboolean qp_player_bus_handler(GstBus *bus, GstMessage *message, gpointer userdata);
void qp_player_bus_error_handler(GstMessage *message, gpointer userdata);
gboolean qp_player_position_handler(gpointer userdata);
void qp_player_status_changed_handler(GstMessage *message, gpointer userdata);
GString *qp_player_alsa_sink_generate(QP_Player *player); // 生成ALSA Sink字符串

//
// 公开方法
//
extern QP_Player *qp_player_new();
extern void qp_player_init(QP_Player *, QP_CmdParam *);                      // 初始化播放器
extern void qp_player_play(QP_Player *);                                     // 播放
extern void qp_player_stop(QP_Player *);                                     // 停止
extern void qp_player_pause(QP_Player *);                                    // 暂停
extern void qp_player_resume(QP_Player *);                                   // 恢复
extern void qp_player_set_volume(QP_Player *player, gint64 volume);          // 设置音量
extern void qp_player_set_port(QP_Player *player, guint port);               // 设置端口
extern void qp_player_set_address(QP_Player *player, const gchar *address);  // 设置IPv4广播地址
extern void qp_player_set_address6(QP_Player *player, const gchar *address); // 设置IPv6广播地址

#endif
