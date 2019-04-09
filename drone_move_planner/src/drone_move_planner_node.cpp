#include "droneMovePlannerClass.h"
#include "LatLong-UTMconversion.h"

#include "communication/path_msg.h"
#include "communication/goalGPS_msg.h"
#include "communication/goalGPSList_msg.h"
#include "communication/obstacle_detection_msg.h"

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>
#include <std_msgs/Float64.h>
#include <iostream>
#include <iomanip>
#include <sensor_msgs/NavSatFix.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include <ros/console.h>
#include <geometry_msgs/PoseStamped.h>
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

using namespace std;

droneMovePlannerClass g_droneMovePlanner; 
int sendOrders = 0;
communication::goalGPS_msg wpTemp;

communication::goalGPSList_msg wpListSimu;

std::vector<communication::goalGPS_msg> vectTemp;
std::string g_sytem_state = "STANDBY";

void compass_callback(const std_msgs::Float64::ConstPtr& a_orientation);
void goal_Callback(const communication::goalGPSList_msg::ConstPtr& a_goal);
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr& a_position);
void anticol_callback(const communication::obstacle_detection_msg::ConstPtr& a_detection);

bool got_path = false;
bool got_goal = false;
mavros_msgs::State current_state;
std::vector<mavros_msgs::Waypoint> wps;
mavros_msgs::Waypoint wp;
mavros_msgs::WaypointList mission_list;

// Callbacks for path and goal coming from multiplexer and state from mavros
void path_Callback(const communication::path_msg a_path);
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
ros::Time lastCol;


void goal_Simu(const communication::goalGPSList_msg a_goal)
{
	double Lat;
	double Lon;
	double UTMNorthing;
	double UTMEasting;
	char UTMZone[4];
	int RefEllipsoid = 23;//WGS-84. See list with file "LatLong- UTM conversion.cpp" for id numbers
	std::vector<geometry_msgs::Pose> wplist;
	geometry_msgs::Pose wp;

	for (int i = (((a_goal).waypoints).size() - 1); i >= 0; i--)
   	{
		LLtoUTM(RefEllipsoid, (double)((a_goal.waypoints[i]).lat), (double)((a_goal.waypoints[i]).lon), UTMNorthing, UTMEasting, UTMZone);
        
		wp.position.x=UTMEasting;
		wp.position.y=UTMNorthing;
		wp.position.z=(a_goal.waypoints[i]).alt;

        	wplist.push_back(wp);
	}
	g_droneMovePlanner.setWaypointList(wplist);
	g_droneMovePlanner.setMode("WAYPOINT");
}

int main (int argc, char ** argv)
{        
	ros::init(argc, argv, "drone_move_planner_node");
	ros::NodeHandle nh;
	//EM, home coordinates : -35.363261,149.165230
	lastCol = ros::Time::now();
	wpTemp.lat = -35.362661; wpTemp.lon=149.165230; wpTemp.alt=5;
	vectTemp.push_back(wpTemp);
	wpTemp.lat=-35.362661; wpTemp.lon=149.165830;
	vectTemp.push_back(wpTemp);
	wpTemp.lat=-35.363261; wpTemp.lon=149.165830;
	vectTemp.push_back(wpTemp);
	wpTemp.lat=-35.363261; wpTemp.lon=149.165230;
	vectTemp.push_back(wpTemp);
	wpListSimu.waypoints=vectTemp;
    
	// subscribe
   	ros::Subscriber v_compass_sub = nh.subscribe("mavros/global_position/compass_hdg", 1, compass_callback);
	ros::Subscriber v_gps_sub = nh.subscribe("mavros/global_position/global", 1, gps_callback);
	ros::Subscriber v_obs_sub = nh.subscribe("obstacle_detection_topic", 1, anticol_callback);
	ros::Subscriber v_state = nh.subscribe("mavros/state", 1, &state_Callback);
	ros::Subscriber v_waypoints = nh.subscribe("mavros/mission/waypoints", 1, &waypoints_Callback);

	//Mavros services client instances
	ros::ServiceClient wpPush_cl = nh.serviceClient<mavros_msgs::WaypointPush>("/mavros/mission/push");
	ros::ServiceClient setMode_cl = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	ros::ServiceClient arming_cl = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
	ros::ServiceClient TakeOff_cl = nh.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/takeoff");
 
	//Creating instances of the required services from MAVROS to send and clear WP to the autopilot
	mavros_msgs::WaypointPush srv_wpPush;
	mavros_msgs::SetMode srv_setMode;
	mavros_msgs::CommandBool srv_arming;
	mavros_msgs::CommandTOL srv_takeOff;

	srv_takeOff.request.latitude = 0;
	srv_takeOff.request.longitude = 0;
	srv_takeOff.request.altitude = 5;

	// spin
	double v_rate_double ;
	if(!nh.getParam("/node_activation_rates/drone_move_planner", v_rate_double))
	{
	ROS_INFO_STREAM("Could not read activation rate for node drone_move_planner. Setting to 20 Hz.");
	v_rate_double = 20.0 ;
	}
	ROS_INFO_STREAM("drone_move_planner node activation rate set to " << v_rate_double);
	ros::Rate v_rate(v_rate_double); 
    
	while (ros::ok())
	{
        ros::spinOnce();
        if(sendOrders==1)
        {
        	goal_Simu(wpListSimu);
        	sendOrders=2;
        }
        // listen to system state
        if(!nh.getParam("/system_state/state", g_sytem_state))
        {
            ROS_ERROR_STREAM_ONCE("Could not read system state.");
        }
        
        // compute goal and publish
        g_droneMovePlanner.update(g_sytem_state);
        if (g_droneMovePlanner.hasPathChanged())
        {
            //v_path_pub.publish(g_droneMovePlanner.getPathMsg());
            path_Callback(g_droneMovePlanner.getPathMsg());
            g_droneMovePlanner.pathSent();
        }
        
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
                                ROS_INFO_STREAM("Am I flying? = " << in_flight);
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
                                ROS_INFO_STREAM("Am I flying? = " << in_flight);
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
                    ROS_INFO("PX4C: I'm not in the air so I cannot land...");
                    got_path = false;
                    in_flight = false;
                    ROS_INFO_STREAM("Am I flying? = " << in_flight);
                }
            }
        }
        
        
        v_rate.sleep();
	}
    
    return 0;
}

