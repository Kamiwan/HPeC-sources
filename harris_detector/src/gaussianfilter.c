/*************************************************************************************
 * File   : gaussianfilter.c,  file for Vision Based Autonomous Landing
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
#include <string.h>
#include "image.h"

#define LINEBUFFERSIZENMS 642

#define CIRCULAR_BUFFER_SIZE (LINEBUFFERSIZEGAUSSIAN+1)

typedef int image_int_t[IMAGE_HEIGH][IMAGE_WIDTH];

static unsigned int gaussian1_circular_pointer_start=0;
static unsigned int gaussian1_circular_pointer_end=0;

static int gaussian1_line_buffer1[CIRCULAR_BUFFER_SIZE];
static int gaussian1_line_buffer2[CIRCULAR_BUFFER_SIZE];
static int gaussian1_line_buffer3[CIRCULAR_BUFFER_SIZE];
static int gaussian1_line_buffer4[CIRCULAR_BUFFER_SIZE];
static  int gaussian1_w_upper[5]={0,0,0,0,0};
static  int gaussian1_w_midup[5]={0,0,0,0,0};
static  int gaussian1_w_middle[5]={0,0,0,0,0};
static  int gaussian1_w_botmid[5]={0,0,0,0,0};
static  int gaussian1_w_bottom[5]={0,0,0,0,0};


static unsigned int gaussian2_circular_pointer_start=0;
static unsigned int gaussian2_circular_pointer_end=0;

static int gaussian2_line_buffer1[CIRCULAR_BUFFER_SIZE];
static int gaussian2_line_buffer2[CIRCULAR_BUFFER_SIZE];
static int gaussian2_line_buffer3[CIRCULAR_BUFFER_SIZE];
static int gaussian2_line_buffer4[CIRCULAR_BUFFER_SIZE];
static  int gaussian2_w_upper[5]={0,0,0,0,0};
static  int gaussian2_w_midup[5]={0,0,0,0,0};
static  int gaussian2_w_middle[5]={0,0,0,0,0};
static  int gaussian2_w_botmid[5]={0,0,0,0,0};
static  int gaussian2_w_bottom[5]={0,0,0,0,0};

static unsigned int gaussian3_circular_pointer_start=0;
static unsigned int gaussian3_circular_pointer_end=0;

static int gaussian3_line_buffer1[CIRCULAR_BUFFER_SIZE];
static int gaussian3_line_buffer2[CIRCULAR_BUFFER_SIZE];
static int gaussian3_line_buffer3[CIRCULAR_BUFFER_SIZE];
static int gaussian3_line_buffer4[CIRCULAR_BUFFER_SIZE];
static  int gaussian3_w_upper[5]={0,0,0,0,0};
static  int gaussian3_w_midup[5]={0,0,0,0,0};
static  int gaussian3_w_middle[5]={0,0,0,0,0};
static  int gaussian3_w_botmid[5]={0,0,0,0,0};
static  int gaussian3_w_bottom[5]={0,0,0,0,0};


/* compute [p1 p2 p3 p4 p 5] [1 4 6 4 1] */
void convolutionVeryFast_1D_5x5(int p1,int p2,int p3,int p4,int p5, int *output )
{
	//create a binary adder tree
	int tmp_add,tmp_add2,tmp_add3,tmp_add4;
	tmp_add=p1+p5;
	tmp_add2=(p3<<2)+(p3<<1);
	tmp_add3=(p2<<2)+(p4<<2);
	tmp_add4=tmp_add+tmp_add2;
	*output = tmp_add4+tmp_add3;
}


