/*************************************************************************************
 * File   : nms.cpp, , file for harris corner detection
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

#define CIRCULAR_BUFFER_SIZE (LINEBUFFERSIZENMS+1)

static int nms_line_buffer1[CIRCULAR_BUFFER_SIZE]; /* +1 Always keep one slot open : Full / Empty Buffer Distinction */
static int nms_line_buffer2[CIRCULAR_BUFFER_SIZE];
static unsigned int nms_circular_pointer_start=0;
static unsigned int nms_circular_pointer_end=0;


/*
Find maximum local: suppress points that are not a local maximum

               	      	     ____   ____
			           		|    |_|    |
cim 	-|-------.---- lr --| lm | | ll |  Lr=cim
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


       cond = (ml<=mm) && (mr<=mm) && (ll<=mm) && (ur<=mm)
              && (lm<=mm) && (um<=mm) && (ul<=mm) && (lr<=mm);
       out= cond ? mm : 0;
	 */
static  int nms_w_upper[3]={0,0,0};
static  int nms_w_middle[3]={0,0,0};
static  int nms_w_down[3]={0,0,0};
#define w_ur  nms_w_upper[0]
#define w_um  nms_w_upper[1]
#define w_ul  nms_w_upper[2]
#define w_mr  nms_w_middle[0]
#define w_mm  nms_w_middle[1]
#define w_ml  nms_w_middle[2]
#define w_lr  nms_w_down[0]
#define w_lm  nms_w_down[1]
#define w_ll  nms_w_down[2]

void compare_window2(int cim,int l1, int l2,int *max_out)
{

    short cond;
    int j;

    cond =  (w_ml<=w_mm) && (w_mr<=w_mm) && (w_ll<=w_mm) && (w_ur<=w_mm) && (w_lm<=w_mm) && (w_um<=w_mm) && (w_ul<=w_mm) && (w_lr<=w_mm);
    *max_out= cond ? w_mm : 0;
    //shift register kernel window
    for (j=2;j>0;j--)
    {
    	nms_w_upper[j]=nms_w_upper[j-1];
    	nms_w_middle[j]=nms_w_middle[j-1];
        nms_w_down[j]=nms_w_down[j-1];
    }
    nms_w_upper[0]=l2;
    nms_w_middle[0]=l1;
    nms_w_down[0]=cim;
}

void nms_reset_SOF(void)
{
   nms_circular_pointer_start=0;
   nms_circular_pointer_end=0;
   memset(nms_w_upper,0,3);
   memset(nms_w_middle,0,3);
   memset(nms_w_down,0,3);
   memset(nms_line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(nms_line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
}

void non_maxima_supression(int calcim,unsigned char *max_out,int threshold)
{


    int pixel_out;

 /* store input in the delay line */
nms_line_buffer2[nms_circular_pointer_end]=nms_line_buffer1[nms_circular_pointer_start]; /* push fifo */
  nms_line_buffer1[nms_circular_pointer_end]=calcim;
  /* incr circular pointer and check for wrap */
  nms_circular_pointer_end = (nms_circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
  if (nms_circular_pointer_end == nms_circular_pointer_start)
	nms_circular_pointer_start = (nms_circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
 compare_window2(calcim,nms_line_buffer1[nms_circular_pointer_start],nms_line_buffer2[nms_circular_pointer_start],&pixel_out);
    if (pixel_out>threshold)
       *max_out=255;
    else
       *max_out=0;
}

