/* 
 * This file is part of the HPeC distribution (https://github.com/Kamiwan/HPeC-sources).
 * Copyright (c) 2018 Lab-STICC Laboratory.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
/*************************************************************************************
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on:  March 20, 2019
 * 
 * Utility file to get GPS or XY position (meters) from camera data
 *  
 *************************************************************************************/
#include "pixel_to_xy_gps_position.hpp"

#include <iostream>
#include <cmath>   // atan2

/*******************************************************************
 * HorizontalFOVLenght
 * Author : EM 
 * @param relative_altitude, distance from the ground (in meters)
 * @param angle_fov_camera, (in radians!)
 * 
 * Give the lenght in meters of the camera horizontal fov
 *******************************************************************/
double HorizontalFOVLenght(double relative_altitude, double angle_fov_camera)
{
    // HFOV = 2 * relative_altitude * tan( AFOV / 2 )
    std::cout << "relative_altitude = " << relative_altitude << std::endl;
    return  2 * relative_altitude * std::tan(angle_fov_camera / 2) ;
}


/*******************************************************************
 * VerticalFOVLenght
 * Author : EM 
 * @param hfov_lenght (in meters)
 * @param cam_width_pixel, cam_height_pixel  camera image size
 * 
 * Give the lenght in meters of the camera vertical fov
 *******************************************************************/
double VerticalFOVLenght(double hfov_lenght, double cam_width_pixel,  double cam_height_pixel)
{
    std::cout << "cam_height_pixel / cam_width_pixel = " << cam_height_pixel / cam_width_pixel << std::endl;
    return  std::abs(cam_height_pixel / cam_width_pixel) * hfov_lenght;
}

/*******************************************************************
 * TwoGpsPositionToXY
 * Author : EM 
 * @param lat1 lat2 long1 long2, (all in radians!)
 * @out x y, (in meters)
 * 
 * Give the distance x and y from 2 GPS positions
 *******************************************************************/
void    TwoGpsPositionToXY(double latitude_1, double latitude_2, 
                            double longitude_1, double longitude_2, 
                            double & x, double & y)
{
    double d_lat 	= latitude_2  - latitude_1;
    double d_long 	= longitude_2 - longitude_1;
    std::cout << "d_long = " << d_long << std::endl;
    
    x = std::cos(latitude_2) * std::sin(d_long);
    y = (std::cos(latitude_1) * std::sin(latitude_2))  - 
        (std::sin(latitude_1) * std::cos(latitude_2) * std::cos(d_long) );

    // To get the result in meters, we have to multiply radians by the radius of the earth: 6371 km
    // Then, multiplied it by 1000 to get result in meters
    x = x * 6371 * 1000; 
    y = y * 6371 * 1000;
}


