/*************************************************************************************
 * File   : stabilisation_imu_node.h, 
 * Copyright (C) 2018 Lab-STICC Laboratory
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 *
 * Perform picture stabilisation using IUM data
 * 
 * You can test this program:
 * 	1. run on a terminal "roscore" to get a ROS master
 *  2. on another terminal, run the command "rosrun stabilisation_imu stabilisation_imu_node"
 *  3. on another terminal, run the command 
 *     "rostopic pub -1 /stabilisation_imu_mgt_topic std_msgs/Int32 "0" or "1" or "2""
 *************************************************************************************/
#ifndef STABILISATION_IMU_NODE_HPP
#define STABILISATION_IMU_NODE_HPP

#include "utils.h"

// EM, HW version headers
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <boost/thread.hpp>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

extern "C" {
    #include "debug.h"
    #include "pr_internal_host.h"   // EM, HW version headers
    #include "mc422.h"              // EM, HW version headers
}

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include "topic_tools/MuxSelect.h"

#include "sensor_msgs/Imu.h"
#include <tf/transform_datatypes.h>

//Erwan Moréac, 05/03/18 
#define HIL	 //Code modifications for Hardware In the Loop
//#define PLATFORM_32_BITS	 // uncomment on target architecture 

#define STAB_IMU_INPUT_TOPIC  "/iris/camera2/image_raw"
#define STAB_IMU_OUTPUT_TOPIC "/stabilised_camera" 
#define CAMERA_MUX_TOPIC      "/iris/camera2/image_raw"

#define LEARN_RATE 0.95
#define FILTERING true
#define HFOV 60
#define VFOV 45
#define REFRESH_RTZ_PARAM 20 //EM, Number of pictures before update of theta, x and y
#define IMAGE_WIDTH  640
#define IMAGE_HEIGHT 480

// EM, HW version defines
#define SDRAM_SPAN   ( 0x4000000 )
#define MC_CTRL_BASEADDR 0xFF201000
#define FPGA_DDR_BASEADDR 0xC0000000
// DMA-less HW control
#define DMA_WRITE_CTRL_REG                  0xFF200000
#define MATCHING_STATUS_REG_OFFSET	        0x0	
#define MATCHING_WRITE_ADDR_REG_OFFSET	    0x1	
#define MATCHING_WRITE_LENGTH_REG_OFFSET	0x2	
#define MATCHING_CTRL_REG_OFFSET	        0x3	

#define PIO_PR_RESET_BASEADDR 0xFF214000


/*!
* \brief ROS callback functions
* Author : EM 
*/
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam);
void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg);
void managing_controller_request(const std_msgs::Int32::ConstPtr &value);

/*!
* \brief Stops the current pThread launched
* Author : Soguy, EM 
*/
void stop();

/*!
* \brief Functions used with pThread to execute the app in SW or HW
* Author : EM 
*/
void stabilisation_imu_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);
void stabilisation_imu_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);


/*!
* \brief This group of functions perform the SW imu stabilization
* Author : EM 
*/
void get_rtz_param_from_ins_values(const cv::Mat & pic, double yawData, double pitchData,
                                    double rollData, double initialYaw, double initialPitch,
                                    double initialRoll, double * theta, double * x, double * y);
cv::Mat  rotozoom_ins(const cv::Mat & pic, bool first_time,
                        const double theta, const double x, const double y,
						const double last_theta, const double last_x, const double last_y);


/*!
* \brief Functions to use the HW version of the rotozoom
* Author : EM 
*/
void rotozoom_compute_params(const cv::Mat & pic, bool first_time,
                        const double theta, const double x, const double y,
						const double last_theta, const double last_x, const double last_y,
						double & theta_cumul, double &  x_cumul, double &  y_cumul);
void mc_input_422_setup(const cv::Mat & YUV_in, 
                        volatile unsigned char * mem_to_mc_input_buffer,
                        volatile unsigned char * uv_mem_to_mc_input_buffer);                        
int  acquire();
void release();
void launch_custom_dma();


#endif