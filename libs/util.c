#include <glib.h>
#include "../headers/utils.h"

/**
 * @brief 字符串比较函数
 * @param a { gchar* }a
 * @param b { gchar* }b
 * @return {gboolean} 如果相等返回 1，否则返回 0
 */

extern gboolean qp_utils_str_equal(const gchar *a, const gchar *b)
{
  return !strcmp((gchar *)a, (gchar *)b) ? 1 : 0;
}