/////////////////////////////////////////////////////////////////////////////////////
/* 2D Separable Convolution with Sliding window:
/*
	     __________	    ____   ____   ____   ____   ____
   	    |   Line   |   |    | |    | |    | |    | |    |
 	  --|  buffer4 |---| ur |-|    |-|    |-|    |-| ul |				| 1 |
	 |  |__________|   |____| |____| |____| |____| |____|
	 |
	 |_______________
	     __________	 |  ____   ____   ____   ____   ____
	    |   Line   | | |    | |    | |    | |    | |    |				| 4 |
	  --|  buffer3 |---|    |-|    |-|    |-|    |-|    |
	 |  |__________|   |____| |____| |____| |____| |____|
	 |
	 |_______________
	     __________	 |  ____   ____   ____   ____   ____
	    |   Line   | | |	| |    | |    | |    | |    |	 	x		| 6 |  ) x [ 1 4 6 4 1 ]    /256 => pixelout
	  --|  buffer2 |---| mr |-|    |-|    |-|    |-| ml |
	 |  |__________|   |____| |____| |____| |____| |____|
	 |
	 |_______________
	     __________	 |  ____   ____   ____   ____   ____
	    |   Line   | | |    | |    | |    | |    | |    |
	  --|  buffer1 |---|    |-|    |-|    |-|    |-|    |				| 4 |
	 |  |__________|   |____| |____| |____| |____| |____|
	 |
	 |______________
			|   ____   ____   ____   ____    ____
			|  |    | |    | |    | |    |  |    |	 			| 1 |
 	      Pixin  ------|    |-|    |-|    |-|    |--|    |
			   |____| |____| |____| |____|  |____|          bottom rigth to left

*/

void gaussian1_slicing_kernel(int pixel_in,int l1,int l2,int l3,int l4,int *pixel_out)
{
    int result_1dConvolution[5];
    int sum;


    //first convolution
    convolutionVeryFast_1D_5x5(gaussian1_w_upper[4],gaussian1_w_upper[3],gaussian1_w_upper[2],gaussian1_w_upper[1],gaussian1_w_upper[0],&result_1dConvolution[0]);
    convolutionVeryFast_1D_5x5(gaussian1_w_midup[4],gaussian1_w_midup[3],gaussian1_w_midup[2],gaussian1_w_midup[1],gaussian1_w_midup[0],&result_1dConvolution[1]);
    convolutionVeryFast_1D_5x5(gaussian1_w_middle[4],gaussian1_w_middle[3],gaussian1_w_middle[2],gaussian1_w_middle[1],gaussian1_w_middle[0],&result_1dConvolution[2]);
    convolutionVeryFast_1D_5x5(gaussian1_w_botmid[4],gaussian1_w_botmid[3],gaussian1_w_botmid[2],gaussian1_w_botmid[1],gaussian1_w_botmid[0],&result_1dConvolution[3]);
    convolutionVeryFast_1D_5x5(gaussian1_w_bottom[4],gaussian1_w_bottom[3],gaussian1_w_bottom[2],gaussian1_w_bottom[1],gaussian1_w_bottom[0],&result_1dConvolution[4]);
    //second convolution
    convolutionVeryFast_1D_5x5(result_1dConvolution[0],result_1dConvolution[1],result_1dConvolution[2],result_1dConvolution[3],result_1dConvolution[4],&sum);
    //normalize
    *pixel_out=sum>>8;
    //shift register window buffer
    gaussian1_w_bottom[4]=gaussian1_w_bottom[3];gaussian1_w_bottom[3]=gaussian1_w_bottom[2];gaussian1_w_bottom[2]=gaussian1_w_bottom[1];gaussian1_w_bottom[1]=gaussian1_w_bottom[0];gaussian1_w_bottom[0]=pixel_in;
    gaussian1_w_botmid[4]=gaussian1_w_botmid[3];gaussian1_w_botmid[3]=gaussian1_w_botmid[2];gaussian1_w_botmid[2]=gaussian1_w_botmid[1];gaussian1_w_botmid[1]=gaussian1_w_botmid[0];gaussian1_w_botmid[0]=l1;
    gaussian1_w_middle[4]=gaussian1_w_middle[3];gaussian1_w_middle[3]=gaussian1_w_middle[2];gaussian1_w_middle[2]=gaussian1_w_middle[1];gaussian1_w_middle[1]=gaussian1_w_middle[0];gaussian1_w_middle[0]=l2;
    gaussian1_w_midup[4]=gaussian1_w_midup[3];gaussian1_w_midup[3]=gaussian1_w_midup[2];gaussian1_w_midup[2]=gaussian1_w_midup[1];gaussian1_w_midup[1]=gaussian1_w_midup[0];gaussian1_w_midup[0]=l3;
    gaussian1_w_upper[4]=gaussian1_w_upper[3];gaussian1_w_upper[3]=gaussian1_w_upper[2];gaussian1_w_upper[2]=gaussian1_w_upper[1];gaussian1_w_upper[1]=gaussian1_w_upper[0];gaussian1_w_upper[0]=l4;
}


