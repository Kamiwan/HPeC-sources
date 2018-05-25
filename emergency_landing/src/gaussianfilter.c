/*************************************************************************************
 * File   : gaussianfilter.c,  file for Vision Based Autonomous Landing
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
#include <string.h>
#include "image.h"

#define ROW_APRON (IMAGE_HEIGH+4)
#define COLUMN_APRON (IMAGE_WIDTH+4)
#define APRON_PIXEL 0 

typedef int image_int_t[IMAGE_HEIGH][IMAGE_WIDTH];
typedef int image_int_apron_t[ROW_APRON][COLUMN_APRON];

static image_int_apron_t apron_image_in;
static   image_int_apron_t apron_image_tmp,apron_image_tmp2;

///////////////////////////////////////////////////////////////////////////////
// Separable 2D Convolution : add an apron to not split treatment in 3 regions
// 1D convolution, transpose, 1D convolution
//
//
//		 |------------------|  |------------------|  |------------------|  |-----------|
//	input >- | 1 D convolution  |--| Transpose Memory |--| 1 D convolution  |--| Normalize | -> output
//		 |------------------|  |------------------|  |------------------|  |-----------|
//
// add an apron of 2 pixels large to have only one region of computation: full kernel
// We use directly values of kernel to optimize operation: change mac operation into add and shift
/////////////////////////////////////////////////////////////////////////////////////

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

void gaussianVeryFast2DSeparable_Apron(const unsigned char * image_in, unsigned char * image_out)
{
   int row, column;
   
   //add an apron
   for(row=0; row < ROW_APRON; ++row)
   {
		apron_image_in[row][0]=apron_image_in[row][1]=apron_image_in[row][COLUMN_APRON-1]=apron_image_in[row][COLUMN_APRON-2]=APRON_PIXEL;
		apron_image_tmp[row][0]=apron_image_tmp[row][1]=apron_image_tmp[row][COLUMN_APRON-1]=apron_image_tmp[row][COLUMN_APRON-2]=APRON_PIXEL;

   }
   for(column=0; column < COLUMN_APRON; ++column)
   {
		apron_image_in[0][column]=apron_image_in[1][column]=apron_image_in[ROW_APRON-1][column]=apron_image_in[ROW_APRON-2][column]=APRON_PIXEL;
		apron_image_tmp[0][column]=apron_image_tmp[1][column]=apron_image_tmp[ROW_APRON-1][column]=apron_image_tmp[ROW_APRON-2][column]=APRON_PIXEL;
   }
   for(row=0; row < IMAGE_HEIGH; ++row)
	   for(column=0; column < IMAGE_WIDTH; ++column)
			apron_image_in[row+2][column+2]=image_in[row*IMAGE_WIDTH+column];
   
   //first 1D convolution on row of source image with apron
   for(row=2; row < ROW_APRON-2; ++row)
	   for(column=2; column < COLUMN_APRON-2; ++column)
		   //can be do in parallel : no data depedency !!!
           convolutionVeryFast_1D_5x5(apron_image_in[row][column-2],apron_image_in[row][column-1],apron_image_in[row][column],apron_image_in[row][column+1],apron_image_in[row][column+2], &apron_image_tmp[row][column]);

   //second 1D convolution on column of the  result of first 1D convolution, direct transposition by array index
   for(column=2; column < COLUMN_APRON-2; ++column)
	   for(row=2; row < ROW_APRON-2; ++row)
		   //can be do in parallel: no data depedency !!!
           convolutionVeryFast_1D_5x5( apron_image_tmp[row-2][column],apron_image_tmp[row-1][column],apron_image_tmp[row][column],apron_image_tmp[row+1][column],apron_image_tmp[row+2][column], &apron_image_tmp2[row][column]);
   
   //normalize
   for(row=0; row < IMAGE_HEIGH; ++row)
	   for(column=0; column < IMAGE_WIDTH; ++column)
		   image_out[row*IMAGE_WIDTH+column]=(apron_image_tmp2[row+2][column+2])>>8;
}

/////////////////////////////////////////////////////////////////////////////////////
// Separable 2D Convolution with Apron and Tiling approach
/* 
	_________________________________________________________________________
	|																		|
	| A	_________________________________________________________________   |
	| P	|																|	|
	| R |																|	|
	| O	|																|	|
	| N |																|	|
	|	|																|	|
	|	|																|	|
	|	|																|	|
	|	|																|	|
	|	|																|	|
	|	|																|	|
	|	|																|	|
	|	|_______________________________________________________________|	|
	|																		|
	_________________________________________________________________________
*/
/////////////////////////////////////////////////////////////////////////////////////
#define TILING 20
typedef int tiling_int_t[TILING][TILING];


