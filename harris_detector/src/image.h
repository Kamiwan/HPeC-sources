/*************************************************************************************
 * File   : image.h, file for Vision Based Autonomous Landing
 *
 * Copyright (C) 2016 Lab-Sticc Laboratory
 * Author(s) :  Dominique Heller, dominique.heller@univ-ubs.fr
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *************************************************************************************/
#ifndef IMAGE_H
#define IMAGE_H
#define LEVEL 256
#define LEVEL_MAX 255

#define IMAGE_WIDTH 640
#define IMAGE_HEIGH 480
#define IMAGE_SIZE IMAGE_WIDTH*IMAGE_HEIGH

/*  MAX_WIDTH+( KERNELGAUSSIAN-1) */
#define LINEBUFFERSIZEGAUSSIAN 644
/*  MAX_WIDTH+( KERNELSOBEL-1) */
#define LINEBUFFERSIZESOBEL 642
/*  MAX_WIDTH+( KERNELNMS-1) */
#define LINEBUFFERSIZENMS 642

//#define MAX_KEYPOINT 1024
#define MAX_KEYPOINT 2048

//#include "fixpointq16.h"
#include "stdint.h"


// Remark : 2D data are usually stored in computer memory as contiguous 1D array.
// So, we can also used  1D array for 2D data : row1, row2, row3... */
//see https://fr.wikipedia.org/wiki/Portable_pixmap
struct {
    int X;
    int Y;
    } typedef Point;

/* bw format image : bitmamp : used unsigned char for bit */
typedef struct{
    int w;
    int h;
    unsigned char * img;
} PBM_IMG;

/* gray format image : graymap */
typedef struct{
    int w;
    int h;
    unsigned char * img;
} PGM_IMG;

/* color format image : pixmap */
typedef struct{
    int w;
    int h;
    unsigned char * img;
} PPM_IMG;

/* integral and square integral of an image */
typedef struct{
    int w;
    int h;
    int32_t *value;  /* need 28 bits : VGA 640x480x256, HD 1280x720x256*/
} INTEGRAL_IMG;

struct keyPoint_t {
	int x; // pixel coordinates
	int y;
	int cim;//calcim from harris
	float size; // gaussian sigma
}; typedef struct keyPoint_t keyPoint;


PBM_IMG read_pbm(const char * path);
PGM_IMG read_pgm(const char * path);
PPM_IMG read_ppm(const char * path);
void write_pbm(PBM_IMG img, const char * path);
void write_pgm(PGM_IMG img, const char * path);
void write_ppm(PPM_IMG img, const char * path);
PGM_IMG rgb2gray(PPM_IMG color_img);
void free_pbm(PGM_IMG img);
void free_pgm(PGM_IMG img);
void free_ppm(PPM_IMG img);

PGM_IMG warpimg(PGM_IMG img_in,float thetaref, int tx, int ty);
PGM_IMG rotoZoomFast(PGM_IMG img_in,float angle, float zoom,int offset_x, int offset_y);
//compute integral intensity of an image
void compute_integral_zeroapron(const PGM_IMG *img_in,INTEGRAL_IMG *integral);
void compute_integral(const PGM_IMG *img_in,INTEGRAL_IMG *integral);
//median filter for gray-scale images
PGM_IMG medianfilter(PGM_IMG img_in);
//harris corner
PGM_IMG harriscorner(PGM_IMG img_in,int threshold,keyPoint *keyPoints,int *kpcount,int sort);
//freak
float integral_blur(const INTEGRAL_IMG *integral, int width, int height, int x, int y, float sigma);
void write_vhdl_constant(PGM_IMG img, const char * path);


#endif
