/** @ingroup drone_move_planner
 */

/**
 * @file
 * 
 * \file droneMovePlannerClass.cpp
 * \brief 
 * \author D. Wallach
 * \version 0.1
 * \date 19 février 2016
 */
 
#include "droneMovePlannerClass.h"
#include <vector>
#include <iterator>
#include <iostream>
#include <ros/ros.h>
#include <ros/console.h>

int cptANTICOL = 0;

droneMovePlannerClass::droneMovePlannerClass() 
{
    resetGoalAndPath();
    m_hasPathChanged = false;
    m_isMoving=false;
    m_isFlying=false;
}
    
bool droneMovePlannerClass::setMode(std::string a_mode)
{
    bool isWaypointForced = false;

    if ((a_mode.compare("WAYPOINT") != 0) && (a_mode.compare("STANDBY") != 0) && (a_mode.compare("ANTICOL") != 0)) 
    {
        ROS_ERROR_STREAM("Trying to set system state to " << a_mode << ". Only WAYPOINT, ANTICOL and STANDBY are recognized modes. Ignoring.");
        return -1;
    } else {
        if(a_mode.compare("ANTICOL") != 0)
        {
            cptANTICOL=0;
        }
    }    

    /*if(a_mode.compare("ANTICOL")==0){
        cptANTICOL++;
        std::cout << "ANTICOL counter VALUE =" << cptANTICOL << std::endl;
        if(cptANTICOL>30)
        {
            ros::param::set("/system_state/state", "WAYPOINT");
            isWaypointForced = true;
            cptANTICOL=0;
        }
    }*/
    
    if(!isWaypointForced)
        ros::param::set("/system_state/state", a_mode);

    ROS_INFO_STREAM("Setting system state to " << a_mode);
    return 0;
}

bool droneMovePlannerClass::setWaypoint (geometry_msgs::Pose a_waypoint)
{
    m_goal.pose = a_waypoint;
    m_hasPathChanged = true;
    return 0;
}
    
bool droneMovePlannerClass::setWaypointList (std::vector<geometry_msgs::Pose> a_waypointlist)
{
    m_isMoving=false;
    m_list = a_waypointlist;
    if (!m_isFlying)
    {
        geometry_msgs::Pose wp;
        wp.position.x=m_pose.pose.position.x;
        wp.position.y=m_pose.pose.position.y;
        m_altRef = m_pose.pose.position.z;
        wp.position.z= 5;
        m_list.push_back(wp);
    }
    return 0;
}
    
bool droneMovePlannerClass::setAnticol(int8_t dir)
{
	double x_l = 7*cos(dir*45*M_PI/180);
	double y_l = 7*sin(dir*45*M_PI/180);
	double x_r = x_l*cos(m_orientation) + y_l*sin(m_orientation);
	double y_r = - x_l*sin(m_orientation) + y_l*cos(m_orientation);
        geometry_msgs::Pose wp;
        wp.position.x=m_pose.pose.position.x + x_r;
        wp.position.y=m_pose.pose.position.y + y_r;
        wp.position.z=5;
        m_isMoving = true;
        setWaypoint(wp);
        m_lastAnticol = ros::Time::now();
}
    
bool droneMovePlannerClass::setPrevMode(std::string a_mode)
{
	if ((a_mode.compare("WAYPOINT") != 0) && (a_mode.compare("STANDBY") != 0) && (a_mode.compare("ANTICOL") != 0)) 
	{
	ROS_ERROR_STREAM("Trying to set previous system state to " << a_mode << ". Only WAYPOINT, ANTICOL and STANDBY are recognized modes. Ignoring.");
	return -1;
	}    

	m_prevMode = a_mode;
	return 0;
}
    
bool droneMovePlannerClass::setPose(const nav_msgs::Odometry::ConstPtr& a_odom)
{
    m_pose.pose = a_odom->pose.pose ;
    return 0; 
    
    }

bool droneMovePlannerClass::setPosition(geometry_msgs::Pose a_wp)
{
    m_pose.pose = a_wp ;
    return 0; 
    
    }

bool droneMovePlannerClass::pathSent()
{
    m_hasPathChanged = false;
    return 0;
}

int droneMovePlannerClass::setCurrentOrientation(double a_orientation)
{
    m_orientation = a_orientation*M_PI/180 ;
    return 0; 
    }
    
