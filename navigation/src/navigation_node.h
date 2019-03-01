
/*************************************************************************************
 * File   : navigation_node.h, file to create easily ros nodes for HPeC
 * Copyright (C) 2018 Lab-STICC Laboratory
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 *
 * This model allows the user to create an application ros node that 
 * the adaptation manager can handle in the HPeC project.
 * 
 * This model must be used by copying the folder, then rename the package and files
 * according to your needs.
 * 
 * You can test this program:
 * 	1. run on a terminal "roscore" to get a ROS master
 *  2. on another terminal, run the command "rosrun navigation navigation_node_node"
 *************************************************************************************/
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

#include <mavros_msgs/GlobalPositionTarget.h>

#include <iostream>

#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/String.h"

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include <tf/transform_datatypes.h>
#include "sensor_msgs/Imu.h"

#include <math.h>       /* atan2 */
#define PI      3.14159265
#define RAD_360 6.28319
#define DEFAULT_HEIGHT 603.450

double current_altitude;
double current_latitude;
double current_longitude;
mavros_msgs::State current_state;
std_msgs::Float64 current_heading;


void state_cb(const mavros_msgs::State::ConstPtr& msg);
void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg);
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
