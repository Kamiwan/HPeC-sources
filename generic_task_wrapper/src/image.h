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
//#include "fixpointq16.h"
#include "cca.h"
#include "stdint.h"
#include "moments.h"


// Remark : 2D data are usually stored in computer memory as contiguous 1D array.
// So, we can also used  1D array for 2D data : row1, row2, row3... */
//see https://fr.wikipedia.org/wiki/Portable_pixmap

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

struct pixel {
	unsigned char b;
	unsigned char g;
	unsigned char r;
};

int read_bmp(char* filename, unsigned char** header, struct pixel** data);
void write_bmp(char* filename, unsigned char* header, struct pixel* data);

PBM_IMG read_pbm(const char * path);
PGM_IMG read_pgm(const char * path);
PPM_IMG read_ppm(const char * path);
void write_pbm(PBM_IMG img, const char * path);
void write_pgm(PGM_IMG img, const char * path);
void write_ppm(PPM_IMG img, const char * path);
void write_vhdl_constant(PGM_IMG img, const char * path);
PGM_IMG rgb2gray(PPM_IMG color_img);
void free_pbm(PGM_IMG img);
void free_pgm(PGM_IMG img);
void free_ppm(PPM_IMG img);

//compute integral intensity of an image
void compute_integral_zeroapron(PGM_IMG *img_in,INTEGRAL_IMG *integral);
//median filter for gray-scale images
PGM_IMG medianfilter(PGM_IMG img_in);
//contrast enhancement
PGM_IMG contrast_enhancement_gray(PGM_IMG img_in);
//Threshold for gray-scale images
PGM_IMG threshold_binarization(PGM_IMG img_in);
//Connected Component Labeling
PGM_IMG  connected_component_labeling(PGM_IMG img_in,int *nb_labels,COMPONENT **ccl,int area_min,int area_max);
//gaussian filter for gray-scale images
PGM_IMG gaussianfilter(PGM_IMG img_in);
//sobel edge detection for gray-scale images
void sobel(PGM_IMG img_in,PGM_IMG sobel_pixel,PGM_IMG sobel_edge);
//canny non-maxima supppression and hysteresis thresholding for gray-scale images
PGM_IMG canny(PGM_IMG sobel_pixel,PGM_IMG sobel_edge, int threshold_low,int threshold_high);
//Hough transform : line detection
PGM_IMG hough(PGM_IMG img_in,int threshold);
//zone detector : find size*size squares
PGM_IMG zone(PGM_IMG img_in, int size);
//Hu moments : lables filtering
PGM_IMG hu_moments(PGM_IMG img_in,COMPONENT *ccl, MOMENTS *humref, int nb_components, double uncerntainty);
//morphological operator
PGM_IMG erode(PGM_IMG img_in);
PGM_IMG dilate(PGM_IMG img_in);
#endif
