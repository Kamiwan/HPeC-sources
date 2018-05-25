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
#ifndef CORDIC_H
#define CORDIC_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "stdint.h"

#define PI 3.14159265358979323846


typedef int bool ;

/* Number of rotation */
#define NB_R 23
/* fractional part  = NB_R+Log2(NB_R) */
#define NFP 28
/* float to fixed point representation 4Q28 */
int32_t float2fix(double R);
double fix2float(int32_t F);
/* QUARTER =  297197971 */
# define QUARTER ((int)(PI / 2.0 * (1 << NFP)))
static int32_t lookup_angle_coeff[NFP] = {
/* MS 4 integral bits for radians */
210828714, 124459457, 65760959, 33381290, 16755422, 8385879,
4193963, 2097109, 1048571, 524287, 262144, 131072, 65536, 32768, 16384,
8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2};

int32_t cordic_atan(int argx, int argy);
void test_cordic();
#endif
