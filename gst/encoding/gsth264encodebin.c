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

#include "gsth264encodebin.h"
#include <gst/video/gsth264encoderinterface.h>

GST_DEBUG_CATEGORY (h264encodebin_debug);
#define GST_CAT_DEFAULT h264encodebin_debug

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-raw, "
        "framerate = (fraction) [0/1, MAX], "
        "width = (int) [ 1, MAX ], " "height = (int) [ 1, MAX ];"
        "video/x-raw(ANY), "
        "framerate = (fraction) [0/1, MAX], "
        "width = (int) [ 1, MAX ], " "height = (int) [ 1, MAX ];")
    );

static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-h264, "
        "framerate = (fraction) [0/1, MAX], "
        "width = (int) [ 1, MAX ], " "height = (int) [ 1, MAX ]")
    );

enum
{
  PROP_0,
  PROP_BITRATE,
  NUM_PROPERTIES
};
static GParamSpec *properties[NUM_PROPERTIES];

#define DEFAULT_BITRATE (2 * 1024 * 1024)

struct _GstH264EncodeBin
{
  GstBin parent_instance;

  GstElement *encoder;

  guint bitrate;
};

struct _GstH264EncodeBinClass
{
  GstBinClass parent_class;
};

G_DEFINE_TYPE (GstH264EncodeBin, gst_h264_encode_bin, GST_TYPE_BIN);

static void
gst_h264_encode_bin_set_bitrate (GstH264EncodeBin * self, guint bitrate)
{
  if (self->encoder) {
    gst_h264_encoder_interface_set_bitrate (GST_H264_ENCODER_INTERFACE
        (self->encoder), bitrate);
  }

  GST_OBJECT_LOCK (self);
  self->bitrate = bitrate;
  GST_OBJECT_UNLOCK (self);
}

static void
gst_h264_encode_bin_sync_values (GstH264EncodeBin * self)
{
  gst_h264_encode_bin_set_bitrate (self, self->bitrate);
}

static gboolean
gst_h264_encode_bin_setup_encoder (GstH264EncodeBin * self)
{
  GList *tmp, *encoders =
      gst_element_factory_list_get_elements
      (GST_ELEMENT_FACTORY_TYPE_VIDEO_ENCODER,
      GST_RANK_MARGINAL);

  for (tmp = encoders; tmp; tmp = tmp->next) {
    if (gst_element_factory_has_interface (tmp->data,
            "GstH264EncoderInterface")) {
      GstElement *encoder = gst_element_factory_create (tmp->data, NULL);

      if (gst_element_set_state (encoder,
              GST_STATE_READY) == GST_STATE_CHANGE_SUCCESS) {
        GstPad *tmppad;
        GstGhostPad *tmpghost;

        GST_INFO_OBJECT (self, "Using %" GST_PTR_FORMAT " as an encoder",
            self->encoder);
        gst_bin_add (GST_BIN (self), encoder);

        tmppad = gst_element_get_static_pad (encoder, "sink");
        tmpghost =
            GST_GHOST_PAD (gst_element_get_static_pad (GST_ELEMENT (self),
                "sink"));
        gst_ghost_pad_set_target (tmpghost, tmppad);
        g_clear_object (&tmppad);
        g_clear_object (&tmpghost);

        tmppad = gst_element_get_static_pad (encoder, "src");
        tmpghost =
            GST_GHOST_PAD (gst_element_get_static_pad (GST_ELEMENT (self),
                "src"));
        gst_ghost_pad_set_target (tmpghost, tmppad);
        g_clear_object (&tmppad);
        g_clear_object (&tmpghost);

      GST_OBJECT_LOCK (self):
        self->encoder = encoder;
      GST_OBJECT_UNLOCK (self):

        gst_h264_encode_bin_sync_values (self);
        break;
      }

      GST_INFO_OBJECT (self,
          "Not using %" GST_PTR_FORMAT " as we couldn't set it to READY",
          encoder);
      g_clear_object (&encoder);
    }
  }
  gst_plugin_feature_list_free (encoders);

  return self->encoder != NULL;
}

static GstStateChangeReturn
gst_h264_encode_bin_change_state (GstElement * element,
    GstStateChange transition)
{
  GstStateChangeReturn res;
  GstH264EncodeBin *self = (GstH264EncodeBin *) element;

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
      if (!gst_h264_encode_bin_setup_encoder (self)) {
        GST_ELEMENT_ERROR (self, CORE, MISSING_PLUGIN,
            ("Couldn't create H264 encoder implementing the GstH264EncoderInterface."),
            (NULL));

        return GST_STATE_CHANGE_FAILURE;
      }
      break;
    default:
      break;
  }

  res =
      GST_ELEMENT_CLASS (gst_h264_encode_bin_parent_class)->change_state
      (element, transition);

  switch (transition) {
    case GST_STATE_CHANGE_READY_TO_NULL:
      if (!gst_h264_encode_bin_setup_encoder (self)) {
        GST_ELEMENT_ERROR (self, CORE, MISSING_PLUGIN,
            ("Couldn't create H264 encoder implementing the GstH264EncoderInterface."),
            (NULL));

        return GST_STATE_CHANGE_FAILURE;
      }
      break;
    default:
      break;
  }
}

static void
gst_h264_encode_bin_dispose (GObject * object)
{
  G_OBJECT_CLASS (gst_h264_encode_bin_parent_class)->dispose (object);
}

static void
gst_h264_encode_bin_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec)
{
  GstH264EncodeBin *self = GST_H264_ENCODE_BIN (object);

  switch (prop_id) {
    case PROP_BITRATE:
      gst_h264_encode_bin_set_bitrate (self, g_value_get_uint (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (self, prop_id, pspec);
  }
}

static void
gst_h264_encode_bin_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec)
{
  GstH264EncodeBin *self = GST_H264_ENCODE_BIN (object);

  switch (prop_id) {
    case PROP_BITRATE:
      GST_OBJECT_LOCK (self);
      g_value_set_uint (value, self->bitrate);
      GST_OBJECT_UNLOCK (self);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (self, prop_id, pspec);
  }
}

static void
gst_h264_encode_bin_class_init (GstH264EncodeBinClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *gstelement_class = GST_ELEMENT_CLASS (klass);

  GST_DEBUG_CATEGORY_INIT (h264encodebin_debug, "h264encodebin", 0,
      "Bin wrapping H264 encoder to unify their APIs");

  gobject_class->dispose = gst_h264_encode_bin_dispose;
  gobject_class->set_property =
      GST_DEBUG_FUNCPTR (gst_h264_encode_bin_set_property);
  gobject_class->get_property =
      GST_DEBUG_FUNCPTR (gst_h264_encode_bin_get_property);

  /**
   * 
   */
  properties[PROP_BITRATE] =
      g_param_spec_uint ("bitrate-bps", "Bitrate in bits per second",
      "Target encoding bitrate in bits per second.", 1024, G_MAXUINT,
      DEFAULT_BITRATE,
      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING);
  g_object_class_install_properties (gobject_class, NUM_PROPERTIES, properties);

  gstelement_class->change_state = gst_h264_encode_bin_change_state;
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_template));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&src_template));
}

static void
gst_h264_encode_bin_init (GstH264EncodeBin * self)
{
  self->bitrate = DEFAULT_BITRATE;

  gst_element_add_pad (GST_ELEMENT (self),
      gst_ghost_pad_new_no_target_from_template ("sink",
          gst_static_pad_template_get (&sink_template)));

  gst_element_add_pad (GST_ELEMENT (self),
      gst_ghost_pad_new_no_target_from_template ("src",
          gst_static_pad_template_get (&src_template)));
}
