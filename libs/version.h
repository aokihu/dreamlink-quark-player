/**
 * 在此定义版本信息
 *
 * #0.1 添加获取软件版本号方法 qp_get_software_version(void)
 */
#include <glib.h>
#ifndef QP_VERSION_MAJOR

#define QP_VERSION_MAJOR 1 // 主版本号
#define QP_VERSION_MINOR 0 // 次版本号
#define QP_VERSION_PATCH 1 // 补丁版本号

extern GString *qp_get_software_version();

#endif