lib_LTLIBRARIES = libgstdmabuf-@GST_MAJORMINOR@.la

libgstdmabuf_@GST_MAJORMINOR@_la_SOURCES = \
	dmabuf.c
libgstdmabuf_@GST_MAJORMINOR@includedir = $(includedir)/gstreamer-@GST_MAJORMINOR@/gst/dmabuf
libgstdmabuf_@GST_MAJORMINOR@include_HEADERS = \
	dmabuf.h

libgstdmabuf_@GST_MAJORMINOR@_la_CFLAGS = \
	$(GST_PLUGINS_BASE_CFLAGS)\
	$(GST_BASE_CFLAGS) \
	$(GST_CFLAGS)

libgstdmabuf_@GST_MAJORMINOR@_la_LIBADD = $(GST_BASE_LIBS) \
	$(GST_LIBS)

libgstdmabuf_@GST_MAJORMINOR@_la_LDFLAGS = \
	$(GST_LIB_LDFLAGS) \
	$(GST_ALL_LDFLAGS) \
	$(GST_LT_LDFLAGS)

