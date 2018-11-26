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
 * Created on: September 17, 2018
 * 
 * Main file header of the Adaptation Manager ROS node.
 *   This application controls the execution of other HPeC nodes and execute a 
 *   reconfiguration automaton to get the right schedule.
 *   The configuration is given according Mission Manager requests 
 *************************************************************************************/

#ifndef ADAPTATION_MANAGER_NODE_H
#define ADAPTATION_MANAGER_NODE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <ctime>

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include <iosfwd>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/thread.hpp>

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "initimg.h"
#include "fpga_header.h"

extern"C"{
	#include "main.h"
	//#include "call.h"
}

#include "am_defines.h"
#include "adaptation_manager_struct.h"

#include "utils.h"
#include "handle_applications.h"
#include "reconfiguration_automate.h"
#include "reconfig.h"

#include "CommSharedMemory.hpp" //EM, shared_memory_lib header

//EM, namespaces use from previous developers...
using namespace std; 
using namespace cv;

//File based functions
vector<Bitstream_map> 	read_BTS_MAP(const char* path);
vector<Task_in> 		read_C3(const char* path);
vector<App_timing_qos> 	read_time_qos(const char* path);

//Functions to interact with the Mission Manager
void notify_Callback(const std_msgs::Int32::ConstPtr& msg);
void publish_to_MM(bool is_achievable, Step_out s);

//Main functions, these are mainly setup functions
void					sh_mem_setup(MemoryCoordinator & shared_memory, vector<Task_in> C3);
vector<Task_in>			sh_mem_read_C3(MemoryCoordinator & shared_memory);
vector<App_timing_qos> 	sh_mem_read_time_qos(MemoryCoordinator & shared_memory);
vector<Map_app_out>  	init_output(Step_out const& step_output);
void					raz_timing_qos(vector<Task_in> & C3);
bool 					compare(std::vector<App_timing_qos> time_qos, std::vector<Task_in> C3);

//Functions needed for the task_mapping function
int		find_BTS_addr(vector<Bitstream_map> bts_map, int version_code);
vector<App_scheduler>	create_scheduler_tab(vector<Map_app_out> const& map_config_app
											, vector<Map_app_out> const& prev_map_config_app
											, vector<Bitstream_map> const& bitstream_map);
void	check_sequence(vector<Map_app_out> & map_config_app);
void	wait_release(int app, int region_id, vector<Map_app_out> const& prev_map_config_app
					, MemoryCoordinator	& shared_memory);
void 	activate_desactivate_task(int app_index, std_msgs::Int32 msg);
void 	task_mapping(vector<Map_app_out> const& map_config_app
				, vector<Map_app_out> const& prev_map_config_app
				, vector<Bitstream_map> const& bitstream_map
				, MemoryCoordinator & shared_memory);

//Functions to check if the configuration satisfies the whole requests
bool 	check_achievable(MemoryCoordinator & shared_memory, Step_out s);
void	sh_mem_write_achievable(MemoryCoordinator & shared_memory, Step_out s);

//EM, just a function to evaluate the data access time
void 	compare_data_access_speed(MemoryCoordinator & shared_memory);


/*********** Global variables ***********/ 
extern vector<Map_app_out> prev_app_output_config;
extern vector<Map_app_out> app_output_config;
extern bool first_step;
#endif


