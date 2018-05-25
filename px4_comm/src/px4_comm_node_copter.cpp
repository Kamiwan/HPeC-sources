/** @defgroup px4_comm px4_comm_node
* 	Pixhawk Communication
* 	This node's objective is to manage the comunication between the
*   message multiplexer (drone_move_commander) running on the Odroid
*   card and the Pixhawk.
*
*  Subscribed topics :
*  	- "drone_move_planner/path" (type communication::path_msg).
*	- "drone_move_planner/collision_avoidance_goal" (type geometry_msgs::PoseStamped).
*	- "mavros/state" (type mavros_msgs::State).
*	- "mavros/mission/waypoints" (type mavros_msgs::WaypointList).
*
*  Services used :
* 	- "/mavros/mission/push" (type mavros_msgs::WaypointPush).
*	- "/mavros/set_mode" (type mavros_msgs::SetMode).
*	- "/mavros/cmd/arming" (type mavros_msgs::CommandBool).
*/

#include <cstdlib>
#include <iostream>
#include <vector>

#include <ros/ros.h>
#include <ros/console.h>
#include <geometry_msgs/PoseStamped.h>
#include <communication/path_msg.h>
#include <mavros_msgs/CommandCode.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/Waypoint.h>
#include <mavros_msgs/WaypointList.h>
#include <mavros_msgs/WaypointSetCurrent.h>
#include <mavros_msgs/WaypointPush.h>
#include <mavros_msgs/CommandTOL.h>
#include "px4_comm_class.h"

//global variables
bool got_path = false;
bool got_goal = false;
mavros_msgs::State current_state;
std::vector<mavros_msgs::Waypoint> wps;
mavros_msgs::Waypoint wp;
mavros_msgs::WaypointList mission_list;

// Callbacks for path and goal coming from multiplexer and state from mavros
void path_Callback(const communication::path_msg::ConstPtr& a_path);
void goal_Callback(const geometry_msgs::PoseStamped::ConstPtr& a_goal);
void state_Callback(const mavros_msgs::State::ConstPtr& a_state);
void waypoints_Callback(const mavros_msgs::WaypointList::ConstPtr& a_waypointList);

// Node Method Definitions
bool set_Mode_LAND(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl);
bool set_Mode_AUTO(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl);
bool set_Mode_LOITER(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl);
bool set_Mode_STABILIZE(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl);
bool set_Mode_GUIDED(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl);
bool arm_drone(ros::ServiceClient& arming_cl, mavros_msgs::CommandBool& srv_arming);
bool disarm_drone(ros::ServiceClient& arming_cl, mavros_msgs::CommandBool& srv_arming);
bool push_waypoints(mavros_msgs::WaypointPush& srv_wpPush, ros::ServiceClient& wpPush_cl);
bool takeoff_order(mavros_msgs::CommandTOL& srv_takeOff, ros::ServiceClient& TakeOff_cl);

//Creating instance of the px4_comm class for methods
px4_comm_class g_px4comm;
bool airborne = false;
bool in_flight = false;

