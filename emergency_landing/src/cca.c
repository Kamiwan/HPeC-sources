/*************************************************************************************
 * File   : cca.c, file for Vision Based Autonomous Landing
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
#include "cca.h"
#include "debug.h"
#define PIXEL_CCA 0


/* TwoPass algorithm https://en.wikipedia.org/wiki/Connected-component_labeling 
see article: A new Parallel Algorithm for two-pass Connect Component Labelling */



/* 	Erwan Moréac, User comparison function for qsort()  
	Used to sort areas founded by connected_components_analysis()
	The order is: from the nearest to the farthest of image center	*/
int compare_position (void const *a, void const *b)
{
   /* Define and initialize pointer's types*/
   COMPONENT const *pa = a;
   COMPONENT const *pb = b;

	/* IF component a or b is skipable, then it's higher to make them go to the end of the array*/
	if(pa->skip)
	{
		return 1;
	}
	else if(pb->skip)
	{
		return -1;
	} 
	else
	{
		/* Intermediate computation to alleviate the writing */
		int distA = 0, distB = 0;
		distA = abs( (((pa->xmax+pa->xmin)>> 1)-IMAGE_CENTER_X) + (((pa->ymax+pa->ymin)>> 1)-IMAGE_CENTER_Y) );
		distB = abs( (((pb->xmax+pb->xmin)>> 1)-IMAGE_CENTER_X) + (((pb->ymax+pb->ymin)>> 1)-IMAGE_CENTER_Y) );

		/* Evaluate and return the state of the comparison (increasing order) */
		return distA - distB;
	}
}




/* Create a UF for n elements */
UNION_FIND init_unionfind(int n)
{
    UNION_FIND uf;
    int k;
    uf.n = n;
    uf.root= (int *)malloc(n * sizeof(int));
    for (k = 0; k < n; k++)
    {
	uf.root[k]   = 0;
    }
    return uf;
}

void free_unionfind(UNION_FIND *uf)
{
   free(uf->root);
   uf->n=-1;
}


int merge(UNION_FIND *uf,int x,int y)
{
   int rootx,rooty,z;

   rootx=x;rooty=y;
   while (uf->root[rootx]!=uf->root[rooty])
   {
	if (uf->root[rootx]>uf->root[rooty])  
	{
		if (rootx==uf->root[rootx]) 
		{
			uf->root[rootx]=uf->root[rooty];
			return uf->root[rootx];
		}
		z=uf->root[rootx];uf->root[rootx]=uf->root[rooty];
		rootx=z;
	}
	else
	{
		if (rooty==uf->root[rooty]) 
		{
			uf->root[rooty]=uf->root[rootx];
			return uf->root[rootx];
		}
		z=uf->root[rooty];uf->root[rooty]=uf->root[rootx];
		rooty=z;
	}
   }
   return uf->root[rootx];
}

int flatten(UNION_FIND *uf,int count)
{
   int i,k;
   k=1;
   for (i = 1; i < count; i ++)
   {
      if (uf->root[i]<i) uf->root[i]=uf->root[uf->root[i]];
      else 
      {
	uf->root[i]=k;
        k++;
      }		
   }
   return k;
}


/* 
 ___ ___ ___
|_a_|_b_|_c_|
|_d_|_e_| 

image(e) = image(row,column)
image(b) = image(row-1,column) ...
*/

