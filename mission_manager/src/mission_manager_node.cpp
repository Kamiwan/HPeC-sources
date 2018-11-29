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
boost::shared_ptr<ros::Publisher> notify_from_MM_pub;


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


   //************** reception de la liste des taches nn realisables, publiée par l'automate ...
   ros::Subscriber achievable_sub = nh.subscribe("/achievable_topic", 1000, achievable_Callback);
   ros::Subscriber cpu_sub = nh.subscribe("/cpu_load_topic", 1000, cpu_load_Callback);
   
   notify_from_MM_pub = boost::make_shared<ros::Publisher>(
      nh.advertise<std_msgs::Int32>("/notify_from_MM_topic", 1000));

   ROS_INFO("[mission_manager][listening]");

   ros::Rate loop_rate(10); //10hz = 100ms, 0.1hz=10s
   while (ros::ok())
   {

      //lecture de C3 apres modif des Qos et Texec
      ROS_INFO("TABLE C3");

      ros::spin();

      loop_rate.sleep();
   }
}
