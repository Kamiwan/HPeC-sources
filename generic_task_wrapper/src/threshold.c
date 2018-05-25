/*************************************************************************************
 * File   : threshold.c, file for Vision Based Autonomous Landing
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
#include <stdio.h>
#include "image.h"

#define DEBUG_VHDL

#ifdef DEBUG_VHDL
void testbench_hardware_stimuli(unsigned int *CH,  unsigned int *CIA)
{
    int lv;
    printf("constant CH: res_array(0 to 255) := (x\"%08x\"",CH[0]);
    for (lv = 1; lv < LEVEL; lv++)
    {
	printf(",x\"%08x\"",CH[lv]);
	if (lv % 4==0) printf("\r\n");
    }	
    printf(");\r\n");
    printf("constant CIA: res_array(0 to 255) := (x\"%08x\"",CIA[0]);
    for (lv = 1; lv < LEVEL; lv++)
    {
	printf(",x\"%08x\"",CIA[lv]);
	if (lv % 4==0) printf("\r\n");
    }	
    printf(");\r\n");
}
#endif

/* Use comparators and accumulators to compute cumulative histogram and cumulative intensity area for Otsu method */

void comparatorcounter(unsigned char * img_in,  unsigned int *CH,  unsigned int *CIA)
{
    int i,lv;
    /* use a comparator and accumulator: 256 comparator and accumulator in parallel for hardware */
    for (i = 0; i < IMAGE_SIZE; i++)
    {
        unsigned char pixel=img_in[i];


        for (lv = 0; lv < LEVEL; lv++)//parallel for or partial unroll for vectorization
        {
            if (pixel<=lv)
            {
                CH[lv]++ ;
                CIA[lv]+=pixel;
            }
        }
    }
}

/* in Hardware CH and CIA will be an internal block RAM with low latency */
void threshold(unsigned char * img_in, unsigned char * img_out, int img_size)
{
    unsigned int CH[LEVEL];
    unsigned int CIA[LEVEL];
    unsigned int th;

    int i,lv,wf,wb;
    float mb,mf,bcv,temp,max;

    memset(CH,(unsigned int)0,sizeof(int)*LEVEL);
    memset(CIA,(unsigned int)0,sizeof(int)*LEVEL);
    /* compute cumulative histograms */
    comparatorcounter(img_in,CH,CIA);
    #ifdef DEBUG_VHDL
    testbench_hardware_stimuli(CH,CIA);
    #endif
    /* compute betwwen class variance and keep maximum */
    th=0;
    max=0;
    for (lv = 0; lv < LEVEL; lv++)//parallel for or partial unroll for vectorization
    {
      wb=CH[lv];//Weight Background
      #ifdef DEBUG_VHDL
	   printf("wb[%d]=%d\r\n",lv,wb);
      #endif
      if (wb==0) continue;
      wf=img_size-CH[lv];//Weight Foreground	
      #ifdef DEBUG_VHDL
	   printf("wf[%d]=%d\r\n",lv,wf);
      #endif
      if (wf==0) continue;	
      mb =(float)CIA[lv]/(float)wb;// Mean Background		
      #ifdef DEBUG_VHDL
	   printf("mb[%d]=%f\r\n",lv,mb);
      #endif
      mf = (float)(CIA[LEVEL-1]-CIA[lv])/(float)wf;// Mean Foreground
      #ifdef DEBUG_VHDL
	   printf("mf[%d]=%f\r\n",lv,mf);
      #endif
      temp=mb-mf;
      #ifdef DEBUG_VHDL
	   printf("temp[%d]=%f\r\n",lv,temp);
	   printf("tmp3[%d]=%f\r\n",lv,(float)wb*temp);
	   printf("tmp4[%d]=%f\r\n",lv,(float)wf*temp);
      #endif
      bcv=(float)wb*(float)wf*temp*temp;
      #ifdef DEBUG_VHDL
	   printf("bcv[%d]=%f\r\n",lv,bcv);
      #endif
      if (bcv>max)
      {
        max=bcv;
        th=lv;
      }
    }
    printf("Optimal threshold =  %d\r\n",th);
    /* Get the result image */
    for (i = 0; i < img_size; i ++){
        if (img_in[i] >= th){
            img_out[i] = 255;
        }
        else{
            img_out[i] = 0;
        }
    }
}



void threshold2(unsigned char * img_in, unsigned char * img_out, int img_size)
{
    unsigned int H[LEVEL];
    unsigned int th;

    int i,lv,wf,wb,sum1,sumB;
    float mb,mf,bcv,temp,max;

    memset(H,(unsigned int)0,sizeof(int)*LEVEL);
    /* compute histogram */
    for (i = 0; i < IMAGE_SIZE; i++)
    {
        H[img_in[i]]++;
    }
    sum1=0;
    for (lv = 0; lv < LEVEL; lv++)
    {
    	sum1+=lv*H[lv];
    }
    /* compute betwwen class variance and keep maximum */
    th=0;
    max=0;
    sumB=0;
    wb=0;
    for (lv = 0; lv < LEVEL; lv++)//parallel for or partial unroll for vectorization
    {
      wb+=H[lv];//Weight Background
      if (wb==0) continue;
      wf=img_size-wb;//Weight Foreground	
      if (wf==0) continue;	
      sumB= sumB + lv*H[lv];
      mb =(float)sumB/(float)wb;// Mean Background		
      mf = (float)(sum1-sumB)/(float)wf;// Mean Foreground
      temp=mb-mf;
      bcv=(float)wb*(float)wf*temp*temp;
      if (bcv>max)
      {
        max=bcv;
        th=lv;
      }
    }
    printf("Optimal threshold2 =  %d\r\n",th);
    /* Get the result image */
    for (i = 0; i < img_size; i ++){
        if (img_in[i] >= th){
            img_out[i] = 255;
        }
        else{
            img_out[i] = 0;
        }
    }
}
