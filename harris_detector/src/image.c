/*************************************************************************************
 * File   : image.c, file for face detection (Viola Jones, AdaBoost)
 *
 * Copyright (C) 2012 Lab-Sticc Laboratory
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
#include <math.h>
#include "image.h"
#include "gaussianfilter.h"
#include "sobel.h"
#include "median.h"
#include "harris.h"

#define MEDIAN_SLICING
/*  Calculate Integral Image and Square Integral Image

    The integral image is computed recursively, by the formulas: I(x,y)= I(x,y-1)+I(x-1,y)+N(x,y)-I(x-1,y-1) with I(-1,y)=I(x,-1)=I(-1,-1)=0,
        therefore requiring only one scan over the input data. */


void compute_integral_zeroapron(const PGM_IMG *img_in,INTEGRAL_IMG *integral)
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

void compute_integral(const PGM_IMG *img_in,INTEGRAL_IMG *integral)
{
    int y,x;
    int height=img_in->h;
    int width=img_in->w;
    integral->w=width;
    integral->h = height;
    int size=width * height;
    integral->value = (int32_t *)calloc(size,sizeof(int32_t));
    int32_t val;
    unsigned char pixel;

    //somme suivant les lignes
	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
            pixel=img_in->img[x+y*width];
			val = (x % width != 0) ? integral->value[width * y + x - 1] : 0;
			integral->value[width * y + x] = val + pixel;
		}
	}

    //somme suivant les colonnes
	for (x = 0; x < width; ++x) {
		for (y = 0; y < height; ++y) {
			val = (y % height != 0) ? integral->value[width * (y - 1) + x] : 0;
			integral->value[width * y + x] += val;
		}
	}
}

float integral_blur(const INTEGRAL_IMG *integral, int width, int height, int x, int y, float sigma) {
	int farX = (x + sigma < width) ? (int)(x + sigma) : (int)(width - 1);
	int nearX = (x - sigma >= 0) ? (int)(x - sigma) : 0;

	int farY = (y + sigma < height) ? (int)(y + sigma) : (int)(height - 1);
	int nearY = (y - sigma >= 0) ? (int)(y - sigma) : 0;

	float result = (float)integral->value[width * farY + farX];
	if (nearX > 0 && nearY > 0) {
		return (result - (float)integral->value[width * nearY + farX] - (float)integral->value[width * farY + nearX] + (float)integral->value[width * nearY + nearX]) / (float)((farX - nearX) * (farY - nearY));
	}
	else if (nearX > 0) {
		return result - (float)integral->value[width * farY + nearX] / (float)((farX - nearX) * (farY - nearY));
	}
	else if (nearY > 0) {
		return result - (float)integral->value[width * nearY + farX] / (float)((farX - nearX) * (farY - nearY));
	}
	else return result / (float)((farX - nearX) * (farY - nearY));
}

PGM_IMG warpimg(PGM_IMG img_in,float thetaref, int tx, int ty)
{
    PGM_IMG result;
    int y,x,newX,newY;
    unsigned char pixel;
    int height=img_in.h;
    int width=img_in.w;
    result.w = width;
    result.h = height;
    float sintheta,costheta;
    float costhetax,sinthetay,costhetay,sinthetax;
    float sinthetaxroundup,costhetayroundup;
    float costhetaxroundup,sinthetayroundup;
    result.img = (unsigned char *)calloc(width * height, sizeof(unsigned char));
    sintheta=sin(thetaref);
    costheta=cos(thetaref);
    for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
                pixel=img_in.img[x+y*width];
		costhetax=costheta*(float)x;
		costhetay=costheta*(float)y;
		sinthetay=sintheta*(float)y;
		sinthetax=sintheta*(float)x;
		costhetaxroundup=costhetax+0.5;
		sinthetayroundup=sinthetay+0.5;
		sinthetaxroundup=sinthetax+0.5;
		costhetayroundup=costhetay+0.5;
                newX = (int)(costhetaxroundup)-(int)(sinthetayroundup)+tx;
                newY = (int)(sinthetaxroundup)+(int)(costhetayroundup)+ty;
                if ((newX>0) && (newX<width) && (newY>0) && (newY<height))
                    result.img[newX+newY*width]=pixel;
                newX = (int)(costhetax)-(int)(sinthetay)+tx;
                newY = (int)(sinthetax)+(int)(costhetay)+ty;
                if ((newX>0) && (newX<width) && (newY>0) && (newY<height))
                    result.img[newX+newY*width]=pixel;
		}
    }
    return result;
}

PGM_IMG rotoZoomFast(PGM_IMG img_in,float angle, float zoom,int offset_x, int offset_y)
{
  uint32_t i,j;
  PGM_IMG result;
  float c = cos(angle);
  float s = sin(angle);
  int height=img_in.h;
  int width=img_in.w;      
  float x_c = (float)width/2.0;
  float y_c = height/2;
  
  float h11 = c /zoom;
  float h12 = -s/zoom;
  float h13 = x_c + offset_y*s - offset_x*c + s*y_c/zoom - c*x_c/zoom ;
  float h21 = s/zoom;
  float h22 =  c/zoom;
  float h23 = y_c - offset_x*s - offset_y*c - c*y_c/zoom - s*x_c/zoom;
  result.img = (unsigned char *)calloc(width * height, sizeof(unsigned char));
   result.w = width;
    result.h = height;
   unsigned char * O_Y=result.img; 
  uint32_t incr =0;
  for(i=0;i < height;i++)
  {
    float x2 = h12*(float)i +h13;
    float y2 = h22*(float)i +h23;
    for(j =0;j < width;j++)
    {
      int32_t nx2 = (int32_t)x2;
      int32_t ny2 = (int32_t)y2;
      if(nx2 >= 0 && ny2 >=0 && nx2 < width && ny2 < height)
      {
        incr = ny2*width+nx2;
        *O_Y = img_in.img[incr];
      }
      else
        *O_Y = 0;

      O_Y++;
      x2 += h11;
      y2 += h21;
    }
  } 
  return result;  
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

PGM_IMG harriscorner(PGM_IMG img_in,int threshold,keyPoint *keyPoints,int *kpcount,int sort)
{
    PGM_IMG result;
    result.w = img_in.w;
    result.h = img_in.h;
    result.img = (unsigned char *)malloc(result.w * result.h * sizeof(unsigned char));
    corner_harris(img_in.img,result.img,threshold,keyPoints,kpcount,sort);
    return result;
}


void write_vhdl_constant(PGM_IMG img, const char * path)
{
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

PBM_IMG read_pbm(const char * path){
    FILE * in_file;
    char sbuf[256];
    PBM_IMG result;
    in_file = fopen(path, "r");
    if (in_file == NULL){
        printf("Input file not found!\n");
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
        printf("Input file not found!\n");
        exit(1);
    }
    fscanf(in_file, "%s", sbuf); /*Skip the magic number*/
    //fscanf(in_file, "%s", sbuf); /*Skip the commentary*/
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
