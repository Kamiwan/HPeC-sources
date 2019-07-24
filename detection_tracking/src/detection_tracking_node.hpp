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
 * Created on: November, 2018
 *
 * Main header of Detection and Tracking ROS node. Use OpenTLD for the software version.
 *************************************************************************************/
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <boost/thread.hpp>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <iostream>

extern "C" {
#include "debug.h"
}

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"

#include "main.hpp" //EM, HIL definition there
#include "utils.h"


void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void termination_handler(int signum);

void detection_tracking_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);
void detection_tracking_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);

void managing_controller_request(const std_msgs::Int32::ConstPtr &value);
void stop();
