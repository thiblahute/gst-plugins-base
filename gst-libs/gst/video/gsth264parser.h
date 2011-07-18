/* Gstreamer
 * Copyright (C) <2011> Intel
 * Copyright (C) <2011> Collabora Ltd.
 * Copyright (C) <2011> Thibault Saunier <thibault.saunier@collabora.com>
 *
 * Some bits C-c,C-v'ed and s/4/3 from h264parse and videoparsers/h264parse.c:
 *    Copyright (C) <2010> Mark Nauwelaerts <mark.nauwelaerts@collabora.co.uk>
 *    Copyright (C) <2010> Collabora Multimedia
 *    Copyright (C) <2010> Nokia Corporation
 *
 *    (C) 2005 Michal Benes <michal.benes@itonis.tv>
 *    (C) 2008 Wim Taymans <wim.taymans@gmail.com>
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

#ifndef __GST_H264_PARAMS_H__
#define __GST_H264_PARAMS_H__

#include <gst/gst.h>

G_BEGIN_DECLS

typedef enum
{
  GST_H264_NAL_UNKNOWN = 0,
  GST_H264_NAL_SLICE = 1,
  GST_H264_NAL_SLICE_DPA = 2,
  GST_H264_NAL_SLICE_DPB = 3,
  GST_H264_NAL_SLICE_DPC = 4,
  GST_H264_NAL_SLICE_IDR = 5,
  GST_H264_NAL_SEI = 6,
  GST_H264_NAL_SPS = 7,
  GST_H264_NAL_PPS = 8,
  GST_H264_NAL_AU_DELIMITER = 9,
  GST_H264_NAL_SEQ_END = 10,
  GST_H264_NAL_STREAM_END = 11,
  GST_H264_NAL_FILTER_DATA = 12
} GstH264NalUnitType;

/* SEI type */
typedef enum
{
  GST_H264_SEI_BUF_PERIOD = 0,
  GST_H264_SEI_PIC_TIMING = 1
      /* and more...  */
} GstH264SEIPayloadType;

/* SEI pic_struct type */
typedef enum
{
  GST_H264_SEI_PIC_STRUCT_FRAME = 0,
  GST_H264_SEI_PIC_STRUCT_TOP_FIELD = 1,
  GST_H264_SEI_PIC_STRUCT_BOTTOM_FIELD = 2,
  GST_H264_SEI_PIC_STRUCT_TOP_BOTTOM = 3,
  GST_H264_SEI_PIC_STRUCT_BOTTOM_TOP = 4,
  GST_H264_SEI_PIC_STRUCT_TOP_BOTTOM_TOP = 5,
  GST_H264_SEI_PIC_STRUCT_BOTTOM_TOP_BOTTOM = 6,
  GST_H264_SEI_PIC_STRUCT_FRAME_DOUBLING = 7,
  GST_H264_SEI_PIC_STRUCT_FRAME_TRIPLING = 8
} GstH264SEIPicStructType;

/*  FIXME, get the real numbers */
typedef enum
{
  GST_H264_P_SLICE,
  GST_H264_B_SLICE,
  GST_H264_I_SLICE,
  GST_H264_SP_SLICE,
  GST_H264_SI_SLICE,
  GST_H264_S_P_SLICE,
  GST_H264_S_B_SLICE,
  GST_H264_S_I_SLICE,
  GST_H264_S_SP_SLICE,
  GST_H264_S_SI_SLICE
} GstH264SliceType;

typedef struct _GstH264NalParser GstH264NalParser;

typedef struct _GstH264NalUnit GstH264NalUnit;

typedef struct _GstH264SPS GstH264SPS;
typedef struct _GstH264PPS GstH264PPS;
typedef struct _GstH264HRDParams GstH264HRDParams;
typedef struct _GstH264VUIParams GstH264VUIParams;

typedef struct _GstH264Picture GstH264Picture;

typedef struct _GstH264DecRefPicMarking GstH264DecRefPicMarking;
typedef struct _GstH264RefPicMarking GstH264RefPicMarking;
typedef struct _GstH264PredWeightTable GstH264PredWeightTable;
typedef struct _GstH264SliceHdr GstH264SliceHdr;
typedef struct _GstH264SEIMessage GstH264SEIMessage;


struct _GstH264NalUnit
{
  guint16 ref_idc;
  guint16 type;

  /* calculated values */
  guint8 IdrPicFlag;
  gint size;
  guint offset;
};

struct _GstH264SPS
{
  gint id;

  guint8 profile_idc;
  guint8 constraint_set0_flag;
  guint8 constraint_set1_flag;
  guint8 constraint_set2_flag;
  guint8 constraint_set3_flag;
  guint8 level_idc;

  guint8 chroma_format_idc;
  guint8 separate_colour_plane_flag;
  guint8 bit_depth_luma_minus8;
  guint8 bit_depth_chroma_minus8;
  guint8 qpprime_y_zero_transform_bypass_flag;

