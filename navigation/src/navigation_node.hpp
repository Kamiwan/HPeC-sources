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
 * File   : navigation_node.h, file to create easily ros nodes for HPeC
 * Copyright (C) 2018 Lab-STICC Laboratory
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: February 21, 2019
 * 
 * You can test this program:
 * 	1. run on a terminal "roscore" to get a ROS master
 *  2. on another terminal, run the command "rosrun navigation navigation_node_node"
 *************************************************************************************/

#ifndef NAVIGATION_NODE_HPP
#define NAVIGATION_NODE_HPP

#include <ros/ros.h>
#include <ros/callback_queue.h>

#include <iostream>

#include "nav_command.hpp"

#define PI          3.14159265
#define RAD_360     6.28319
#define DEG_TO_RAD  0.0174532925
#define DEFAULT_HEIGHT 603.450

#define MASK_IGNORE_VXYZ_AFXYZ_F 1016
#define MASK_IGNORE_LLA_YAW_RATE 3079

#endif