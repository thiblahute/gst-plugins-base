/* Gstreamer
 * Copyright (C) <2011> Intel
 * Copyright (C) <2011> Collabora Ltd.
 * Copyright (C) <2011> Thibault Saunier <thibault.saunier@collabora.com>
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

#include <gst/check/gstcheck.h>
#include <gst/video/gstmpegvideoparser.h>

/* actually seq + gop */
static guint8 mpeg2_seq[] = {
  0x00, 0x00, 0x01, 0xb3, 0x02, 0x00, 0x18, 0x15, 0xff, 0xff, 0xe0, 0x28,
  0x00, 0x00, 0x01, 0xb3, 0x78, 0x04, 0x38, 0x37, 0xff, 0xff, 0xf0, 0x00,
  0x00, 0x00, 0x01, 0xb5, 0x14, 0x8a, 0x00, 0x11, 0x03, 0x71,
  0x00, 0x00, 0x01, 0xb8, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x03, 0x00, 0x08, 0x00, 0x00
};

static GstMpegVideoPacketTypeCode ordercode[] = {
  GST_MPEG_VIDEO_PACKET_SEQUENCE,
  GST_MPEG_VIDEO_PACKET_EXTENSION,
  GST_MPEG_VIDEO_PACKET_GOP,
};

GST_START_TEST (test_mpeg_parse)
{
  gint i;
  GList *list, *tmp;
  GstMpegVideoTypeOffsetSize *typeoffsz;

  list = gst_mpeg_video_parse (mpeg2_seq, sizeof (mpeg2_seq), 12);

  assert_equals_int (g_list_length (list), 4);
  for (tmp = list, i = 0; tmp; tmp = g_list_next (tmp), i++) {
    typeoffsz = tmp->data;
    if (i == 3) {
      fail_unless (GST_MPEG_VIDEO_PACKET_SLICE_MIN <= typeoffsz->type &&
          typeoffsz->type <= GST_MPEG_VIDEO_PACKET_SLICE_MAX);
      fail_unless (typeoffsz->size < 0);
    } else
      assert_equals_int (ordercode[i], typeoffsz->type);
  }

  g_list_free_full (list, (GDestroyNotify) g_free);
}

GST_END_TEST;

GST_START_TEST (test_mpeg_parse_sequence_header)
{
  GList *list;
  GstMpegVideoTypeOffsetSize *typeoffsz;
  GstMpegVideoSequenceHdr seqhdr;

  list = gst_mpeg_video_parse (mpeg2_seq, sizeof (mpeg2_seq), 12);

  typeoffsz = list->data;
  fail_unless (typeoffsz->type == GST_MPEG_VIDEO_PACKET_SEQUENCE);
  fail_unless (gst_mpeg_video_parse_sequence_header (&seqhdr, mpeg2_seq,
          sizeof (mpeg2_seq), typeoffsz->offset));
  assert_equals_int (seqhdr.width, 1920);
  assert_equals_int (seqhdr.height, 1080);
  assert_equals_int (seqhdr.aspect_ratio_info, 3);
  assert_equals_int (seqhdr.par_w, 17280);
  assert_equals_int (seqhdr.par_h, 17280);
  assert_equals_int (seqhdr.frame_rate_code, 7);
  assert_equals_int (seqhdr.fps_n, 60000);
  assert_equals_int (seqhdr.fps_d, 1001);
  assert_equals_int (seqhdr.bitrate_value, 262143);
  assert_equals_int (seqhdr.bitrate, 0);
  assert_equals_int (seqhdr.vbv_buffer_size_value, 512);
  fail_unless (seqhdr.constrained_parameters_flag == FALSE);

  g_list_free_full (list, (GDestroyNotify) g_free);
}

GST_END_TEST;

GST_START_TEST (test_mpeg_parse_sequence_extension)
{
  GList *list;
  GstMpegVideoTypeOffsetSize *typeoffsz;
  GstMpegVideoSequenceExtension seqext;

  list = gst_mpeg_video_parse (mpeg2_seq, sizeof (mpeg2_seq), 12);

  typeoffsz = list->next->data;
  fail_unless (typeoffsz->type == GST_MPEG_VIDEO_PACKET_EXTENSION);
  fail_unless (gst_mpeg_video_parse_sequence_extension (&seqext,
          mpeg2_seq, sizeof (mpeg2_seq), typeoffsz->offset));
  assert_equals_int (seqext.profile, 4);
  assert_equals_int (seqext.level, 8);
  assert_equals_int (seqext.progressive, 1);
  assert_equals_int (seqext.chroma_format, 1);
  assert_equals_int (seqext.horiz_size_ext, 0);
  assert_equals_int (seqext.vert_size_ext, 0);
  assert_equals_int (seqext.vert_size_ext, 0);
  assert_equals_int (seqext.bitrate_ext, 8);
  assert_equals_int (seqext.vbv_buffer_size_extension, 3);
  assert_equals_int (seqext.low_delay, 0);
  assert_equals_int (seqext.fps_n_ext, 3);
  assert_equals_int (seqext.fps_d_ext, 2);

  g_list_free_full (list, (GDestroyNotify) g_free);
}

GST_END_TEST;

static Suite *
videoparsers_suite (void)
{
  Suite *s = suite_create ("Video Parsers library");

  TCase *tc_chain = tcase_create ("general");

  suite_add_tcase (s, tc_chain);
  tcase_add_test (tc_chain, test_mpeg_parse);
  tcase_add_test (tc_chain, test_mpeg_parse_sequence_header);
  tcase_add_test (tc_chain, test_mpeg_parse_sequence_extension);

  return s;
}

int
main (int argc, char **argv)
{
  int nf;

  Suite *s = videoparsers_suite ();

  SRunner *sr = srunner_create (s);

  gst_check_init (&argc, &argv);

  srunner_run_all (sr, CK_NORMAL);
  nf = srunner_ntests_failed (sr);
  srunner_free (sr);

  return nf;
}
