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

//EM, for CPU load functions, init_cpu_load() and current_cpu_value()
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

/*********** Global variables ***********/ 
int	 verbose;
double roll, pitch, yaw, prev_roll, prev_pitch, prev_yaw, delta_roll, delta_pitch, delta_yaw;
double altitude, longitude, latitude, illuminance;
double target_latitude, target_longitude;
double ang_vel_x, ang_vel_y, ang_vel_z, lin_vel_x, lin_vel_y, lin_vel_z;
float  battery_level;
bool   first_time_imu;
boost::shared_ptr<ros::Publisher> notify_from_MM_pub;
static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle; //EM, CPU load

ros::Time scenario_ref_time;
ros::Time scenario_current_time;

ros::Subscriber cam_light_sub;
ros::Subscriber light_sub;
ros::Subscriber imu_sub;
ros::Subscriber bat_sub;
ros::Subscriber vel_sub;
ros::Subscriber pos_sub;
ros::Subscriber achievable_sub;
ros::Subscriber obstacle_sub;

ros::Publisher  nav_order_pub;

ros::Publisher search_land_pub;
ros::Publisher stab_imu_pub;
ros::Publisher obstacle_avoidance_pub;
ros::Publisher detection_pub;
/*********** Global variables ***********/ 





/*******************************************************************
 * init_cpu_load
 * Author : EM 
 * 
 * First read of /proc/stat to get reference to compute cpu_load
*******************************************************************/
void init_cpu_load()
{
   FILE* file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
      &lastTotalSys, &lastTotalIdle);
   fclose(file);
}

/*******************************************************************
 * current_cpu_value
 * Author : EM 
 * @return percent
 * 
 * Function to the current cpu load in % 
*******************************************************************/
double current_cpu_value()
{
   double percent;
   FILE* file;
   unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

   file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
      &totalSys, &totalIdle);
   fclose(file);

   if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
       totalSys < lastTotalSys || totalIdle < lastTotalIdle)
   {
      //Overflow detection. Just skip this value.
      percent = -1.0;
   }
   else{
      total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
         (totalSys - lastTotalSys);
      percent = total;
      total += (totalIdle - lastTotalIdle);
      percent /= total;
      percent *= 100;
   }

   lastTotalUser = totalUser;
   lastTotalUserLow = totalUserLow;
   lastTotalSys = totalSys;
   lastTotalIdle = totalIdle;

   return percent;
}



/*******************************************************************
 * sensor_cam_callback
 * Author : EM 
 * @param img_msg
 * 
 * Allow to get light sensor data, Subscription mandatory to the 
 * camera used as light sensor. We do not need raw data so we use a
 * dumb callback.
*******************************************************************/
void sensor_cam_callback(const sensor_msgs::Image::ConstPtr &img_msg)
{
   if(verbose > VERBOSITY_HIGH)
   {
	   std::cout << "Seq: [" << img_msg->header.seq << "]" << " \n";
      std::cout << "DUMB light sensor callback OK \n";
   }
}

