/*************************************************************************************
 * File   : moments.c, file for Vision Based Autonomous Landing
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
#include "moments.h"
#include "math.h"
//!!!image (column,row)
#define image(a,b) img_in[a+(b)*IMAGE_WIDTH] 

//#define VERBOSE

/* see opencv source /master/modules/imgproc/src/moments.cpp : opencl, neon, optimisation etc...*/

double sumofpower1(int x1b,int x2b)
{
   double x1 = (double)(x1b);
   double x2 = (double)(x2b);
   double result=((x2*(x2+1))-(x1*(x1-1)))/2.0;
   return result;
}

double sumofpower2(int x1b,int x2b)
{
   double x1 = (double)(x1b);
   double x2 = (double)(x2b);
   double result=(double)((x2*(x2+1)*(2*x2+1))-(x1*(x1-1)*(2*x1-1)))/6.0;
   return result;
}

double sumofpower3(int x1b,int x2b)
{
   double x1= (double)(x1b);
   double x2= (double)(x2b);
   double result=((x2*x2*(x2+1)*(x2+1))-(x1*x1*(x1-1)*(x1-1)))/4.0;
   return result;
}

void moments(unsigned char * img_in, unsigned char * img_out,COMPONENT *ccl, MOMENTS *humref, int nb_components, double uncerntainty)
{
    int i,x,x1,x2,y,xmin,xmax,ymin,ymax,line;
    int root,rootT;
    double m00,m01,m02,m03,m10,m11,m12,m20,m21,m30;
    double sop1,sop2,sop3,yp2,yp3;
    double xg,yg;
    double mu00,mu20,mu02,mu11,mu30,mu12,mu21,mu03;
    double inv_m00,s2,s3;
    double n20,n02,n11,n30,n12,n21,n03;
    double t0,t1,q0,q1,n4,s,d;
    double phi0,phi1,phi2,phi3,phi4,phi5,phi6;
    double errhu0,errhu1,errhu2,errhu3,errhu4,errhu5,errhu6,error;
    double nb_points;  
    //find blocks on connected component  
    for (i=1;i<nb_components;i++)
    {
       if (ccl[i].skip==0)
       {
      	  printf("Compute Moments of component label %d, area %d, rect[%d,%d,%d,%d]\r\n",i,
		ccl[i].area,ccl[i].xmin,ccl[i].ymin,ccl[i].xmax,ccl[i].ymax);
	  root=i;
  	  //printf("root = %d\r\n",root);
	  m00=0.0;m01=0.0;m02=0.0;m03=0.0;m10=0.0;
	  m11=0.0;m12=0.0;m20=0.0;m21=0.0;m30=0.0;
	  ymin=ccl[i].ymin;
	  ymax=ccl[i].ymax;
   	  xmin=ccl[i].xmin;
	  xmax=ccl[i].xmax;
         //find blocks of components
	  //for each row, find segments of connected points
	  line=0;
	  for (y=ymin;y<=ymax;y++)
          {
		//find first points of connected component
		x=xmin;
		//printf("image(x,y) = %d\r\n",image(x,y));
		while (x<=xmax)
		{
			//find one segment(block) of connected points
			while ((x<xmax) && (image(x,y)!=root)) 
			{
			 	//printf("image(x,y) = %d\r\n",image(x,y));
				x++;
			}
			x1=x;x2=0;
			while ((x<=xmax) && (image(x,y)==root))
			{
				//printf("image(x,y) = %d\r\n",image(x,y));
				x2=x;
				x++;
			}
			if (x2==0) x=xmax+1;//no more segment in current line
		        else /* if (x1!=x2) */
			{
				//printf("segment[%d,%d]\r\n",x1,x2);
				//compute ten moments for block x1,x2,y,y of connected points: m00,m01,m10,m11...
				yp2=y*y;yp3=yp2*y;
				sop1=sumofpower1(x1,x2);
				sop2=sumofpower2(x1,x2);
				sop3=sumofpower3(x1,x2);
				nb_points=(double)(x2-x1)+1.0;
				//geometrical moments
				m00+=nb_points;m01+=nb_points*y;m02+=nb_points*yp2;m03+=nb_points*yp3;
				m10+=sop1;m11+=sop1*y;m12+=sop1*yp2;
				m20+=sop2;m21+=sop2*y;m30+=sop3; 
			}
		} 
	  }
          #ifdef VERBOSE
	  printf("m00= %f\r\n",m00);	
	  printf("m01= %f\r\n",m01);	
	  printf("m02= %f\r\n",m02);	
	  printf("m03= %f\r\n",m03);	
	  printf("m10= %f\r\n",m10);	
	  printf("m11= %f\r\n",m11);	
	  printf("m12= %f\r\n",m12);	
	  printf("m20= %f\r\n",m20);	
	  printf("m21= %f\r\n",m21);	
	  printf("m30= %f\r\n",m30);	
	  #endif
 	  //center of gravity
	  inv_m00=1.0/m00;
	  xg=m10*inv_m00;yg=m01*inv_m00;
	  //central moments (translation invariance)
	  mu00=m00;
	  //mu11 = m11 - m10*cy 
	  mu11=m11-yg*m10;
	  // mu20 = m20 - m10*cx
	  mu20=m20-xg*m10;
	  //mu02 = m02 - m01*cy
	  mu02=m02-yg*m01;
	 // mu21 = m21 - cx*(2*mu11 + cx*m01) - cy*mu20
	  mu21=m21-xg*(2*mu11+xg*m01)-yg*mu20;
	  //mu12 = m12 - cy*(2*mu11 + cy*m10) - cx*mu02
	  mu12=m12-yg*(2*mu11+yg*m10)-xg*mu02;
	  //mu30 = m30 - cx*(3*mu20 + cx*m10)
	  mu30=m30-xg*(3*mu20+xg*m10);
	  // mu03 = m03 - cy*(3*mu02 + cy*m01)
	  mu03=m03-yg*(3*mu02+yg*m01);


          #ifdef VERBOSE
	  printf("u00= %f\r\n",mu00);	
	  printf("u11= %f\r\n",mu11);	
	  printf("u02= %f\r\n",mu02);	
	  printf("u20= %f\r\n",mu20);	
	  printf("u21= %f\r\n",mu21);	
	  printf("u12= %f\r\n",mu12);	
	  printf("u30= %f\r\n",mu30);	
	  printf("u03= %f\r\n",mu03);	
	  #endif

	  /* normalized central moments (invariance with respect to isometric scale
	   and to scalar intensity changes */			
	  s2=inv_m00*inv_m00;
	  n20=mu20*s2;
	  n02=mu02*s2; n11=mu11*s2;
	  s3=s2*sqrt(fabs(inv_m00)); 
          #ifdef VERBOSE
	  printf("inv_m00 = %f\r\n",inv_m00);
	  printf("s2 = %f\r\n",s2);
	  printf("s3 = %f\r\n",s3);
	  #endif
	  n30=mu30*s3; n12=mu12*s3;
	  n21=mu21*s3;n03=mu03*s3;
          #ifdef VERBOSE
	  printf("nu02= %f\r\n",n02);	
	  printf("nu11= %f\r\n",n11);	
	  printf("nu02= %f\r\n",n02);	
	  printf("nu30= %f\r\n",n30);	
	  printf("nu21= %f\r\n",n21);	
	  printf("nu12= %f\r\n",n12);	
	  printf("nu03= %f\r\n",n03);	
	  #endif
	  /*Hu invariant moments (area,variance, skewness and kurtosis...) */
	  t0 = n30 + n12;
	  t1 = n21 + n03;
	  q0 = t0 * t0;
	  q1 = t1 * t1;
	  n4 = 4 * n11;
	  s = n20 + n02;
	  d = n20 - n02;
	  phi0 = s;
    	  phi1 = d * d + n4 * n11;
	  phi3 = q0 + q1;
    	  phi5 = d * (q0 - q1) + n4 * t0 * t1;
    	  t0 *= q0 - 3 * q1;
          t1 *= 3 * q0 - q1;
	  q0 = n30 - 3 * n12;
          q1 = 3 * n21 - n03;
	  phi2 = q0 * q0 + q1 * q1;
	  phi4 = q0 * t0 + q1 * t1;
    	  phi6 = q1 * t0 - q0 * t1;
	  printf("m00 = ii = area = %f\r\n",m00);
	  printf("center of gravity = [%f,%f]\r\n",xg,yg);
	  printf("hu_0=  %f\r\n",phi0);
	  printf("hu_1=  %f\r\n",phi1);
	  printf("hu_2=  %f\r\n",phi2);
	  printf("hu_3=  %f\r\n",phi3);
	  printf("hu_4=  %f\r\n",phi4);
	  printf("hu_5=  %f\r\n",phi5);
	  printf("hu_6=  %f\r\n",phi6);
	  errhu0=(phi0-humref->hu_0);
	  errhu1=(phi1-humref->hu_1);
	  errhu2=(phi2-humref->hu_2);
	  errhu3=(phi3-humref->hu_3);
	  errhu4=(phi4-humref->hu_4);
	  errhu5=(phi5-humref->hu_5);
	  errhu6=(phi6-humref->hu_6);
	  error=sqrt(errhu0*errhu0+errhu1*errhu1+errhu2*errhu2+errhu3*errhu3+errhu4*errhu4+errhu5*errhu5
	             +errhu6*errhu6);
	  printf("error=%f\r\n",error);
	 if (error>uncerntainty)
	 {
	   ccl[i].skip=1;
      	   printf("Skip component label %d, area %d, rect[%d,%d,%d,%d]\r\n",i,
	   ccl[i].area,ccl[i].xmin,ccl[i].ymin,ccl[i].xmax,ccl[i].ymax);
	 }
	  else 
	  {
      	   printf("Keep component label %d, area %d, rect[%d,%d,%d,%d] root=%d\r\n",i,
	   ccl[i].area,ccl[i].xmin,ccl[i].ymin,ccl[i].xmax,ccl[i].ymax,i);
			rootT=i;
       	  }
       }
    }
    //keep only helipad T
    for (i = 0; i < IMAGE_SIZE; i ++)
    {
	if (img_in[i]==rootT)
		img_out[i]=0;
	else
		img_out[i]=255;
    }
}
