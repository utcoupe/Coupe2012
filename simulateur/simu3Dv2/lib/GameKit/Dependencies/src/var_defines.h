/* -*- C++ -*-
 * File: var_defines.h
 * Copyright 2008-2009 LibRaw LLC (info@libraw.org)
 * Created: Sat Mar  8, 2008
 *
 * LibRaw redefinitions of dcraw internal variables
 *
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef VAR_DEFINES_H
#define VAR_DEFINES_H

// imgdata.idata
#define make            (imgdata.idata.make)
#define model           (imgdata.idata.model)
#define is_raw          (imgdata.idata.raw_count)
#define dng_version     (imgdata.idata.dng_version)
#define is_foveon       (imgdata.idata.is_foveon)
#define colors          (imgdata.idata.colors)
#define cdesc           (imgdata.idata.cdesc)
#define filters         (imgdata.idata.filters)

//imgdata image
#define image           (imgdata.image)

// imgdata.sizes
#define raw_height      (imgdata.sizes.raw_height)
#define raw_width       (imgdata.sizes.raw_width)
#define height          (imgdata.sizes.height)
#define width           (imgdata.sizes.width)
#define top_margin      (imgdata.sizes.top_margin)
#define left_margin     (imgdata.sizes.left_margin)
#define bottom_margin   (imgdata.sizes.bottom_margin)
#define right_margin    (imgdata.sizes.right_margin)
#define iheight         (imgdata.sizes.iheight)
#define iwidth          (imgdata.sizes.iwidth)
#define pixel_aspect    (imgdata.sizes.pixel_aspect)
#define flip            (imgdata.sizes.flip)

//imgdata.color
#define white           (imgdata.color.white)
#define cam_mul         (imgdata.color.cam_mul)
#define pre_mul         (imgdata.color.pre_mul)
#define cmatrix         (imgdata.color.cmatrix)
#define rgb_cam         (imgdata.color.rgb_cam)
#ifndef SRC_USES_CURVE
#define curve           (imgdata.color.curve)
#endif
#ifndef SRC_USES_BLACK
#define black           (imgdata.color.black)
#endif
#define maximum         (imgdata.color.maximum)
#define profile_length  (imgdata.color.profile_length)
#define color_flags     (imgdata.color.color_flags)
#define ph1             (imgdata.color.phase_one_data)
#define flash_used      (imgdata.color.flash_used)
#define canon_ev        (imgdata.color.canon_ev)
#define model2          (imgdata.color.model2)

//imgdata.thumbnail

#define thumb_width     (imgdata.thumbnail.twidth)
#define thumb_height    (imgdata.thumbnail.theight)
#define thumb_length    (imgdata.thumbnail.tlength)


//imgdata.others
#define iso_speed       (imgdata.other.iso_speed)
#define shutter         (imgdata.other.shutter)
#define aperture        (imgdata.other.aperture)
#define focal_len       (imgdata.other.focal_len)
#define timestamp       (imgdata.other.timestamp)
#define shot_order      (imgdata.other.shot_order)
#define gpsdata         (imgdata.other.gpsdata)
#define desc            (imgdata.other.desc)
#define artist          (imgdata.other.artist)

//imgdata.output
#define greybox         (imgdata.params.greybox)
#define aber            (imgdata.params.aber)
#define gamm            (imgdata.params.gamm)
#define user_mul        (imgdata.params.user_mul)
#define shot_select     (imgdata.params.shot_select)
#define bright          (imgdata.params.bright)
#define threshold       (imgdata.params.threshold)
#define half_size       (imgdata.params.half_size)
#define four_color_rgb  (imgdata.params.four_color_rgb)
#define document_mode   (imgdata.params.document_mode)
#define highlight       (imgdata.params.highlight)
//#define verbose         (imgdata.params.verbose)
#define use_auto_wb     (imgdata.params.use_auto_wb)
#define use_camera_wb   (imgdata.params.use_camera_wb)
#define use_camera_matrix (imgdata.params.use_camera_matrix)
#define output_color    (imgdata.params.output_color)
#define output_bps      (imgdata.params.output_bps)
#define gamma_16bit      (imgdata.params.gamma_16bit)
#define output_tiff     (imgdata.params.output_tiff)
#define med_passes      (imgdata.params.med_passes)
#define no_auto_bright  (imgdata.params.no_auto_bright)
#define use_fuji_rotate (imgdata.params.use_fuji_rotate)

//rgb_constants
#define xyz_rgb         (rgb_constants.xyz_rgb)
#define d65_white       (rgb_constants.d65_white)

//libraw_internal_data.internal_data
#define meta_data       (libraw_internal_data.internal_data.meta_data)
#define ifp             libraw_internal_data.internal_data.input
#define ifname          ((char*)libraw_internal_data.internal_data.input->fname())
#define profile_offset  (libraw_internal_data.internal_data.profile_offset)
#define thumb_offset    (libraw_internal_data.internal_data.toffset)

//libraw_internal_data.internal_output_params
#define mix_green       (libraw_internal_data.internal_output_params.mix_green)
#define raw_color       (libraw_internal_data.internal_output_params.raw_color)
#define use_gamma       (libraw_internal_data.internal_output_params.use_gamma)
#define zero_is_bad     (libraw_internal_data.internal_output_params.zero_is_bad)
#ifndef SRC_USES_SHRINK
#define shrink          (libraw_internal_data.internal_output_params.shrink)
#endif
#define fuji_width      (libraw_internal_data.internal_output_params.fuji_width)


//libraw_internal_data.output_data
#define histogram       (libraw_internal_data.output_data.histogram)
#define oprof           (libraw_internal_data.output_data.oprof)

//libraw_internal_data.identify_data
#define exif_cfa        (libraw_internal_data.identify_data.olympus_exif_cfa)
#define unique_id       (libraw_internal_data.identify_data.unique_id)
#define tiff_nifds      (libraw_internal_data.identify_data.tiff_nifds)
#define tiff_flip       (libraw_internal_data.identify_data.tiff_flip)

//libraw_internal_data.unpacker_data
#define order           (libraw_internal_data.unpacker_data.order)
#define data_error      (libraw_internal_data.unpacker_data.data_error)
#define cr2_slice       (libraw_internal_data.unpacker_data.cr2_slice)
#define sraw_mul        (libraw_internal_data.unpacker_data.sraw_mul)
#define kodak_cbpp      (libraw_internal_data.unpacker_data.kodak_cbpp)
#define strip_offset    (libraw_internal_data.unpacker_data.strip_offset)
#define data_offset     (libraw_internal_data.unpacker_data.data_offset)
#define meta_offset     (libraw_internal_data.unpacker_data.meta_offset)
#define meta_length     (libraw_internal_data.unpacker_data.meta_length)
#define thumb_misc      (libraw_internal_data.unpacker_data.thumb_misc)
#define fuji_layout     (libraw_internal_data.unpacker_data.fuji_layout)
#define tiff_samples    (libraw_internal_data.unpacker_data.tiff_samples)
#define tiff_bps        (libraw_internal_data.unpacker_data.tiff_bps)
#define tiff_compress   (libraw_internal_data.unpacker_data.tiff_compress)
#define zero_after_ff   (libraw_internal_data.unpacker_data.zero_after_ff)
#define tile_width      (libraw_internal_data.unpacker_data.tile_width)
#define tile_length     (libraw_internal_data.unpacker_data.tile_length)
#define load_flags      (libraw_internal_data.unpacker_data.load_flags)

#ifdef LIBRAW_IO_REDEFINED
#define fread(ptr,size,n,stream) stream->read(ptr,size,n)
#define fseek(stream,o,w)	 stream->seek(o,w)
#define fseeko(stream,o,w)	 stream->seek(o,w)
#define ftell(stream)		 stream->tell()
#define ftello(stream)		 stream->tell()
#define getc(stream)		 stream->get_char()
#define fgetc(stream)		 stream->get_char()
#define fgets(str,n,stream)	 stream->gets(str,n)
#define fscanf(stream,fmt,ptr)	 stream->scanf_one(fmt,ptr)
#endif

#endif
