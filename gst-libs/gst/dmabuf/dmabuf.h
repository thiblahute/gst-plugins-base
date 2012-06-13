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

#ifndef __GST_DMA_BUF_H__
#define __GST_DMA_BUF_H__

#include <gst/gst.h>

G_BEGIN_DECLS

/**
 * GstDmaBuf:
 *
 * A handle to a dma-buf suitable for sharing across devices.
 *
 * Since: 0.10.?
 */
#define GST_TYPE_DMA_BUF			\
  (gst_dma_buf_get_type ())
#define GST_DMA_BUF(obj)			\
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DMA_BUF, GstDmaBuf))
#define GST_IS_DMA_BUF(obj)			\
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DMA_BUF))

typedef struct _GstDmaBuf      GstDmaBuf;
typedef struct _GstDmaBufClass GstDmaBufClass;

/**
 * gst_dma_buf_ref:
 * @dmabuf: a a #GstDmaBuf.
 *
 * Increases the refcount by one.
 *
 * Returns: (transfer full): @dmabuf
 *
 * Since: 0.10.?
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC GstDmaBuf *
gst_dma_buf_ref (GstDmaBuf * dmabuf);
#endif

static inline GstDmaBuf *
gst_dma_buf_ref (GstDmaBuf * dmabuf)
{
  return (GstDmaBuf *) gst_mini_object_ref (GST_MINI_OBJECT_CAST (dmabuf));
}

/**
 * gst_dma_buf_unref:
 * @dmabuf: (transfer full): a #GstDmaBuf.
 *
 * Decreases the refcount. If the refcount reaches 0, the dmabuf will be
 * freed and closed.
 *
 * Since: 0.10.?
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void
gst_dma_buf_unref (GstDmaBuf * dmabuf);
#endif

static inline void
gst_dma_buf_unref (GstDmaBuf * dmabuf)
{
  gst_mini_object_unref (GST_MINI_OBJECT_CAST (dmabuf));
}

GType                        gst_dma_buf_get_type (void);

GstDmaBuf *                  gst_dma_buf_new (int fd);

int                          gst_dma_buf_get_fd (GstDmaBuf * buf);

/* attach/retrieve dmabuf from buffers */

void                         gst_buffer_set_dma_buf (GstBuffer * buf,
                                                     GstDmaBuf * dmabuf);

GstDmaBuf *                  gst_buffer_get_dma_buf (GstBuffer * buf);

G_END_DECLS

#endif /* __GST_DMA_BUF_H__ */
