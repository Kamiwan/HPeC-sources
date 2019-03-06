
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

#include <iostream>
#include <math.h>   // atan2

#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/String.h"
#include "geometry_msgs/TwistStamped.h"
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/NavSatFix.h>
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
    NavCommand(ros::NodeHandle* nodehandle);
    ~NavCommand();


 private:   
    // ### Attributes ###
    // NodeHandle variable to use ROS topics and services
    ros::NodeHandle nh_;

    // Topic subscribers to get data from MAVROS
    ros::Subscriber state_sub_;
    ros::Subscriber pos_sub_;
    ros::Subscriber compass_heading_sub_;

    // Topic publishers to send new movement order
    ros::Publisher local_pos_pub_;
    ros::Publisher global_pos_pub_;
    ros::Publisher cmd_vel_pub_;

    // Services for one-time orders
    ros::ServiceClient arming_client_;
    ros::ServiceClient takeoff_client_;
    ros::ServiceClient landing_client_;
    ros::ServiceClient set_mode_client_;

    // Messages attributes for services
    mavros_msgs::SetMode     set_mode_cmd_;
    mavros_msgs::CommandBool arm_cmd_;
    mavros_msgs::CommandTOL  takeoff_cmd_;
    mavros_msgs::CommandTOL  landing_cmd_;

    // Attributes to store current and destination data
    mavros_msgs::State  current_state_;
    std_msgs::Float64   current_heading_;
    double              current_altitude_;
    double              current_latitude_;
    double              current_longitude_;
    bool                flying_;

    double              destination_altitude_;
    double              destination_latitude_;
    double              destination_longitude_;

    enum NavOrder {
        LAND = 0,
        TAKEOFF,
        GPS_MOVE,
        VELOCITY_MOVE,
        OBS_AVOIDANCE_MOVE,
        TRACKING_MOVE,
        INVALID_ORDER
    };

    //!\ You must adapt these values to your context
    static constexpr double kHomeAltitude  = 603.4486;
    static constexpr double kHomeLatitude  = -35.363261;
    static constexpr double kHomeLongitude = 149.16523;

    // ### Methods ###
    void InitializeSubscribers();
    void InitializePublishers();
    void InitializeServices();

    void StateCallback(const mavros_msgs::State::ConstPtr& msg);
    void CompassCallback(const std_msgs::Float64::ConstPtr& msg);
    void GpsCallback(const sensor_msgs::NavSatFix::ConstPtr& position);
    void ControlCallback(const communication::nav_control::ConstPtr& next_order);

    NavOrder ResolveNavOrder(std::string input);

};








#endif
