/*************************************************************************************
 * File   : histequalization.c, file for face detection (Viola Jones, AdaBoost)
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
//#include <arm_neon.h>
/* FIXED POINT REPRESENTATION Q8.24 */
#define SCALE_FACTOR (1<<24)
/* ACCUMULATOR_STEP 255/(640x480)*SCALE FACTOR for VGA =>  13926 */
/* #define ACCUMULATOR_STEP 13926 for VGA */
#define ACCUMULATOR_STEP 13926
/* Use comparators and accumulators to compute LUT(cumulative histrogram with normalization) for histogram equalization */
void lut_comparatorcounter(unsigned char * img_in,  unsigned int *LUT)
{
    int i,l;

    uint32_t hist[LEVEL];
    uint32_t cumul,maplevel;

    memset(hist,(unsigned int)0,sizeof(uint32_t)*LEVEL);
    /* compute histogram */
    for (i = 0; i < IMAGE_SIZE; i++)
    {
        unsigned int pixel=img_in[i];
        hist[pixel]++;
    }
	/* change scale and trunc : from fixed point representation to integer */
	cumul=0;
	for (l = 0; l < LEVEL; l++)
	{
		cumul+=hist[l];
		maplevel =(cumul*ACCUMULATOR_STEP)>>24;
		LUT[l] = maplevel;
	}
}

/* in Hardware LUT will be an internal block RAM with low latency */
void histogram_equalization(unsigned char * img_out, unsigned char * img_in, int img_size)
{
    unsigned int lut[LEVEL];
    //unsigned int *lut=(unsigned int *)0x08000; //use local memory buffer
    int i;

    /* Reset :use VSync for Hardware */
    /* for (i = 0; i < LEVEL; i++)
     {
        LUT[i] = 0;
    } */
    memset(lut,(unsigned char)0,sizeof(int)*LEVEL);
    /* compute LUT */
    lut_comparatorcounter(img_in,lut);
    /* Get the result image */
    for (i = 0; i < img_size; i ++){
        if (lut[img_in[i]] > LEVEL_MAX){
            img_out[i] = LEVEL_MAX;
        }
        else{
            img_out[i] = (unsigned char)lut[img_in[i]];
        }
    }
}
