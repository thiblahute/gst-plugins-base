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

#ifndef __GST_H264_ENCODE_BIN_H__
#define __GST_H264_ENCODE_BIN_H__

#include <glib-object.h>
#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_H264_ENCODE_BIN            (gst_h264_encode_bin_get_type ())
#define GST_H264_ENCODE_BIN(o)              (G_TYPE_CHECK_INSTANCE_CAST  ((o), GST_TYPE_H264_ENCODE_BIN, GstH264EncodeBin))
#define GST_IS_H264_ENCODE_BIN(o)           (G_TYPE_CHECK_INSTANCE_TYPE  ((o), GST_TYPE_H264_ENCODE_BIN))
#define GST_H264_ENCODE_BIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_H264_ENCODE_BIN, GstH264EncodeBinClass))
#define GST_IS_H264_ENCODE_BIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_H264_ENCODE_BIN))
#define GST_H264_ENCODE_BIN_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), GST_TYPE_H264_ENCODE_BIN, GstH264EncodeBinClass))

typedef struct _GstH264EncodeBin        GstH264EncodeBin;
typedef struct _GstH264EncodeBinClass   GstH264EncodeBinClass;

GType             gst_h264_encode_bin_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __GST_H264_ENCODE_BIN_H__ */
