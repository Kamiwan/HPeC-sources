/*************************************************************************************
 * File   : image.c, file for Vision Based Autonomous Landing
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "canny.h"
#include "gaussianfilter.h"
#include "histequalization.h"
#include "hough.h"
#include "image.h"
#include "median.h"
#include "morpho.h"
#include "sobel.h"
#include "threshold.h"
#include "zone.h"
#define MEDIAN_SLICING



// Read BMP file and extract the pixel values (store in data) and header (store in header)
// data is data[0] = BLUE, data[1] = GREEN, data[2] = RED, etc...

int read_bmp(char* filename, unsigned char** header, struct pixel** data)
{
    // int i;
    FILE* file = fopen(filename, "rb");
	
	if (!file) return -1;
	
	// read the 54-byte header
	unsigned char * header_ = (unsigned char *) malloc(54*sizeof(unsigned char));
    fread(header_, sizeof(unsigned char), 54, file); 

    // get height and width of image
    int width = *(int*)&header_[18];
    int height = *(int*)&header_[22];

	// Read in the image
    int size = width * height;
    struct pixel * data_ = (struct pixel *)malloc(size*sizeof(struct pixel)); 
    fread(data_, sizeof(struct pixel), size, file); // read the rest of the data at once
    fclose(file);
	
	*header = header_;
	*data = data_;
	
	return 0;
}

void write_bmp(char* filename, unsigned char* header, struct pixel* data)
{
    FILE* file = fopen(filename, "wb");
	
	// write the 54-byte header
    fwrite(header, sizeof(unsigned char), 54, file); 

    // extract image height and width from header
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    int size = width * height;
    fwrite(data, sizeof(struct pixel), size, file); // read the rest of the data at once
    fclose(file);
}

/*  Calculate Integral Image and Square Integral Image

    The integral image is computed recursively, by the formulas: I(x,y)= I(x,y-1)+I(x-1,y)+N(x,y)-I(x-1,y-1) with I(-1,y)=I(x,-1)=I(-1,-1)=0,
        therefore requiring only one scan over the input data. */


void compute_integral_zeroapron(PGM_IMG *img_in,INTEGRAL_IMG *integral)
{
    int y,x;
    int height=img_in->h+1;
    int width=img_in->w+1;
    int width_in=img_in->w;
    int size=width * height;
    integral->w=width;
    integral->h = height;
    integral->value = (int32_t *)calloc(size,sizeof(int32_t));
    for (y=1;y<height;++y)
    {
	for (x=1;x<width;++x)
        {
		unsigned char pixel=img_in->img[(x-1)+(y-1)*width_in];
        	integral->value[x+y*width] = integral->value[x+(y-1)*width] + integral->value[(x-1)+y*width] + pixel - integral->value[(x-1)+(y-1)*width];
        }
    }
}


PGM_IMG medianfilter(PGM_IMG img_in)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    #ifdef MEDIAN_SLICING
         median_slicing(img_in.img,result.img);
    #else
         median_histo(img_in.img,result.img);
    #endif
    return result;
}

PGM_IMG contrast_enhancement_gray(PGM_IMG img_in)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    histogram_equalization(result.img,img_in.img,result.w*result.h);
    return result;
}

PGM_IMG threshold_binarization(PGM_IMG img_in)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    threshold2(img_in.img,result.img,IMAGE_SIZE);
    return result;
}

PGM_IMG  connected_component_labeling(PGM_IMG img_in,int *nb_labels,COMPONENT **ccl,int area_min,int area_max)
{
    PGM_IMG  result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    *ccl=connected_components_analysis(img_in.img,result.img,area_min,area_max,nb_labels);
    return result;
}


PGM_IMG hough(PGM_IMG img_in,int threshold)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    hough_slicing(img_in.img,result.img,threshold);
    return result;
}


PGM_IMG gaussianfilter(PGM_IMG img_in)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
	//debug_ref gaussianVeryFast2DSeparable_Apron(img_in.img,result.img);
	#ifdef GAUSSIAN_TILING
		gaussianVeryFast2DSeparable_Apron_Tiling(img_in.img,result.img);
	#else
		gaussianSlidingWindown_PixelStreaming(img_in.img,result.img);
	#endif
    return result;
}


PGM_IMG erode(PGM_IMG img_in)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    erode_slicing(img_in.img,result.img);
    return result;
}


PGM_IMG dilate(PGM_IMG img_in)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    dilate_slicing(img_in.img,result.img);
    return result;
}


PGM_IMG zone(PGM_IMG img_in, int size)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    zone_detecting(img_in.img,result.img, size);
    return result;
}

void sobel(PGM_IMG img_in,PGM_IMG sobel_pixel,PGM_IMG sobel_edge)
{
	#ifdef SOBEL_TILING
		sobel_tiling(img_in.img,sobel_pixel.img,sobel_edge.img);
	#else
		sobel_slicing(img_in.img,sobel_pixel.img,sobel_edge.img);
	#endif
}