/* compute convolution only on a tile not full image: function may be accelerated in hardware */
void gaussian_tiling(tiling_int_t tiling_gaussian_in, tiling_int_t tiling_gaussian_out)
{
	int row, column;
	tiling_int_t tiling_image_tmp,tiling_image_tmp2;
  //first 1D convolution on row of source tiling with apron
   for(row=0; row < TILING; ++row)//we must compute all row for next convolution on column
	   for(column=  2   ;column < TILING-2  ; ++column)
           convolutionVeryFast_1D_5x5(tiling_gaussian_in[row][column-2],tiling_gaussian_in[row][column-1],tiling_gaussian_in[row][column],tiling_gaussian_in[row][column+1],tiling_gaussian_in[row][column+2], &tiling_image_tmp[row][column]);
   /* second 1D convolution on column of the  result of first 1D convolution */
   for(column=2; column < TILING-2; ++column)//we don't need compute all column: result will be valid in [2,TILING-2][2,TILING-2] only
	   for(row=  2; row < TILING-2;++row)
           convolutionVeryFast_1D_5x5( tiling_image_tmp[row-2][column],tiling_image_tmp[row-1][column],tiling_image_tmp[row][column],tiling_image_tmp[row+1][column],tiling_image_tmp[row+2][column], &tiling_image_tmp2[row][column]); 
	   //normalize
   for(row=2; row < TILING-2; ++row)
	   for(column=2; column < TILING-2; ++column)
	   {
		   //clipping
		   int clipping = (tiling_image_tmp2[row][column])>>8;
		   /* if (clipping>255)
			   clipping=255; */
		   tiling_gaussian_out[row][column]=clipping;
	   }
}