/*******************************************************************
 * light_sensor_callback
 * Author : EM 
 * @param light_msg
 * 
 * Callback function to get illuminance between 0 and 255
*******************************************************************/
void light_sensor_callback(const sensor_msgs::Illuminance::ConstPtr &light_msg)
{
   illuminance = light_msg->illuminance;
   
   if(verbose > VERBOSITY_OFF)
   {
	   std::cout << "Seq: [" << light_msg->header.seq << "]" << " \n";
      std::cout << "Illuminance = " << std::setprecision(3) << illuminance  << " \n";
   }
}

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
	   std::cout << "Seq: [" << bat_msg->header.seq << "]" << " \n";
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
	   std::cout << "Seq: [" << position->header.seq << "]" << " \n";
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
	   std::cout << "Seq: [" << vel_msg->header.seq << "]" << " \n";
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

   delta_roll  = roll  - prev_roll;       
   delta_pitch = pitch - prev_pitch;      
   delta_yaw   = yaw   - prev_yaw;  
	
   if(verbose > VERBOSITY_OFF)
   {
	   std::cout << "Seq: [" << imu_msg->header.seq << "]" << " \n";
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

void achievable_callback(const std_msgs::Int32::ConstPtr &msg1)
{
   std_msgs::Int32 msg;
   ROS_INFO("[MSG][ADAPTATION_MANAGER]: At least one task config is not achievable ");
   ROS_INFO("[%d]", msg1->data);
   
   ROS_INFO("C3 array modified, send message to Adaptation Manager...");
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

   cam_light_sub  = nh.subscribe("iris/light_sensor/rgb/image_raw", 1000, sensor_cam_callback);
   light_sub      = nh.subscribe("light_sensor_plugin/lightSensor", 1000, light_sensor_callback);
   imu_sub        = nh.subscribe("mavros/imu/data", 1000, imu_callback);
   bat_sub        = nh.subscribe("mavros/battery", 1000, battery_callback);
   vel_sub        = nh.subscribe("mavros/global_position/raw/gps_vel", 1000, gps_vel_callback);
   pos_sub        = nh.subscribe("mavros/global_position/global", 1000, gps_pos_callback);
   achievable_sub = nh.subscribe("/achievable_topic", 1000, achievable_callback);
   obstacle_sub   = nh.subscribe("obstacle_detection_topic", 1000, obstacle_callback);
   
   notify_from_MM_pub = boost::make_shared<ros::Publisher>(
      nh.advertise<std_msgs::Int32>("/notify_from_MM_topic", 1000));

   nav_order_pub = nh.advertise<communication::nav_control>
            ("navigation/order", 10);

   // EM : Publishers to bypass the Adaptation Manager for Static Scenario
   search_land_pub = nh.advertise<std_msgs::Int32>
            ("/search_landing_area_mgt_topic", 10);
   stab_imu_pub = nh.advertise<std_msgs::Int32>
            ("/stabilisation_imu_mgt_topic", 10);
   obstacle_avoidance_pub = nh.advertise<std_msgs::Int32>
            ("/obstacle_avoidance_mgt_topic", 10);
   detection_pub = nh.advertise<std_msgs::Int32>
            ("/detection_tracking_mgt_topic", 10);


   MemoryCoordinator sh_mem_access("User"); //EM, may MM able to use the shared memory

   ROS_INFO("[MISSION_MANAGER]: listening");
   ros::spinOnce();
   ROS_INFO("Wait 1 second!");
   ros::Duration(1).sleep(); // sleep needed to get the right time with scenario_ref_time


   scenario_ref_time       = ros::Time::now();
   scenario_current_time   = ros::Time::now();
   ROS_INFO_STREAM("[MISSION_MANAGER]: Reference time = " << scenario_ref_time);

   step_1 = false;
   step_2 = false;
   step_3 = false;
   step_4 = false;
   step_5 = false;

   ros::Rate loop_rate(1); //10hz = 100ms, 0.1hz=10s
   while (ros::ok())
   {
      ros::spinOnce();

      std::cout << "Current CPU load = " << current_cpu_value() << " %" << std::endl;
      std::cout << "Current Tiles used, Tile 1 = " << sh_mem_access.Busy_Tile_Read(TILE_1)
                  <<   ", Tile 2 = " << sh_mem_access.Busy_Tile_Read(TILE_2)
                  <<   ", Tile 3 = " << sh_mem_access.Busy_Tile_Read(TILE_3)
                  << std::endl;
      std::cout << "Current Detection exec time = " << sh_mem_access.Read_ExecTime(DETECTION) << std::endl
               <<  "Current Tracking  exec time = " << sh_mem_access.Read_ExecTime(TRACKING) << std::endl
               <<  "Current Detection       qos = " << sh_mem_access.Read_QoS(DETECTION) << std::endl
               <<  "Current Tracking        qos = " << sh_mem_access.Read_QoS(TRACKING)
               << std::endl;

      StaticScenario_1();

      ROS_INFO("TIME TO SLEEP");
      loop_rate.sleep();
   }
}



/******************************************************************************
 * StaticScenario_1
 * Author : EM 
 * 
 * Script which emulates Mission Manager orders for the UAV Mission
 * This function gives orders to the UAV FCU with navigation ROS node
 * It is also able to turn on/off other applications
******************************************************************************/
void StaticScenario_1()
{
   scenario_current_time            = ros::Time::now();
   ros::Duration scenario_duration  = scenario_current_time - scenario_ref_time;
   
   ROS_INFO_STREAM("Scenario Duration = " << scenario_duration.toSec());

   if(scenario_duration.toSec() > 10 && !step_1)
   {
      ROS_INFO("Send TAKEOFF order!");
      communication::nav_control nav_order_msg;
      nav_order_msg.order     = "TAKEOFF"; 
      nav_order_msg.altitude  = altitude + 11;

      nav_order_pub.publish(nav_order_msg);
      step_1 = true;
   }

   if(Compare( altitude, HOME_ALTITUDE+10, 0) && !step_2)
   {
      ROS_INFO("GO TO RESEARCH AREA!");
      communication::nav_control nav_order_msg;
      nav_order_msg.order     = "GPS_MOVE"; 
      nav_order_msg.altitude  = altitude;
      nav_order_msg.latitude  = -35.363086;
      nav_order_msg.longitude = 149.165250;
      target_latitude         = nav_order_msg.latitude;
      target_longitude        = nav_order_msg.longitude;
 
      nav_order_pub.publish(nav_order_msg);
      step_2 = true;

      ROS_INFO("Application activation!");
      std_msgs::Int32 activation_msg;
      activation_msg.data = 1;
      search_land_pub.publish(activation_msg);
      stab_imu_pub.publish(activation_msg);
      obstacle_avoidance_pub.publish(activation_msg);
      detection_pub.publish(activation_msg);
   }

   if(CompareGpsPositions(latitude, target_latitude, longitude, target_longitude, 5) && !step_3)
   {
      ROS_INFO("LOOKING FOR TARGET!");
      communication::nav_control nav_order_msg;
      nav_order_msg.order     = "GPS_MOVE"; 
      nav_order_msg.altitude  = altitude;
      nav_order_msg.latitude  = -35.362823;
      nav_order_msg.longitude = 149.165586;
      target_latitude         = nav_order_msg.latitude;
      target_longitude        = nav_order_msg.longitude;
 
      nav_order_pub.publish(nav_order_msg);
      step_3 = true;
   }

   /*
   if(scenario_duration.toSec() > 90 && !step_4)
   {
      ROS_INFO("Send GPS_MOVE order!");
      communication::nav_control nav_order_msg;
      nav_order_msg.order     = "GPS_MOVE"; 
      nav_order_msg.altitude  = altitude;
      nav_order_msg.latitude  = -35.363261;
      nav_order_msg.longitude = 149.165030;


      nav_order_pub.publish(nav_order_msg);
      step_4 = true;
   }


   if(scenario_duration.toSec() > 110 && !step_5)
   {
      ROS_INFO("Send LAND order!");
      communication::nav_control nav_order_msg;
      nav_order_msg.order = "LAND"; //Other parameters not needed for LAND order

      nav_order_pub.publish(nav_order_msg);
      step_5 = true;
   }
   */

}


bool Compare(double value1, double value2, int precision)
{
    return std::abs(value1 - value2) < std::pow(10, -precision);
}

bool CompareGpsPositions(double latitude_1, double latitude_2, 
      double longitude_1, double longitude_2, int precision)
{
   return Compare(latitude_1, latitude_2, precision) && Compare( longitude_1, longitude_2, precision);
}





/*******************************************************************
 * SetWaypointsAreaCovering
 * Author : EM 
 * @param lata latb latc latd longa longb longc longd, (all in radians)
 *    B_________C          B    B ------> C           C  
 *    | Area   |           ^                          v
 *    |  to    |           |                    x<----x
 *    | Cover  |           |                    v   
 *    |________|     A     A                    x--->x   A <-----D
 *    A        D                                     v
 *                                                   D
 *                  [        Step 1       ][   Step 2   ][ Step 3  ]
 * 
 *******************************************************************/
void SetWaypointsAreaCovering(double latitude_a, double latitude_b, 
        double latitude_c, double latitude_d, double longitude_a, double longitude_b, 
        double longitude_c, double longitude_d)
{
   std::vector<communication::nav_control> area_path;

   communication::nav_control next_waypoint;
   next_waypoint.order     = "GPS_MOVE";
   next_waypoint.altitude  = altitude;

   // EM Step 1: Go to A, then B, then C
   next_waypoint.latitude  = latitude_a;
   next_waypoint.longitude = longitude_a;
   area_path.push_back(next_waypoint);
   
   next_waypoint.latitude  = latitude_b;
   next_waypoint.longitude = longitude_b;
   area_path.push_back(next_waypoint);
   
   next_waypoint.latitude  = latitude_c;
   next_waypoint.longitude = longitude_c;
   area_path.push_back(next_waypoint);

   // EM Step 2: compute intermediate WPs before to go to D to cover the whole area



}





