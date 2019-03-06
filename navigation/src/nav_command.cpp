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
 * The heading is in degree 
*******************************************************************/
void NavCommand::StateCallback(const mavros_msgs::State::ConstPtr& msg)
{
    current_state_ = *msg;
    ROS_INFO_STREAM("Current UAV mode = " << current_state_.mode <<
                    " , status = " << (int)current_state_.system_status);
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
    ROS_INFO_STREAM("Current heading" << current_heading_.data);
}


/*******************************************************************
 * GpsCallback
 * Author : EM 
 * @param position, UAV GPS position from topic listened
 * 
 * Callback function to get GPS data position
*******************************************************************/
void NavCommand::GpsCallback(const sensor_msgs::NavSatFix::ConstPtr &position)
{
   current_altitude_  = position->altitude;
   current_latitude_  = position->latitude;
   current_longitude_ = position->longitude;
   ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",current_altitude_, current_longitude_, current_latitude_);
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
   
    switch (ResolveNavOrder(next_order->order))
    {
        case LAND:
            /*if( current_state.armed &&  current_state.mode == "GUIDED")
            {
                if( takeoff_client.call(takeoff_cmd) &&
                    takeoff_cmd.response.success)
                {
                    ROS_INFO("Takeoff started");
                    ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",altitude, longitude, latitude);
                    flying = true;
                    pose.pose.position.x = 0;
                    pose.pose.position.y = 0;
                    pose.pose.position.z = 10;
                }
                last_request = ros::Time::now();
            }*/
            break;

        case TAKEOFF:
            /* code */
            break;

        case GPS_MOVE:
            /* code */
            break;

        case VELOCITY_MOVE:
            /* code */
            break;

        case OBS_AVOIDANCE_MOVE:
            /* code */
            break;

        case TRACKING_MOVE:
            /* code */
            break;

        // handles INVALID_MOVE and any other missing/unmapped cases
        default:
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



