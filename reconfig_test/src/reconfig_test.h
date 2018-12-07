
/*************************************************************************************
 * File   : reconfig_test.h, file to create easily ros nodes for HPeC
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
 *  2. on another terminal, run the command "rosrun reconfig_test reconfig_test_node"
 *  3. on another terminal, run the command 
 *     "rostopic pub -1 /reconfig_test_mgt_topic std_msgs/Int32 "0" or "1" or "2""
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

#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <sys/mman.h>

/************************************************************************
 * EM, Insert here:
 * 	- Defines for the app
 *  - Topic names to subscribe or publish
**********************************************************************/
#define HIL     //Code modifications for Hardware In the Loop

//EM, defines for FPGA DPR tests
#define LW_HPS2FPGA_AXI_MASTER  0xFF200000
#define HPS2FPGA_AXI_MASTER     0xC0000000
#define HARD_DDR3_CONTROLLER    0x00000000
#define PAGE_SIZE               16384
#define PERSONNA_1              0
#define PERSONNA_2              8028160
#define DDR_PERSONNA_SPAN       0x800000

#define NM_INPUT_TOPIC "/iris/camera2/image_raw" //example of Topic name def

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
boost::shared_ptr<ros::Publisher> node_model_pub;

void *virtual_reconfig_ctrl;
int fd;

/************************************************************************
 * EM, Insert here global variables if needed
**********************************************************************/

long elapse_time_u(struct timeval *end, struct timeval *start);
long time_micros(struct timeval *end, struct timeval *start);
long  fsize(FILE *fp);

void appname_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);
void appname_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);

void managing_controller_request(const std_msgs::Int32::ConstPtr &value);
void stop();

void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg);
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam);

//EM, Functions for FPGA DPR tests
void test_reconfiguration(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);
int  init_FPGA_reconfiguration();
void release();

int  load_bitstream(std::string bitstream_path, int fd_mem,int mem_str_adrss,int offset_addr);
void start_reconfiguration(int bitstream_address, int region_id);
int  reconfiguration_done();
