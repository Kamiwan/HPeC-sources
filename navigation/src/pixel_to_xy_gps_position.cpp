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
 * @param angle_fov_camera, (in DEGREES!)
 * 
 * Give the lenght in meters of the camera horizontal fov
 *******************************************************************/
double HorizontalFOVLenght(double relative_altitude, double angle_fov_camera)
{
    // HFOV = 2 * relative_altitude * tan( AFOV / 2 )
    return  2 * relative_altitude * std::tan( (angle_fov_camera * (M_PI / 180)) / 2) ;
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
 * @param lat1 lat2 long1 long2, (all in DEGREES!)
 * @out x_lenght y_lenght, (in meters)
 * 
 * Give the distance x and y from 2 GPS positions
 *******************************************************************/
void    DistanceTwoGpsPositions(double latitude_1, double latitude_2, 
                            double longitude_1, double longitude_2, 
                            double & x_lenght, double & y_lenght)
{
    double rad_latitude_1, rad_latitude_2, rad_longitude_1, rad_longitude_2;
    rad_latitude_1  = latitude_1 * M_PI / 180;
    rad_latitude_2  = latitude_2 * M_PI / 180;
    rad_longitude_1 = longitude_1 * M_PI / 180;
    rad_longitude_2 = longitude_2 * M_PI / 180;

    double d_long 	= rad_longitude_2 - rad_longitude_1;
    
    x_lenght = std::cos(rad_latitude_2) * std::sin(d_long);
    y_lenght = (std::cos(rad_latitude_1) * std::sin(rad_latitude_2))  - 
        (std::sin(rad_latitude_1) * std::cos(rad_latitude_2) * std::cos(d_long) );

    // To get the result in meters, we have to multiply radians 
    // by the radius of the earth: 6371000 m
    x_lenght = x_lenght * kEarthRadiusMeters; 
    y_lenght = y_lenght * kEarthRadiusMeters;
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
                            double origin_latitude, double origin_longitude,
                            double hfov_lenght, int cam_width_pixel, int cam_height_pixel,
                            double & latitude, double & longitude)
{
    int center_x       = cam_width_pixel  >> 1; // EM, Divide by 2 so >> 1
    int center_y       = cam_height_pixel >> 1;
    int x_distance_px  = x - center_x;
    int y_distance_px  = y - center_y;

    double  meters_per_px = hfov_lenght / cam_width_pixel;  // EM, pixels are squares so we only use hfov, 
                                                            // no need to use vfov

    double x_distance_meters = x_distance_px * meters_per_px;
    double y_distance_meters = y_distance_px * meters_per_px;

    latitude  = origin_latitude  + ( y_distance_meters / kMetersPerLatDegree );
    //longitude = current_longitude + ( x_distance_meters / (kMetersPerLatDegree * std::cos(current_latitude * M_PI / 180)) );
    longitude = origin_longitude + (x_distance_meters / kEarthRadiusMeters) * (M_PI / 180) / std::cos(origin_latitude * M_PI / 180);
}                            


/*******************************************************************
 * XYLenghtsToHypotenuse
 * Author : EM 
 * @param x_lenght, y_lenght (in meters)
 * 
 * Use Pythagore theorem to get hypetnuse lenght in meters
 *******************************************************************/
double  XYLenghtsToHypotenuse(double x_lenght, double y_lenght)
{
    return std::sqrt( std::pow(x_lenght, 2) + std::pow(y_lenght, 2) );
}



/*******************************************************************
 * LatLongOffsetMeters
 * Author : EM 
 * @param x_lenght, y_lenght    (in meters)
 * @param curr_lat, curr_long   (in degrees)
 * @out   lat, long             (in degrees)
 * 
 * Give the new gps position (Lat-Long) with a translation vector in 
 * Cartesian coordinates in meters
 *******************************************************************/
void    LatLongOffsetMeters(double x_lenght, double y_lenght, 
        double current_latitude, double current_longitude, 
        double & latitude, double & longitude)
{
    latitude  = current_latitude  + ( y_lenght / kMetersPerLatDegree );
    longitude = current_longitude + (x_lenght / kEarthRadiusMeters) * (M_PI / 180) / std::cos(current_latitude * M_PI / 180);
}



