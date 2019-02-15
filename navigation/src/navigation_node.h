
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

#include <iostream>

#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include <tf/transform_datatypes.h>
#include "sensor_msgs/Imu.h"


time_t start, ends;
time_t imcpy_start,imcpy_end;
struct timeval beginning, current, end;

bool run = false;
int current_ver = 0;
int hardware = 0;
double altitude;

boost::shared_ptr<ros::Publisher> node_model_pub;

long elapse_time_u(struct timeval *end, struct timeval *start);
long time_micros(struct timeval *end, struct timeval *start);

void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg);
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
