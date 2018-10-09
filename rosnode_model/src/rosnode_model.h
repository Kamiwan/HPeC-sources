
/*************************************************************************************
 * File   : rosnode_model.h, file to create easily ros nodes for HPeC
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
 *  2. on another terminal, run the command "rosrun rosnode_model rosnode_model_node"
 *  3. on another terminal, run the command 
 *     "rostopic pub -1 /rosnode_model_mgt_topic std_msgs/Int32 "0" or "1" or "2""
 *************************************************************************************/
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <boost/thread.hpp>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

extern "C" {
#include "debug.h"
}

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"

#include "sensor_msgs/Imu.h"
#include <tf/transform_datatypes.h>

//Erwan Moréac, 05/03/18 : Setup #define
#define HIL				 //Code modifications for Hardware In the Loop

/************************************************************************
 * EM, Insert here:
 * 	- Defines for the app
 *  - Topic names to subscribe or publish
**********************************************************************/

#define SL_INPUT_TOPIC "/iris/camera2/image_raw" //example of Topic name def

/*******************************************
 * EM, Sample code for image topic
*******************************************/
/* color format image : pixmap */
typedef struct{
    int w;
    int h;
    unsigned char * img;
} PPM_IMG;

PPM_IMG img_ibuf_color;
/*******************************************
 * EM, Sample code for image topic
 * /!\	USE image.h 
 * to really use PPM_IMG struct!
*******************************************/


time_t start, ends;
time_t imcpy_start,imcpy_end;
struct timeval beginning, current, end;

bool run = false;
int current_ver = 0;
int hardware = 0;
double altitude;

boost::shared_ptr<boost::thread> worker_thread;
boost::shared_ptr<ros::NodeHandle> workerHandle_ptr;
boost::shared_ptr<ros::Publisher> search_land_pub;

/************************************************************************
 * EM, Insert here global variables if needed
**********************************************************************/

long elapse_time_u(struct timeval *end, struct timeval *start);
long time_micros(struct timeval *end, struct timeval *start);


void appname_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);
void appname_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);


void managing_controller_request(const std_msgs::Int32::ConstPtr &value);
void stop();


void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg);
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam);
