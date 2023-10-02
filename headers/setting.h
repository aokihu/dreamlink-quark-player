/**
 * 配置数据定义
 *
 * @author aokihu <aokihu@gmail.com>
 * @copyright Reasonbox
 * @version 1.1.0
 * @date Mon Apr  5 16:43:03 CST 2021
 *
 */

#pragma once
#ifndef QP_H_SETTING
#define QP_H_SETTING

#include <glib.h>

/**
 * 输入方式类型枚举
 * @enum QP_SET_INPUT_TYPE
 */
typedef enum _QP_SET_INPUT_TYPE
{
  QP_SET_INPUT_TYPE_URI, // 使用URI作为输入
  QP_SET_INPUT_TYPE_UDP, // 使用udp socket作为输入
  QP_SET_INPUT_TYPE_FD,  // 使用文件描述符作为输入
} QP_SET_INPUT_TYPE;

/**
 * 输出类型枚举
 * @enum QP_SET_OUTPUT_TYPE
 * @brief 3.0.0 增加Fusion输出方式，可以同时输出到NET和LOCAL
 */
typedef enum _QP_SET_OUTPUT_TYPE
{
  QP_SET_OUTPUT_TYPE_NET,    // 网络广播输出
  QP_SET_OUTPUT_TYPE_LOCAL,  // 本地声卡输出
  QP_SET_OUTPUT_TYPE_FUSION, // 混合输出，同时兼容网络与本地声卡输出
} QP_SET_OUTPUT_TYPE;

/**
 * 网络传输质量类型枚举
 * @enum QP_SET_QUALITY
 */
typedef enum _QP_SET_QUALITY
{
  QP_SET_QUALITY_LOW,
  QP_SET_QUALITY_NORMAL,
  QP_SET_QUALITY_HIGH,
} QP_SET_QUALITY;

/**
 * 启动参数结构体
 * @struct QP_CmdParam
 * @brief 自3.0.0开始，增加字段output_list，用于设置混合输出的参数
 */
typedef struct _QP_CMD_PARAM
{
  guint port;
  guint volume;
  guint card;
  guint card_sub;
  guint src_port;
  GString *uri;
  GString *address;
  GString *address6;
  GString *src_address;
  GPtrArray *alsa_devices;
  GPtrArray *output_list;
  QP_SET_INPUT_TYPE input;
  QP_SET_OUTPUT_TYPE output;
  QP_SET_QUALITY quality;
} QP_CmdParam;

#endif