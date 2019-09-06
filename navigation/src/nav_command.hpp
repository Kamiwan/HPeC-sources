
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
 * File   : nav_command.hpp, 
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

#include "tld_msgs/BoundingBox.h"
#include "communication/nav_control.h"

#define RAD_360     6.28319
#define DEG_TO_RAD  0.0174532925
#define RAD_90      1.57079

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

    // Topic subscriber to get orders from the navigation USER
    ros::Subscriber nav_order_sub_;

    // Topic subscriber to get target object position from detection_tracking ROS node
    ros::Subscriber tracked_object_pos_sub_;

    // Topic publishers to send new movement order
    ros::Publisher local_pos_pub_;
    ros::Publisher global_pos_pub_;
    ros::Publisher cmd_vel_pub_;

    // Services for one-time orders
    ros::ServiceClient arming_client_;
    ros::ServiceClient takeoff_client_;
    ros::ServiceClient landing_client_;
    ros::ServiceClient set_mode_client_;

    // Messages attributes for services and topics
    mavros_msgs::SetMode              set_mode_cmd_;
    mavros_msgs::CommandBool          arm_cmd_;
    mavros_msgs::CommandTOL           takeoff_cmd_;
    mavros_msgs::CommandTOL           landing_cmd_;
    mavros_msgs::GlobalPositionTarget next_gps_position_;

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

    // Attributes to follow a target (TRACKING_MOVE)
    // Pixels data
    int     target_x_, target_y_;
    int     previous_target_x_, previous_target_y_;
    int     absolute_target_x_, absolute_target_y_;
    int     delta_target_x_, delta_target_y_;
    // Timing data
    double  target_time_, previous_target_time_, delta_time_;
    // Confidence data
    float   target_confidence_, prev_target_confidence_;
    // Latitude Longitude data
    double  target_latitude_, target_longitude_;
    double  prev_target_latitude_,prev_target_longitude_;
    // Distance data in meters
    double  delta_target_meters_x_, delta_target_meters_y_;
    double  distance_from_prev_position_, target_uav_distance_;
    double  uav_dist_x_, uav_dist_y_;
    // Speed data
    double  target_speed_, prev_target_speed;
    // Flags
    bool    is_target_;
    bool    first_detection_;
    bool    tracking_online_;
    bool    updated_tracking_data;
    
    // Yaw is in radian, so I give a default value outside -3.14 < val < 3.14
    // Because values 0 and -1 can be relevant
    static constexpr double kDefaultNoYaw = 42.0;

    //!\ You must adapt these values to your context
    static constexpr double kHomeAltitude   = 603.4486;
    static constexpr double kHomeLatitude   = -35.363261;
    static constexpr double kHomeLongitude  = 149.16523;
    // Camera caracteristics used for TRACKING_MOVE
    static constexpr double kHFOV           = 60.0; //1.0472 in radian
    static constexpr int    kCamWidthPixel  = 640;
    static constexpr int    kCamHeightPixel = 480;
    // Min and max UAV speed used for tracking in m/s
    static constexpr double kMinUAVSpeed        = 0.2;
    static constexpr double kMaxUAVSpeed        = 1;
    static constexpr double kDistanceThreshold  = 2.5; // in meters

    // ### Methods ###
    void InitializeSubscribers();
    void InitializePublishers();
    void InitializeServices();

    void StateCallback(const mavros_msgs::State::ConstPtr& msg);
    void CompassCallback(const std_msgs::Float64::ConstPtr& msg);
    void GpsCallback(const sensor_msgs::NavSatFix::ConstPtr& position);
    void ControlCallback(const communication::nav_control::ConstPtr& next_order);
    void TrackingCallback(const tld_msgs::BoundingBox::ConstPtr& target);

    void LandOrder();
    void TakeoffOrder(double target_altitude);
    void GpsMoveOrder(double target_altitude, double target_latitude, double target_longitude,  double yaw = kDefaultNoYaw);
    void NoYawGpsMoveOrder(double target_altitude, double target_latitude, double target_longitude);
    void VelMoveOrder(double vel_linear_x, double vel_linear_y, double vel_linear_z, double distance);
    void TrackingOrder();
    
    void setGuidedMode();
    void setArmThrottle();

    double   ComputeHeadingYaw(double target_altitude, double target_latitude, double target_longitude);
    NavOrder ResolveNavOrder(std::string input);
};


#endif



