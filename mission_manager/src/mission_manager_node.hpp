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
 * Created on:  November 29, 2018
 * 
 * Main file header of the Mission Manager ROS node.
 *    This application controls the UAV in order to fullfill the given mission.
 *    Its role is to select which applications must be executed by giving requests to
 *    the Adaptation Manager. A shared memory is used for IPC.
 *************************************************************************************/

#ifndef MISSION_MANAGER_NODE_H
#define MISSION_MANAGER_NODE_H

#include <ros/ros.h>
#include <boost/thread.hpp>
#include <image_transport/image_transport.h>

#include <iostream>
#include <string>

#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/TwistWithCovarianceStamped.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/Illuminance.h>
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/BatteryState.h"
#include <sensor_msgs/NavSatFix.h>
#include <tf/transform_datatypes.h>

#include "communication/obstacle_detection_msg.h"
#include "communication/nav_control.h"

#include "comm_shared_memory.hpp" //EM, shared_memory_lib header


//EM, Verbosity levels
#define VERBOSITY_DEFAULT   0
#define VERBOSITY_OFF       0
#define VERBOSITY_LOW       1
#define VERBOSITY_MEDIUM    2
#define VERBOSITY_HIGH      3


void sensor_cam_callback(const sensor_msgs::Image::ConstPtr &img_msg);
void light_sensor_callback(const sensor_msgs::Illuminance::ConstPtr &light_msg);
void battery_callback(const sensor_msgs::BatteryState::ConstPtr &bat_msg);
void gps_vel_callback(const geometry_msgs::TwistStamped::ConstPtr &vel_msg);
void gps_pos_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg);

void obstacle_callback(const communication::obstacle_detection_msg::ConstPtr &detection_msg);
void achievable_callback(const std_msgs::Int32::ConstPtr &msg1);

void   init_cpu_load();
double current_cpu_value();

void StaticScenario_1();

/*********** Global variables ***********/ 
extern int	    verbose;
extern double   roll, pitch, yaw, prev_roll, prev_pitch, prev_yaw, delta_roll, delta_pitch, delta_yaw;
extern double   altitude, longitude, latitude, illuminance;
extern double   ang_vel_x, ang_vel_y, ang_vel_z, lin_vel_x, lin_vel_y, lin_vel_z;
extern float    battery_level;
extern bool     first_time_imu;

extern ros::Time scenario_ref_time;
extern ros::Time scenario_current_time;

extern ros::Subscriber cam_light_sub;
extern ros::Subscriber light_sub;
extern ros::Subscriber imu_sub;
extern ros::Subscriber bat_sub;
extern ros::Subscriber vel_sub;
extern ros::Subscriber pos_sub;
extern ros::Subscriber achievable_sub;
extern ros::Subscriber obstacle_sub;

extern ros::Publisher  nav_order_pub;

extern ros::Publisher search_land_pub;
extern ros::Publisher motion_estim_imu_pub;
extern ros::Publisher obstacle_avoidance_pub;
extern ros::Publisher detection_pub;


bool    step_1;
bool    step_2;
bool    step_3;
bool    step_4;
bool    step_5;


#endif