/* Main
* Initializes the node handler, subscribe to the topics that are
* published by the multiplexer and mavros and does the processing to
* publish the WP thru the WaypointPush service provided by mavros
* in order to link the Odroid Card with the PixHawk autopilot
*/
int main(int argc, char **argv)
{
	int rate = 10;

	ros::init(argc, argv, "px4_comm");
	ros::NodeHandle v_nh;
	ros::Rate r(rate);

	//subscribe to Path and Goal and drone mode
	ros::Subscriber v_path = v_nh.subscribe("drone_move_planner/path", 1, &path_Callback);
	//ros::Subscriber v_goal = v_nh.subscribe("drone_move_commander/collision_avoidance_goal", 1, &goal_Callback);
	ros::Subscriber v_state = v_nh.subscribe("mavros/state", 1, &state_Callback);
	ros::Subscriber v_waypoints = v_nh.subscribe("mavros/mission/waypoints", 1, &waypoints_Callback);

	//Mavros services client instances
	ros::ServiceClient wpPush_cl = v_nh.serviceClient<mavros_msgs::WaypointPush>("/mavros/mission/push");
	ros::ServiceClient setMode_cl = v_nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	ros::ServiceClient arming_cl = v_nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
	ros::ServiceClient TakeOff_cl = v_nh.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/takeoff");

	//Creating instances of the required services from MAVROS to send and clear WP to the autopilot
	mavros_msgs::WaypointPush srv_wpPush;
	mavros_msgs::SetMode srv_setMode;
	mavros_msgs::CommandBool srv_arming;
	mavros_msgs::CommandTOL srv_takeOff;

    srv_takeOff.request.latitude = 0;
    srv_takeOff.request.longitude = 0;
    srv_takeOff.request.altitude = 5;
    
	while (ros::ok())
	{
		ros::spinOnce();

		if (got_path)
		{
            if (airborne)
            {
                srv_wpPush.request.waypoints.clear();
                wps = g_px4comm.path_wp_vector();

                for (int i = 0; i < wps.size(); i++)
                {
                    wps[i].command = 16;
                    if (i == 1)
                    {
                        wps[i].is_current = true;
                    }
                    else
                    {
                        wps[i].is_current = false;
                    }
                    if (i<wps.size() - 1)
                    {
                        wps[i].autocontinue = true;
                    }
                    else
                    {
                        wps[i].autocontinue = false;
                    }
                    srv_wpPush.request.waypoints.push_back(wps[i]);
                }

                if (!in_flight)
                {
                    if (!current_state.armed)
                    {
                        if (current_state.mode.compare("LAND")!=0)
                        {
                            if (arm_drone(arming_cl, srv_arming))
                            {
                                if (set_Mode_GUIDED(srv_setMode, setMode_cl))
                                {
                                    if (takeoff_order(srv_takeOff, TakeOff_cl))
                                    {
                                        got_path = false;
                                        ROS_INFO("take off order sent");
                                        in_flight = true;
                                        ROS_INFO_STREAM("I'm I flying? = " << in_flight);
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (set_Mode_GUIDED(srv_setMode, setMode_cl))
                            {
                                if (arm_drone(arming_cl, srv_arming))
                                {
                                    if (takeoff_order(srv_takeOff, TakeOff_cl))
                                    {
                                        got_path = false;
                                        ROS_INFO("take off order sent");
                                        in_flight = true;
                                        ROS_INFO_STREAM("I'm I flying? = " << in_flight);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if (current_state.mode.compare("GUIDED")!=0)
                        {
                            if (set_Mode_GUIDED(srv_setMode, setMode_cl))
                            {
                                if (takeoff_order(srv_takeOff, TakeOff_cl))
                                {
                                    got_path = false;
                                    ROS_INFO("take off order sent");
                                    in_flight = true;
                                    ROS_INFO_STREAM("I'm I flying? = " << in_flight);
                                }
                            }
                        }
                        else
                        {
                            if (takeoff_order(srv_takeOff, TakeOff_cl))
                            {
                                got_path = false;
                                ROS_INFO("take off order sent");
                                in_flight = true;
                                ROS_INFO_STREAM("I'm I flying? = " << in_flight);
                            }
                        }
                    }
                }
                else
                {
                    if (set_Mode_LOITER(srv_setMode, setMode_cl))
                    {
                        if(push_waypoints(srv_wpPush, wpPush_cl))
                        {
                            if(set_Mode_AUTO(srv_setMode, setMode_cl))
                            {	
                                got_path = false;
                                in_flight = true;
                                ROS_INFO("I SHOULD BE MOVING! AM I MOVING? PLZ SAY YES!");
                                ROS_INFO_STREAM("I'm I flying? = " << in_flight);
                            }
                        }
                    }
                }
            }
            else
            {
                if (in_flight)
                {
                    if (set_Mode_LAND(srv_setMode, setMode_cl))
                    {
                        got_path = false;
                        in_flight = false;
                        ROS_INFO_STREAM("I'm I flying? = " << in_flight);
                    }
                }
                else
                {
                    ROS_INFO("PX4C: I'm not on the air so I cannot land...");
                    got_path = false;
                    in_flight = false;
                    ROS_INFO_STREAM("I'm I flying? = " << in_flight);
                }
            }
        }
		r.sleep();
	}
}
//CALLBACKS
void path_Callback(const communication::path_msg::ConstPtr& a_path)
{
	ROS_INFO_STREAM("PX4C: Received Mission Path.");
	g_px4comm.setPosesFromPath(a_path->poses);
	airborne = a_path->flying;
	got_path = true;
}

void state_Callback(const mavros_msgs::State::ConstPtr& a_state)
{
	//ROS_INFO_STREAM("PX4C: Received Drone State.");
	current_state = *a_state;
}

void waypoints_Callback(const mavros_msgs::WaypointList::ConstPtr& a_waypointList)
{
	ROS_INFO_STREAM("PX4C: Got drone current Mission List");
	mission_list = *a_waypointList;
}

//CLASS METHODS
bool set_Mode_LAND(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl)
{
	srv_setMode.request.base_mode = 0; //"MAV_MODE_PREFLIGHT";
	srv_setMode.request.custom_mode = "LAND";
	setMode_cl.call(srv_setMode);
	if (srv_setMode.response.mode_sent)
	{
		ROS_DEBUG("PX4C: SetMode LAND send ok %d value:", srv_setMode.response.mode_sent);
		return true;
	}
	else
	{
		ROS_ERROR("PX4C: Failed to Set Mode LAND");
		return false;
	}
}

bool set_Mode_AUTO(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl)
{
	srv_setMode.request.base_mode = 0; //"MAV_MODE_PREFLIGHT";
	srv_setMode.request.custom_mode = "AUTO";
	setMode_cl.call(srv_setMode);
	if (srv_setMode.response.mode_sent)
	{
		ROS_DEBUG("PX4C: SetMode AUTO send ok %d value:", srv_setMode.response.mode_sent);
		return true;
	}
	else
	{
		ROS_ERROR("PX4C:mavros_msgs/CommandTOL Failed to Set Mode AUTO");
		return false;
	}
}

bool set_Mode_LOITER(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl)
{
	srv_setMode.request.base_mode = 0; //"MAV_MODE_PREFLIGHT";
	srv_setMode.request.custom_mode = "LOITER";
	setMode_cl.call(srv_setMode);
	if (srv_setMode.response.mode_sent)
	{
		ROS_DEBUG("PX4C: SetMode LOITER send ok %d value:", srv_setMode.response.mode_sent);
		return true;
	}
	else
	{
		ROS_ERROR("PX4C:mavros_msgs/CommandTOL Failed to Set Mode LOITER");
		return false;
	}
}

bool set_Mode_GUIDED(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl)
{
	srv_setMode.request.base_mode = 0; //"MAV_MODE_PREFLIGHT";
	srv_setMode.request.custom_mode = "GUIDED";
	setMode_cl.call(srv_setMode);
	if (srv_setMode.response.mode_sent)
	{
		ROS_DEBUG("PX4C: SetMode GUIDED send ok %d value:", srv_setMode.response.mode_sent);
		return true;
	}
	else
	{
		ROS_ERROR("PX4C: Failed to Set Mode GUIDED");
		return false;
	}
}

bool set_Mode_STABILIZE(mavros_msgs::SetMode& srv_setMode, ros::ServiceClient& setMode_cl)
{
	srv_setMode.request.base_mode = 0; //"MAV_MODE_PREFLIGHT";
	srv_setMode.request.custom_mode = "STABILIZE";
	setMode_cl.call(srv_setMode);
	if (srv_setMode.response.mode_sent)
	{
		ROS_DEBUG("PX4C: SetMode STABILIZE send ok %d value:", srv_setMode.response.mode_sent);
		return true;
	}
	else
	{
		ROS_ERROR("PX4C: Failed to Set Mode STABILIZE");
		return false;
	}
}
bool arm_drone(ros::ServiceClient& arming_cl, mavros_msgs::CommandBool& srv_arming)
{
	srv_arming.request.value = true;
	arming_cl.call(srv_arming);
	if (srv_arming.response.success)
	{
		ROS_DEBUG("PX4C: ARM send ok %d value", srv_arming.response.success);
		return true;
	}else{
		ROS_ERROR("PX4C: Failed to Arm Drone");
		return false;
	}
}

bool disarm_drone(ros::ServiceClient& arming_cl, mavros_msgs::CommandBool& srv_arming)
{
	srv_arming.request.value = false;
	arming_cl.call(srv_arming);
	if (srv_arming.response.success) {
		ROS_DEBUG("PX4C: DISARM send ok %d value", srv_arming.response.success);
		return true;
	}else{
		ROS_ERROR("PX4C: Failed to Disarm Drone");
		return false;
	}
}

bool push_waypoints(mavros_msgs::WaypointPush& srv_wpPush, ros::ServiceClient& wpPush_cl)
{
    wpPush_cl.call(srv_wpPush);
    ROS_INFO("PCX4: WP(s) Pushed to srv_wpPush");
    ROS_INFO_STREAM("got_path= " << got_path);
	if (srv_wpPush.response.success)
	{
		ROS_INFO("PCX4: WP SEND OK. Response Value = %d", srv_wpPush.response.success);
		return true;
	}else{
		ROS_ERROR("FAILED TO SEND WPs");
		return false;
	}
}

bool takeoff_order(mavros_msgs::CommandTOL& srv_takeOff, ros::ServiceClient& TakeOff_cl)
{
    TakeOff_cl.call(srv_takeOff);
	if (srv_takeOff.response.success)
	{
		ROS_INFO("PCX4: TakeOff erder sent OK. Response Value = %d", srv_takeOff.response.success);
		return true;
	}else{
		ROS_ERROR("FAILED TO SEND TakeOff order");
		return false;
	}
}
