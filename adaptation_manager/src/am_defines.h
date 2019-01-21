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
 * Header file for Adaptation Manager defines
 *************************************************************************************/
#ifndef AM_DEFINES_H
#define AM_DEFINES_H

#define PATH_TIME_QOS  		"./src/parameters/time_qos_task.txt"
#define PATH_TABLE_C3  		"./src/parameters/tableC3.txt"
#define PATH_ACHIEVABLE_TAB "./src/parameters/achievable_tab.txt"
#define PATH_MAP_TAB 		"./src/parameters/map_tab.txt"
#define PATH_DONE 			"./src/parameters/done.txt"
#define PATH_RELEASE_HW 	"./src/parameters/release_hw.txt"

#define APPLICATION_NUMBER		 11 	//EM, the number of apps in C3 table
#define MULTI_APP_THRESHOLD_CODE 140 	//EM, every code above means a bitstream fusion of apps

//EM
#define VERBOSITY_DEFAULT   0
#define VERBOSITY_OFF       0
#define VERBOSITY_LOW       1
#define VERBOSITY_MEDIUM    2
#define VERBOSITY_HIGH      3


#endif