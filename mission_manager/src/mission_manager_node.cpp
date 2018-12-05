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
int	 verbose;
double roll, pitch, yaw, prev_roll, prev_pitch, prev_yaw;
double altitude, longitude, latitude;
double ang_vel_x, ang_vel_y, ang_vel_z, lin_vel_x, lin_vel_y, lin_vel_z;
float  battery_level;
bool   first_time_imu;
boost::shared_ptr<ros::Publisher> notify_from_MM_pub;
/*********** Global variables ***********/ 



/*******************************************************************
 * battery_callback
 * Author : EM 
 * @param bat_msg
 * 
 * Callback function to get battery % between 0 and 1
*******************************************************************/
void battery_callback(const sensor_msgs::BatteryState::ConstPtr &bat_msg)
{
   battery_level = bat_msg->percentage;

   if(verbose > VERBOSITY_OFF)
   {
	   ROS_INFO("Seq: [%d]", bat_msg->header.seq);
      std::cout << "Battery left : [" << std::setprecision(3) << battery_level << "]% \n";
   }
}


/*******************************************************************
 * gps_pos_callback
 * Author : EM 
 * @param position
 * 
 * Callback function to get position
 * Altitude in m (MSL)
*******************************************************************/
void gps_pos_callback(const sensor_msgs::NavSatFix::ConstPtr &position)
{
	altitude  = position->altitude;
   latitude  = position->latitude;
   longitude = position->longitude;

   if(verbose > VERBOSITY_OFF)
   {
	   ROS_INFO("Seq: [%d]", position->header.seq);
      std::cout << "GPS POS Alt: [" << altitude << std::setprecision(10)
                  <<"], Long: [" << longitude  
                  << "], Lat: [" << latitude << "] \n";
   }
}

/*******************************************************************
 * gps_vel_callback
 * Author : EM 
 * @param vel_msg
 * 
 * Callback function to get UAV speed m/s
*******************************************************************/
void gps_vel_callback(const geometry_msgs::TwistStamped::ConstPtr &vel_msg)
{
   ang_vel_x = vel_msg->twist.angular.x;
   ang_vel_y = vel_msg->twist.angular.y;
   ang_vel_z = vel_msg->twist.angular.z;

   lin_vel_x = vel_msg->twist.linear.x;
   lin_vel_y = vel_msg->twist.linear.y;
   lin_vel_z = vel_msg->twist.linear.z;

   if(verbose > VERBOSITY_OFF)
   {
	   ROS_INFO("Seq: [%d]", vel_msg->header.seq);
      std::cout << "LINEAR SPEED x: [" << std::setprecision(3) << lin_vel_x 
                  << "], y: [" << lin_vel_y 
                  <<"], z: [" << lin_vel_z <<"]";
      std::cout << "\nANGULAR SPEED x: [" << ang_vel_x 
                  <<"], y: [" << ang_vel_y 
                  <<"], z: [" << ang_vel_z << "] \n";
   }
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
	
   if(verbose > VERBOSITY_LOW)
   {
	   ROS_INFO("Seq: [%d]", imu_msg->header.seq);
      std::cout   << std::setprecision(4) << "Roll: ["  << roll 
                  << "],Pitch: [" << pitch 
                  << "],Yaw: ["   << yaw << "] \n";
   }
}


/*******************************************************************
 * obstacle_callback
 * Author : EM 
 * @param detection_msg, 
 * 
 * Callback function to know if an obstacle is detected
*******************************************************************/
void obstacle_callback(const communication::obstacle_detection_msg::ConstPtr& detection_msg)
{
	int8_t dir = detection_msg->direction; 
   if(dir!=0) //EM TODO: add check if current state diff of STANDBY
   {
      if(verbose > VERBOSITY_OFF)
         ROS_INFO("OBSTACLE DETECTED !!!");
   }
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



//***********************
int main(int argc, char **argv)
{
   ros::init(argc, argv, "mission_manager");
   ros::NodeHandle nh;  //EM non-private Node Handle to get topics notifications from other nodes
   ros::NodeHandle n("~"); //EM, use ~ for private parameters, here it's for verbose
   
   //EM, to use the verbose argument in command line write this: _verbose:=value
   if (!n.hasParam("verbose"))
      ROS_ERROR("No param named 'verbose'");
    
   if(n.getParam("verbose",  verbose))
      ROS_INFO("Verbose level = %d", verbose);
   else
   {
      ROS_ERROR("FAILED TO GET verbose, default value %d",VERBOSITY_DEFAULT);
      verbose = VERBOSITY_DEFAULT;
   }

   ros::Subscriber imu_sub        = nh.subscribe("mavros/imu/data", 1000, imu_callback);
   ros::Subscriber bat_sub        = nh.subscribe("mavros/battery", 1000, battery_callback);
   ros::Subscriber vel_sub        = nh.subscribe("mavros/global_position/raw/gps_vel", 1000, gps_vel_callback);
   ros::Subscriber pos_sub        = nh.subscribe("mavros/global_position/global", 1000, gps_pos_callback);
   
   ros::Subscriber achievable_sub = nh.subscribe("/achievable_topic", 1000, achievable_Callback);
   ros::Subscriber obstacle_sub   = nh.subscribe("/cpu_load_topic", 1000, obstacle_callback);
   
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
