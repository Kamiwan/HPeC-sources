/*************************************************************************************
 * File   : moments.h, file for Vision Based Autonomous Landing
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
#ifndef MOMENTS_H
#define MOMENTS_H
typedef struct{
    double hu_0;
    double hu_1;
    double hu_2;
    double hu_3;
    double hu_4;
    double hu_5;
    double hu_6;
    double eccentricity;
} MOMENTS;

void moments(unsigned char * img_in, unsigned char * img_out,COMPONENT *ccl, MOMENTS *humref, int nb_components, double uncerntainty);
#endif
