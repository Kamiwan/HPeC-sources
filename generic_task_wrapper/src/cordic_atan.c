/*************************************************************************************
 * File   : cordic_atan.c, file for Vision Based Autonomous Landing
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
#include <stdlib.h>
#include "image.h"
#include "math.h"
#include "stdint.h"
#include "cordic_atan.h"

/* float to fixed point representation 4Q28 */
int32_t float2fix(double R)
{
 return (int32_t)(R * (1<<NFP) + (R>=0 ? 0.5 : -0.5));
}
/* 4Q28 to float */
double fix2float(int32_t F)
{
 return (double)F / (1<<NFP);
}

int32_t cordic_atan(int argx, int argy)
{
	int32_t x, y, theta; int i;
	x = argx;y = argy;
	/* quadrant correction : Get the vector into the right half plane */
        if (y >= 0)
          theta = QUARTER; 
        else
          theta = -QUARTER; 
        if (x<0)
          theta = -theta; 
	/* micro rotation */
	for (i = 0; i < NB_R; ++i)
	 {
		int32_t angle = lookup_angle_coeff[i];
		int32_t x_inc = (y >> i);
		int32_t y_inc =(x >> i);
		bool x_sign_bit = x < 0;
		bool y_sign_bit = y < 0;
		bool different_sign = x_sign_bit ^ y_sign_bit;

		if (different_sign)
		 { /* Rotate positive */
			x += x_inc;y -= y_inc;theta += angle;
		}
		else
		 {/* Rotate negative */
			x -= x_inc; y += y_inc; theta -= angle;
		}

	}
	return theta;
}

void test_cordic()
{
	double atanfloat, erroratan,cordicfloat;
	int32_t atanfixed;
	printf("error atan\n");
	short gx,gy;
	//test all quadrants
	   gx=-1020;gy=-1020;
	      atanfloat=atan((double)gy/(double)gx);
	      atanfixed=cordic_atan(gx,gy);
	      cordicfloat = fix2float(atanfixed);
	      erroratan = fabs(atanfloat - cordicfloat);
	      printf("%ld %ld %f %f %f\r\n",gx,gy,atanfloat,cordicfloat,erroratan);
	   gx=-1020;gy=1020;
	      atanfloat=atan((double)gy/(double)gx);
	      atanfixed=cordic_atan(gx,gy);
	      cordicfloat = fix2float(atanfixed);
	      erroratan = fabs(atanfloat - cordicfloat);
	      printf("%ld %ld %f %f %f\r\n",gx,gy,atanfloat,cordicfloat,erroratan);
	   gx=1020;gy=-1020;
	      atanfloat=atan((double)gy/(double)gx);
	      atanfixed=cordic_atan(gx,gy);
	      cordicfloat = fix2float(atanfixed);
	      erroratan = fabs(atanfloat - cordicfloat);
	      printf("%ld %ld %f %f %f\r\n",gx,gy,atanfloat,cordicfloat,erroratan);
	   gx=1020;gy=1020;
	      atanfloat=atan((double)gy/(double)gx);
	      atanfixed=cordic_atan(gx,gy);
	      cordicfloat = fix2float(atanfixed);
	      erroratan = fabs(atanfloat - cordicfloat);
	      printf("%ld %ld %f %f %f\r\n",gx,gy,atanfloat,cordicfloat,erroratan);
	//test specific values (gx->0))
	   gx=0;gy=1020;
	      atanfixed=cordic_atan(gx,gy);
	      cordicfloat = fix2float(atanfixed);
		printf("%f\r\n",cordicfloat);
	   gx=0;gy=-1020;
	      atanfixed=cordic_atan(gx,gy);
	      cordicfloat = fix2float(atanfixed);
		printf("%f\r\n",cordicfloat);
}
