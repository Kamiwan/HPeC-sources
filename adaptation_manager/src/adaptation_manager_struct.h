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
 * Header file of the Adaptation Manager Structures.
 * 	Structure and method definition
 *************************************************************************************/

#ifndef ADAPTATION_MANAGER_STRUCT_H
#define ADAPTATION_MANAGER_STRUCT_H

#include <iostream>
#include <string>
#include <vector>

#include "am_defines.h"
extern"C"{
	#include "call.h"
}

//EM, new struct to allow timing and qos updates
struct App_timing_qos
{  
    int texec; // texec , [mintexec, maxtexec]
    int qos;   //qos , [minqos, maxqos]

	//EM, useful functions to use App_timing_qos easily
	void print();
};

void 	Task_inRazTimingQos(Task_in & task);
void 	Task_inRazAll(Task_in & task);
void 	Task_inPrint(const Task_in & task);
void	Task_inCopy(const Task_in & task_to_copy, Task_in & new_copy);
Task_in Task_inUpdateTimeQos(const App_timing_qos & app_feedback,
				Task_in & task);


struct Step_in
{
	Task_in contrast_img;
	Task_in motion_estim_imu;
	Task_in motion_estim_img;
	Task_in search_landing; 
	Task_in obstacle_avoidance;
	Task_in t_landing;
	Task_in rotoz_s;
	Task_in rotoz_b;
	Task_in replanning;
	Task_in detection; 
	Task_in tracking;

	Hw_st h1;
	Hw_st h2;
	Hw_st h3;

	//EM, useful functions to use Step_in easily
	void init();
	void raz_timing_qos();
	void load_C3(std::vector<Task_in> const& C3);
	void update_timing_qos(std::vector<App_timing_qos> time_qos);
	std::vector<Task_in> record_step_in();
};





struct Map_app_out
{
	int 	    active;
	int 	    version_code;
	int 	    region_id;
	std::string	fusion_sequence;

	//EM, useful functions to use Map_app_out easily
	void init();
	std::string set_fusion(Map_app_out const& config_app);
	Map_app_out& operator=(Task_out const& rhs);
};


struct App_scheduler
{
	int		    app_index; //EM, according to the C3 app order to activate-desactivate taskes
	int 	    active;
	int 	    version_code;
	int 	    region_id;
	std::string	fusion_sequence;
	int		    bitstream_addr;
	bool	    loaded;
	bool	    done;

	//EM, useful functions to use App_scheduler easily
	void print();
	App_scheduler& operator=(Map_app_out const& rhs);
};

struct Bitstream_map
{
	int 	version_code;
	int		bitstream_addr;

	//EM, useful functions to use Bitstream_map easily
	void print();
};


#endif


