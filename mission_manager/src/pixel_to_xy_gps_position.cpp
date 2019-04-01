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
#include <cmath>   // tan

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
    return  std::abs(cam_height_pixel / cam_width_pixel) * hfov_lenght;
}

/*******************************************************************
 * DistanceTwoGpsPositions
 * Author : EM 
 * @param lat1 lat2 long1 long2, (all in radians!)
 * @out x y, (in meters)
 * 
 * Give the distance x and y from 2 GPS positions
 *******************************************************************/
void    DistanceTwoGpsPositions(double latitude_1, double latitude_2, 
                            double longitude_1, double longitude_2, 
                            double & x, double & y)
{
    double d_lat 	= latitude_2  - latitude_1;
    double d_long 	= longitude_2 - longitude_1;
    
    x = std::cos(latitude_2) * std::sin(d_long);
    y = (std::cos(latitude_1) * std::sin(latitude_2))  - 
        (std::sin(latitude_1) * std::cos(latitude_2) * std::cos(d_long) );

    // To get the result in meters, we have to multiply radians 
    // by the radius of the earth: 6 378 137 m
    x = x * kEarthRadiusMeters; 
    y = y * kEarthRadiusMeters;
}




/*******************************************************************
 * XYinPicToGpsPosition
 * Author : EM 
 * @param x, y (pixels coordinates)
 * @param curr_lat curr_long, (in DEGREES!)
 * @param hfov_lenght (in meters)
 * @param cam_width_pixel, cam_height_pixel (in pixels)
 * 
 * @out lat long, (in DEGREES)
 * 
 * Give the latitude and longitude coordinates to a given pixel position
 *******************************************************************/
void    XYinPicToGpsPosition(double x, double y, 
                            double current_latitude, double current_longitude,
                            double hfov_lenght, int cam_width_pixel, int cam_height_pixel,
                            double & latitude, double & longitude)
{
    int center_x       = cam_width_pixel  >> 1; // EM, Divide by 2 so >> 1
    int center_y       = cam_height_pixel >> 1;
    int x_distance_px  = std::abs(x - center_x);
    int y_distance_px  = std::abs(y - center_y);

    double  meters_per_px = hfov_lenght / cam_width_pixel;  // EM, pixels are squares so we only use hfov, 
                                                            // no need to use vfov

    double x_distance_meters = x_distance_px * meters_per_px;
    double y_distance_meters = y_distance_px * meters_per_px;

    latitude  = current_latitude  + ( y_distance_meters / kMetersPerLatDegree );
    longitude = current_longitude + ( x_distance_meters / (kMetersPerLatDegree * std::cos(current_latitude * M_PI / 180)) );
}                            


double  XYLenghtsToHypotenuse(double x_lenght, double y_lenght)
{
    return std::sqrt( std::pow(x_lenght, 2) + std::pow(y_lenght, 2) );
}

