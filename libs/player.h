/**
 * 播放器对象定义 
 * 
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.0
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
#define QP_PLAYER_ELEMENT_AUDIOCONVERT "audioconvert"
#define QP_PLAYER_ELEMENT_AUDIORATE "audiorate"
#define QP_PLAYER_ELEMENT_VOLUME "volume"
#define QP_PLAYER_ELEMENT_RESAMPLE "resample"

/**
 * 
 * 播放器结构体定义
 * @struct QP_Player
 * @property status_ready 播放器准备状态,初始化完成后状态为TRUE
 * @property status_playing 播放器正在播放状态
 * @property opt_port UDP广播端口
 * @property opt_address UDP广播地址
 */
typedef struct _QP_Player
{
  // 状态定义
  gboolean status_ready;
  gboolean status_playing;

  // 参数定义
  GString *opt_uri;
  guint opt_port;
  GString *opt_address;
  guint opt_card;
  guint opt_card_sub;
  guint opt_volume;
  QP_SET_QUALITY opt_quality;
  QP_SET_INPUT_TYPE opt_input;
  QP_SET_OUTPUT_TYPE opt_output;

  // GStreamer对象
  GstPipeline *gst_pipeline;
  GstElement *gst_source_bin;
  GstElement *gst_sink_bin;
} QP_Player;

//
// 私有方法
//
void qp_player_make_pipeline(QP_Player *player);
void qp_player_pad_added_handler(GstElement *src, GstPad *pad, gpointer data);
void qp_player_bus_handler(GstBus *bus, GstMessage *message, gpointer userdata);

//
// 公开方法
//
extern QP_Player *qp_player_new();
extern void qp_player_init(QP_Player *, QP_CmdParam *);
extern void qp_player_play(QP_Player *);
extern void qp_player_stop(QP_Player *);
extern void qp_player_pause(QP_Player *);
extern void qp_player_resume(QP_Player *);
extern void qp_player_set_volume(QP_Player *player, gint64 volume);
extern void qp_player_set_port(QP_Player *player, guint port);
extern void qp_player_set_address(QP_Player *player, gchar *address);

#endif