  guint8 scaling_matrix_present_flag;
  guint8 scaling_lists_4x4[6][16];
  guint8 scaling_lists_8x8[6][64];

  guint8 log2_max_frame_num_minus4;
  guint8 pic_order_cnt_type;

  /* if pic_order_cnt_type == 0 */
  guint8 log2_max_pic_order_cnt_lsb_minus4;

  /* else if pic_order_cnt_type == 1 */
  guint8 delta_pic_order_always_zero_flag;
  gint32 offset_for_non_ref_pic;
  gint32 offset_for_top_to_bottom_field;
  guint8 num_ref_frames_in_pic_order_cnt_cycle;
  gint32 offset_for_ref_frame[255];

  guint32 num_ref_frames;
  guint8 gaps_in_frame_num_value_allowed_flag;
  guint32 pic_width_in_mbs_minus1;
  guint32 pic_height_in_map_units_minus1;
  guint8 frame_mbs_only_flag;

  guint8 mb_adaptive_frame_field_flag;

  guint8 direct_8x8_inference_flag;

  guint8 frame_cropping_flag;

  /* if frame_cropping_flag */
  guint32 frame_crop_left_offset;
  guint32 frame_crop_right_offset;
  guint32 frame_crop_top_offset;
  guint32 frame_crop_bottom_offset;

  guint8 vui_parameters_present_flag;
  /* if vui_parameters_present_flag */
  GstH264VUIParams *vui_parameters;

  /* calculated values */
  guint8 ChromaArrayType;
  guint32 MaxFrameNum;
  gint width, height;
  gint fps_num, fps_den;
};

struct _GstH264VUIParams
{
  guint8 aspect_ratio_idc;
  /* if aspect_ratio_idc == 255 */
  guint16 sar_width;
  guint16 sar_height;

  guint8 overscan_info_present_flag;
  /* if overscan_info_present_flag */
  guint8 overscan_appropriate_flag;

  guint8 video_format;
  guint8 video_full_range_flag;
  guint8 colour_description_present_flag;
  guint8 colour_primaries;
  guint8 transfer_characteristics;
  guint8 matrix_coefficients;

  guint8 chroma_sample_loc_type_top_field;
  guint8 chroma_sample_loc_type_bottom_field;

  guint8 timing_info_present_flag;
  /* if timing_info_present_flag */
  guint32 num_units_in_tick;
  guint32 time_scale;
  guint8 fixed_frame_rate_flag;

  guint8 nal_hrd_parameters_present_flag;
  /* if nal_hrd_parameters_present_flag */
  GstH264HRDParams nal_hrd_parameters;

  guint8 vcl_hrd_parameters_present_flag;
  /* if nal_hrd_parameters_present_flag */
  GstH264HRDParams vcl_hrd_parameters;

  guint8 low_delay_hrd_flag;
  guint8 pic_struct_present_flag;
};

struct _GstH264HRDParams
{
  guint8 cpb_cnt_minus1;
  guint8 bit_rate_scale;
  guint8 cpb_size_scale;

  guint32 bit_rate_value_minus1[32];
  guint32 cpb_size_value_minus1[32];
  guint8 cbr_flag[32];

  guint8 initial_cpb_removal_delay_length_minus1;
  guint8 cpb_removal_delay_length_minus1;
  guint8 dpb_output_delay_length_minus1;
  guint8 time_offset_length;
};

struct _GstH264PPS
{
  gint id;

  guint sequenceid;

  guint8 entropy_coding_mode_flag;
  guint8 pic_order_present_flag;

  guint32 num_slice_groups_minus1;

  /* if num_slice_groups_minus1 > 0 */
  guint8 slice_group_map_type;
  /* and if slice_group_map_type == 0 */
  guint32 run_length_minus1[8];
  /* or if slice_group_map_type == 2 */
  guint32 top_left[8];
  guint32 bottom_right[8];
  /* or if slice_group_map_type == (3, 4, 5) */
  guint8 slice_group_change_direction_flag;
  guint32 slice_group_change_rate_minus1;
  /* or if slice_group_map_type == 6 */
  guint32 pic_size_in_map_units_minus1;
  guint8 *slice_group_id;

  guint8 num_ref_idx_l0_active_minus1;
  guint8 num_ref_idx_l1_active_minus1;
  guint8 weighted_pred_flag;
  guint8 weighted_bipred_idc;
  gint8 pic_init_qp_minus26;
  gint8 pic_init_qs_minus26;
  gint8 chroma_qp_index_offset;
  guint8 deblocking_filter_control_present_flag;
  guint8 constrained_intra_pred_flag;
  guint8 redundant_pic_cnt_present_flag;

  guint8 transform_8x8_mode_flag;

  guint8 scaling_lists_4x4[6][16];
  guint8 scaling_lists_8x8[6][64];

