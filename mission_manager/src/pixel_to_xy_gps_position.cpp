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




/******************************************************************************
 * XYinPicToGpsPosition
 * Author : EM 
 * @param x, y                              (pixels coordinates)
 * @param cam_width_pixel, cam_height_pixel (in pixels)
 * @param hfov_lenght                       (in meters)
 * @param theta                             (in degrees)
 * @param curr_lat curr_long,               (in DEGREES!)
 * 
 * @out lat long,                           (in DEGREES)
 * 
 * Give the latitude and longitude coordinates to a given pixel position
 * 
 * x and y coordinates are based as the topleft corner of the picture is 0,0
 * theta is the orientation angle of the picture compared to the North
 * curr_lat curr_long are GPS coordinates that represents the picture center
 *****************************************************************************/
void    XYinPicToGpsPosition(double x, double y, int cam_width_pixel, int cam_height_pixel,
                            double hfov_lenght, double theta, 
                            double current_latitude, double current_longitude,
                            double & latitude, double & longitude)
{
    // We have to translate the origin of the coordinate system to the picture center
    int relative_to_center_x = x - cam_width_pixel/2;
    int relative_to_center_y = -(y - cam_height_pixel/2);
    //We negate y because in the picture axis, y increases by going down and we want the opposite

    int absolute_x, absolute_y;
    XYPositionAxisRotation(relative_to_center_x, relative_to_center_y, theta,
                           absolute_x, absolute_y);
        
    double x_center_distance, y_center_distance;
    XYLenghtOffsetPixels(absolute_x, absolute_y, 0, 0,
                         hfov_lenght, cam_width_pixel,
                         x_center_distance, y_center_distance);

    LatLongOffsetMeters(x_center_distance, y_center_distance, 
        current_latitude, current_longitude, 
        latitude, longitude);
    //longitude = current_longitude + ( x_distance_meters / (kMetersPerLatDegree * std::cos(current_latitude * M_PI / 180)) );
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



/*******************************************************************
 * XYLenghtOffsetPixels
 * Author : EM 
 * @param x_pix, y_pix,                       (in pixels)
 * @param x_origin_pix, y_origin_pix          (in pixels)
 * @param cam_width_pixel                     (in pixels)
 * @param hfov_lenght                         (in meters)
 * @out   x_lenght, y_lenght                  (in meters)
 * 
 * Give the x and y distance in meters of 2 pixels in a picture
 *******************************************************************/
void    XYLenghtOffsetPixels(int x_pix, int y_pix, 
        int x_origin_pix, int  y_origin_pix,
        double hfov_lenght, double cam_width_pixel, 
        double & x_lenght, double & y_lenght)
{
    int x_distance_px  = x_pix - x_origin_pix;
    int y_distance_px  = y_pix - y_origin_pix;

    double  meters_per_px = hfov_lenght / cam_width_pixel;  // EM, pixels are squares so we only use hfov, 
                                                            // no need to use vfov
    x_lenght = x_distance_px * meters_per_px;
    y_lenght = y_distance_px * meters_per_px;
}
         

/*******************************************************************
 * XYPositionAxisRotation
 * Author : EM 
 * @param x_pix, y_pix,                 (in pixels)
 * @param theta                         (in degrees)
 * @out   rotated_x_pix, rotated_y_pix  (in pixels)
 * 
 * x' and y' axes are obtained by rotating the x and y axes 
 * counterclockwise through an angle theta
 *******************************************************************/
void    XYPositionAxisRotation(int x_pix, int y_pix, double theta,
                                int & rotated_x_pix, int & rotated_y_pix)
{
    double rad_theta = theta * M_PI / 180;
    rotated_x_pix =  x_pix * std::cos(rad_theta) + y_pix * std::sin(rad_theta);
    rotated_y_pix = -x_pix * std::sin(rad_theta) + y_pix * std::cos(rad_theta);
}

