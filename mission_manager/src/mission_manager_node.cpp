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
 * Created on:  November 28, 2018
 * 
 * Main file of the Mission Manager ROS node.
 *    This application controls the UAV in order to fullfill the given mission.
 *    Its role is to select which applications must be executed by giving requests to
 *    the Adaptation Manager. A shared memory is used for IPC.
 *************************************************************************************/

#include "mission_manager_node.hpp"

/*********** Global variables ***********/ 
int	verbose;
double roll, pitch, yaw;
double prev_roll, prev_pitch, prev_yaw;
float    battery_level;
bool   first_time_imu;
boost::shared_ptr<ros::Publisher> notify_from_MM_pub;
/*********** Global variables ***********/ 



/*******************************************************************
 * battery_callback
 * Author : EM 
 * @param bat_msg
 * 
 * Callback function to get battery %
*******************************************************************/
void battery_callback(const sensor_msgs::BatteryState::ConstPtr &bat_msg)
{
   battery_level = bat_msg->percentage;
}


/*******************************************************************
 * imu_callback
 * Author : EM 
 * @param imu_msg, 
 * 
 * Callback function to get IMU data
*******************************************************************/
void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg)
{
   double quatx= imu_msg->orientation.x;
   double quaty= imu_msg->orientation.y;
   double quatz= imu_msg->orientation.z;
   double quatw= imu_msg->orientation.w;

	double tmp_roll, tmp_pitch, tmp_yaw;

   tf::Quaternion q(quatx, quaty, quatz, quatw);
   tf::Matrix3x3 m(q);
   m.getRPY(tmp_roll, tmp_pitch, tmp_yaw);

	if(first_time_imu)	
	{
		prev_roll = tmp_roll;
		prev_pitch = tmp_pitch;
		prev_yaw = tmp_yaw;

		first_time_imu = false;
	} else {
		prev_roll = roll;
		prev_pitch = pitch;
		prev_yaw = yaw;
	}

	roll = tmp_roll;
	pitch = tmp_pitch;
	yaw = tmp_yaw;
	
	ROS_INFO("\nSeq: [%d]", imu_msg->header.seq);
   ROS_INFO("\nRoll: [%f],Pitch: [%f],Yaw: [%f]",roll,pitch,yaw);
}



void achievable_Callback(const std_msgs::Int32::ConstPtr &msg1)
{
   std_msgs::Int32 msg;
   ROS_INFO("[RECU][ADAPTATION_MANAGER][Achievable_VALUE OF TASKS]");
   ROS_INFO("[%d]", msg1->data);
   
   

   ROS_INFO("CHANGEMENT DE TABLE C3, ENVOIE DE NOTIF A AUTOMATE EN COURS...");
   msg.data = 1;
   notify_from_MM_pub->publish(msg);
}


//******************
void cpu_load_Callback(const std_msgs::Float32::ConstPtr &load)
{
   ROS_INFO("[CPU][LOAD] : [%f]", load->data);
}


//***********************
int main(int argc, char **argv)
{
   ros::init(argc, argv, "mission_manager");
   ros::NodeHandle nh("~"); //EM, use ~ for private parameters, here it's for verbose
   
   //EM, to use the verbose argument in command line write this: _verbose:=value
   if (!nh.hasParam("verbose"))
      ROS_ERROR("No param named 'verbose'");
    
   if(nh.getParam("verbose",  verbose))
      ROS_INFO("Verbose level = %d", verbose);
   else
   {
      ROS_ERROR("FAILED TO GET verbose, default value %d",VERBOSITY_DEFAULT);
      verbose = VERBOSITY_DEFAULT;
   }


   ros::Subscriber achievable_sub = nh.subscribe("/achievable_topic", 1000, achievable_Callback);
   ros::Subscriber cpu_sub = nh.subscribe("/cpu_load_topic", 1000, cpu_load_Callback);
   ros::Subscriber imu_sub = nh.subscribe("mavros/imu/data", 1000, imu_callback);
   ros::Subscriber bat_sub = nh.subscribe("mavros/battery", 1000, battery_callback);
   
   notify_from_MM_pub = boost::make_shared<ros::Publisher>(
      nh.advertise<std_msgs::Int32>("/notify_from_MM_topic", 1000));

   ROS_INFO("[MISSION_MANAGER]: listening");

   ros::Rate loop_rate(1); //10hz = 100ms, 0.1hz=10s
   while (ros::ok())
   {
      ros::spinOnce();

      ROS_INFO("TIME TO SLEEP");
      loop_rate.sleep();
   }
}
