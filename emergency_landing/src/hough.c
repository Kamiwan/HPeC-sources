/*************************************************************************************
 * File   : hough.c, file for Vision Based Autonomous Landing
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cordic_atan.h"
#include "hough.h"
#include "image.h"
#include "math.h"
#include "stdint.h"
#define image(a,b) img_in[(a)*IMAGE_WIDTH+b] 
//max_rho = ceil(sqrt(640^2 + 480^2));
#define X_ORIGIN 320
#define Y_ORIGIN 240
#define MAX_RHO_ABS 800
#define MAX_RHO 800*2
#define MAX_THETA 181
#define APRON_PIXEL 0
#define PI 3.14159265358979323846

/* first architecture line buffer is Circular Buffer  */

typedef unsigned char image_apron_t[IMAGE_HEIGH+2][IMAGE_WIDTH+2];
static image_apron_t apron_image_in;
#define CIRCULAR_BUFFER_SIZE (IMAGE_WIDTH+2+1)
static unsigned char line_buffer1[CIRCULAR_BUFFER_SIZE]; /* +1 Always keep one slot open : Full / Empty Buffer Distinction */
static unsigned char line_buffer2[CIRCULAR_BUFFER_SIZE];


void create_LUT_table()
{
	int i;
	double theta;
	printf("double cos_lut[180]={%f",cos(0));
	for (i=1;i<180;i++)
	{
		theta=i*PI/180;
		printf(",%f",cos(theta));
		if (i % 4==0) printf("\r\n");
	}
	printf("};\r\n");
	printf("double sin_lut[180]={%f",sin(0));
	for (i=1;i<180;i++)
	{
		theta=i*PI/180;
		printf(",%f",sin(theta));
		if (i % 4==0) printf("\r\n");
	}
	printf("};\r\n");


}


double cos_lut[181]={1.000000,0.999848,0.999391,0.998630,0.997564
,0.996195,0.994522,0.992546,0.990268
,0.987688,0.984808,0.981627,0.978148
,0.974370,0.970296,0.965926,0.961262
,0.956305,0.951057,0.945519,0.939693
,0.933580,0.927184,0.920505,0.913545
,0.906308,0.898794,0.891007,0.882948
,0.874620,0.866025,0.857167,0.848048
,0.838671,0.829038,0.819152,0.809017
,0.798636,0.788011,0.777146,0.766044
,0.754710,0.743145,0.731354,0.719340
,0.707107,0.694658,0.681998,0.669131
,0.656059,0.642788,0.629320,0.615661
,0.601815,0.587785,0.573576,0.559193
,0.544639,0.529919,0.515038,0.500000
,0.484810,0.469472,0.453990,0.438371
,0.422618,0.406737,0.390731,0.374607
,0.358368,0.342020,0.325568,0.309017
,0.292372,0.275637,0.258819,0.241922
,0.224951,0.207912,0.190809,0.173648
,0.156434,0.139173,0.121869,0.104528
,0.087156,0.069756,0.052336,0.034899
,0.017452,0.000000,-0.017452,-0.034899
,-0.052336,-0.069756,-0.087156,-0.104528
,-0.121869,-0.139173,-0.156434,-0.173648
,-0.190809,-0.207912,-0.224951,-0.241922
,-0.258819,-0.275637,-0.292372,-0.309017
,-0.325568,-0.342020,-0.358368,-0.374607
,-0.390731,-0.406737,-0.422618,-0.438371
,-0.453990,-0.469472,-0.484810,-0.500000
,-0.515038,-0.529919,-0.544639,-0.559193
,-0.573576,-0.587785,-0.601815,-0.615661
,-0.629320,-0.642788,-0.656059,-0.669131
,-0.681998,-0.694658,-0.707107,-0.719340
,-0.731354,-0.743145,-0.754710,-0.766044
,-0.777146,-0.788011,-0.798636,-0.809017
,-0.819152,-0.829038,-0.838671,-0.848048
,-0.857167,-0.866025,-0.874620,-0.882948
,-0.891007,-0.898794,-0.906308,-0.913545
,-0.920505,-0.927184,-0.933580,-0.939693
,-0.945519,-0.951057,-0.956305,-0.961262
,-0.965926,-0.970296,-0.974370,-0.978148
,-0.981627,-0.984808,-0.987688,-0.990268
,-0.992546,-0.994522,-0.996195,-0.997564
,-0.998630,-0.999391,-0.999848,-1.000000};