  guint8 second_chroma_qp_index_offset;
};

struct _GstH264SliceHdr
{
  guint32 first_mb_in_slice;
  guint32 type;
  GstH264PPS *picture;

  /* if seq->separate_colour_plane_flag */
  guint8 colour_plane_id;

  guint16 frame_num;

  guint8 field_pic_flag;
  guint8 bottom_field_flag;

  /* if nal_unit.type == 5 */
  guint16 idr_pic_id;

  /* if seq->pic_order_cnt_type == 0 */
  guint16 pic_order_cnt_lsb;
  /* if seq->pic_order_present_flag && !field_pic_flag */
  gint32 delta_pic_order_cnt_bottom;

  gint32 delta_pic_order_cnt[2];
  guint8 redundant_pic_cnt;

  /* if slice_type == B_SLICE */
  guint8 direct_spatial_mv_pred_flag;

  guint8 num_ref_idx_l0_active_minus1;
  guint8 num_ref_idx_l1_active_minus1;

  GstH264PredWeightTable pred_weight_table;
  /* if nal_unit.ref_idc != 0 */
  GstH264DecRefPicMarking dec_ref_pic_marking;

  /* calculated values */
  guint32 mac_pic_num;
};

struct _GstH264SEIMessage
{
  guint32 payloadType;

  union {
    GstH264BufferingPeriod buffering_period;
    GstH264PicTiming pic_timing;
    /* ... could implement more */
  };
};

struct _GstH264NalParser
{
  /**
   * Might we use an adapter and add *_push() and *_flush()?
   */
  GstBuffer *buffer; /* last parsed buffer */
  GstH264NalUnit *nalunit; /*  Last parsed nal unit */

  /* In the cas of  slice header parsing */
  gboolean parse_pred_weight_table;
  gboolean parse_dec_ref_pic_marking;

  /*  In the case of SPS parsing, (we also parse
   *  the nal_hrd_parameters and vcl_hrd_parameters) */
  gboolean parse_vui_params;

  union { /* Check the nal unit type. */
    GstH264SliceHdr *slicehdr;
    GstH264SEIMessage *seimessage;
    GstH264SPS *sps;
    GstH264PPS *pps;
  };

  /* private */
  GHashTable *pictures; /* Picture ID -> GstH264PPS */
  GHashTable *sequences; /* Sequence ID -> GstH264SPS */
};

/**
 * gst_h264_parse:
 * @buf: The buffer to parse
 * @offset: the offset from which to parse  @buf
 *
 * Parses the buffer and returns a list of nals
 *
 * Returns: A list of nal, ordered as they are in the stream
 */
GList gst_h264_parse                        (GstBuffer *buf, guint offset);

/**
 * gst_h264_parse_nal:
 * @nalparser: The nal parser that specifies what to parse and contains
 * the previously parsed pps and sps
 * @buf: The buffer to parse
 * @offset: the nal unit to parse
 *
 * Parses the buffer and set the corresponding structure of @nalparser
 *
 * Returns: %TRUE if the parsing went well, %FALSE otherwise
 */
gboolean *gst_h264_parse_nal                (GstH264NalParser *nalparser, GstBuffer *buffer,
    GstH264NalUnit *nalunit);

GstH264NalParser *gst_h264_nal_parser_new   (void);
void gst_h264_nal_parser_free               (GstH264NalParser *nalparser);

G_END_DECLS
#endif


/* Use example: */
void h264_bitstream_parsing_example (GstBuffer *buf)
{
  GList *nals, *tmp;
  GstH264NalUnit *nal;
  GstH264NalParser *nalparser;

  nals = gst_h264_parse (buf);

  nalparser = gst_h264_nal_parser_new ();
  /*  We need to parse the pred_weight_table and
   *  dec_ref_pic_marking */
  nalparser->parse_pred_weight_table = TRUE;
  nalparser->parse_dec_ref_pic_marking = TRUE;

  for (tmp=nals; tmp; tmp = tmp->next) {
    nal = tmp->data;

    switch (nal->type)
    {
      case GST_H264_NAL_PPS:
        GstH264PPS *pps;

        if (gst_h264_parse_nal (nalparser, buf, nal)) {
          pps = nalparser->pps;
          /*  Do wathever you need with the struct */
        }
        break;
      case GST_H264_NAL_SPS:
        GstH264PPS *sps;

        if (gst_h264_parse_nal (nalparser, buf, nal)) {
          sps = nalparser->sps;
          /*  Do wathever you need with the struct */
        }
        /* Do watever you want */
        break;
      case GST_H264_NAL_SEI:
        GstH264SEIMessage *seimess;

        if (gst_h264_parse_nal (nalparser, buf, nal)) {
          seimessage = nalparser->seimessage;
          /*  Do wathever you need with the struct */
        }
        break;
      default:
        /*  In the example, we don't need to parse the slice header */
        break;
    }
  }
}
