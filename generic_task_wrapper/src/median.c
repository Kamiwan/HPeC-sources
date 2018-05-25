/*************************************************************************************
 * File   : median.c, file for Vision Based Autonomous Landing
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
/* first architecture line buffer  : Circular Buffer  */
typedef unsigned char image_apron_t[IMAGE_HEIGH+2][IMAGE_WIDTH+2];
static image_apron_t apron_image_in;

#define CIRCULAR_BUFFER_SIZE (IMAGE_WIDTH+2+1)
static unsigned char line_buffer1[CIRCULAR_BUFFER_SIZE]; /* +1 Always keep one slot open : Full / Empty Buffer Distinction */
static unsigned char line_buffer2[CIRCULAR_BUFFER_SIZE];

/*	              ____   ____   ____
		     |    | |    |_|    |
Pixelin -|-----------| p1 |-| p2 | | p3 |  p1=Pixel_in
         |           |____| |____| |____|
 ________|
|   __________        ____   ____   ____
|__|   Line   |__l1__|    |_|    |_|    |
   |  buffer  | |    | p4 | | p5 | | p6 |
   |__________|	|    |____| |____| |____|
 _______________|
|   __________        ____   ____   ____
|__|   Line   |__l2__|    |_|    |_|    |
   |  buffer  |      | p7 | | p8 | | p9 |
   |__________|	     |____| |____| |____|


	 */
static  unsigned char w_upper[3]={0,0,0};
static  unsigned char w_middle[3]={0,0,0};
static  unsigned char w_down[3]={0,0,0};
#define p7  w_upper[0]
#define p8  w_upper[1]
#define p9  w_upper[2]
#define p4  w_middle[0]
#define p5  w_middle[1]
#define p6  w_middle[2]
#define p1  w_down[0]
#define p2  w_down[1]
#define p3  w_down[2]

 //inline 
 void compare(unsigned char a,unsigned char b, unsigned char *lo,unsigned char *hi)
 {
   if (a>b)
   {
      *lo=b;
      *hi=a;
   }
   else
   {
      *lo=a;
      *hi=b;
   }
 }

 void median_slicing_kernel(unsigned char pixel_in,unsigned char l1, unsigned char l2,unsigned char *pixel_out)
{
    int j;
    //see  http://www.xilinx.com/publications/archives/xcell/Xcell23.pdf  : Implementing Median Filters
    //Minimum exchange network required to produce a median from nine input pixels
    unsigned char c1lo,c1hi,c2lo,c2hi,c3lo,c3hi,unused;
    unsigned char c4lo,c4hi,c5lo,c5hi,c6lo,c6hi;
    unsigned char c7lo,c7hi,c8lo,c8hi,c9lo,c9hi;
    unsigned char c10hi,c11lo,c12lo,c12hi,c13hi;
    unsigned char c14lo,c15hi,c16lo,c17lo,c17hi,c18hi;

    compare(p2,p3,&c1lo,&c1hi);compare(p5,p6,&c2lo,&c2hi);compare(p8,p9,&c3lo,&c3hi);//123
    compare(p1,c1lo,&c4lo,&c4hi);compare(p4,c2lo,&c5lo,&c5hi);compare(p7,c3lo,&c6lo,&c6hi);//456
    compare(c4hi,c1hi,&c7lo,&c7hi);compare(c5hi,c2hi,&c8lo,&c8hi);compare(c6hi,c3hi,&c9lo,&c9hi);//789
    compare(c4lo,c5lo,&unused,&c10hi);compare(c8hi,c9hi,&c11lo,&unused);compare(c8lo,c9lo,&c12lo,&c12hi);//101112
    compare(c7lo,c12lo,&unused,&c13hi);//13
    compare(c13hi,c12hi,&c14lo,&unused);//14
    compare(c10hi,c6lo,&unused,&c15hi);compare(c7hi,c11lo,&c16lo,&unused);//1516
    compare(c14lo,c16lo,&c17lo,&c17hi);//17
    compare(c15hi,c17lo,&unused,&c18hi);//18
    compare(c18hi,c17hi,pixel_out,&unused);//19
    //shift register window
    for (j=2;j>0;j--)
    {
	w_upper[j]=w_upper[j-1];
	w_middle[j]=w_middle[j-1];
        w_down[j]=w_down[j-1];
    }
    w_upper[0]=l2;
    w_middle[0]=l1;
    w_down[0]=pixel_in;
 }

