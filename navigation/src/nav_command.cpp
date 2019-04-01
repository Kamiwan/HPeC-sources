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
 * File   : nav_command.cpp, 
 * Copyright (C) 2018 Lab-STICC Laboratory
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: February 21, 2019
 * 
 *************************************************************************************/
#include "nav_command.hpp"

NavCommand::NavCommand(ros::NodeHandle* nodehandle):nh_(*nodehandle)
{
    InitializeSubscribers();
    InitializePublishers();
    InitializeServices();

    current_heading_.data  = 0.0;
    current_altitude_      = kHomeAltitude;
    current_latitude_      = kHomeLatitude;
    current_longitude_     = kHomeLongitude;
    flying_                = false;

    destination_altitude_  = 0.0;
    destination_latitude_  = 0.0;
    destination_longitude_ = 0.0;

    previous_target_x_ = 0;
    previous_target_y_ = 0;
    target_x_   = 0;
    target_y_   = 0;
    target_time_ = 0.0;
    previous_target_time_ = 0.0;
    delta_time_ = 0.0;
    target_confidence_ = 0.0;
    prev_target_confidence_ = 0.0;
    is_target_ = false;
    
    ros::Rate rate(20.0);

    // wait for FCU connection
    ROS_INFO("NavCommand object wait for FCU connection!");
    while(ros::ok() && !current_state_.connected){
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("NavCommand object connected to FCU!");
}

NavCommand::~NavCommand()
{
}

// member helper function to set up subscribers;
// note odd syntax: &NavCommand::subscriberCallback is a pointer to a member function of NavCommand 
// "this" keyword is required, to refer to the current instance of NavCommand
void NavCommand::InitializeSubscribers()
{
    state_sub_ = nh_.subscribe<mavros_msgs::State>
            ("mavros/state", 10, &NavCommand::StateCallback, this);
    pos_sub_ = nh_.subscribe<sensor_msgs::NavSatFix>
            ("mavros/global_position/global", 10, &NavCommand::GpsCallback, this);
    compass_heading_sub_ = nh_.subscribe<std_msgs::Float64>
            ("mavros/global_position/compass_hdg", 10, &NavCommand::CompassCallback, this);
    nav_order_sub_ = nh_.subscribe<communication::nav_control>
            ("navigation/order", 10, &NavCommand::ControlCallback, this);     
    tracked_object_pos_sub_ = nh_.subscribe<tld_msgs::BoundingBox>
            ("tld_tracked_object", 10, &NavCommand::TrackingCallback, this);  
}

void NavCommand::InitializePublishers()
{
    local_pos_pub_ = nh_.advertise<geometry_msgs::PoseStamped>
            ("mavros/setpoint_position/local", 10);
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::TwistStamped>
            ("mavros/setpoint_velocity/cmd_vel",10);
    global_pos_pub_ = nh_.advertise<mavros_msgs::GlobalPositionTarget>
            ("mavros/setpoint_position/global", 10);
}

void NavCommand::InitializeServices()
{
    arming_client_ = nh_.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    takeoff_client_ = nh_.serviceClient<mavros_msgs::CommandTOL>
            ("mavros/cmd/takeoff");
    landing_client_ = nh_.serviceClient<mavros_msgs::CommandTOL>
            ("mavros/cmd/land");
    set_mode_client_ = nh_.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");
}


/*******************************************************************
 * StateCallback
 * Author : EM 
 * @param msg, UAV current state
 * 
 * Known modes listed here:
 * http://wiki.ros.org/mavros/CustomModes
 *
 * For system_status values
 * see https://mavlink.io/en/messages/common.html#MAV_STATE
*******************************************************************/
void NavCommand::StateCallback(const mavros_msgs::State::ConstPtr& msg)
{
    current_state_ = *msg;
    //ROS_INFO_STREAM("Current UAV mode = " << current_state_.mode <<
    //                " , status = " << (int)current_state_.system_status);
}

/*******************************************************************
 * CompassCallback
 * Author : EM 
 * @param msg, UAV orientation (heading) compared to the north
 * 
 * The heading is in degree 
*******************************************************************/
void NavCommand::CompassCallback(const std_msgs::Float64::ConstPtr& msg)
{
    current_heading_ = *msg;
    //ROS_INFO_STREAM("Current heading" << current_heading_.data);
}


/*******************************************************************
 * GpsCallback
 * Author : EM 
 * @param position, UAV GPS position from topic listened
 * 
*******************************************************************/
void NavCommand::GpsCallback(const sensor_msgs::NavSatFix::ConstPtr &position)
{
   current_altitude_  = position->altitude;
   current_latitude_  = position->latitude;
   current_longitude_ = position->longitude;
   //ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",current_altitude_, current_longitude_, current_latitude_);
}




/*******************************************************************
 * TrackingCallback
 * Author : EM 
 * @param target, target bounding box properties
 * 
*******************************************************************/
void NavCommand::TrackingCallback(const tld_msgs::BoundingBox::ConstPtr& target)
{
    is_target_ = (target->confidence > 0) ? true : false;
    if(is_target_)
    {
        previous_target_x_ = target_x_;
        previous_target_y_ = target_y_;
        previous_target_time_ = target_time_;
        prev_target_confidence_ = target_confidence_;
        
        target_x_   = target->x;
        target_y_   = target->y;
        target_time_ = ros::Time::now().toSec(); 
        target_confidence_ = target->confidence;

        delta_target_x_ = target_x_ - previous_target_x_;
        delta_target_y_ = target_y_ - previous_target_y_;
        delta_time_     = target_time_ - previous_target_time_;
    } 
}

/*******************************************************************
 * ControlCallback
 * Author : EM 
 * @param next_order, order from Mission Manager node
 * 
 * Callback function to execute the next order from Mission Manager
*******************************************************************/
void NavCommand::ControlCallback(const communication::nav_control::ConstPtr& next_order)
{
    ROS_INFO("[NAV_COMMAND]: New command received!");

    switch (ResolveNavOrder(next_order->order))
    {
        case LAND:
            if(flying_)
            {
                LandOrder();
            }

            break;

        case TAKEOFF:
            if(!flying_)
            {
                TakeoffOrder(next_order->altitude);
            }
            break;

        case GPS_MOVE:
            if(flying_)
            {
                GpsMoveOrder(next_order->altitude, next_order->latitude, next_order->longitude);
            } else
            {
                ROS_ERROR("The UAV is not flying!");
                //TakeoffOrder(next_order->altitude);
                //GpsMoveOrder(next_order->altitude, next_order->latitude, next_order->longitude);
            }
            break;

        case VELOCITY_MOVE:
            if(flying_)
            {
                VelMoveOrder(next_order->vel_linear_x, next_order->vel_linear_y, next_order->vel_linear_z, next_order->distance);
            } else
            {
                ROS_ERROR("The UAV is not flying!");
                //TakeoffOrder(next_order->altitude);
            }
            break;

        case OBS_AVOIDANCE_MOVE:
            /* TODO: think of how this function will be stopped */
            break;

        case TRACKING_MOVE:
            if(flying_)
            {
                TrackingOrder();
            } else
            {
                ROS_ERROR("The UAV is not flying!");
            }
            
            break;

        // handles INVALID_MOVE and any other missing/unmapped cases
        default:
            ROS_ERROR_STREAM("Wrong order given to NavCommand: " << next_order->order);
            break;
    }

}


/*******************************************************************
 * ResolveNavOrder
 * Author : EM 
 * @param input, order from Mission Manager node
 * 
 * Convert an input string into a NavOrder enum argument
*******************************************************************/
NavCommand::NavOrder NavCommand::ResolveNavOrder(std::string input)
{
    if( input == "LAND"               ) return LAND;
    if( input == "TAKEOFF"            ) return TAKEOFF;
    if( input == "GPS_MOVE"           ) return GPS_MOVE;
    if( input == "VELOCITY_MOVE"      ) return VELOCITY_MOVE;
    if( input == "OBS_AVOIDANCE_MOVE" ) return OBS_AVOIDANCE_MOVE;
    if( input == "TRACKING_MOVE"      ) return TRACKING_MOVE;
    return INVALID_ORDER;
}


/*******************************************************************
 * LandOrder
 * Author : EM 
 * 
 * Send LAND Order to the FCU
*******************************************************************/
void NavCommand::LandOrder()
{
    landing_cmd_.request.altitude 	= 0; //local value in meters
    if( landing_client_.call(landing_cmd_) &&
        landing_cmd_.response.success)
    {
        ROS_INFO("Landing started");
        ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",
            current_altitude_, current_longitude_, current_latitude_);
        flying_ = false;
    }
}