PGM_IMG hu_moments(PGM_IMG img_in,COMPONENT *ccl, MOMENTS *humref, int nb_components, double uncerntainty)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    moments(img_in.img,result.img ,ccl,humref,nb_components,uncerntainty);
    return result;
}

PGM_IMG canny(PGM_IMG sobel_pixel,PGM_IMG sobel_edge,int threshold_low, int threshold_high)
{
    PGM_IMG result;
    result.w = sobel_pixel.w;
    result.h = sobel_pixel.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    non_maxima_supression_hysteresis_thresholding(sobel_pixel.img,sobel_edge.img,threshold_high,threshold_low,result.img);
    return result;
}

PBM_IMG read_pbm(const char * path){
    FILE * in_file;
    char sbuf[256];
    PBM_IMG result;
    int v_max;//, i;
    in_file = fopen(path, "r");
    if (in_file == NULL){
        printf("Input file not found! DE GENERIC TASK WRAPPER\n");
        exit(1);
    }
    fscanf(in_file, "%s", sbuf); /*Skip the magic number P4*/
    fscanf(in_file, "%d",&result.w);
    fscanf(in_file, "%d",&result.h);
    printf("Image size: %d x %d\n", result.w, result.h);
    result.img = (unsigned char *)malloc(result.w * result.h);
    fread(result.img,sizeof(unsigned char), (result.w*result.h)/sizeof(unsigned char), in_file); //bit are group in byte
    fclose(in_file);

    return result;
}


PPM_IMG read_ppm(const char * path)
{
	FILE * in_file;
    char sbuf[256];
    PPM_IMG color_img;

    int v_max;

    in_file = fopen(path, "rb");
    if (in_file == NULL){
        printf("Input file not found! DE GENERIC TASK WRAPPER\n");
        exit(1);
    }
    fscanf(in_file, "%s", sbuf); /*Skip the magic number*/
    //printf("Magic Number: %s\n", sbuf);
    fscanf(in_file, "%d",&color_img.w);
    fscanf(in_file, "%d",&color_img.h);
    fscanf(in_file, "%d\n",&v_max);
    //printf("V Max: %d\n", v_max);
    printf("Image size: %d x %d\n", color_img.w, color_img.h);
    color_img.img = (unsigned char *)malloc(3*color_img.w * color_img.h * sizeof(unsigned char));
    fread(color_img.img,sizeof(unsigned char), 3*color_img.w*color_img.h* sizeof(unsigned char), in_file);
    fclose(in_file);
    return color_img;

 }

PGM_IMG rgb2gray(PPM_IMG color_img)
{
    PGM_IMG result;
    int image_size;
    int i,j;
    unsigned char r,g,b;
    int luminance;
    image_size=color_img.w*color_img.h;
    result.w = color_img.w;
    result.h = color_img.h;
    result.img = (unsigned char *)malloc(image_size * sizeof(unsigned char));
    for(i = 0,j=0; i < image_size; i ++,j+=3)
    {
    	r=color_img.img[j];
	g=color_img.img[j+1];
	b=color_img.img[j+2];
	luminance = ((r*77)+(g*151)+(b*28))>>8;
	result.img[i]=(unsigned char)luminance;
    }
    return result;
}

void write_pgm(PGM_IMG img, const char * path){
    FILE * out_file;
    out_file = fopen(path, "wb");
    fprintf(out_file, "P5\n");
    fprintf(out_file, "%d %d\n255\n",img.w, img.h);
    fwrite(img.img,sizeof(unsigned char), img.w*img.h, out_file);
    fclose(out_file);
}


void write_vhdl_constant(PGM_IMG img, const char * path){
    FILE * out_file;
    int i;

    out_file = fopen(path, "wb");
    fprintf(out_file,"constant IMG: res_img(0 to %d) := (x\"%02x\"",img.w*img.h,img.img[0]);
    for (i = 1; i < img.w*img.h; i++)
    {
	fprintf(out_file,",x\"%02x\"",img.img[i]);
	if (i % 16==0) fprintf(out_file,"\r\n");
    }	
    fprintf(out_file,");\r\n");
 
    fclose(out_file);
}


void write_ppm(PPM_IMG img, const char * path){
    FILE * out_file;
    out_file = fopen(path, "wb");
    fprintf(out_file, "P6\n");
    fprintf(out_file, "%d %d\n255\n",img.w, img.h);
    fwrite(img.img,sizeof(unsigned char), 3*img.w*img.h, out_file);
    fclose(out_file);
}

void free_pgm(PGM_IMG img)
{
    free(img.img);
}

void free_ppm(PPM_IMG img)
{
    free(img.img);
}
