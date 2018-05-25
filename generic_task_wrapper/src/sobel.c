/*************************************************************************************
 * File   : sobel.c, file for Vision Based Autonomous Landing
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
#include <stdlib.h>
#include <string.h>
#include "image.h"
/* first architecture line buffer is Circular Buffer  */

typedef unsigned char image_apron_t[IMAGE_HEIGH+2][IMAGE_WIDTH+2];
static image_apron_t apron_image_in;
#define CIRCULAR_BUFFER_SIZE (IMAGE_WIDTH+2+1)
static unsigned char line_buffer1[CIRCULAR_BUFFER_SIZE]; /* +1 Always keep one slot open : Full / Empty Buffer Distinction */
static unsigned char line_buffer2[CIRCULAR_BUFFER_SIZE];

/* TILING-2 must be a multiple of pgcd of IMAGE_HEIGH,IMAGE_WIDTH 640/16=40 et 480/16=30 */
#define  TILING 18
typedef unsigned char tiling_t[TILING][TILING];
typedef unsigned char tiling_noborder_t[TILING-2][TILING-2];
typedef short tiling_short_noborder_t[TILING-2][TILING-2];

#define APRON_PIXEL 0
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


	  pixel_out = (|Gx|+|Gy|)/4

	  edge_out

			if  |Gx| > 2 |Gy| then edge_out = 0
			else if |Gy| > 2 |Gx| then edge_out = 90
			else if sign(Gx)==sign(Gy) then edge_out = 45
			else edge_out = 135

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

