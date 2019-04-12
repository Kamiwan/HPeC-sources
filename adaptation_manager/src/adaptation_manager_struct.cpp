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
 * Main file of the Adaptation Manager structure.
 *************************************************************************************/

#include "adaptation_manager_struct.h"

void Task_inRazTimingQos(Task_in & task)
{
	task.texec	= 0;
    task.qos	= 0;
}

void Task_inRazAll(Task_in & task)
{
	task.req		= 0; // 1 : activation, 0 : arrêt
	task.texec		= 0; // texec , [mintexec, maxtexec]
	task.mintexec	= 0;
	task.maxtexec	= 0;
	task.qos		= 0; //qos , [minqos, maxqos]
	task.minqos 	= 0;
	task.maxqos 	= 0;
	task.priority	= 0;
}

void Task_inPrint(const Task_in & task){
	std::cout 	<< "; req = " 	<< task.req << std::endl
				<< "texec = "   << task.texec << std::endl
				<< "mintexec = "<< task.mintexec << std::endl
				<< "maxtexec = "<< task.maxtexec << std::endl
				<< "qos = " 	<< task.qos << std::endl
				<< "minqos = "  << task.minqos << std::endl
				<< "maxqos = "  << task.maxqos << std::endl
				<< "priority = "<< task.priority << std::endl;
}

void	Task_inCopy(const Task_in & task_to_copy, Task_in & new_copy)
{
	new_copy.priority	= task_to_copy.priority;
    new_copy.req		= task_to_copy.req;
    new_copy.texec		= task_to_copy.texec;
    new_copy.mintexec	= task_to_copy.mintexec;
    new_copy.maxtexec	= task_to_copy.maxtexec;
    new_copy.qos    	= task_to_copy.qos;
    new_copy.minqos		= task_to_copy.minqos;
    new_copy.maxqos		= task_to_copy.maxqos;
}

void Task_inUpdateTimeQos(const App_timing_qos & app_feedback, Task_in & task)
{
	task.texec	= app_feedback.texec; 	// texec, [mintexec, maxtexec]
	task.qos	= app_feedback.qos; 	// qos,   [minqos, maxqos]
}



void Step_inInit(Step_in & input_step)
{
	Task_inRazAll( input_step.contrast_img );
	Task_inRazAll( input_step.motion_estim_imu );
	Task_inRazAll( input_step.motion_estim_img );
	Task_inRazAll( input_step.search_landing );
	Task_inRazAll( input_step.obstacle_avoidance );
	Task_inRazAll( input_step.t_landing );
	Task_inRazAll( input_step.rotoz_s );
	Task_inRazAll( input_step.rotoz_b );
	Task_inRazAll( input_step.replanning );
	Task_inRazAll( input_step.detection );
	Task_inRazAll( input_step.tracking );

	input_step.h1.av =	1;	// EM, Everything is available at the beginning
	input_step.h2.av =	1;
	input_step.h3.av =	1;
}

void Step_inRazTimingQos(Step_in & input_step)
{
	Task_inRazTimingQos( input_step.contrast_img );
	Task_inRazTimingQos( input_step.motion_estim_imu );
	Task_inRazTimingQos( input_step.motion_estim_img );
	Task_inRazTimingQos( input_step.search_landing );
	Task_inRazTimingQos( input_step.obstacle_avoidance );
	Task_inRazTimingQos( input_step.t_landing );
	Task_inRazTimingQos( input_step.rotoz_s );
	Task_inRazTimingQos( input_step.rotoz_b );
	Task_inRazTimingQos( input_step.replanning );
	Task_inRazTimingQos( input_step.detection );
	Task_inRazTimingQos( input_step.tracking );
}

void Step_inLoadC3(const std::vector<Task_in> & C3, Step_in & input_step)
{
	if(C3.size() < APPLICATION_NUMBER)
	{
		std::cout << "The C3 table provided is too small! C3 size=" << C3.size() << std::endl;
		return; // EM, to leave a void function
	}
	// EM, I know it's dirty, it would have been better with an array of attributes...
	Task_inCopy( C3[0],  input_step.contrast_img );
	Task_inCopy( C3[1],  input_step.motion_estim_imu );
	Task_inCopy( C3[2],  input_step.motion_estim_img );
	Task_inCopy( C3[3],  input_step.search_landing );
	Task_inCopy( C3[4],  input_step.obstacle_avoidance );
	Task_inCopy( C3[5],  input_step.t_landing );
	Task_inCopy( C3[6],  input_step.rotoz_s );
	Task_inCopy( C3[7],  input_step.rotoz_b );
	Task_inCopy( C3[8],  input_step.replanning );
	Task_inCopy( C3[9],  input_step.detection );
	Task_inCopy( C3[10], input_step.tracking );
}

