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
#include <iostream>
#include <string>

#include <sensor_msgs/NavSatFix.h>
#include <tf/transform_datatypes.h>
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/BatteryState.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"

#include "CommSharedMemory.hpp" //EM, shared_memory_lib header

//EM, Verbosity levels
#define VERBOSITY_DEFAULT   0
#define VERBOSITY_OFF       0
#define VERBOSITY_LOW       1
#define VERBOSITY_MEDIUM    2
#define VERBOSITY_HIGH      3

void battery_callback(const sensor_msgs::BatteryState::ConstPtr &bat_msg);
void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg);
void cpu_load_Callback(const std_msgs::Float32::ConstPtr &load);
void achievable_Callback(const std_msgs::Int32::ConstPtr &msg1);

/*********** Global variables ***********/ 
extern int	verbose;
extern double   roll, pitch, yaw;
extern double   prev_roll, prev_pitch, prev_yaw;
extern float    battery_level;
extern bool     first_time_imu;

#endif