void gaussianVeryFast2DSeparable_Apron_Tiling(const unsigned char * image_in, unsigned char * image_out)
{
	int row, column,ti,tj;
	//add an apron
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
			apron_image_in[row+2][column+2]=image_in[row*IMAGE_WIDTH+column];
  //tiling
   for (tj=0;tj<IMAGE_HEIGH;tj+=TILING-4)
      for (ti=0; ti < IMAGE_WIDTH; ti+=TILING-4 )
	  {
		tiling_int_t tiling_gaussian_in;
		tiling_int_t tiling_gaussian_out;
		for (row=0; row < TILING; row++)
          for (column=0; column < TILING; column++)
                tiling_gaussian_in[row][column]=apron_image_in[row+tj][column+ti];
	    gaussian_tiling(tiling_gaussian_in,tiling_gaussian_out);
        for (row=2; row < TILING-2 ; row++)
          for (column=2; column < TILING-2; column++)
               image_out[(row+tj-2)*IMAGE_WIDTH	+(column+ti-2)]=tiling_gaussian_out[row][column];
	  }
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

#define CIRCULAR_BUFFER_SIZE (IMAGE_WIDTH+2+1)
static unsigned char line_buffer1[CIRCULAR_BUFFER_SIZE]; /* +1 Always keep one slot open : Full / Empty Buffer Distinction */
static unsigned char line_buffer2[CIRCULAR_BUFFER_SIZE];
static unsigned char line_buffer3[CIRCULAR_BUFFER_SIZE];
static unsigned char line_buffer4[CIRCULAR_BUFFER_SIZE];

static  unsigned char w_upper[5]={0,0,0,0,0};
static  unsigned char w_midup[5]={0,0,0,0,0};
static  unsigned char w_middle[5]={0,0,0,0,0};
static  unsigned char w_botmid[5]={0,0,0,0,0};
static  unsigned char w_bottom[5]={0,0,0,0,0};

void gaussian_slicing_kernel(unsigned char pixel_in,unsigned char l1,unsigned char l2,unsigned char l3,unsigned char l4,unsigned char *pixel_out)
{
    int result_1dConvolution[5];
    int sum;

    //first convolution
    convolutionVeryFast_1D_5x5(w_upper[4],w_upper[3],w_upper[2],w_upper[1],w_upper[0],&result_1dConvolution[0]);
    convolutionVeryFast_1D_5x5(w_midup[4],w_midup[3],w_midup[2],w_midup[1],w_midup[0],&result_1dConvolution[1]);
    convolutionVeryFast_1D_5x5(w_middle[4],w_middle[3],w_middle[2],w_middle[1],w_middle[0],&result_1dConvolution[2]);
    convolutionVeryFast_1D_5x5(w_botmid[4],w_botmid[3],w_botmid[2],w_botmid[1],w_botmid[0],&result_1dConvolution[3]);
    convolutionVeryFast_1D_5x5(w_bottom[4],w_bottom[3],w_bottom[2],w_bottom[1],w_bottom[0],&result_1dConvolution[4]);
    //second convolution
    convolutionVeryFast_1D_5x5(result_1dConvolution[0],result_1dConvolution[1],result_1dConvolution[2],result_1dConvolution[3],result_1dConvolution[4],&sum);
    //normalize
    *pixel_out=sum>>8;
    //shift register window buffer
    w_bottom[4]=w_bottom[3];w_bottom[3]=w_bottom[2];w_bottom[2]=w_bottom[1];w_bottom[1]=w_bottom[0];w_bottom[0]=pixel_in;
    w_botmid[4]=w_botmid[3];w_botmid[3]=w_botmid[2];w_botmid[2]=w_botmid[1];w_botmid[1]=w_botmid[0];w_botmid[0]=l1;
    w_middle[4]=w_middle[3];w_middle[3]=w_middle[2];w_middle[2]=w_middle[1];w_middle[1]=w_middle[0];w_middle[0]=l2;
    w_midup[4]=w_midup[3];w_midup[3]=w_midup[2];w_midup[2]=w_midup[1];w_midup[1]=w_midup[0];w_midup[0]=l3;
    w_upper[4]=w_upper[3];w_upper[3]=w_upper[2];w_upper[2]=w_upper[1];w_upper[1]=w_upper[0];w_upper[0]=l4;
}



void gaussianSlidingWindown_PixelStreaming(const unsigned char * image_in, unsigned char * image_out)
{
  int row, column;
   unsigned int circular_pointer_start;
   unsigned int circular_pointer_end;
   //reset
   memset(line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
   memset(line_buffer3, 0, CIRCULAR_BUFFER_SIZE);
   memset(line_buffer4, 0, CIRCULAR_BUFFER_SIZE);
   circular_pointer_start=0;
   circular_pointer_end=0;
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
			apron_image_in[row+2][column+2]=image_in[row*IMAGE_WIDTH+column];
   //process pixel in streaming: 3  stages: fill the fifos, main processing, empty the fifos.
   for (row=0; row < ROW_APRON; row++) {
      for (column=0; column < COLUMN_APRON; column++)
      {
	  unsigned char pixel_out;
	  //we compute always the data even if not necessary (fill the fifo) like a "streaming computing"

          /* store input in the delay line */
	  line_buffer4[circular_pointer_end]=line_buffer3[circular_pointer_start]; /* push fifo */
	  line_buffer3[circular_pointer_end]=line_buffer2[circular_pointer_start]; /* push fifo */
	  line_buffer2[circular_pointer_end]=line_buffer1[circular_pointer_start]; /* push fifo */
	  line_buffer1[circular_pointer_end]=apron_image_in[row][column];
	  /* incr circular pointer and check for wrap */
	  circular_pointer_end = (circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
	  if (circular_pointer_end == circular_pointer_start)
		circular_pointer_start = (circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
	  gaussian_slicing_kernel(apron_image_in[row][column],line_buffer1[circular_pointer_start],line_buffer2[circular_pointer_start],line_buffer3[circular_pointer_start],line_buffer4[circular_pointer_start],&pixel_out);
	  //skip invalid data
	  if ((row>=4) && (column>=4))
	  {
		//row<5 : first stage fill the fifo and not keep output else second stage  main processing
		//column >2: skip apron
		image_out[(row-4)*IMAGE_WIDTH+(column-4)]=pixel_out;
	  }
      }
   }
}