void Step_inUpdateTimingQos(const std::vector<App_timing_qos> & time_qos, Step_in & input_step)
{
	if(time_qos.size() < APPLICATION_NUMBER)
	{
		std::cout << "The time_qos table provided is too small! time_qos size=" << time_qos.size() << std::endl;
		return; // EM, to leave a void function
	}
	// EM, I know it's dirty, it would have been better with an array of attributes...
	Task_inUpdateTimeQos( time_qos[0],  input_step.contrast_img );
	Task_inUpdateTimeQos( time_qos[1],  input_step.motion_estim_imu );
	Task_inUpdateTimeQos( time_qos[2],  input_step.motion_estim_img );
	Task_inUpdateTimeQos( time_qos[3],  input_step.search_landing );
	Task_inUpdateTimeQos( time_qos[4],  input_step.obstacle_avoidance );
	Task_inUpdateTimeQos( time_qos[5],  input_step.t_landing );
	Task_inUpdateTimeQos( time_qos[6],  input_step.rotoz_s );
	Task_inUpdateTimeQos( time_qos[7],  input_step.rotoz_b );
	Task_inUpdateTimeQos( time_qos[8],  input_step.replanning );
	Task_inUpdateTimeQos( time_qos[9],  input_step.detection );
	Task_inUpdateTimeQos( time_qos[10], input_step.tracking );
}


std::vector<Task_in> Step_inRecord(const Step_in & input_step)
{
	std::vector<Task_in> C3;

	C3.push_back(input_step.contrast_img);
	C3.push_back(input_step.motion_estim_imu);
	C3.push_back(input_step.motion_estim_img);
	C3.push_back(input_step.search_landing);
	C3.push_back(input_step.obstacle_avoidance);
	C3.push_back(input_step.t_landing);
	C3.push_back(input_step.rotoz_s);
	C3.push_back(input_step.rotoz_b);
	C3.push_back(input_step.replanning);
	C3.push_back(input_step.detection);
	C3.push_back(input_step.tracking);

	return C3;
}


void 	Task_outPrint(const Task_out & task)
{
	std::cout 	<< "; act = " 		<< task.act << std::endl
				<< "size = "   		<< task.size << std::endl
				<< "version = "		<< task.version << std::endl
				<< "code = "		<< task.code << std::endl
				<< "achievable = " 	<< task.achievable << std::endl
				<< "up_pos = "  	<< task.up_pos << std::endl
				<< "down_pos = "  	<< task.down_pos << std::endl
				<< "keep_pos = "  	<< task.keep_pos << std::endl
				<< "qos_pos = "		<< task.qos_pos << std::endl;
}

void App_timing_qos::print(){
	std::cout 	<< "texec = " << texec << std::endl
				<< "qos = " << qos << std::endl;
}


std::string Map_app_out::set_fusion(Map_app_out const& config_app)
{
	if(config_app.active != 0 && config_app.region_id != 0)
	{
		if(config_app.version_code >= MULTI_APP_THRESHOLD_CODE)
			return "f";
		else 
			return "-";
	}
	else
		return "-";
}

Map_app_out& Map_app_out::operator=(Task_out const& rhs)
{
	Map_app_out curr_app_out; // EM, intermediate variable to set fusion_sequence
	curr_app_out.active			= rhs.act;
	curr_app_out.version_code	= rhs.code;
	curr_app_out.region_id		= rhs.code %10;

	active			= rhs.act;
	version_code	= rhs.code;
	region_id		= curr_app_out.region_id;
	fusion_sequence	= set_fusion(curr_app_out); // EM, function needed for this attribute
	return *this;
}

void Map_app_out::init()
{
	active			= 0;
	version_code	= 0;
	region_id		= 0;
	fusion_sequence	= "";
}



void Bitstream_map::print()
{
	std::cout 	<< "version_code = " << version_code << std::endl
				<< "bitstream_addr = " << bitstream_addr << std::endl;
}

void App_scheduler::print()
{
	std::cout 	<< "app_index = " << app_index << std::endl
				<< "active = " << active << std::endl
				<< "version_code = " << version_code << std::endl
				<< "region_id = " << region_id << std::endl
				<< "fusion_sequence = " << fusion_sequence << std::endl
				<< "bitstream_addr = " << bitstream_addr << std::endl
				<< "loaded = " << loaded << std::endl
				<< "done = " << done << std::endl;
}


App_scheduler& App_scheduler::operator=(Map_app_out const& rhs)
{
	active			= rhs.active;
	version_code 	= rhs.version_code;
	region_id 		= rhs.region_id;
	fusion_sequence = rhs.fusion_sequence;
	return *this;
}

