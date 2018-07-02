/*************************************************************************************
 * File   : sobel.c, file for Vision Based Autonomous Landing
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
#include <stdlib.h>
#include <string.h>
#include "image.h"
/* first architecture line buffer is Circular Buffer  */

#define CIRCULAR_BUFFER_SIZE (LINEBUFFERSIZESOBEL+1)

static unsigned char sobel_line_buffer1[CIRCULAR_BUFFER_SIZE]; /* +1 Always keep one slot open : Full / Empty Buffer Distinction */
static unsigned char sobel_line_buffer2[CIRCULAR_BUFFER_SIZE];
static unsigned int sobel_circular_pointer_start=0;
static unsigned int sobel_circular_pointer_end=0;
 
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
static  unsigned char sobel_w_upper[3]={0,0,0};
static  unsigned char sobel_w_middle[3]={0,0,0};
static  unsigned char sobel_w_down[2]={0,0};
#define w_ur  sobel_w_upper[0]
#define w_um  sobel_w_upper[1]
#define w_ul  sobel_w_upper[2]
#define w_mr  sobel_w_middle[0]
#define w_mm  sobel_w_middle[1]
#define w_ml  sobel_w_middle[2]
#define w_lr  pixel_in
#define w_lm  sobel_w_down[0]
#define w_ll  sobel_w_down[1]

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

 void sobel_slicing_kernel(unsigned char pixel_in,unsigned char l1, unsigned char l2,unsigned char *pixel_out,unsigned char *edge_out,short *gix,short *giy)
{
        int j;
	short Sum,Gx,Gy;
	short absGx,absGy;
	unsigned char edge_orientation;
	Gx=(w_ur+(w_mr<<1)+w_lr)-(w_ul+(w_ml<<1)+w_ll);
	Gy=(w_ll+(w_lm<<1)+w_lr)-(w_ul+(w_um<<1)+w_ur);
	*gix=Gx>>3; //normalize 1/8
	*giy=Gy>>3;//normalize 1/8
	if (Gx<0) absGx=-Gx; else absGx=Gx;
	if (Gy<0) absGy=-Gy; else absGy=Gy;
	Sum = (absGx+absGy)>>1;
        *pixel_out= (Sum < 0) ? 0 : ((Sum > 255) ? 255 : Sum) ;
	if (absGx>(absGy<<1)) edge_orientation=0;
	else if  (absGy>(absGx<<1)) edge_orientation=90;
	else if (sameSignFastshort(Gx,Gy)) edge_orientation=45;
	else edge_orientation=135;
	*edge_out=edge_orientation;
	//shift register kernel window
	for (j=2;j>0;j--)
        {
		sobel_w_upper[j]=sobel_w_upper[j-1];
		sobel_w_middle[j]=sobel_w_middle[j-1];
        }
	sobel_w_upper[0]=l2;
	sobel_w_middle[0]=l1;
   	sobel_w_down[1]=sobel_w_down[0];
	sobel_w_down[0]=pixel_in;
 }

void sobel_reset_SOF(void)
{
   sobel_circular_pointer_start=0;
   sobel_circular_pointer_end=0;
   memset(sobel_w_upper,0,3);
   memset(sobel_w_middle,0,3);
   memset(sobel_w_down,0,2);
   memset(sobel_line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(sobel_line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
}

void sobel_slicing(unsigned char pixel_in,unsigned char * pixel_out,unsigned char * edge_out,short *gx,short *gy)
{
  /* store input in the delay line */
  sobel_line_buffer2[sobel_circular_pointer_end]=sobel_line_buffer1[sobel_circular_pointer_start]; /* push fifo */
  sobel_line_buffer1[sobel_circular_pointer_end]=pixel_in;
  /* incr circular pointer and check for wrap */
  sobel_circular_pointer_end = (sobel_circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
  if (sobel_circular_pointer_end == sobel_circular_pointer_start)
	sobel_circular_pointer_start = (sobel_circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
  sobel_slicing_kernel(pixel_in,sobel_line_buffer1[sobel_circular_pointer_start],sobel_line_buffer2[sobel_circular_pointer_start],pixel_out,edge_out,gx,gy);
}


