/** @ingroup px4_comm
 *
 */

/**
 * @file
 *
 * \file px4_comm_class.cpp
 * \brief
 * \author J.J. Estimbre
 * \version 0.1
 * \date 24 novembre 2016
 */

#include "px4_comm_class.h"
#include "LatLong-UTMconversion.h"
#include <iomanip>
#include <ros/ros.h>
#include <ros/console.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <mavros_msgs/Waypoint.h>
#include <mavros_msgs/CommandCode.h>
#include <communication/path_msg.h>

using namespace std;

px4_comm_class::px4_comm_class()
{
    mavros_msgs::Waypoint wp;
    std::vector<mavros_msgs::Waypoint> wpv_path;
    mavros_msgs::Waypoint wpv_goal;

    //Filling the WP non-variable parameters
    wp.frame = mavros_msgs::Waypoint::FRAME_GLOBAL; //0; //GLOBAL COOR (GPS)
    wp.command = 16; //16; //NAVIGATE TO WAYPOINT
    wp.is_current = false;
    wp.autocontinue = false;
    wp.param1 = 0.0;
    wp.param2 = 2;
    wp.param3 = 0.0;
    wp.param4 = 0.0;
    wp.z_alt = 0.0;
}

px4_comm_class::~px4_comm_class()
{
}

std::vector<mavros_msgs::Waypoint> px4_comm_class::path_wp_vector()
{
    std::vector<mavros_msgs::Waypoint> wpv;
    wpv = wpv_path;
    return wpv;
}

mavros_msgs::Waypoint px4_comm_class::goal_wp()
{
    return wpv_goal;
}

void px4_comm_class::setPosesFromPath(std::vector<geometry_msgs::PoseStamped> a_poses)
{
    int RefEllipsoid = 23;
    //char UTMZone[4] = {'3','0','U'}; // Other zones in France include 30T 31U 31T 32U 32T
    char UTMZone[4] = {'5','5','H'};
    double Lat, Long;
    wpv_path.clear();
    for (int i = 0; i < a_poses.size(); i++)
    {
		UTMtoLL(RefEllipsoid, (double)a_poses[i].pose.position.y, (double)a_poses[i].pose.position.x, UTMZone, Lat, Long);
        wp.x_lat = (double)Lat;
		wp.y_long = (double)Long;
		//wp.z_alt = (double)a_poses[i].pose.position.z;
		ROS_ERROR("Lat %f, Long %f", Lat, Long);
		wp.z_alt = 0;
        if (i==0)
        {
            wpv_path.push_back(wp);
        }
        wpv_path.push_back(wp);
	}

    //UTMtoLL(RefEllipsoid, a_poses[0].pose.position.y, a_poses[0].pose.position.x, UTMZone, lat, lon);
    //wp.command = 16;
    //wp.x_lat = (float)lat;
    //wp.y_long = (float)lon;
    //wpv_path.push_back(wp);
    //wpv_path.push_back(wp);

}

void px4_comm_class::setPoseFromGoal(geometry_msgs::Pose a_goal)
{
    int RefEllipsoid = 23;
    //char UTMZone[4] = {'3','0','U'}; // Other zones in France include 30T 31U 31T 32U 32T
    char UTMZone[4] = {'5','5','H'};
    double Lat, Long;
    uint16_t command = 16;

    UTMtoLL(RefEllipsoid, (double)a_goal.position.y, (double)a_goal.position.x, UTMZone, Lat, Long);
    if (wp.command!=16)
        wp.command=command;
    wp.x_lat = (double)Lat;
    wp.y_long = (double)Long;
	wpv_goal = wp;
}