void median_slicing(unsigned char * img_in,unsigned char * img_out)
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
	apron_image_in[0][column]=apron_image_in[IMAGE_HEIGH+1][column]=0;
   }
   for (row=0; row < IMAGE_HEIGH+2; row++) {
	apron_image_in[row][0]=apron_image_in[row][IMAGE_WIDTH+1]=0;
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
	  median_slicing_kernel(apron_image_in[row][column],line_buffer1[circular_pointer_start],line_buffer2[circular_pointer_start],&pixel_out);
	  /* store input in the delay line */
 	  line_buffer2[circular_pointer_end]=line_buffer1[circular_pointer_start]; /* push fifo */
	  line_buffer1[circular_pointer_end]=apron_image_in[row][column];
	  /* incr circular pointer and check for wrap */
	  circular_pointer_end = (circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
	  if (circular_pointer_end == circular_pointer_start)
		circular_pointer_start = (circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
          if ((row>=2) && (column>=2))
          {
	    img_out[(row-2)*IMAGE_WIDTH+(column-2)]=pixel_out;
	  }
      }
   }
}

/*	              ____   ____   ____
		     |    |_|    |_|    |
Pixelin -|-----------| p1 |-| p2 | | p3 |
         |           |____| |____| |____|
 ________|
|   __________        ____   ____   ____
|__|   Line   |__l1__|    |_|    |_|    |
   |  buffer  | |    | p4 | | p5 | | p6 |
   |__________|	|    |____| |____| |____|
 _______________|
|   __________        ____   ____   ____
|__|   Line   |__l2__|    |_|    |_|    |
   |  buffer  |      | p7 | | p8 | | p9 |
   |__________|	     |____| |____| |____|


	 */

 void median_histo_kernel(unsigned char pixel_in,unsigned char l1, unsigned char l2,unsigned int *histo,unsigned char *pixel_out)
{
  //middle (kxk=1)/2=(3x3+1)/2=5
  #define middle 5
    //see  Huang'a algorithm
    int m,n,j;
    // find Median in histogram
    for(m=0,n=0; n < LEVEL; n++)
    {
	m += histo[n];
	if(m>=middle)
		break;
    }
    *pixel_out=n;
    //shift register window and update histogram window
    histo[p3]--;histo[p6]--;histo[p9]--;
    histo[pixel_in]++;histo[l1]++;histo[l2]++;
   //shift register window
    for (j=2;j>0;j--)
    {
	w_upper[j]=w_upper[j-1];
	w_middle[j]=w_middle[j-1];
        w_down[j]=w_down[j-1];
    }
    w_upper[0]=l2;
    w_middle[0]=l1;
    w_down[0]=pixel_in;
 }

void median_histo(unsigned char * img_in,unsigned char * img_out)
{
   int row, column;
   unsigned int circular_pointer_start;
   unsigned int circular_pointer_end;
   unsigned int histo[LEVEL];

   //reset
   memset(histo,(unsigned char)0,sizeof(int)*LEVEL);
   memset(line_buffer1, 0, CIRCULAR_BUFFER_SIZE);
   memset(line_buffer2, 0, CIRCULAR_BUFFER_SIZE);
   circular_pointer_start=0;
   circular_pointer_end=0;
   histo[0]=9;
   //add apron
   for (column=0; column < IMAGE_WIDTH+2; column++) {
	apron_image_in[0][column]=apron_image_in[IMAGE_HEIGH+1][column]=0;
   }
   for (row=0; row < IMAGE_HEIGH+2; row++) {
	apron_image_in[row][0]=apron_image_in[row][IMAGE_WIDTH+1]=0;
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
	  median_histo_kernel(apron_image_in[row][column],line_buffer1[circular_pointer_start],line_buffer2[circular_pointer_start],histo,&pixel_out);
	  /* store input in the delay line */
 	  line_buffer2[circular_pointer_end]=line_buffer1[circular_pointer_start]; /* push fifo */
	  line_buffer1[circular_pointer_end]=apron_image_in[row][column];
	  /* incr circular pointer and check for wrap */
	  circular_pointer_end = (circular_pointer_end +1 ) % CIRCULAR_BUFFER_SIZE;
	  if (circular_pointer_end == circular_pointer_start)
		circular_pointer_start = (circular_pointer_start +1 ) % CIRCULAR_BUFFER_SIZE; /* full, overwrite */
          if ((row>=2) && (column>=2))
          {
	    img_out[(row-2)*IMAGE_WIDTH+(column-2)]=pixel_out;
	  }
      }
   }
}
