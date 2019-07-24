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
 *   The configuration is given according Mission Manager requests with C3 table 
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
#include <chrono>

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

#include <thread>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

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
#include "reconfiguration_automate.h"
#include "reconfig.h"

#include "comm_shared_memory.hpp" //EM, shared_memory_lib header

#define MUTEX_NAME_BTS_LOAD		"Mutex_BTS_load"

//EM, namespaces use from previous developers...
using namespace std; 
using namespace cv;

//File based functions
vector<Bitstream_map> 	read_BTS_MAP(const char* path);
vector<Task_in> 		read_C3(const char* path);
vector<App_timing_qos> 	read_time_qos(const char* path);

//##### Functions to interact with the Mission Manager (MM) #####
/* notify_Callback
ROS callback  function to get requests from MM
@param msg    the number associated to the app
*/
void notify_Callback(const std_msgs::Int32::ConstPtr& msg);
/* publish_to_MM
Publish on a ROS topic an alert if there is no perfect solution
@param is_achievable 	tells if the next configuration respects all requests
@param s				reconfiguration automaton output	
*/
void publish_to_MM(bool is_achievable, Step_out s);

//##### Main functions, these are mainly setup functions #####
void					sh_mem_setup(MemoryCoordinator & shared_memory, vector<Task_in> C3);
vector<Task_in>			sh_mem_read_C3(MemoryCoordinator & shared_memory);
/* sh_mem_read_time_qos
Read only a part of C3 shared_memory to reduce resource occupancy time
@param shared_memory 	
@return		current timing and qos of each application
*/
vector<App_timing_qos> 	sh_mem_read_time_qos(MemoryCoordinator & shared_memory);
vector<Map_app_out>  	init_output(Step_out const& step_output);
void					raz_timing_qos(vector<Task_in> & C3);
/* compare
Compare current app QoS and exec time to MM requests
@param time_qos
@param C3
@return		0 if an application does not respect requests from MM, 1 otherwise
*/
bool	compare(std::vector<App_timing_qos> time_qos, std::vector<Task_in> C3);

//##### Functions needed for the task_mapping function #####
int		find_BTS_addr(vector<Bitstream_map> bts_map, int version_code);
/* create_scheduler_tab
The scheduler array tells which application has a modification in its configuration
@param map_config_app		the next app configuration to move to
@param prev_map_config_app	the current app configuration
@param bitstream_map		it stores bitstream @ of each HW app configuration
@return		the scheduler array
*/
vector<App_scheduler>	create_scheduler_tab(vector<Map_app_out> const& map_config_app
											, vector<Map_app_out> const& prev_map_config_app
											, vector<Bitstream_map> const& bitstream_map);
/* check_sequence
Check if there is a couple of application that are working in sequence 
(In HPeC, it can only be Detection & Tracking)
@param map_config_app		the next app configuration to move to
*/											
void	check_sequence(vector<Map_app_out> & map_config_app);
/* wait_release
busy-waiting loop (polling) in order to wait the HW resource (a FPGA Tile) has been released
by the previous application that used it.
@param map_config_app		the next app configuration to move to
@param app					index of the app waiting for the HW resource
@param region_id			the ID of FPGA Tile
@param prev_map_config_app	previous system configuration to know which app to wait
@param shared_memory
*/	
void	wait_release(int app, int region_id, vector<Map_app_out> const& prev_map_config_app
					, MemoryCoordinator	& shared_memory);
/* activate_desactivate_task
Send a msg using a ROS topic to give orders to applications
@param app_index	target app to receive orders
@param msg			message to send (ROS data type)
*/	
void 	activate_desactivate_task(int app_index, std_msgs::Int32 msg);
/* task_mapping
Set up the next system configuration from automaton results. 
@param map_config_app		the next app configuration to move to
@param prev_map_config_app	the current app configuration	
@param bitstream_map		it stores bitstream @ of each HW app configuration
@param shared_memory		
*/	
void 	task_mapping(vector<Map_app_out> const& map_config_app
				, vector<Map_app_out> const& prev_map_config_app
				, vector<Bitstream_map> const& bitstream_map
				, MemoryCoordinator & shared_memory);


/* sequence_exec_routine
Launch a thread which performs the sequence execution of 2 apps in the same Tile
@param seq_app[2]		The two application configuration		
*/
void sequence_exec_routine(App_scheduler seq_app[2]);
/* secured_load_BTS
Function wrapper to allow user to perform 
@param ? TODO: complete it
*/
void secured_reconfiguration();
/* stop_sequence
Stop the thread which executes the routine execution
@param tile_index		The index of the tile used for the sequence routine
*/
void stop_sequence(int tile_index);

//##### Functions to check if it exists a configuration that satisfies the whole requests #####
bool 	check_achievable(MemoryCoordinator & shared_memory, Step_out s);
void	sh_mem_write_achievable(MemoryCoordinator & shared_memory, Step_out s);

//##### EM, just a function to evaluate the data access time #####
void 	compare_data_access_speed(MemoryCoordinator & shared_memory);


/*********** Global variables ***********/ 
extern vector<Map_app_out> prev_app_output_config;
extern vector<Map_app_out> app_output_config;
extern int	verbose;
extern boost::shared_ptr<boost::thread> sequence_thread[TILE_NUMBER];
extern bool active_thread[TILE_NUMBER];
extern App_scheduler sequence_apps[2];

extern int ncc_heptagon; //Global variable for reconfiguration automaton

#endif


