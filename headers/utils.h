/**
 * 自定义工具库
 * @brief 由于GLib库中的工具函数不够全面，所以自己写了一些工具函数
 */

/**
 * @brief 字符串比较函数
 * @param a { gchar* }a
 * @param b { gchar* }b
 * @return {gboolean} 如果相等返回 1，否则返回 0
 */
extern gboolean qp_utils_str_equal(const gchar *a, const gchar *b);
