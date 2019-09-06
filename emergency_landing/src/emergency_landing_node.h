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
#include "sgdma_dispatcher.h"
#include "sgdma_dispatcher_regs.h"

#define SDRAM_SPAN (0x04000000)
#define HW_REGS_SPAN (0x00020000)
#define HWREG_BASE (0xff200000)
#define DDR_FPGA_CMA_BASE_ADDR 0xC0000000
#define LW_HPS2FPGA_AXI_MASTER 0xFF200000

#define USE_DDR_FPGA 0 //EM, parameter to use or not the FPGA DDR RAM

//EM, 11/04/18, Hardware version preparation
#define DMA_READ_CSR_BASEADDR 0xFF200000
#define DMA_READ_DESC_BASEADDR 0xFF201000
#define DMA_WRITE_CSR_BASEADDR 0xFF202000
#define DMA_WRITE_DESC_BASEADDR 0xFF203000

//Erwan Moréac, 05/03/18 : Setup #define
#define HIL				 //Code modifications for Hardware In the Loop
#define AREA_MIN 400	 //TODO : create a function to make it dependent of the UAV height and FOV
#define AREA_MAX 2073601 //This value exceeds the area of an HD picture 1920 x 1080 in pixels
#define SL_INPUT_TOPIC "/video_flow"
#define SL_OUTPUT_IMAGE_TOPIC "search_output/image"
#define SL_OUTPUT_AREA_LOCATION_TOPIC "search_output/areas"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
//#define WRITE_IMG //DEGUG only

boost::shared_ptr<ros::Publisher> search_land_pub;

PPM_IMG img_ibuf_color;
PGM_IMG img_ibuf_g;
PGM_IMG img_ibuf_ceh;
PGM_IMG img_ibuf_me1;
PGM_IMG img_ibuf_me;
PGM_IMG img_ibuf_bw;
PGM_IMG img_ibuf_ccl;
PGM_IMG img_ibuf_mop;
PGM_IMG img_ibuf_mop2;
PGM_IMG img_ibuf_mop3;
PGM_IMG img_ibuf_ero;
PGM_IMG img_ibuf_gs;
PGM_IMG img_ibuf_sp;
PGM_IMG img_ibuf_se;
PGM_IMG img_ibuf_ms;
int nb_labels;
COMPONENT *ccl;
int count = 0;

int hardware = 0;
int fd;

FILE *cma_dev_file;
char cma_addr_string[10];

time_t start, ends,t1;
time_t imcpy_start,imcpy_end;
struct timeval beginning, current, end;

unsigned int sl_cma_base_addr;
void *sl_virtual_base_sdram;
void *sl_virtual_reconfig_ctrl;

volatile unsigned int *sl_mem_to_stream_dma_buffer = NULL;
volatile unsigned char *sl_stream_to_mem_dma_buffer = NULL;

// Create  Dispatcher
tcSGDMADispatcher     sl_dispatcher_read;
tcSGDMADispatcher     sl_dispatcher_write;
// Create  descriptor
tsSGDMADescriptor sl_descriptor_read;
tsSGDMADescriptor sl_descriptor_write;

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

// source ppm color image 24bits RGB (packed)
// The video IP cores used for edge detection require the RGB 24 bits of each pixel to be
// word aligned (aka 1 byte of padding per pixel). | unused 8 bits | red 8 bits | green 8 bits | blue 8 bits |
void memcpy_consecutive_to_padded(unsigned char *from, volatile unsigned int *to, int pixels);

int acquire();
void release();

void search_landing_area_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);
void search_landing_area_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr);

void managing_controller_request(const std_msgs::Int32::ConstPtr &value);
void stop();

void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam);