double sin_lut[181]={0.000000,0.017452,0.034899,0.052336,0.069756
,0.087156,0.104528,0.121869,0.139173
,0.156434,0.173648,0.190809,0.207912
,0.224951,0.241922,0.258819,0.275637
,0.292372,0.309017,0.325568,0.342020
,0.358368,0.374607,0.390731,0.406737
,0.422618,0.438371,0.453990,0.469472
,0.484810,0.500000,0.515038,0.529919
,0.544639,0.559193,0.573576,0.587785
,0.601815,0.615661,0.629320,0.642788
,0.656059,0.669131,0.681998,0.694658
,0.707107,0.719340,0.731354,0.743145
,0.754710,0.766044,0.777146,0.788011
,0.798636,0.809017,0.819152,0.829038
,0.838671,0.848048,0.857167,0.866025
,0.874620,0.882948,0.891007,0.898794
,0.906308,0.913545,0.920505,0.927184
,0.933580,0.939693,0.945519,0.951057
,0.956305,0.961262,0.965926,0.970296
,0.974370,0.978148,0.981627,0.984808
,0.987688,0.990268,0.992546,0.994522
,0.996195,0.997564,0.998630,0.999391
,0.999848,1.000000,0.999848,0.999391
,0.998630,0.997564,0.996195,0.994522
,0.992546,0.990268,0.987688,0.984808
,0.981627,0.978148,0.974370,0.970296
,0.965926,0.961262,0.956305,0.951057
,0.945519,0.939693,0.933580,0.927184
,0.920505,0.913545,0.906308,0.898794
,0.891007,0.882948,0.874620,0.866025
,0.857167,0.848048,0.838671,0.829038
,0.819152,0.809017,0.798636,0.788011
,0.777146,0.766044,0.754710,0.743145
,0.731354,0.719340,0.707107,0.694658
,0.681998,0.669131,0.656059,0.642788
,0.629320,0.615661,0.601815,0.587785
,0.573576,0.559193,0.544639,0.529919
,0.515038,0.500000,0.484810,0.469472
,0.453990,0.438371,0.422618,0.406737
,0.390731,0.374607,0.358368,0.342020
,0.325568,0.309017,0.292372,0.275637
,0.258819,0.241922,0.224951,0.207912
,0.190809,0.173648,0.156434,0.139173
,0.121869,0.104528,0.087156,0.069756
,0.052336,0.034899,0.017452,0.000000};


/*	        	     ____   ____
			    |    |_|    |
Pixelin -|------------ lr --| lm | | ll |  Lr=Pixel_in
         |                  |____| |____|
 ________|
|   __________        ____   ____   ____
|__|   Line   |__l1__|    |_|    |_|    |
   |  buffer  | |    | mr | | mm | | ml |
   |__________|	|    |____| |____| |____|
 _______________|
|   __________        ____   ____   ____
|__|   Line   |__l2__|    |_|    |_|    |
   |  buffer  |      | ur | | um | | ul |
   |__________|	     |____| |____| |____|


  KERNEL window :

		-------------------------
		| w_ul |  w_um  | w_ur  |
	 	-------------------------
		| w_ml |  w_mm  | w_mr  |
	 	-------------------------
		| w_ll |  w_lm  | w_lr  |
	 	-------------------------

	  Gx=(w_ur+(w_mr<<1)+w_lr)-(w_ul+(w_ml<<1)+w_ll)
	  Gy=(w_ll+(w_lm<<1)+w_lr)-(w_ul+(w_um<<1)+w_ur)
	  gradient_out = |Gx|+|Gy|
	  edge_out = Atan(gx/gy);

	 */
static  unsigned char w_upper[3]={0,0,0};
static  unsigned char w_middle[3]={0,0,0};
static  unsigned char w_down[2]={0,0};
#define w_ur  w_upper[0]
#define w_um  w_upper[1]
#define w_ul  w_upper[2]
#define w_mr  w_middle[0]
#define w_mm  w_middle[1]
#define w_ml  w_middle[2]
#define w_lr  pixel_in
#define w_lm  w_down[0]
#define w_ll  w_down[1]

void gradient_slicing_kernel(unsigned char pixel_in,unsigned char l1, unsigned char l2,unsigned char *gradient_out,double *edge_out)
{
        int j;
	short Gx,Gy;
	short absGx,absGy;
	double  edge_orientation=0;
	Gx=(w_ur+(w_mr<<1)+w_lr)-(w_ul+(w_ml<<1)+w_ll);
	Gy=(w_ll+(w_lm<<1)+w_lr)-(w_ul+(w_um<<1)+w_ur);

	if (Gx<0) absGx=-Gx; else absGx=Gx;
	if (Gy<0) absGy=-Gy; else absGy=Gy;
        *gradient_out= (absGx+absGy)>>2;
	*edge_out=fix2float(cordic_atan(Gx,Gy));
	//shift register kernel window
	for (j=2;j>0;j--)
        {
		w_upper[j]=w_upper[j-1];
		w_middle[j]=w_middle[j-1];
        }
	w_upper[0]=l2;
	w_middle[0]=l1;
   	w_down[1]=w_down[0];
	w_down[0]=pixel_in;
 }


