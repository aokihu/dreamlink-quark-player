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
  guint card;
  guint card_sub;
  QP_SET_QUALITY opt_quality;
  QP_SET_OUTPUT_TYPE opt_output;

  // GStreamer对象
  GstPipeline *gst_pipeline;
  GstBus *gst_bus;
  GstElement *gst_source;
  GstElement *gst_sink;
} QP_Player;

//
// 公开方法
//
extern QP_Player *qp_player_new();
extern void qp_player_init(QP_Player *, QP_CmdParam *);
extern void qp_player_play(QP_Player *);
extern void qp_player_stop();
extern void qp_player_pause();
extern void qp_player_resume();
extern void qp_player_set_port(gint port);
extern void qp_player_set_address(gchar *address);

#endif
