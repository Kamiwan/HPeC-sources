/*************************************************************************************
 * File   : cca.h, file for Vision Based Autonomous Landing
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
#ifndef CCA_H
#define CCA_H
#define MAX_LABELS 255
/* Union Find Data Structure: equivalence array */
typedef struct{
    int *root;
    int n;
} UNION_FIND;


typedef struct{
    int xmin,ymin,xmax,ymax;
    int area;
    int skip;
} COMPONENT;


COMPONENT *connected_components_analysis(unsigned char * img_in, unsigned char *img_out,int area_min, int area_max, int *nb_components);
#endif
