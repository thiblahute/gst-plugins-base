/* GStreamer video-crop
 *
 * Copyright (c) 2012, Texas Instruments Incorporated
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_VIDEO_CROP_H__
#define __GST_VIDEO_CROP_H__

#include <gst/gst.h>

G_BEGIN_DECLS

/**
 * GstVideoCrop:
 *
 * Crop coordinates for a video buffer.
 *
 * Since: 0.10.?
 */
#define GST_TYPE_VIDEO_CROP			\
  (gst_video_crop_get_type ())
#define GST_VIDEO_CROP(obj)			\
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEO_CROP, GstVideoCrop))
#define GST_IS_VIDEO_CROP(obj)			\
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEO_CROP))

typedef struct _GstVideoCrop      GstVideoCrop;
typedef struct _GstVideoCropClass GstVideoCropClass;

/**
 * gst_video_crop_ref:
 * @crop: a a #GstVideoCrop.
 *
 * Increases the refcount by one.
 *
 * Returns: (transfer full): @crop
 *
 * Since: 0.10.?
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC GstVideoCrop *
gst_video_crop_ref (GstVideoCrop * crop);
#endif

static inline GstVideoCrop *
gst_video_crop_ref (GstVideoCrop * crop)
{
  return (GstVideoCrop *) gst_mini_object_ref (GST_MINI_OBJECT_CAST (crop));
}

/**
 * gst_video_crop_unref:
 * @crop: (transfer full): a #GstVideoCrop.
 *
 * Decreases the refcount. If the refcount reaches 0, the crop will be
 * freed.
 *
 * Since: 0.10.?
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void
gst_video_crop_unref (GstVideoCrop * crop);
#endif

static inline void
gst_video_crop_unref (GstVideoCrop * crop)
{
  gst_mini_object_unref (GST_MINI_OBJECT_CAST (crop));
}

GType                        gst_video_crop_get_type (void);

GstVideoCrop *               gst_video_crop_new (guint top, guint left, guint width, guint height);

guint                        gst_video_crop_top (GstVideoCrop * crop);
guint                        gst_video_crop_left (GstVideoCrop * crop);
guint                        gst_video_crop_width (GstVideoCrop * crop);
guint                        gst_video_crop_height (GstVideoCrop * crop);

/* attach/retrieve crop from buffers */

void                         gst_buffer_set_video_crop (GstBuffer * buf,
                                                        GstVideoCrop * crop);

GstVideoCrop *               gst_buffer_get_video_crop (GstBuffer * buf);

G_END_DECLS

#endif /* __GST_VIDEO_CROP_H__ */
