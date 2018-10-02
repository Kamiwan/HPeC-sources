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
#include <opencv2/opencv.hpp>
#include <math.h>

//Erwan Moréac, 05/03/18 
//#define HIL	 //Code modifications for Hardware In the Loop

#define STAB_IMU_INPUT_TOPIC "/iris/camera2/image_raw"
#define STAB_IMU_OUTPUT_TOPIC "/stab_imu/image" 

#define LEARN_RATE 0.95
#define FILTERING true
#define HFOV 60
#define VFOV 45
#define REFRESH_RTZ_PARAM 20 //EM, Number of pictures before update of theta, x and y


/*!
* \brief ROS callback functions
* Author : Soguy 
*/
long elapse_time_u(struct timeval *end, struct timeval *start);
long time_micros(struct timeval *end, struct timeval *start);


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