/*******************************************************************
 * TakeoffOrder
 * Author : EM 
 * 
 * Send Takeoff order to the FCU
 * This function also checks if the UAV is in GUIDED mode
 * If not, GUIDED mode is set and it arms throttles
*******************************************************************/
void NavCommand::TakeoffOrder(double target_altitude)
{
    setGuidedMode();
    setArmThrottle();

    takeoff_cmd_.request.altitude = target_altitude - current_altitude_;  //local value in meters
    if( takeoff_client_.call(takeoff_cmd_) &&
       takeoff_cmd_.response.success)
    {	
        ROS_INFO("Takeoff started");
        ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",
            current_altitude_, current_longitude_, current_latitude_);
        flying_ = true;
    }
}


/*******************************************************************
 * GpsMoveOrder
 * Author : EM 
 * 
 * Ask to the FCU to move to the given GPS position
*******************************************************************/
void NavCommand::GpsMoveOrder(double target_altitude, 
        double target_latitude, double target_longitude, double yaw)
{
    next_gps_position_.header.stamp	= ros::Time::now();
    next_gps_position_.header.frame_id = "fcu";
    next_gps_position_.altitude 	= target_altitude;
    next_gps_position_.latitude 	= target_latitude;
    next_gps_position_.longitude 	= target_longitude;
    next_gps_position_.yaw_rate     = 1;

    if(yaw == kDefaultNoYaw)
        yaw = ComputeHeadingYaw(target_altitude, target_latitude, target_longitude);

    next_gps_position_.yaw = yaw;
    ROS_INFO_STREAM("Next Yaw = " << next_gps_position_.yaw);

    //Update Class Attributes
    destination_altitude_   = target_altitude;
    destination_latitude_   = target_latitude;
    destination_longitude_  = target_longitude;

    global_pos_pub_.publish(next_gps_position_);
}



