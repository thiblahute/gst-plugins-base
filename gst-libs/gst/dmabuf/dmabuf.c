/* GStreamer dmabuf
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
 * SECTION:gstdmabuf
 * @short_description: GStreamer dmabuf support
 *
 * Since: 0.10.?
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include "dmabuf.h"

struct _GstDmaBuf
{
  GstMiniObject parent;

  int fd;
};

struct _GstDmaBufClass
{
  GstMiniObjectClass parent_class;
};

#define GST_DMA_BUF_QUARK gst_dma_buf_quark_get_type()
static GST_BOILERPLATE_QUARK (GstDmaBuf, gst_dma_buf_quark);

#define DMA_BUF_QUARK dma_buf_quark_get_type()
static GST_BOILERPLATE_QUARK (DmaBuf, dma_buf_quark);

/**
 * gst_buffer_set_dma_buf:
 * @buf: a #GstBuffer
 * @dmabuf: (allow-none): a #GstDmaBuf, or NULL to clear a
 *     previously-set dmabuf
 *
 * Sets an dmabuf handle on a buffer.  The buffer will obtain its own
 * reference to the dmabuf handle, meaning this function does not take
 * ownership of @dmabuf.
 *
 * Since: 0.10.?
 */
void
gst_buffer_set_dma_buf (GstBuffer * buf, GstDmaBuf * dmabuf)
{
  gst_buffer_set_qdata (buf, GST_DMA_BUF_QUARK,
      gst_structure_id_new (GST_DMA_BUF_QUARK,
          DMA_BUF_QUARK, GST_TYPE_DMA_BUF, dmabuf, NULL));
}

/**
 * gst_buffer_get_dma_buf:
 * @buf: a #GstBuffer
 *
 * Get the dmabuf handle that has previously been attached to a buffer
 * with gst_buffer_set_dma_buf(), usually by another element
 * upstream.
 *
 * Returns: (transfer none): the #GstDmaBuf attached to
 *    this buffer, or NULL. Does not return a reference to the dmabuf handle,
 *    caller must obtain her own ref via gst_dma_buf_ref()
 *    if needed.
 *
 * Since: 0.10.?
 */
GstDmaBuf *
gst_buffer_get_dma_buf (GstBuffer * buf)
{
  const GstStructure *s;
  const GValue *val;

  s = gst_buffer_get_qdata (buf, GST_DMA_BUF_QUARK);
  if (s == NULL)
    return NULL;

  val = gst_structure_id_get_value (s, DMA_BUF_QUARK);
  if (val == NULL)
    return NULL;

  return GST_DMA_BUF (gst_value_get_mini_object (val));
}

static void
gst_dma_buf_finalize (GstMiniObject * mini_obj)
{
  GstDmaBuf *dmabuf = (GstDmaBuf *) mini_obj;
  close (dmabuf->fd);

  /* not chaining up to GstMiniObject's finalize for now, we know it's empty */
}

static void
gst_dma_buf_class_init (GstDmaBufClass * klass)
{
  GST_MINI_OBJECT_CLASS (klass)->finalize = gst_dma_buf_finalize;
}

GST_BOILERPLATE_MINI_OBJECT (GstDmaBuf, gst_dma_buf);

/**
 * gst_dma_buf_new:
 * @fd: (transfer full) the dmabuf fd
 *
 * Creates a new dmabuf handle object, which takes ownership of the
 * specified dmabuf file descriptor.  When the dmabuf handle object
 * is finalized, the dmabuf fd is closed.
 *
 * Returns: (transfer full): a new #GstDmaBuf. Unref with
 *     gst_dma_buf_unref() when no longer needed.
 *
 * Since: 0.10.?
 */
GstDmaBuf *
gst_dma_buf_new (int fd)
{
  GstDmaBuf *dmabuf;

  dmabuf = (GstDmaBuf *) gst_mini_object_new (GST_TYPE_DMA_BUF);

  dmabuf->fd = fd;

  return dmabuf;
}

/**
 * gst_dma_buf_get_fd:
 *
 * Returns: a the dmabuf file descriptor object; if the caller wants
 * to hold on to the handle after the reference on the corresponding
 * gst buffer is lost, she should probably dup() the fd.  (Although
 * it is probably better to hang on to the ref to the GstBuffer and
 * let that hold the ref to the dmabuf fd).
 *
 * Since: 0.10.?
 */
int
gst_dma_buf_get_fd (GstDmaBuf * dmabuf)
{
  return dmabuf->fd;
}
