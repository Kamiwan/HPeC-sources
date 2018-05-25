/*************************************************************************************
 * File   : canny.c, file for Vision Based Autonomous Landing
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
/* Hardware architecture, line buffer are in communication interface of HWPU : FIFO  */
/* In pure software implementation, the advantage of circular buffers is that they alleviate the need to move data samples around in the delay line */

#define CIRCULAR_BUFFER_SIZE (IMAGE_WIDTH+2+1)
static unsigned char gradient_buffer1[CIRCULAR_BUFFER_SIZE]; /* +1 Always keep one slot open : Full / Empty Buffer Distinction */
static unsigned char gradient_buffer2[CIRCULAR_BUFFER_SIZE];
static unsigned char orientation_buffer[CIRCULAR_BUFFER_SIZE];

/*
Non-maximum suppression: suppress points that ar not a local maximum along the direction of th steepest gradient.

Given estimates of the image gradients, a search is then carried out to determine if the gradient magnitude assumes a local maximum in the gradient direction. So, for example,
if the rounded gradient angle is zero degrees (i.e. the edge is in the north-south direction) the point will be considered to be on the edge if its gradient magnitude is greater than the magnitudes in the west and east directions,
if the rounded gradient angle is 90 degrees (i.e. the edge is in the east-west direction) the point will be considered to be on the edge if its gradient magnitude is greater than the magnitudes in the north and south directions,
if the rounded gradient angle is 135 degrees (i.e. the edge is in the north east-south west direction) the point will be considered to be on the edge if its gradient magnitude is greater than the magnitudes in the north west and south east directions,
if the rounded gradient angle is 45 degrees (i.e. the edge is in the north west-south east direction)the point will be considered to be on the edge if its gradient magnitude is greater than the magnitudes in the north east and south west directions.
                	     ____   ____
			    |    |_|    |
Gradient-|------------ lr --| lm | | ll |  Lr=Pixel_in
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


                  __________
   Orientation  _|   Line   |__ the Orientation must be delayed by one row to compensate for the latency
                 |  buffer  |   of getting the magnitude on the row below the current pixel.
                 |__________|

   Compare window :

		----------------------------------
		| w_ul/135 |  w_um/90  | w_ur/45  |
	 	----------------------------------
		| w_ml/0   |  w_mm     | w_mr /0  |
	 	----------------------------------
		| w_ll/45  |  w_lm/90  | w_lr/135 |
    	 	----------------------------------

       cond = (((orientation==0) && (ml<=mm) and (mr<=mm)) || ((orientation==45) && (ll<=mm) and (ur<=mm)) ||
               ((orientation==90) && (lm<=mm) and (um<=mm)) ||  ((orientation==135) && (ul<=mm) and (lr<=mm));
       out= cond ? mm : 0;
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
#define w_lr  gradient
#define w_lm  w_down[0]
#define w_ll  w_down[1]

void compare_window(unsigned char gradient,unsigned char l1, unsigned char l2,unsigned char orientation,int threshold_high,int threshold_low,unsigned char *max_out)
{
        short cond;
        short connected;
	int j;
        unsigned char pixel_out;

        cond = (((orientation==0) && (w_ml<=w_mm) && (w_mr<=w_mm)) || ((orientation==45) && (w_ll<=w_mm) && (w_ur<=w_mm)) ||
               ((orientation==90) && (w_lm<=w_mm) && (w_um<=w_mm)) ||  ((orientation==135) && (w_ul<=w_mm) && (w_lr<=w_mm)));
        connected = (w_ur> threshold_low) | (w_um> threshold_low) | (w_ul> threshold_low) | (w_mr> threshold_low) |
                    (w_ml> threshold_low) | (w_lr> threshold_low) | (w_lm> threshold_low) | (w_ll> threshold_low);
       pixel_out= cond ? w_mm : 0;
       if ((pixel_out>threshold_high) | (connected && (pixel_out>threshold_low)))
           *max_out=255;
       else
           *max_out=0;
       //shift register kernel window
       for (j=2;j>0;j--)
       {
		w_upper[j]=w_upper[j-1];
		w_middle[j]=w_middle[j-1];
        }
	w_upper[0]=l2;
	w_middle[0]=l1;
   	w_down[1]=w_down[0];
	w_down[0]=gradient;
 }

void non_maxima_supression_hysteresis_thresholding(unsigned char *gradient,unsigned char *orientation,int threshold_high,int threshold_low,unsigned char *max_out)
{
   int row, column, noapron;
   unsigned int circular_pointer_start;
   unsigned int circular_pointer_end;
   unsigned char gradient_in,orientation_in;
    //reset
   memset(orientation_buffer, 0, CIRCULAR_BUFFER_SIZE);
   memset(gradient_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(gradient_buffer2, 0, CIRCULAR_BUFFER_SIZE);
   circular_pointer_start=0;
   circular_pointer_end=0;
   for (row=0; row < IMAGE_HEIGH+2; row++)
   {
      for (column=0; column < IMAGE_WIDTH+2; column++)
      {
          unsigned char pixel_out;
	  noapron=(row>=1) && (row < IMAGE_HEIGH+1) && (column >=1) && (column < IMAGE_WIDTH+1);
	  if (noapron)
	  {	
		 gradient_in=gradient[(row-1)*IMAGE_WIDTH+(column-1)];
		 orientation_in=orientation[(row-1)*IMAGE_WIDTH+(column-1)];
	  }
	  else
	  {
		gradient_in=0;
		orientation_in=0;
	  }
	  /* store input in the delay line */
 	  gradient_buffer2[circular_pointer_end]=gradient_buffer1[circular_pointer_start]; /* push fifo */
	  gradient_buffer1[circular_pointer_end]=gradient_in;
          orientation_buffer[circular_pointer_end]=orientation_in;
	  /* incr circular pointer and check for wrap */
	  circular_pointer_end = (circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
	  if (circular_pointer_end == circular_pointer_start)
		circular_pointer_start = (circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
          compare_window(gradient_in,gradient_buffer1[circular_pointer_start],gradient_buffer2[circular_pointer_start],orientation_buffer[circular_pointer_start],threshold_high,threshold_low,&pixel_out);
          if ((row>=2) && (column>=2))
	  {
 		  max_out[(row-2)*IMAGE_WIDTH+(column-2)]=pixel_out;
	  }
      }
   }
}