#define image(a,b) ((a>=0) && (b>=0) && (b<IMAGE_WIDTH)) ? img_in[(a)*IMAGE_WIDTH+b] : 255-PIXEL_CCA
#define labels(a,b) labels[(a)*IMAGE_WIDTH+b] 
COMPONENT *connected_components_analysis(unsigned char * img_in,unsigned char *img_out,int area_min, int area_max, int *nb_components)
{
  UNION_FIND uf; /* equivalence info */
  unsigned char pixel;
  int row,column,i,j,root;
  int *labels;
  int cpt;
  COMPONENT *ccl;
  COMPONENT *ccl_limited;
  int count_labels;

  labels = (int *)malloc(IMAGE_SIZE * sizeof(int));
  //memset(labels, 0, IMAGE_SIZE* sizeof(int)); /* assume labels as integer image   */
  uf=init_unionfind(65535);
  count_labels=1; /*   PIXEL_CCA foreground */
  /* scan phase decision tree : e is the current pixel to be labeled */
  for(row=0; row < IMAGE_HEIGH; row++)
	   for(column=0; column < IMAGE_WIDTH; column++)
   {
	pixel = image(row,column); /* image(e) */
	if (pixel==PIXEL_CCA) 
        {
		pixel = image(row-1,column); /* image(b) */
		if (pixel==PIXEL_CCA) 
			labels(row,column)=labels(row-1,column); /* copy(b) */
		else 
		{
		   pixel = image(row-1,column+1); /* image(c) */
		   if (pixel==PIXEL_CCA) 
		   {
		      pixel = image(row-1,column-1); /* image(a) */
		      if (pixel==PIXEL_CCA) 
                      {
			 labels(row,column)=merge(&uf,labels(row-1,column+1),
						  labels(row-1,column-1)); /* copy (c,a) */
                      }
		      else
		      {
			 pixel=image(row,column-1); /* image(d) */
		         if (pixel==PIXEL_CCA) 
                         {
			    labels(row,column)=merge(&uf,labels(row-1,column+1),
						  labels(row,column-1)); /* copy (c,d) */
                         }
		         else
		         {
				labels(row,column)=labels(row-1,column+1); /* copy(c) */
		         }
		      }
		   }
		   else
		   {
		      pixel = image(row-1,column-1); /* image(a) */
		      if (pixel==PIXEL_CCA) 
                      {
			labels(row,column)=labels(row-1,column-1); /* copy(a) */
		      }	
		      else
		      {
			 pixel=image(row,column-1); /* image(d) */
		         if (pixel==PIXEL_CCA)
                         {
			    labels(row,column)=labels(row,column-1); /* copy(d) */
		         }	
		         else 
		         {
			    /* new label */
			    labels(row,column)=count_labels;
			    uf.root[count_labels]=count_labels;
			    count_labels++;
		         }
                      }			
		   }
		}
	}	
	else
	    labels(row,column)=0;

   }
   dbprintf("Provisional labels count : %d\r\n",count_labels);
   /* Analysis phase */
   count_labels=flatten(&uf,count_labels);
   dbprintf("Effective labels count : %d\r\n",count_labels);
   ccl=(COMPONENT *)malloc((count_labels+1)* sizeof(COMPONENT));
   for (i=0;i<count_labels;i++)
   {
     ccl[i].area=0;
     ccl[i].xmin=IMAGE_WIDTH;
     ccl[i].xmax=0;
     ccl[i].ymin=IMAGE_HEIGH;
     ccl[i].ymax=0;
     ccl[i].skip=0;
   }
   /* labeling Phase */
   i=0;
   for(row=0; row < IMAGE_HEIGH; row++)
	   for(column=0; column < IMAGE_WIDTH; column++)
   {
	    pixel = image(row,column);
	    root=uf.root[labels[i]];	
	    labels[i]=root;
	    ccl[root].area++;
	    if (ccl[root].xmin>column) ccl[root].xmin=column;
	    if (ccl[root].xmax<column) ccl[root].xmax=column;
	    if (ccl[root].ymin>row) ccl[root].ymin=row;
	    if (ccl[root].ymax<row) ccl[root].ymax=row;
	    i++;
    }
	/* Filter Phase: skip too small/too big components */
    for (i = 0; i < IMAGE_SIZE; i ++)
    {
   	    root=labels[i];		
	    if ((root!=0) && (ccl[root].area>area_min) && (ccl[root].area<area_max))
            {
	        img_out[i] = 255;
	    }
	    else 
	    {
		if (ccl[root].skip==0) ccl[root].skip=1;
		img_out[i]=0;
	    }
    }
	/* info */
    for (i=0;i<count_labels;i++)
    {
      if (ccl[i].skip==0)
      	dbprintf("Component label %d, area %d, rect[%d,%d,%d,%d]\r\n",i,ccl[i].area,ccl[i].xmin,
		ccl[i].ymin,ccl[i].xmax,ccl[i].ymax);
    } 

	/* Erwan Moréac, sort areas from the nearest of the picture center to the farthest */
	qsort(ccl, count_labels, sizeof *ccl, compare_position);

    /* info */
    for (i=0;i<count_labels;i++)
    {
      if (ccl[i].skip==0)
      	dbprintf("SORTED Component label %d, area %d, rect[%d,%d,%d,%d]\r\n",i,ccl[i].area,ccl[i].xmin,
		ccl[i].ymin,ccl[i].xmax,ccl[i].ymax);
    } 

	/*Erwan Moréac, reduce the output array ccl to an array of MAX_LABELS_OUTPUT COMPONENT if higher */
	if(count_labels > MAX_LABELS_OUTPUT){
		ccl_limited=(COMPONENT *)malloc((MAX_LABELS_OUTPUT)* sizeof(COMPONENT));
		for (int j=0;j<MAX_LABELS_OUTPUT;j++)
    	{
			ccl_limited[j]=ccl[j];
			dbprintf("CCL_LIMITED %d, area %d, rect[%d,%d,%d,%d]\r\n",j,ccl_limited[j].area,ccl_limited[j].xmin,
		ccl_limited[j].ymin,ccl_limited[j].xmax,ccl_limited[j].ymax);
		}

		free_unionfind(&uf);
    	free(labels);
		free(ccl); //ce free était commenté
    	*nb_components=MAX_LABELS_OUTPUT;
    	return ccl_limited;
	} else{
		free_unionfind(&uf);
    	free(labels);
    	*nb_components=count_labels;
    	return ccl;
	}
}
