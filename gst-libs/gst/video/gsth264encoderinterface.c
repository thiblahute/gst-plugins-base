 /*
  *  Copyright (C) 2018 Igalia S.L
  *  Copyright (C) 2018 Thibault Saunier <tsaunier@igalia.com>
  *
  *  This library is free software; you can redistribute it and/or
  *  modify it under the terms of the GNU Lesser General Public
  *  License as published by the Free Software Foundation; either
  *  version 2 of the License, or (at your option) any later version.
  *
  *  This library is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  *  Lesser General Public License for more details.
  *
  *  You should have received a copy of the GNU Lesser General Public
  *  License along with this library; if not, write to the Free Software
  *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  */

#include "gsth264encoderinterface.h"
#include "gstvideoencoder.h"

G_DEFINE_INTERFACE (GstH264EncoderInterface, gst_h264_encoder_interface,
    GST_TYPE_VIDEO_ENCODER)

     static void
         gst_h264_encoder_interface_default_init
         (GstH264EncoderInterfaceInterface * iface)
{
}

void
gst_h264_encoder_interface_set_bitrate (GstH264EncoderInterface * encoder,
    guint bitrate)
{
  GstH264EncoderInterfaceInterface *encoder_iface =
      GST_H264_ENCODER_INTERFACE_GET_INTERFACE (encoder);

  g_return_if_fail (GST_H264_ENCODER_INTERFACE_GET_INTERFACE
      (encoder)->set_bitrate);

  encoder_iface->set_bitrate (encoder, bitrate);
}

guint
gst_h264_encoder_interface_get_bitrate (GstH264EncoderInterface * encoder)
{
  GstH264EncoderInterfaceInterface *encoder_iface =
      GST_H264_ENCODER_INTERFACE_GET_INTERFACE (encoder);
  g_return_val_if_fail (encoder_iface->get_bitrate, 0);

  return encoder_iface->get_bitrate (encoder);
}
