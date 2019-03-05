
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
 * File   : navigation_node.h, file to create easily ros nodes for HPeC
 * Copyright (C) 2019 Lab-STICC Laboratory
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: February 21, 2019
 * 
 *************************************************************************************/

#ifndef NAV_COMMAND_HPP
#define NAV_COMMAND_HPP

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <geometry_msgs/PoseStamped.h>

#include <iostream>
#include <math.h>   /* atan2 */

#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/String.h"
#include "geometry_msgs/TwistStamped.h"
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include "sensor_msgs/Imu.h"
#include <tf/transform_datatypes.h>

#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/GlobalPositionTarget.h>

#include "communication/nav_control.h"



class NavCommand
{
public:
    NavCommand(/* args */);
    ~NavCommand();


private:    
    ros::NodeHandle nh_;

};








#endif
