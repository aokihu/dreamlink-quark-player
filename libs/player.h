/**
 * 
 * 播放器相关定义
 * @author aokihu
 * @copyright Reasonbox
 * @date Sat Apr  3 16:52:54 CST 2021
 * 
 */

#ifndef QP_H_PLAYER
#define QP_H_PLAYER

/**
 * 播放器初始化
 */
extern void qp_player_init();

/**
 * 设置播放器的输出模式
 */
extern void qp_player_set_output();

extern void qp_plauer_set_volume(guint);
extern void qp_player_play_uri(gchar *);

#endif