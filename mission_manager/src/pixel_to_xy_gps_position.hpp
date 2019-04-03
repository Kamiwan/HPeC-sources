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
 * Utility header to get GPS or XY position (meters) from camera data
 *      It also computes the Horizontal and Vertical FOV in meters from the angle value
 *  
 *************************************************************************************/

#ifndef PIXEL_TO_XY_GPS_POSITION_HPP
#define PIXEL_TO_XY_GPS_POSITION_HPP

static constexpr double kMetersPerLatDegree = 111111.1;
static constexpr double kEarthRadiusMeters  = 6371000;

double  HorizontalFOVLenght(double relative_altitude, double angle_fov_camera);
double  VerticalFOVLenght(double hfov_lenght, double cam_width_pixel,  double cam_height_pixel);
void    DistanceTwoGpsPositions(double latitude_1, double latitude_2, 
                            double longitude_1, double longitude_2, 
                            double & x_lenght, double & y_lenght);

void    XYinPicToGpsPosition(double x, double y, 
                            double current_latitude, double current_longitude,
                            double hfov_lenght, int cam_width_pixel, int cam_height_pixel,
                            double & latitude, double & longitude);

double  XYLenghtsToHypotenuse(double x_lenght, double y_lenght);

void    LatLongOffsetMeters(double x_lenght, double y_lenght, 
        double current_latitude, double current_longitude, 
        double & latitude, double & longitude);

#endif
