/*************************************************************************************
 * File   : sobel.c, file for Site Landing Research
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

	 erode=w_ml and w_mm and w_mr and w_lm and w_um
	 dilate=w_ml and w_mm and w_mr and w_lm and w_um

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


void erode_kernel(unsigned char pixel_in,unsigned char l1, unsigned char l2,unsigned char *pixel_out)
{
	int j,cond;
	
        cond=(w_ml==255) && (w_mm==255) && (w_mr==255) && (w_lm==255) && (w_um==255);
	*pixel_out=cond ? 255 : 0;
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

void dilate_kernel(unsigned char pixel_in,unsigned char l1, unsigned char l2,unsigned char *pixel_out)
{
	int j;
	int cond;
	
        cond=(w_ml==255) || (w_mm==255) || (w_mr==255) || (w_lm==255) || (w_um==255);
	*pixel_out=cond ? 255 : 0;
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


void erode_slicing(unsigned char * img_in,unsigned char * img_out)
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
          unsigned char pixel_out;
          /* store input in the delay line */
	  line_buffer2[circular_pointer_end]=line_buffer1[circular_pointer_start]; /* push fifo */
	  line_buffer1[circular_pointer_end]=apron_image_in[row][column];
	  /* incr circular pointer and check for wrap */
	  circular_pointer_end = (circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
	  if (circular_pointer_end == circular_pointer_start)
		circular_pointer_start = (circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
	  erode_kernel(apron_image_in[row][column],line_buffer1[circular_pointer_start],line_buffer2[circular_pointer_start],&pixel_out);
          if ((row>=2) && (column>=2))
	  {
             img_out[(row-2)*IMAGE_WIDTH+(column-2)]=pixel_out;
	  }
      }
   }
}

void dilate_slicing(unsigned char * img_in,unsigned char * img_out)
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
          unsigned char pixel_out;
          /* store input in the delay line */
	  line_buffer2[circular_pointer_end]=line_buffer1[circular_pointer_start]; /* push fifo */
	  line_buffer1[circular_pointer_end]=apron_image_in[row][column];
	  /* incr circular pointer and check for wrap */
	  circular_pointer_end = (circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
	  if (circular_pointer_end == circular_pointer_start)
		circular_pointer_start = (circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
	  dilate_kernel(apron_image_in[row][column],line_buffer1[circular_pointer_start],line_buffer2[circular_pointer_start],&pixel_out);

          if ((row>=2) && (column>=2))
	  {
             img_out[(row-2)*IMAGE_WIDTH+(column-2)]=pixel_out;
	  }
      }
   }
}
