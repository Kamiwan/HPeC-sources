/*************************************************************************************
 * File   : harris.c, file for harris corner detection
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
#include "image.h"
#include "sobel.h"
#include "gaussianfilter.h"
#include "nms.h"
#include "freak.h"
#include "harris.h"

int compare2(const void *p1, const void *p2)
{
    const keyPoint *elem1 = ( const keyPoint *)p1;    
    const keyPoint *elem2 = ( const keyPoint *)p2;

   if (elem1->cim < elem2->cim)
      return -1;
   else if (elem1->cim > elem2->cim)
      return 1;
   else
      return 0;
}

void corner_harris(unsigned char *img_in, unsigned char *img_out,int threshold,keyPoint *keyPoints,int *kpCount,int sort)
{

	unsigned char current_pixel,gradient_out,edge_out,output_pixel;
	int row, column;
	//The output image is offset from the input to account for the line buffer
	//sobel (3x3) : one row  offset
	//gaussian(5x5) :  2 row offset
	//nms (3x3) : one row offset
	 unsigned char apron_sobel_ok,apron_gaussian_ok,output_ok,compute_sobel_ok,compute_gaussian_ok,apron_nms_ok,compute_nms_ok;

	 nms_reset_SOF();
	 sobel_reset_SOF();
	 gaussian1_reset_SOF();
	 gaussian2_reset_SOF();
	 gaussian3_reset_SOF();
	 *kpCount=0;
    	//output/input sequential order !!!: add apron of 2 rows and 2 column
	for (row=0; row < IMAGE_HEIGH+5; row++)
	{
            for (column=0; column < IMAGE_WIDTH+5; column++)
            {
            short gx,gy;
            int ixx,ixy,iyy,gixx,gixy,giyy;
        	int det, trac,cim ;
        	//apron of one row and one column for sobel 3x3
        	apron_sobel_ok=(row>=1) && (column>=1) && (row<IMAGE_HEIGH+1) && (column<IMAGE_WIDTH+1);
        	if (apron_sobel_ok)
        	{
        		current_pixel=img_in[(row-1)*IMAGE_WIDTH+(column-1)];
        	}
        	else current_pixel=0;
        	//input sobel 8 bits: 0-255
        	compute_sobel_ok=(row>=0) && (column>=0) && (row<IMAGE_HEIGH+2) && (column<IMAGE_WIDTH+2);
        	if (compute_sobel_ok)
        		sobel_slicing(current_pixel,&gradient_out,&edge_out,&gx,&gy);
        	else gx=gy=0;
        	//output sobel : 9 bits -255...255 (normalize sobel )
        	//ixx,ixy,iyy : 18 bits -65025...650525
        	apron_gaussian_ok=(row>=2) && (column>=2) && (row<IMAGE_HEIGH+2) && (column<IMAGE_WIDTH+2);
        	//first valid output sobel is for row=2 and column=2
        	if (apron_gaussian_ok)
        	{
           		//valid domain of sobel output
	                ixx=gx*gx;
        		ixy=gx*gy;
        		iyy=gy*gy;
        	}
        	else ixx=ixy=iyy=0;
           	//apron of two rows and two columns for binomial gaussian 5x5
            	compute_gaussian_ok=(row>=0) && (column>=0) && (row<IMAGE_HEIGH+4) && (column<IMAGE_WIDTH+4);
        	if (compute_gaussian_ok)
        	{
        		//normalize gaussian filter : gixx,gixy,giyy 18 bits
        		gaussian1_sliding_windown(ixx, &gixx);
        		gaussian2_sliding_windown(ixy, &gixy);
        		gaussian3_sliding_windown(iyy, &giyy);
        	}
        	else gixx=gixy=giyy=0;
        	//find local maximum 3x3 window : cim, threshold => IMAGE_out
           	apron_nms_ok=(row>=4) && (column>=4) && (row<IMAGE_HEIGH+4) && (column<IMAGE_WIDTH+4);
           	if (apron_nms_ok)
           	{
           		//valid domain of gaussian output
            		//to avoid overflow (32 bits): change threshold scale/step
	            	gixx=gixx>>3;//now gixx,gixy,giyy on 15 bits
        	    	gixy=gixy>>3;
        	    	giyy=giyy>>3;
        	    	//k � [0.04,0.06]
        	    	//1/32 = 0.03125
        	    	//1/64 = 0.015625
        	    	//k = 1/32+1/64 = (1>>5) + (1>>6) = 0.046875
        	    	//k = 1/32+1/128+1/256 = (1>>5) + (1>>7) + (1>>8) = 0,04296875
        	    	det=gixx*giyy-gixy*gixy;//31 bits
        	    	trac=gixx+giyy;//16 bits
        	    	cim=det-(((trac>>5)+(trac>>7)+(trac>>8))*trac); //(cim>0 : cornerpixel , cim#0 : pixel in flat region,  cim<0 edge pixel)
           	}
           	else
           		cim=0;
           	compute_nms_ok=(row>=3) && (column>=3) && (row<IMAGE_HEIGH+5) && (column<IMAGE_WIDTH+5);
           	if (compute_nms_ok)
           		non_maxima_supression(cim,&output_pixel,threshold);
		else output_pixel=0;
               output_ok=(row>=5) && (column>=5);
             	if (output_ok)
        	{
                	if ((output_pixel==255) && (*kpCount<MAX_KEYPOINT))
                	{
                    	keyPoints[*kpCount].x=row-5;
                    	keyPoints[*kpCount].y=column-5;
                    	keyPoints[*kpCount].size=1.0;
                    	keyPoints[*kpCount].cim=cim;
                    	*kpCount=*kpCount+1;
                	}
        		img_out[(row-5)*IMAGE_WIDTH+(column-5)] = output_pixel;
            	}
           }
	}
 if (sort!=0)						 			
 {  
    qsort(keyPoints,sizeof(keyPoints)/sizeof(keyPoint),sizeof(keyPoint),compare2);
    *kpCount=1024; //keep only highest first points
 }
}

