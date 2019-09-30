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
 * Created on: January, 2018
 * 
 * Main header for emergency_landing node
 *************************************************************************************/
#ifndef HARRIS_DETECTOR_NODE_HPP
#define HARRIS_DETECTOR_NODE_HPP



#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <image_transport/image_transport.h>
#include <vector>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>

extern "C" {
	#include "image.h"
	#include "debug.h"
	#include "harris.h"
	#include "freak.h"
	#include "simple_dma.h"
}

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>

#include <ros/callback_queue.h>
#include <boost/thread.hpp>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"

#include "std_msgs/String.h"
#include <sstream>
#include "communication/area_location.h"



#define SDRAM_SPAN   ( 0x400000 )
#define DMA_READ_CSR_BASEADDR 0xFF200000
#define DMA_WRITE_CSR_BASEADDR 0xFF201000
#define FPGA_DDR_BASEADDR 0xC0000000

#define DMAWRITE_STATUS_REG_OFFSET  0x0	
#define DMAWRITE_ADDR_REG_OFFSET    0x1	
#define DMAWRITE_LENGTH_REG_OFFSET  0x2	
#define DMAWRITE_CTRL_REG_OFFSET    0x3	

//Erwan Moréac, 05/03/18 : Setup #define
#define HIL				 //Code modifications for Hardware In the Loop
#define AREA_MIN 400	 //TODO : create a function to make it dependent of the UAV height and FOV
#define AREA_MAX 2073601 //This value exceeds the area of an HD picture 1920 x 1080 in pixels
#define HARRIS_INPUT_TOPIC "/video_flow"
#define HARRIS_OUTPUT_IMAGE_TOPIC "search_output/image"
//#define HARRIS_OUTPUT_AREA_LOCATION_TOPIC "search_output/areas"

//DEGUG only #define WRITE_IMG

boost::shared_ptr<ros::Publisher> search_land_pub;

cv::Mat * picture;
bool img_acquired;

PPM_IMG img_ibuf_color;
PGM_IMG img_ibuf_g;

time_t start, ends,t1;
time_t imcpy_start,imcpy_end;
struct timeval beginning, current, end;

int hardware = 0;
int fd;
FILE *cma_dev_file;
char cma_addr_string[10];

unsigned int            cma_base_addr;
void                    *virtual_base_sdram;
volatile unsigned int   *simple_dma_read_addr;
volatile unsigned int   *simple_dma_write_addr;
volatile unsigned int   *mem_to_stream_dma_buffer = NULL;
volatile unsigned char  *stream_to_mem_dma_buffer = NULL;

struct pixel *data;
unsigned char *header;

double diameter;
double altitude;
double camera_angle;
double image_height;

bool run = false;
int current_ver = 0;
boost::shared_ptr<boost::thread> worker_thread;
boost::shared_ptr<ros::NodeHandle> workerHandle_ptr;

long elapse_time_u(struct timeval *end, struct timeval *start);
long time_micros(struct timeval *end, struct timeval *start);

int acquire();
void release();

void harris_detector_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);
void harris_detector_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);

void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam);

extern void RST_RANSAC(Point *point1,Point *point2,int N_SAMPLES,int threshold,float *theta,int *tx,int *ty );
static void save_keypoints( const char* filename, keyPoint* keypoints, size_t count   );
static void save_descriptor( const  char *filename, descriptor* descriptors, size_t desc_count );

PGM_IMG greyharriscornerhw(PGM_IMG img_ibuf_g,int threshold,keyPoint *keyPoints,int *kpcount,int sort);


#endif