short sameSignFastshort(short x, short y)
 {
   // XOR both numbers
   short temp = x^y;
   // shift sign bit to the lowest bit position and clear the rest
   temp >>= 15; // 32 bit: use 31 instead of 15
   temp  &=  1;
   // negate
   return !temp;
}

 void sobel_slicing_kernel(unsigned char pixel_in,unsigned char l1, unsigned char l2,unsigned char *pixel_out,unsigned char *edge_out)
{
        int j;
	short Sum,Gx,Gy;
	short absGx,absGy;
	unsigned char edge_orientation;
	Gx=(w_ur+(w_mr<<1)+w_lr)-(w_ul+(w_ml<<1)+w_ll);
	Gy=(w_ll+(w_lm<<1)+w_lr)-(w_ul+(w_um<<1)+w_ur);
	if (Gx<0) absGx=-Gx; else absGx=Gx;
	if (Gy<0) absGy=-Gy; else absGy=Gy;
	Sum = (absGx+absGy)>>1;
        *pixel_out= (Sum < 0) ? 0 : ((Sum > 255) ? 255 : Sum) ;
	if (absGx>(absGy<<1)) edge_orientation=0;
	else if  (absGy>(absGx<<1)) edge_orientation=90;
	else if (sameSignFastshort(Gx,Gy)) edge_orientation=45;
	else edge_orientation=135;
	*edge_out=edge_orientation;
	//shift register kernel window: for GAUT aging vector at the end of algorithm
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

void sobel_slicing(unsigned char * img_in,unsigned char * img_out,unsigned char * edge_out)
{
   int row, column;
   unsigned int circular_pointer_start;
   unsigned int circular_pointer_end;
   //reset
   memset(line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
   circular_pointer_start=0;
   circular_pointer_end=0;
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
          unsigned char pixel_out,orientation_out;
          /* store input in the delay line */
	  line_buffer2[circular_pointer_end]=line_buffer1[circular_pointer_start]; /* push fifo */
	  line_buffer1[circular_pointer_end]=apron_image_in[row][column];
	  /* incr circular pointer and check for wrap */
	  circular_pointer_end = (circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
	  if (circular_pointer_end == circular_pointer_start)
		circular_pointer_start = (circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
	  sobel_slicing_kernel(apron_image_in[row][column],line_buffer1[circular_pointer_start],line_buffer2[circular_pointer_start],&pixel_out,&orientation_out);
          if ((row>=2) && (column>=2))
	  {
             img_out[(row-2)*IMAGE_WIDTH+(column-2)]=pixel_out;
	     edge_out[(row-2)*IMAGE_WIDTH+(column-2)]=orientation_out;
	  }
      }
   }
}

/*  Sobel Edge detection with Tiling Approach
	______________________________________
	|
	| A  _______________
	| P |				|
	| R |		        |
	| O |		        |
	| N |	TILE	    |
	|   |		        |
	|   |		        |
	|   |_______________|
	|
	|
	|
	|
	_______________________________________
*/
void gradient(unsigned char a1,unsigned char a2,unsigned char a3,unsigned char a4,unsigned char a5,unsigned char a6, short *output )
{
    *output = ((a4+(a5<<1)+a6) - (a1+(a2<<1)+a3));
}

void absVal(short x, short y, unsigned char *output, unsigned char *edge )
{
    short sum;
    short abs_x,abs_y;
	unsigned char edge_orientation;

    if (x<0) abs_x=-x;
    else abs_x=x;
    if (y<0) abs_y=-y;
    else abs_y=y;

	if (abs_x>(abs_y<<1)) edge_orientation=0;
	else if  (abs_y>(abs_x<<1)) edge_orientation=90;
	else if (sameSignFastshort(x,y)) edge_orientation=45;
	else edge_orientation=135;

	*edge=edge_orientation;

    sum = (abs_x+abs_y)>>2;
    if (sum<0)
	*output =0;
    else if (sum>255)
 	*output =255;
    else
	*output = (unsigned char) sum;
}

void compute_sobel_tiling(tiling_t image, tiling_noborder_t av,tiling_noborder_t orientation)
{
   int i, j;

   tiling_short_noborder_t Jx;
   tiling_short_noborder_t Jy;

   //we work only on a tiling-2 window (apron)
   for (j=1; j < TILING-1; j++) {
      for (i=1; i < TILING-1; i++) {
           gradient( image[j-1][i-1], image[j][i-1], image[j+1][i-1], image[j-1][i+1], image[j][i+1], image[j+1][i+1], &Jx[j-1][i-1] );
      }
   }

   for (j=1; j < TILING-1; j++) {
      for (i=1; i < TILING-1; i++) {
          gradient( image[j-1][i-1], image[j-1][i], image[j-1][i+1], image[j+1][i-1], image[j+1][i], image[j+1][i+1], &Jy[j-1][i-1] );
      }
   }

   for (j=0; j < TILING-2; j++) {
      for (i=0; i < TILING-2; i++) {
           absVal( Jx[j][i], Jy[j][i], &av[j][i], &orientation[j][i] );
      }
   }
}


#define ROW_APRON (IMAGE_HEIGH+2)
#define COLUMN_APRON (IMAGE_WIDTH+2)

void sobel_tiling(unsigned char * img_in,unsigned char * img_out, unsigned char * edge_out)
{
   int row, column, trow, tcol;
   //add apron 
   for(row=0; row < ROW_APRON; ++row)
   {
		apron_image_in[row][0]=apron_image_in[row][1]=apron_image_in[row][COLUMN_APRON-1]=apron_image_in[row][COLUMN_APRON-2]=APRON_PIXEL;
   }
   for(column=0; column < COLUMN_APRON; ++column)
   {
		apron_image_in[0][column]=apron_image_in[1][column]=apron_image_in[ROW_APRON-1][column]=apron_image_in[ROW_APRON-2][column]=APRON_PIXEL;
   }

   for(row=0; row < IMAGE_HEIGH; ++row)
	   for(column=0; column < IMAGE_WIDTH; ++column)
			apron_image_in[row+2][column+2]=img_in[row*IMAGE_WIDTH+column];
   for (trow=0;trow<IMAGE_HEIGH;trow+=TILING-2)
     for (tcol=0; tcol < IMAGE_WIDTH; tcol+=TILING-2)
     {
       tiling_t tiling_sobel_in;
       tiling_noborder_t tiling_sobel_out,tiling_edge_out;
       for (row=0; row < TILING; row++)
          for (column=0; column < TILING; column++)
                tiling_sobel_in[row][column]=apron_image_in[row+trow][column+tcol];
       compute_sobel_tiling(tiling_sobel_in,tiling_sobel_out,tiling_edge_out);
       for (row=1; row < TILING-1; row++)
          for (column=1; column < TILING-1; column++)
	  {
               img_out[(row+trow-1)*IMAGE_WIDTH+(column+tcol-1)]=tiling_sobel_out[row-1][column-1];
               edge_out[(row+trow-1)*IMAGE_WIDTH+(column+tcol-1)]=tiling_edge_out[row-1][column-1];
	  }
     }
}
