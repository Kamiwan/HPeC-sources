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
 * File   : navigation_node.cpp,
 * Copyright (C) 2018 Lab-STICC Laboratory
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: February 21, 2019 
 *
 *************************************************************************************/
#include "navigation_node.hpp"



/**##################################################################
 * MAIN
 * Author : EM 
 * 
 * Launch the ROS node and subscribe to topics in order to wait 
 * for orders.
##################################################################**/
int main(int argc, char **argv)
{
    ros::init(argc, argv, "navigation_node");
    ros::NodeHandle nh;

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    NavCommand my_nav_command(&nh);
    
    while(ros::ok())
    {
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}





   // wait for FCU connection
    /*while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    ROS_INFO("Navigation connected!");

    geometry_msgs::PoseStamped pose;
    pose.header.stamp = ros::Time::now();
    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = 10;
    pose.header.stamp = ros::Time::now();
    pose.header.frame_id = "fcu";

    //send a few setpoints before starting
    for(int i = 50; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }
    
    
    guided_set_mode.request.custom_mode = "GUIDED";

    
    arm_cmd.request.value = true;


    
    takeoff_cmd.request.altitude 	= 10;//613.448;
    takeoff_cmd.request.latitude 	= 0; //-35.363;
    takeoff_cmd.request.longitude 	= 0; //149.165;
    takeoff_cmd.request.yaw			= 0;
    takeoff_cmd.request.min_pitch	= 0;
    
    landing_cmd.request.altitude 	= 0;
    //landing_cmd.request.latitude 	= 0;
    //landing_cmd.request.longitude = 0;
    

    flying = false;

    ros::Time last_request = ros::Time::now();
    ros::Time test_pose    = ros::Time::now();

    geometry_msgs::PoseStamped positest;
    positest.pose.position.x = 10;
    positest.pose.position.y = 10;
    positest.pose.position.z = ;


    //pose.header.stamp = ros::Time::now();
    //pose.pose.position.x = -2;
    //pose.pose.position.y = 5;
    //pose.pose.position.z = 10;

    if( current_state.mode != "GUIDED")
        {
            if( set_mode_client.call(guided_set_mode) &&
                guided_set_mode.response.mode_sent)
            {
                ROS_INFO("GUIDED enabled");
                ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",current_altitude, current_longitude, current_latitude);
            }
        last_request = ros::Time::now();
    } 

    ros::spinOnce();
    rate.sleep();

    if(!current_state.armed )
    {
        if( arming_client.call(arm_cmd) &&
            arm_cmd.response.success)
        {
            ROS_INFO("Vehicle armed");
            ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",current_altitude, current_longitude, current_latitude);
        }
        last_request = ros::Time::now();
    }

    ROS_INFO("Wait 1 second!");
    ros::Duration(1).sleep(); // sleep for 1 second

    ros::spinOnce();
    rate.sleep();

    if( current_state.armed &&  current_state.mode == "GUIDED" )
    {
      if( takeoff_client.call(takeoff_cmd) &&
          takeoff_cmd.response.success)
        {	
            ROS_INFO("Takeoff started");
            ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",current_altitude, current_longitude, current_latitude);
            flying = true;
            pose.pose.position.x = 3;
            pose.pose.position.y = 3;
            pose.pose.position.z = 10;
        }
        last_request = ros::Time::now();
    }

    ROS_INFO("Wait 2 seconds!");
    ros::Duration(2).sleep(); // sleep for 2 seconds

    ros::spinOnce();
    rate.sleep();

    pose.pose.position.x = -2;
    pose.pose.position.y = 5;
    pose.pose.position.z = 10;
    pose.header.stamp = ros::Time::now();
    pose.header.frame_id = "fcu";
    pose.pose.orientation.w = 1;


    //EM, test with global position
    mavros_msgs::GlobalPositionTarget target;
    target.header.stamp	= ros::Time::now();
    target.header.frame_id = "fcu";
    */
    /* EM, GLOBAL position with GPS and compute desired YAW 
    target.altitude 	= 613.448;
    target.latitude 	= -35.3631;
    target.longitude 	= 149.1648;

    double d_lat, d_long;
    d_lat 	= current_latitude - target.altitude;
    d_long 	= current_longitude - target.longitude;
    
    double x, y, heading;
    x = std::cos(target.latitude) * std::sin(d_long);
    y = (std::cos(current_latitude) * std::sin(target.latitude))  - 
        (std::sin(current_latitude) * std::cos(target.latitude) * std::cos(d_long) );
    heading = std::atan2(x,y);

    double new_heading = current_heading.data * DEG_TO_RAD - heading; //current_heading DEG TO RAD
    ROS_INFO_STREAM("NEW HEADING = " << new_heading);

    ROS_INFO_STREAM( "COMPUTED HEADING = " << (heading * (180/PI))); 
    ROS_INFO_STREAM("  NEW_HEADING" << (new_heading * (180/PI)));
    target.yaw			= new_heading;
    target.yaw_rate		= 1;*/

    //EM, Move UAV with velocity commands
    /*geometry_msgs::TwistStamped vel_msg;
    vel_msg.header.stamp = ros::Time::now();
    vel_msg.header.frame_id = "fcu";
    vel_msg.twist.linear.y = 2;

    ROS_INFO("SENDING VELOCITY COMMANDS");
    for(int i = 100; ros::ok() && i > 0; --i){
        //local_pos_pub.publish(pose);
        //global_pos_pub.publish(target);
        if(i<50)
            vel_msg.twist.linear.y = -2;

        cmd_vel_pub.publish(vel_msg);
        ros::spinOnce();
        rate.sleep();
    }

    ROS_INFO("NEW GLOBAL position published");*/

    /*
    while(ros::ok()){
        if( current_state.mode != "GUIDED" && altitude <= DEFAULT_HEIGHT  && 
            (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(guided_set_mode) &&
                guided_set_mode.response.mode_sent)
            {
                ROS_INFO("GUIDED enabled");
                ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",altitude, longitude, latitude);
            }
            last_request = ros::Time::now();
        } 
        else
        {
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0)))
            {
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success)
                {
                    ROS_INFO("Vehicle armed");
                    ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",altitude, longitude, latitude);
                }
                last_request = ros::Time::now();
            }

            if( current_state.armed &&  altitude < (DEFAULT_HEIGHT + 2) &&
                current_state.mode == "GUIDED" &&
                (ros::Time::now() - last_request > ros::Duration(0.2)))
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
            }

            
            if(ros::Time::now() - test_pose > ros::Duration(30.0) && flying)
            {
                if( landing_client.call(landing_cmd) &&
                    landing_cmd.response.success)
                {
                    ROS_INFO("Landing started");
                    ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",altitude, longitude, latitude);
                    flying = false;
                    pose.pose.position.x = 0;
                    pose.pose.position.y = 0;
                    pose.pose.position.z = 0;
                }
                last_request = ros::Time::now();
            }
        }
        
        ros::spinOnce();
        rate.sleep();
    }*/


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



