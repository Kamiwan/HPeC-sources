/*************************************************************************************
 * File   : gaussianfilter.h, file for Vision Based Autonomous Landing
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
#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H
void gaussian1_reset_SOF();
void gaussian1_sliding_windown(int pixel_in, int * pixel_out);
void gaussian2_reset_SOF();
void gaussian2_sliding_windown(int pixel_in, int * pixel_out);
void gaussian3_reset_SOF();
void gaussian3_sliding_windown(int pixel_in, int * pixel_out);
#endif