void gaussian1_reset_SOF()
{
   //reset
   memset(gaussian1_line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian1_line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian1_line_buffer3, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian1_line_buffer4, 0, CIRCULAR_BUFFER_SIZE);
   gaussian1_circular_pointer_start=0;
   gaussian1_circular_pointer_end=0;
   memset(gaussian1_w_upper,0,5);
   memset(gaussian1_w_midup,0,5);
   memset(gaussian1_w_middle,0,5);
   memset(gaussian1_w_botmid,0,5);
   memset(gaussian1_w_bottom,0,5);
}

void gaussian1_sliding_windown(int pixel_in, int * pixel_out)
{
    /* store input in the delay line */
   gaussian1_line_buffer4[gaussian1_circular_pointer_end]=gaussian1_line_buffer3[gaussian1_circular_pointer_start]; /* push fifo */
   gaussian1_line_buffer3[gaussian1_circular_pointer_end]=gaussian1_line_buffer2[gaussian1_circular_pointer_start]; /* push fifo */
   gaussian1_line_buffer2[gaussian1_circular_pointer_end]=gaussian1_line_buffer1[gaussian1_circular_pointer_start]; /* push fifo */
   gaussian1_line_buffer1[gaussian1_circular_pointer_end]=pixel_in;
   /* incr circular pointer and check for wrap */
   gaussian1_circular_pointer_end = (gaussian1_circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
   if (gaussian1_circular_pointer_end == gaussian1_circular_pointer_start)
	gaussian1_circular_pointer_start = (gaussian1_circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
   gaussian1_slicing_kernel(pixel_in,gaussian1_line_buffer1[gaussian1_circular_pointer_start],gaussian1_line_buffer2[gaussian1_circular_pointer_start],gaussian1_line_buffer3[gaussian1_circular_pointer_start],
	gaussian1_line_buffer4[gaussian1_circular_pointer_start],pixel_out);
}

void gaussian2_slicing_kernel(int pixel_in,int l1,int l2,int l3,int l4,int *pixel_out)
{
    int result_1dConvolution[5];
    int sum;


    //first convolution
    convolutionVeryFast_1D_5x5(gaussian2_w_upper[4],gaussian2_w_upper[3],gaussian2_w_upper[2],gaussian2_w_upper[1],gaussian2_w_upper[0],&result_1dConvolution[0]);
    convolutionVeryFast_1D_5x5(gaussian2_w_midup[4],gaussian2_w_midup[3],gaussian2_w_midup[2],gaussian2_w_midup[1],gaussian2_w_midup[0],&result_1dConvolution[1]);
    convolutionVeryFast_1D_5x5(gaussian2_w_middle[4],gaussian2_w_middle[3],gaussian2_w_middle[2],gaussian2_w_middle[1],gaussian2_w_middle[0],&result_1dConvolution[2]);
    convolutionVeryFast_1D_5x5(gaussian2_w_botmid[4],gaussian2_w_botmid[3],gaussian2_w_botmid[2],gaussian2_w_botmid[1],gaussian2_w_botmid[0],&result_1dConvolution[3]);
    convolutionVeryFast_1D_5x5(gaussian2_w_bottom[4],gaussian2_w_bottom[3],gaussian2_w_bottom[2],gaussian2_w_bottom[1],gaussian2_w_bottom[0],&result_1dConvolution[4]);
    //second convolution
    convolutionVeryFast_1D_5x5(result_1dConvolution[0],result_1dConvolution[1],result_1dConvolution[2],result_1dConvolution[3],result_1dConvolution[4],&sum);
    //normalize
    *pixel_out=sum>>8;
    //shift register window buffer
    gaussian2_w_bottom[4]=gaussian2_w_bottom[3];gaussian2_w_bottom[3]=gaussian2_w_bottom[2];gaussian2_w_bottom[2]=gaussian2_w_bottom[1];gaussian2_w_bottom[1]=gaussian2_w_bottom[0];gaussian2_w_bottom[0]=pixel_in;
    gaussian2_w_botmid[4]=gaussian2_w_botmid[3];gaussian2_w_botmid[3]=gaussian2_w_botmid[2];gaussian2_w_botmid[2]=gaussian2_w_botmid[1];gaussian2_w_botmid[1]=gaussian2_w_botmid[0];gaussian2_w_botmid[0]=l1;
    gaussian2_w_middle[4]=gaussian2_w_middle[3];gaussian2_w_middle[3]=gaussian2_w_middle[2];gaussian2_w_middle[2]=gaussian2_w_middle[1];gaussian2_w_middle[1]=gaussian2_w_middle[0];gaussian2_w_middle[0]=l2;
    gaussian2_w_midup[4]=gaussian2_w_midup[3];gaussian2_w_midup[3]=gaussian2_w_midup[2];gaussian2_w_midup[2]=gaussian2_w_midup[1];gaussian2_w_midup[1]=gaussian2_w_midup[0];gaussian2_w_midup[0]=l3;
    gaussian2_w_upper[4]=gaussian2_w_upper[3];gaussian2_w_upper[3]=gaussian2_w_upper[2];gaussian2_w_upper[2]=gaussian2_w_upper[1];gaussian2_w_upper[1]=gaussian2_w_upper[0];gaussian2_w_upper[0]=l4;
}


void gaussian2_reset_SOF()

{
   //reset
   memset(gaussian2_line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian2_line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian2_line_buffer3, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian2_line_buffer4, 0, CIRCULAR_BUFFER_SIZE);
   gaussian2_circular_pointer_start=0;
   gaussian2_circular_pointer_end=0;
   memset(gaussian2_w_upper,0,5);
   memset(gaussian2_w_midup,0,5);
   memset(gaussian2_w_middle,0,5);
   memset(gaussian2_w_botmid,0,5);
   memset(gaussian2_w_bottom,0,5);
}

void gaussian2_sliding_windown(int pixel_in, int * pixel_out)
{
    /* store input in the delay line */
   gaussian2_line_buffer4[gaussian2_circular_pointer_end]=gaussian2_line_buffer3[gaussian2_circular_pointer_start]; /* push fifo */
   gaussian2_line_buffer3[gaussian2_circular_pointer_end]=gaussian2_line_buffer2[gaussian2_circular_pointer_start]; /* push fifo */
   gaussian2_line_buffer2[gaussian2_circular_pointer_end]=gaussian2_line_buffer1[gaussian2_circular_pointer_start]; /* push fifo */
   gaussian2_line_buffer1[gaussian2_circular_pointer_end]=pixel_in;
   /* incr circular pointer and check for wrap */
   gaussian2_circular_pointer_end = (gaussian2_circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
   if (gaussian2_circular_pointer_end == gaussian2_circular_pointer_start)
	gaussian2_circular_pointer_start = (gaussian2_circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
   gaussian2_slicing_kernel(pixel_in,gaussian2_line_buffer1[gaussian2_circular_pointer_start],gaussian2_line_buffer2[gaussian2_circular_pointer_start],gaussian2_line_buffer3[gaussian2_circular_pointer_start],
	gaussian2_line_buffer4[gaussian2_circular_pointer_start],pixel_out);
}

void gaussian3_slicing_kernel(int pixel_in,int l1,int l2,int l3,int l4,int *pixel_out)
{
    int result_1dConvolution[5];
    int sum;


    //first convolution
    convolutionVeryFast_1D_5x5(gaussian3_w_upper[4],gaussian3_w_upper[3],gaussian3_w_upper[2],gaussian3_w_upper[1],gaussian3_w_upper[0],&result_1dConvolution[0]);
    convolutionVeryFast_1D_5x5(gaussian3_w_midup[4],gaussian3_w_midup[3],gaussian3_w_midup[2],gaussian3_w_midup[1],gaussian3_w_midup[0],&result_1dConvolution[1]);
    convolutionVeryFast_1D_5x5(gaussian3_w_middle[4],gaussian3_w_middle[3],gaussian3_w_middle[2],gaussian3_w_middle[1],gaussian3_w_middle[0],&result_1dConvolution[2]);
    convolutionVeryFast_1D_5x5(gaussian3_w_botmid[4],gaussian3_w_botmid[3],gaussian3_w_botmid[2],gaussian3_w_botmid[1],gaussian3_w_botmid[0],&result_1dConvolution[3]);
    convolutionVeryFast_1D_5x5(gaussian3_w_bottom[4],gaussian3_w_bottom[3],gaussian3_w_bottom[2],gaussian3_w_bottom[1],gaussian3_w_bottom[0],&result_1dConvolution[4]);
    //second convolution
    convolutionVeryFast_1D_5x5(result_1dConvolution[0],result_1dConvolution[1],result_1dConvolution[2],result_1dConvolution[3],result_1dConvolution[4],&sum);
    //normalize
    *pixel_out=sum>>8;
    //shift register window buffer
    gaussian3_w_bottom[4]=gaussian3_w_bottom[3];gaussian3_w_bottom[3]=gaussian3_w_bottom[2];gaussian3_w_bottom[2]=gaussian3_w_bottom[1];gaussian3_w_bottom[1]=gaussian3_w_bottom[0];gaussian3_w_bottom[0]=pixel_in;
    gaussian3_w_botmid[4]=gaussian3_w_botmid[3];gaussian3_w_botmid[3]=gaussian3_w_botmid[2];gaussian3_w_botmid[2]=gaussian3_w_botmid[1];gaussian3_w_botmid[1]=gaussian3_w_botmid[0];gaussian3_w_botmid[0]=l1;
    gaussian3_w_middle[4]=gaussian3_w_middle[3];gaussian3_w_middle[3]=gaussian3_w_middle[2];gaussian3_w_middle[2]=gaussian3_w_middle[1];gaussian3_w_middle[1]=gaussian3_w_middle[0];gaussian3_w_middle[0]=l2;
    gaussian3_w_midup[4]=gaussian3_w_midup[3];gaussian3_w_midup[3]=gaussian3_w_midup[2];gaussian3_w_midup[2]=gaussian3_w_midup[1];gaussian3_w_midup[1]=gaussian3_w_midup[0];gaussian3_w_midup[0]=l3;
    gaussian3_w_upper[4]=gaussian3_w_upper[3];gaussian3_w_upper[3]=gaussian3_w_upper[2];gaussian3_w_upper[2]=gaussian3_w_upper[1];gaussian3_w_upper[1]=gaussian3_w_upper[0];gaussian3_w_upper[0]=l4;
}


void gaussian3_reset_SOF()
{
   //reset
   memset(gaussian3_line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian3_line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian3_line_buffer3, 0, CIRCULAR_BUFFER_SIZE);
   memset(gaussian3_line_buffer4, 0, CIRCULAR_BUFFER_SIZE);
   gaussian3_circular_pointer_start=0;
   gaussian3_circular_pointer_end=0;
   memset(gaussian3_w_upper,0,5);
   memset(gaussian3_w_midup,0,5);
   memset(gaussian3_w_middle,0,5);
   memset(gaussian3_w_botmid,0,5);
   memset(gaussian3_w_bottom,0,5);
}

void gaussian3_sliding_windown(int pixel_in, int * pixel_out)
{
    /* store input in the delay line */
   gaussian3_line_buffer4[gaussian3_circular_pointer_end]=gaussian3_line_buffer3[gaussian3_circular_pointer_start]; /* push fifo */
   gaussian3_line_buffer3[gaussian3_circular_pointer_end]=gaussian3_line_buffer2[gaussian3_circular_pointer_start]; /* push fifo */
   gaussian3_line_buffer2[gaussian3_circular_pointer_end]=gaussian3_line_buffer1[gaussian3_circular_pointer_start]; /* push fifo */
   gaussian3_line_buffer1[gaussian3_circular_pointer_end]=pixel_in;
   /* incr circular pointer and check for wrap */
   gaussian3_circular_pointer_end = (gaussian3_circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
   if (gaussian3_circular_pointer_end == gaussian3_circular_pointer_start)
	gaussian3_circular_pointer_start = (gaussian3_circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
   gaussian3_slicing_kernel(pixel_in,gaussian3_line_buffer1[gaussian3_circular_pointer_start],gaussian3_line_buffer2[gaussian3_circular_pointer_start],gaussian3_line_buffer3[gaussian3_circular_pointer_start],
	gaussian3_line_buffer4[gaussian3_circular_pointer_start],pixel_out);
}