void goal_Callback(const communication::goalGPSList_msg::ConstPtr& a_goal)
{
	double Lat;
	double Lon;
	double UTMNorthing;
	double UTMEasting;
	char UTMZone[4];
	int RefEllipsoid = 23;//WGS-84. See list with file "LatLong- UTM conversion.cpp" for id numbers
	std::vector<geometry_msgs::Pose> wplist;
	geometry_msgs::Pose wp;

	for (int i = (((a_goal)->waypoints).size() - 1); i >= 0; i--)
	{
		LLtoUTM(RefEllipsoid, (double)((a_goal->waypoints[i]).lat), (double)((a_goal->waypoints[i]).lon), UTMNorthing, UTMEasting, UTMZone);
        
        wp.position.x=UTMEasting;
        wp.position.y=UTMNorthing;
        wp.position.z=(a_goal->waypoints[i]).alt;
        
        wplist.push_back(wp);
	}
	g_droneMovePlanner.setWaypointList(wplist);
	g_droneMovePlanner.setMode("WAYPOINT");
}


void compass_callback(const std_msgs::Float64::ConstPtr& a_orientation)
{
	g_droneMovePlanner.setCurrentOrientation (a_orientation->data);
}

void gps_callback(const sensor_msgs::NavSatFix::ConstPtr& a_position)
{
	double Lat;
	double Lon;
	double UTMNorthing;
	double UTMEasting;
	char UTMZone[4];
	int RefEllipsoid = 23;//WGS-84. See list with file "LatLong- UTM conversion.cpp" for id numbers
	geometry_msgs::Pose wp;
	LLtoUTM(RefEllipsoid, (double)(a_position->latitude), (double)(a_position->longitude), UTMNorthing, UTMEasting, UTMZone);

	wp.position.x=UTMEasting;
	wp.position.y=UTMNorthing;
	wp.position.z=a_position->altitude;
	g_droneMovePlanner.setPosition(wp);
	sendOrders||sendOrders++;
}


void anticol_callback(const communication::obstacle_detection_msg::ConstPtr& a_detection)
{
	int8_t dir = a_detection->direction; 
        if((dir!=0)&&(g_sytem_state.compare("STANDBY")!=0))
        {
        	ros::Duration anticolTimer(0.2);
        	if ((ros::Time::now() - lastCol)>= anticolTimer)
        	{
        		lastCol = ros::Time::now();
				g_droneMovePlanner.setAnticol(dir);
				g_droneMovePlanner.setPrevMode(g_sytem_state);
				g_droneMovePlanner.setMode("ANTICOL");
			}
        }
}

void path_Callback(const communication::path_msg a_path)
{
	ROS_INFO_STREAM("PX4C: Received Mission Path.");
	g_px4comm.setPosesFromPath(a_path.poses);
	airborne = a_path.flying;
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
/** @} */ 
