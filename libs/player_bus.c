/**
 * Player消息总线处理
 * @author aokihu
 * @copyright Reasonbox
 */

#include "player.h"
#include "stdmessage.h"

void qp_player_bus_handler(GstBus *bus, GstMessage *message, gpointer userdata)
{
  switch (GST_MESSAGE_TYPE(message))
  {
  case GST_MESSAGE_EOS:
  {
    qp_std_output(QP_STD_MESSAGE_PREFIX_STATUS, "eos");
  }
  }
}