bool droneMovePlannerClass::update(std::string a_sytem_state)
{
    std::string v_mode = a_sytem_state ;
    
    // Comportement dépend du mode
    if (a_sytem_state.compare("ANTICOL") == 0)
    {
   	ros::Duration v_anticolTimer(2.5);
    	if ( (isGoalReached())||((ros::Time::now() - m_lastAnticol)>=v_anticolTimer) )
    	{
                v_mode = m_prevMode;
                m_isMoving= false;
                //If Goal reached after obstacle avoidance, we MUST set WAYPOINT MODE to continue the mission
                if(isGoalReached())
                {
                    setMode("WAYPOINT"); //std::cout << "GOAL REACHED : " << isGoalReached() << std::endl;
                } else
                {
                    setMode(v_mode);
                }

    	} else 
        {
            //std::cout << "WE ARE IN ELSE PART!!!" << std::endl;
        }

    }
    
    if (a_sytem_state.compare("WAYPOINT") == 0)
    {
        //std::cout << "DRONE POSITION : x=" << m_pose.pose.position.x  << " ; y="<< m_pose.pose.position.y << " ; z=" << m_pose.pose.position.z << std::endl;

        if (m_isMoving)
        {
            if (isGoalReached())
            {
                if (m_path.empty())
                {
                    m_list.pop_back();
                    m_isMoving= false;
                }
                else
                {
                    setWaypoint((m_path.back()).pose);
                    m_path.pop_back();
                }
            }
        }
        else
        {
            if(m_list.empty())
            {
                geometry_msgs::Pose wp;
                wp.position.x=0;
                wp.position.y=0;
                wp.position.z=0;
                m_isFlying = false;
                setWaypoint(wp);
                v_mode = "STANDBY";
                setMode(v_mode); 
                return 0;
            }
            else
            {
                resetGoalAndPath();
                m_isFlying = true;
                m_isMoving = true;
                computePathToWaypoint(m_list.back());
                return 0;
            }
        }
        // Sinon, on ne fait rien
        return 0; 
    }
    
    if (a_sytem_state.compare("STANDBY") == 0)
    {
        return 0;
    }
    
    // L'état n'est pas reconnu
    ROS_INFO_STREAM("System state is: " << a_sytem_state << ". It is not WAYPOINT or STANDBY. Doing nothing.");
}

bool droneMovePlannerClass::isGoalReached()
{
    if (!isGoalSet())
    {
        ROS_INFO_STREAM("No goal is set.");
        return true;
    }
    //ROS_ERROR("%f,%f",std::abs(m_pose.pose.position.x - m_goal.pose.position.x),std::abs(m_pose.pose.position.y - m_goal.pose.position.y));
    return ((std::abs(m_pose.pose.position.x - m_goal.pose.position.x)<3.0) && 
        (std::abs(m_pose.pose.position.y - m_goal.pose.position.y)<3.0) && 
        (std::abs(m_pose.pose.position.z - (m_goal.pose.position.z + m_altRef))<1.0));
}

communication::path_msg droneMovePlannerClass::getPathMsg()
{
    communication::path_msg v_msg;
    std::vector<geometry_msgs::PoseStamped> newpath;
    newpath.push_back(m_goal);
    v_msg.hasChanged = m_hasPathChanged;
    v_msg.poses = newpath;
    v_msg.flying = m_isFlying;
    
    return v_msg; 
    
}

bool droneMovePlannerClass::hasPathChanged()
{
    return m_hasPathChanged;
}

bool droneMovePlannerClass::computePathToWaypoint(geometry_msgs::Pose a_waypoint)
{    
    // Compute path
    geometry_msgs::PoseStamped temp;
    temp.pose = a_waypoint;  
    m_path.clear();
    m_path.push_back(temp);
    
    return 0;
}

bool droneMovePlannerClass::resetGoalAndPath() 
{
    m_goal.pose.position.x = -1.0 ; 
    m_goal.pose.position.y = -1.0 ; 
    m_goal.pose.position.z = -1.0 ; 
    m_path.clear();
    
    return 0;
}

bool droneMovePlannerClass::isGoalSet()
{
    return (m_goal.pose.position.x != -1.0);
}
