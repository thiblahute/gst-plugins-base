/* GStreamer video crop
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

/**
 * SECTION:gstvideocrop
 * @short_description: GStreamer video cropping support
 *
 * Since: 0.10.?
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include "video-crop.h"

struct _GstVideoCrop
{
  GstMiniObject parent;

  gint top, left, width, height;
};

struct _GstVideoCropClass
{
  GstMiniObjectClass parent_class;
};

#define GST_VIDEO_CROP_QUARK gst_video_crop_quark_get_type()
static GST_BOILERPLATE_QUARK (GstVideoCrop, gst_video_crop_quark);

#define VIDEO_CROP_QUARK video_crop_quark_get_type()
static GST_BOILERPLATE_QUARK (VideoCrop, video_crop_quark);

/**
 * gst_buffer_set_video_crop:
 * @buf: a #GstBuffer
 * @crop: (allow-none): a #GstVideoCrop, or NULL to clear a
 *     previously-set crop
 *
 * Sets an video crop on a buffer. The buffer will obtain its own
 * reference to the crop, meaning this function does not take ownership
 * of @crop.
 *
 * Since: 0.10.?
 */
void
gst_buffer_set_video_crop (GstBuffer * buf, GstVideoCrop * crop)
{
  gst_buffer_set_qdata (buf, GST_VIDEO_CROP_QUARK,
      gst_structure_id_new (GST_VIDEO_CROP_QUARK,
          VIDEO_CROP_QUARK, GST_TYPE_VIDEO_CROP, crop, NULL));
}

/**
 * gst_buffer_get_video_crop:
 * @buf: a #GstBuffer
 *
 * Get the video crop that has previously been attached to a buffer
 * with gst_buffer_set_video_crop(), usually by another element
 * upstream.
 *
 * Returns: (transfer none): the #GstVideoCrop attached to
 *    this buffer, or NULL. Does not return a reference to the crop,
 *    caller must obtain her own ref via gst_video_crop_ref()
 *    if needed.
 *
 * Since: 0.10.?
 */
GstVideoCrop *
gst_buffer_get_video_crop (GstBuffer * buf)
{
  const GstStructure *s;
  const GValue *val;

  s = gst_buffer_get_qdata (buf, GST_VIDEO_CROP_QUARK);
  if (s == NULL)
    return NULL;

  val = gst_structure_id_get_value (s, VIDEO_CROP_QUARK);
  if (val == NULL)
    return NULL;

  return GST_VIDEO_CROP (gst_value_get_mini_object (val));
}

static void
gst_video_crop_finalize (GstMiniObject * mini_obj)
{
  /* not chaining up to GstMiniObject's finalize for now, we know it's empty */
}

static void
gst_video_crop_class_init (GstVideoCropClass * klass)
{
  GST_MINI_OBJECT_CLASS (klass)->finalize = gst_video_crop_finalize;
}

GST_BOILERPLATE_MINI_OBJECT (GstVideoCrop, gst_video_crop);

/**
 * gst_video_crop_new:
 * @top: top coordinate of cropping region
 * @left: left coordinate of cropping region
 * @width: width of cropping region
 * @height: height of cropping region
 *
 * Creates a new crop object.
 *
 * Returns: (transfer full): a new #GstVideoCrop. Unref with
 *     gst_video_crop_unref() when no longer needed.
 *
 * Since: 0.10.?
 */
GstVideoCrop *
gst_video_crop_new (guint top, guint left, guint width, guint height)
{
  GstVideoCrop *crop;

  crop = (GstVideoCrop *) gst_mini_object_new (GST_TYPE_VIDEO_CROP);

  crop->top = top;
  crop->left = left;
  crop->width = width;
  crop->height = height;

  return crop;
}

guint
gst_video_crop_top (GstVideoCrop * crop)
{
  return crop->top;
}

guint
gst_video_crop_left (GstVideoCrop * crop)
{
  return crop->left;
}

guint
gst_video_crop_width (GstVideoCrop * crop)
{
  return crop->width;
}

guint
gst_video_crop_height (GstVideoCrop * crop)
{
  return crop->height;
}
