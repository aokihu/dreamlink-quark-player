#include "version.h"

/**
 * qp_get_software_version()
 * 获取软件的版本信息
 *
 * @param void
 * @return Gstring* 版本号字符串
 *
 * @since 0.1
 */
GString *qp_get_software_version()
{
  GString_autoptr version = g_string_new(NULL);
  g_string_printf(version, "%d.%d.%d",
                  QP_VERSION_MAJOR,
                  QP_VERSION_MINOR,
                  QP_VERSION_PATCH);
  return version;
}