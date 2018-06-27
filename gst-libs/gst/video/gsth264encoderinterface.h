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
#ifndef __GST_H264_ENCODER_INTERFACE_H__
#define __GST_H264_ENCODER_INTERFACE_H__

#include <gst/video/gstvideoutils.h>

G_BEGIN_DECLS

#define GST_TYPE_H264_ENCODER_INTERFACE               (gst_h264_encoder_interface_get_type())
#define GST_H264_ENCODER_INTERFACE(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_H264_ENCODER_INTERFACE, GstH264EncoderInterface))
#define GST_IS_H264_ENCODER_INTERFACE(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_H264_ENCODER_INTERFACE))
#define GST_H264_ENCODER_INTERFACE_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GST_TYPE_H264_ENCODER_INTERFACE, GstH264EncoderInterfaceInterface))

/**
 * GstH264EncoderInterface:
 *
 * Opaque #GstH264EncoderInterface data structure.
 */
typedef struct _GstH264EncoderInterface GstH264EncoderInterface;
typedef struct _GstH264EncoderInterfaceInterface GstH264EncoderInterfaceInterface;

/**
 * GstH264EncoderInterfaceInterface:
 * @parent: parent interface type.
 * @set_bitrate: Set bitrate (in bits per second)
 * @get_bitrate: Get bitrate (in bits per second)
 *
 * #GstH264EncoderInterface interface.
 */
struct _GstH264EncoderInterfaceInterface
{
  GTypeInterface parent;

  void  (*set_bitrate) (GstH264EncoderInterface *iface, guint bitrate);
  guint (*get_bitrate) (GstH264EncoderInterface *iface);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GST_VIDEO_API
GType        gst_h264_encoder_interface_get_type (void);

GST_VIDEO_API
void  gst_h264_encoder_interface_set_bitrate (GstH264EncoderInterface *encoder_iface, guint bitrate);

GST_VIDEO_API
guint gst_h264_encoder_interface_get_bitrate (GstH264EncoderInterface *encoder_iface);

G_END_DECLS

#endif /* __GST_H264_ENCODER_INTERFACE_H__ */
