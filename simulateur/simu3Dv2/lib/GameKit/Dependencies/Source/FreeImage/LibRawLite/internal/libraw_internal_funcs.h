/* -*- C++ -*-
 * File: libraw_internal_funcs.h
 * Copyright 2008-2009 LibRaw LLC (info@libraw.org)
 * Created: Sat Mar  14, 2008
 *
 * LibRaw-Lite internal data structures (not visible outside)
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

#ifndef _LIBRAW_INTERNAL_FUNCS_H
#define _LIBRAW_INTERNAL_FUNCS_H

#ifndef LIBRAW_LIBRARY_BUILD
#error "This file should be used only for libraw library build"
#else
// inline functions
    ushort      sget2 (uchar *s);
    ushort      get2();
    unsigned    sget4 (uchar *s);
    unsigned    getint (int type);
    float       int_to_float (int i);
    double      getreal (int type);
    void        read_shorts (ushort *pixel, int count);

// Canon P&S cameras
    void        canon_600_fixed_wb (int temp);
    int         canon_600_color (int ratio[2], int mar);
    void        canon_600_auto_wb();
    void        canon_600_coeff();
    void        canon_600_load_raw();
    int         canon_s2is();
    void        canon_a5_load_raw();
    void        parse_ciff (int offset, int length);
    void        ciff_block_1030();

// LJPEG decoder
    unsigned    getbits (int nbits);
    void        init_decoder();
    uchar *     make_decoder (const uchar *source, int level);
    int         ljpeg_start (struct jhead *jh, int info_only);
    int         ljpeg_diff (struct decode *dindex);
    ushort *    ljpeg_row (int jrow, struct jhead *jh);

// Canon DSLRs
    void        crw_init_tables (unsigned table);
    int         canon_has_lowbits();
    void        canon_compressed_load_raw();
    void        lossless_jpeg_load_raw();
    void        canon_sraw_load_raw();
    void        canon_black(double *);
// Adobe DNG
    void        adobe_copy_pixel (int row, int col, ushort **rp);
    void        adobe_dng_load_raw_lj();
    void        adobe_dng_load_raw_nc();

// Pentax
    void        pentax_k10_load_raw();
    void        pentax_tree();

// Nikon (and Minolta Z2)
    void        nikon_compressed_load_raw();
    void        nikon_load_raw();
    int         nikon_is_compressed();
    int         nikon_e995();
    int         nikon_e2100();
    void        nikon_3700();
    int         minolta_z2();
    void        nikon_e900_load_raw();
    void        nikon_e2100_load_raw();

// Fuji
    void        fuji_load_raw();
    void        parse_fuji (int offset);




// Rollei
    void        rollei_load_raw();
    void        parse_rollei();

// MF backs
    int         bayer (unsigned row, unsigned col);
    void        phase_one_flat_field (int is_float, int nc);
    void        phase_one_correct();
    void        phase_one_load_raw();
    unsigned    ph1_bits (int nbits);
    void        phase_one_load_raw_c();
    void        hasselblad_load_raw();
    void        leaf_hdr_load_raw();
    void        sinar_4shot_load_raw();
    void        imacon_full_load_raw();
    void        packed_12_load_raw();
    void        unpacked_load_raw();
    void        parse_sinar_ia();
    void        parse_phase_one (int base);

// Misc P&S cameras
    void        nokia_load_raw();
    unsigned    pana_bits (int nbits);
    void        panasonic_load_raw();
    void        olympus_e300_load_raw();
    void        olympus_e410_load_raw();
    void        olympus_cseries_load_raw();
    void        minolta_rd175_load_raw();
    void        casio_qv5700_load_raw();
    void        quicktake_100_load_raw();
    const int*  make_decoder_int (const int *source, int level);
    int         radc_token (int tree);
    void        kodak_radc_load_raw();
    void        kodak_jpeg_load_raw();
    void        kodak_dc120_load_raw();
    void        eight_bit_load_raw();
    void        smal_decode_segment (unsigned seg[2][2], int holes);
    void        smal_v6_load_raw();
    int         median4 (int *p);
    void        fill_holes (int holes);
    void        smal_v9_load_raw();
    void        parse_riff();
    void        parse_cine();
    void        parse_smal (int offset, int fsize);
    int         parse_jpeg (int offset);

// Kodak
    void        kodak_262_load_raw();
    int         kodak_65000_decode (short *out, int bsize);
    void        kodak_65000_load_raw();
    void        kodak_rgb_load_raw();
    void        kodak_yrgb_load_raw();

// It's a Sony (and K&M)
    void        sony_decrypt (unsigned *data, int len, int start, int key);
    void        sony_load_raw();
    void        sony_arw_load_raw();
    void        sony_arw2_load_raw();
    void        parse_minolta (int base);

// Foveon/Sigma
    void        foveon_load_camf();
    void        foveon_load_raw();
    const char* foveon_camf_param (const char *block, const char *param);
    void *      foveon_camf_matrix (unsigned dim[3], const char *name);
    int         foveon_fixed (void *ptr, int size, const char *name);
    float       foveon_avg (short *pix, int range[2], float cfilt);
    short *     foveon_make_curve (double max, double mul, double filt);
    void        foveon_make_curves(short **curvep, float dq[3], float div[3], float filt);
    int         foveon_apply_curve (short *curve, int i);
    void        foveon_interpolate();
    char *      foveon_gets (int offset, char *str, int len);
    void        parse_foveon();

// CAM/RGB
    void        pseudoinverse (double (*in)[3], double (*out)[3], int size);
    void        cam_xyz_coeff (double cam_xyz[4][3]);
    void        adobe_coeff (const char *, const char *);
    void        simple_coeff (int index);


// Tiff/Exif parsers
    void        tiff_get (unsigned base,unsigned *tag, unsigned *type, unsigned *len, unsigned *save);
    void        parse_thumb_note (int base, unsigned toff, unsigned tlen);
    void        parse_makernote (int base, int uptag);
    void        parse_exif (int base);
    void        linear_table (unsigned len);
    void        parse_kodak_ifd (int base);
    int         parse_tiff_ifd (int base);
    void        parse_tiff (int base);
    void        parse_gps (int base);
    void        romm_coeff (float romm_cam[3][3]);
    void        parse_mos (int offset);
    void        get_timestamp (int reversed);

// External JPEGs, what cameras uses it ?
    void        parse_external_jpeg();

// The identify
    short       guess_byte_order (int words);

// Tiff writer
    void        tiff_set (ushort *ntag, ushort tag, ushort type, int count, int val);
    void        tiff_head (struct tiff_hdr *th, int full);
#endif

#endif
