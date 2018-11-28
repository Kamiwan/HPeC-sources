#include <ros/ros.h>
#include <boost/thread.hpp>
#include <iostream>
#include <string>

#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"

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
   ros::NodeHandle n;
   //************** reception de la liste des taches nn realisables, publiée par l'automate ...
   ros::Subscriber achievable_sub = n.subscribe("/achievable_topic", 1000, achievable_Callback);
   notify_from_MM_pub = boost::make_shared<ros::Publisher>(
      n.advertise<std_msgs::Int32>("/notify_from_MM_topic", 1000));

   ros::Subscriber cpu_sub = n.subscribe("/cpu_load_topic", 1000, cpu_load_Callback);

   ROS_INFO("[mission_manager][listening]");

   ros::Rate loop_rate1(10);
   while (ros::ok())
   {

      //lecture de C3 apres modif des Qos et Texec
      ROS_INFO("TABLE C3");

      ros::spin();

      loop_rate1.sleep();
   }
}
