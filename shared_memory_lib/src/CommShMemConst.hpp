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
 * Created on: November 12, 2018
 * 
 * Defines and includes for MemoryCoordinator class.
 * Allow HPeC ROS nodes to communicate using shared memory, the fastest communication
 *************************************************************************************/


#ifndef COMM_SHMEM_CONST_HPP
#define COMM_SHMEM_CONST_HPP

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <string>
#include <vector>
#include <iostream>

//Alias an STL compatible allocator of ints that allocates ints from the managed
//shared memory segment.  This allocator will allow to place containers
//in managed shared memory segments
typedef boost::interprocess::allocator<int, boost::interprocess::managed_shared_memory::segment_manager> ShmemAllocator;
//Alias a vector that uses the previous STL-like allocator
typedef std::vector<int, ShmemAllocator> SharedVector;

#define NAME_C3_SEGMENT           "C3_table_segment"
#define NAME_ACHIEVABLE_SEGMENT   "Achievable_segment"
#define NAME_RELEASE_SEGMENT      "Release_hw_segment"
#define NAME_DONE_SEGMENT         "Done_segment"
#define NAME_BUSY_TILE_SEGMENT    "Busy_Tile_segment"

#define VECTOR_NAME_C3_SEGMENT           "Vector_C3_table_segment"
#define VECTOR_NAME_ACHIEVABLE_SEGMENT   "Vector_Achievable_segment"
#define VECTOR_NAME_RELEASE_SEGMENT      "Vector_Release_hw_segment"
#define VECTOR_NAME_DONE_SEGMENT         "Vector_Done_segment"
#define VECTOR_NAME_BUSY_TILE_SEGMENT    "Vector_Busy_Tile_segment"

#define MUTEX_NAME_C3_SEGMENT           "Mutex_C3_table_segment"
#define MUTEX_NAME_ACHIEVABLE_SEGMENT   "Mutex_Achievable_segment"
#define MUTEX_NAME_RELEASE_SEGMENT      "Mutex_Release_hw_segment"
#define MUTEX_NAME_DONE_SEGMENT         "Mutex_Done_segment"
#define MUTEX_NAME_BUSY_TILE_SEGMENT    "Mutex_Busy_Tile_segment"

#define PAGE_SIZE_BYTES      4096

//EM, Indexes of C3 table shared memory
#define C3_NB_ATTRIBUTES    8
#define C3_ACTIVE_REQUEST   0
#define C3_CURRENT_TEXEC    1
#define C3_MIN_TEXEC        2
#define C3_MAX_TEXEC        3
#define C3_CURRENT_QOS      4
#define C3_MIN_QOS          5
#define C3_MAX_QOS          6
#define C3_PRIORITY         7

//EM, App index
#define APP_NUMBER         11  
#define CONTRAST_IMG       0  
#define MOTION_ESTIM_IMU   1     
#define MOTION_ESTIM_IMG   2    
#define SEARCH_LANDING     3   
#define OBSTACLE_AVOIDANCE 4      
#define T_LANDING          5
#define ROTOZ_S            6
#define ROTOZ_B            7
#define REPLANNING         8   
#define DETECTION          9  
#define TRACKING           10

//EM, Tile index
#define TILE_NUMBER        3
#define TILE_1             0
#define TILE_2             1
#define TILE_3             2

#endif