void hough_slicing(unsigned char * img_in,unsigned char * img_out,int threshold)
{
   int row, column;
   unsigned int circular_pointer_start;
   unsigned int circular_pointer_end;
   unsigned int vote[MAX_THETA+1][MAX_RHO+1];
   int theta_idx,rho_idx;
   //reset
   memset(line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
   circular_pointer_start=0;
   circular_pointer_end=0;
   memset(vote, 0, (MAX_THETA+1)*(MAX_RHO+1)*sizeof(unsigned int));
   //add apron
   for (column=0; column < IMAGE_WIDTH+2; column++) {
	apron_image_in[0][column]=apron_image_in[IMAGE_HEIGH+1][column]=APRON_PIXEL;
   }
   for (row=0; row < IMAGE_HEIGH+2; row++) {
	apron_image_in[row][0]=apron_image_in[row][IMAGE_WIDTH+1]=APRON_PIXEL;
   }
   for (row=1; row < IMAGE_HEIGH+1; row++) {
	   for (column=1; column < IMAGE_WIDTH+1; column++) {
         apron_image_in[row][column]=img_in[(row-1)*IMAGE_WIDTH+(column-1)];
      }
   }
   for (row=0; row < IMAGE_HEIGH+2; row++)
   {
      for (column=0; column < IMAGE_WIDTH+2; column++)
      {
          unsigned char gradient_out;
	  double theta,rho;
          /* store input in the delay line */
	  line_buffer2[circular_pointer_end]=line_buffer1[circular_pointer_start]; /* push fifo */
	  line_buffer1[circular_pointer_end]=apron_image_in[row][column];
	  /* incr circular pointer and check for wrap */
	  circular_pointer_end = (circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
	  if (circular_pointer_end == circular_pointer_start)
		circular_pointer_start = (circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
	  gradient_slicing_kernel(apron_image_in[row][column],line_buffer1[circular_pointer_start],line_buffer2[circular_pointer_start],&gradient_out,&theta);
          if ((row>=2) && (column>=2))
	  {
	     if (gradient_out!=0)
	     {
		theta_idx=(int)((theta*180/PI) /* +0.5 */);
		if (theta_idx<0) 
		{
			theta_idx+=180;
			rho=-((double)(row-2-Y_ORIGIN))*cos_lut[theta_idx]-((double)(column-2-X_ORIGIN))*sin_lut[theta_idx];
		}
		else
			rho=((double)(row-2-Y_ORIGIN))*cos_lut[theta_idx]+((double)(column-2-X_ORIGIN))*sin_lut[theta_idx];
		rho_idx=(int)(rho /* +0.5*/)+MAX_RHO_ABS;
	     	vote[theta_idx][rho_idx]+=gradient_out;
	     	printf("x %d y %d gradient %d theta %f rho %f theta_idx %d rho_idx %d vote %d\r\n",(row-2),(column-2),gradient_out,theta,rho,theta_idx,rho_idx,vote[theta_idx][rho_idx]);
	     }
	     //img_out[(row-2)*IMAGE_WIDTH+(column-2)]=gradient_out;
	     img_out[(row-2)*IMAGE_WIDTH+(column-2)]=img_in[(row-2)*IMAGE_WIDTH+(column-2)];
	  }
      }
   }
   for (theta_idx=0; theta_idx < MAX_THETA; theta_idx++)
   {
      int max_value_theta=0;	
      int max_value_rho_idx=0;	
      for (rho_idx=0; rho_idx < MAX_RHO; rho_idx++)
      {
	if (vote[theta_idx][rho_idx]>max_value_theta)
	{
		max_value_theta=vote[theta_idx][rho_idx];
		max_value_rho_idx=rho_idx;
	}
      }
      if (max_value_theta>threshold)
      {
	int theta=theta_idx;
	max_value_rho_idx=max_value_rho_idx-MAX_RHO_ABS;
	printf("line theta %d rho %d max-value %d\r\n",theta,max_value_rho_idx,max_value_theta);
	//horintal line y=rho+y_origi,
	if (theta==0)
	{
      	   for (column=0; column < IMAGE_WIDTH; column++)
			img_out[column+IMAGE_WIDTH*(max_value_rho_idx+Y_ORIGIN)]=255;
 
	}
	//horintal line x=rho+x_origin
	else if (theta==90)
	{
      	   for (row=0; row < IMAGE_HEIGH; row++)
			img_out[(row)*IMAGE_WIDTH+(max_value_rho_idx+X_ORIGIN)]=255;
		
	}
	else
	if ((theta >= 135) || (theta <= 45)) 
	{ 
	   /* draw line y = (rho- cos(theta)x)/sin(theta) 
	   	with center origin 
		 draw line y = (rho- cos(theta)(x-X_ORIGIN))/sin(theta) + Y_ORIGIN*/
      	   for (column=0; column < IMAGE_WIDTH; column++)
	   {
		row = (max_value_rho_idx-cos_lut[theta_idx]*(column-X_ORIGIN))/sin_lut[theta_idx]+Y_ORIGIN;
		if ((row>0) && (row<IMAGE_HEIGH))
			img_out[(row)*IMAGE_WIDTH+column]=255;
	   }
	}
	else /* theta >45 && theta <135 */
	{
	   /* draw line x = (rho- sin(theta)y)/cos(theta) */
      	   for (row=0; row < IMAGE_HEIGH; row++)
	   {
		column = (max_value_rho_idx-sin_lut[theta_idx]*(row-Y_ORIGIN))/cos_lut[theta_idx]+X_ORIGIN;
		if ((column>0) && (column<IMAGE_WIDTH))
			img_out[(row)*IMAGE_WIDTH+column]=255;
	   }
	} 
	
      }
   }
   
}


