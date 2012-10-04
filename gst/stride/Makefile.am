plugin_LTLIBRARIES = libgststridetransform.la

ARM_FILES=armv7.s

if HAVE_CPU_ARM
ARCH_FILES = $(ARM_FILES)
else
ARCH_FILES =
endif

libgststridetransform_la_SOURCES = \
	gststridetransform.c \
	convert.c \
	plugin.c \
	$(ARCH_FILES)

libgststridetransform_la_CFLAGS = $(GST_PLUGINS_BASE_CFLAGS) $(GST_BASE_CFLAGS) $(GST_CFLAGS)
libgststridetransform_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgststridetransform_la_LIBADD = \
	$(top_builddir)/gst-libs/gst/video/libgstvideo-@GST_MAJORMINOR@.la \
	$(GST_BASE_LIBS) $(GST_LIBS) $(LIBM)
libgststridetransform_la_LIBTOOLFLAGS = --tag=disable-static

noinst_HEADERS = \
	gststridetransform.h