/*******************************************************************
 * VelMoveOrder
 * Author : EM 
 * 
 * Ask to the FCU to move to the given distance at the given speed
 * We must know the distance to estimate the time we need to send
 * the velocity command
 * 
 * We simplify the computation by choosing the highest speed among
 * the 3 axis velocities
*******************************************************************/
void NavCommand::VelMoveOrder(double vel_linear_x, 
        double vel_linear_y, double vel_linear_z, int distance)
{
    geometry_msgs::TwistStamped vel_msg;
    vel_msg.header.stamp     = ros::Time::now();
    vel_msg.header.frame_id  = "fcu";
    vel_msg.twist.linear.x   = vel_linear_x;
    vel_msg.twist.linear.y   = vel_linear_y;
    vel_msg.twist.linear.z   = vel_linear_z;

    double highest_vel;
    if(vel_linear_x >= vel_linear_y && vel_linear_x >= vel_linear_z)
        highest_vel = vel_linear_x;
    else
    {
        if(vel_linear_y >= vel_linear_x && vel_linear_y >= vel_linear_z)
            highest_vel = vel_linear_y;
        else
            highest_vel = vel_linear_z;
    }

    double time_to_send = distance / highest_vel;
    ros::Rate rate(20.0);
    int count = (int)time_to_send * 20;

    ROS_INFO_STREAM("Send Vel command for " << time_to_send << 
                    " seconds to move " << distance << " meters." );
    for(int i = 0; i < count && ros::ok(); i++)
    {
        cmd_vel_pub_.publish(vel_msg);
        rate.sleep();
    }
    ROS_INFO_STREAM("Velocity command COMPLETE");

}


/*******************************************************************
 * TrackingOrder
 * Author : EM 
 * 
 * Make enter the NavCommand object in a loop to follow a tracked
 * object.
 * This loop ends either if the object is lost or if a new order
 * is received.
*******************************************************************/
void NavCommand::TrackingOrder()
{



}



/*******************************************************************
 * setGuidedMode
 * Author : EM 
 * 
 * Set ardupilot GUIDED mode to control the UAV
*******************************************************************/
void NavCommand::setGuidedMode()
{
    set_mode_cmd_.request.custom_mode = "GUIDED";
    if( current_state_.mode != "GUIDED")
    {
        if( set_mode_client_.call(set_mode_cmd_) &&
            set_mode_cmd_.response.mode_sent)
        {
            ROS_INFO("GUIDED enabled");
        } else
        {
            ROS_ERROR("Set GUIDED mode FAILED");
        }
    } 
}


/*******************************************************************
 * setArmThrottle
 * Author : EM 
 * 
 * Arm UAV throttles
*******************************************************************/
void NavCommand::setArmThrottle()
{
    arm_cmd_.request.value = true;
    if(!current_state_.armed)
    {
        if( arming_client_.call(arm_cmd_) &&
            arm_cmd_.response.success)
        {
            ROS_INFO("Vehicle armed");
        } else
        {
            ROS_ERROR("Vehicle arming FAILED");
        }
    }
}



/*******************************************************************
 * ComputeHeadingYaw
 * Author : EM 
 * 
 * target_latitude and target_longitude must be in degree
 * 
 * Compute the angle the UAV has to rotate to move in the right 
 * direction, the head forward
 * https://www.igismap.com/formula-to-find-bearing-or-heading-angle-between-two-points-latitude-longitude/
*******************************************************************/
double NavCommand::ComputeHeadingYaw(double target_altitude, 
        double target_latitude, double target_longitude)
{
    //Conv degrees to radians
    target_latitude  = target_latitude  * (PI / 180);
    target_longitude = target_longitude * (PI / 180);
    double rad_curr_lat  = current_latitude_  * (PI / 180);
    double rad_curr_long = current_longitude_ * (PI / 180);

    double d_lat 	= target_latitude  - rad_curr_lat;
    double d_long 	= target_longitude - rad_curr_long;
    
    double x = std::cos(target_latitude) * std::sin(d_long);
    double y = (std::cos(rad_curr_lat) * std::sin(target_latitude))  - 
        (std::sin(rad_curr_lat) * std::cos(target_latitude) * std::cos(d_long) );
    double absolute_yaw = std::atan2(y,x);
    
    ROS_INFO_STREAM("ABSOLUTE YAW    (rad) = " << absolute_yaw); 
    ROS_INFO_STREAM("CURRENT HEADING (deg) = " << current_heading_.data);

    return absolute_yaw;
}




