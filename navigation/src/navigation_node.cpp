/*************************************************************************************
 * File   : navigation_nodel.cpp, file to create easily ros nodes for HPeC
 * Copyright (C) 2018 Lab-STICC Laboratory
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 *
 * This model allows the user to create an application ros node that 
 * the adaptation manager can handle in the HPeC project.
 * 
 * This model must be used by copying the folder, then rename the package and files
 * according to your needs.
 * 
 * You can test this program:
 * 	1. run on a terminal "roscore" to get a ROS master
 *  2. on another terminal, run the command "rosrun navigation_nodel navigation_nodel_node"
 *************************************************************************************/
#include "navigation_node.h"


void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

void compass_cb(const std_msgs::Float64::ConstPtr& msg){
    current_heading = *msg;
	ROS_INFO_STREAM("Current heading" << current_heading.data);
}

/**##################################################################
 * MAIN
 * Author : EM 
 * 
 * Launch the ROS node and subscribe to topics in order to wait 
 * for orders.
##################################################################**/
int main(int argc, char **argv)
{
	ros::init(argc, argv, "navigation_node");
	ros::NodeHandle nh;

	ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("mavros/setpoint_position/local", 10);


	ros::Subscriber compass_heading = nh.subscribe<std_msgs::Float64>
            ("mavros/global_position/compass_hdg", 10, compass_cb);

    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
	ros::ServiceClient takeoff_client = nh.serviceClient<mavros_msgs::CommandTOL>
            ("/mavros/cmd/takeoff");
	ros::ServiceClient landing_client = nh.serviceClient<mavros_msgs::CommandTOL>
            ("/mavros/cmd/land");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");

			

	ros::Subscriber pos_sub = nh.subscribe("mavros/global_position/global", 
								1000, 
								gps_callback);

	ros::Publisher global_pos_pub = nh.advertise<mavros_msgs::GlobalPositionTarget>
            ("mavros/setpoint_position/global", 10);
	//ros::Publisher global_pos_pub = nh.advertise<mavros_msgs::GlobalPositionTarget>
    //        ("mavros/setpoint_raw/global", 10);

	
	//the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

	ROS_INFO("Wait 1 second!");
	ros::Duration(1).sleep(); // sleep for 1 second

    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

	ROS_INFO("Navigation connected!");

    geometry_msgs::PoseStamped pose;
	pose.header.stamp = ros::Time::now();
    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = 10;
	pose.header.stamp = ros::Time::now();
	pose.header.frame_id = "fcu";

    //send a few setpoints before starting
    for(int i = 50; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }
	
	mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "GUIDED";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;
	mavros_msgs::CommandTOL takeoff_cmd;
	takeoff_cmd.request.altitude 	= 10;//613.448;
	takeoff_cmd.request.latitude 	= 0; //-35.363;
	takeoff_cmd.request.longitude 	= 0; //149.165;
	takeoff_cmd.request.yaw			= 0;
	takeoff_cmd.request.min_pitch	= 0;
	mavros_msgs::CommandTOL landing_cmd;
	landing_cmd.request.altitude 	= 0;
	landing_cmd.request.latitude 	= 0;
	landing_cmd.request.longitude 	= 0;
	

	bool flying = false;

    ros::Time last_request = ros::Time::now();
	ros::Time test_pose    = ros::Time::now();

	geometry_msgs::PoseStamped positest;
    positest.pose.position.x = 10;
    positest.pose.position.y = 10;
    positest.pose.position.z = 20;


	//pose.header.stamp = ros::Time::now();
	//pose.pose.position.x = -2;
    //pose.pose.position.y = 5;
    //pose.pose.position.z = 10;

	if( current_state.mode != "GUIDED")
		{
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent)
			{
                ROS_INFO("GUIDED enabled");
				ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",current_altitude, current_longitude, current_latitude);
            }
        last_request = ros::Time::now();
    } 

	ros::spinOnce();
	rate.sleep();

	if(!current_state.armed )
	{
        if( arming_client.call(arm_cmd) &&
            arm_cmd.response.success)
		{
            ROS_INFO("Vehicle armed");
			ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",current_altitude, current_longitude, current_latitude);
        }
        last_request = ros::Time::now();
    }

	ROS_INFO("Wait 1 second!");
	ros::Duration(1).sleep(); // sleep for 1 second

	ros::spinOnce();
	rate.sleep();

	if( current_state.armed &&  current_state.mode == "GUIDED" )
	{
      if( takeoff_client.call(takeoff_cmd) &&
          takeoff_cmd.response.success)
		{	
			ROS_INFO("Takeoff started");
			ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",current_altitude, current_longitude, current_latitude);
			flying = true;
			pose.pose.position.x = 3;
    		pose.pose.position.y = 3;
    		pose.pose.position.z = 10;
    	}
    	last_request = ros::Time::now();
    }

	ROS_INFO("Wait 2 seconds!");
	ros::Duration(2).sleep(); // sleep for 2 seconds

	ros::spinOnce();
	rate.sleep();

	pose.pose.position.x = -2;
    pose.pose.position.y = 5;
    pose.pose.position.z = 10;
	pose.header.stamp = ros::Time::now();
	pose.header.frame_id = "fcu";
	pose.pose.orientation.w = 1;


	//EM, test with global position
	mavros_msgs::GlobalPositionTarget target;
	target.header.stamp	= ros::Time::now();
	target.header.frame_id = "fcu";
    target.altitude 	= 613.448;
    target.latitude 	= -35.3631;
    target.longitude 	= 149.1648;

	double d_lat, d_long;
	d_lat = current_latitude - target.altitude;
	d_long = current_longitude - target.longitude;
	
	double x, y, heading;
	x = std::cos(target.latitude) * std::sin(d_long);
	y = (std::cos(current_latitude) * std::sin(target.latitude))  - 
		(std::sin(current_latitude) * std::cos(target.latitude) * std::cos(d_long) );
	heading = std::atan2(x,y);

	double new_heading = current_heading.data*0.0174532925 - heading; //current_heading DEG TO RAD
	ROS_INFO_STREAM("NEW HEADING = " << new_heading);

	ROS_INFO_STREAM( "COMPUTED HEADING = " << (heading * (180/PI))); 
	ROS_INFO_STREAM("  NEW_HEADING" << (new_heading * (180/PI)));
	target.yaw			= new_heading;
	//current_heading.data *0.0174532925; //DEG to RAD
	target.yaw_rate		= 1;
	target.type_mask	= 0; //consider binary value



	


	for(int i = 100; ros::ok() && i > 0; --i){
        //local_pos_pub.publish(pose);
        global_pos_pub.publish(target);
        ros::spinOnce();
        rate.sleep();
    }

	ROS_INFO("NEW GLOBAL position published");

	/*
    while(ros::ok()){
        if( current_state.mode != "GUIDED" && altitude <= DEFAULT_HEIGHT  && 
            (ros::Time::now() - last_request > ros::Duration(5.0)))
		{
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent)
			{
                ROS_INFO("GUIDED enabled");
				ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",altitude, longitude, latitude);
            }
            last_request = ros::Time::now();
        } 
		else
		{
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0)))
			{
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success)
				{
                    ROS_INFO("Vehicle armed");
					ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",altitude, longitude, latitude);
                }
                last_request = ros::Time::now();
            }

			if( current_state.armed &&  altitude < (DEFAULT_HEIGHT + 2) &&
				current_state.mode == "GUIDED" &&
                (ros::Time::now() - last_request > ros::Duration(0.2)))
			{
                if( takeoff_client.call(takeoff_cmd) &&
                    takeoff_cmd.response.success)
				{
                    ROS_INFO("Takeoff started");
					ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",altitude, longitude, latitude);
					flying = true;
					pose.pose.position.x = 0;
    				pose.pose.position.y = 0;
    				pose.pose.position.z = 10;
                }
                last_request = ros::Time::now();
            }

			
			if(ros::Time::now() - test_pose > ros::Duration(30.0) && flying)
			{
				if( landing_client.call(landing_cmd) &&
					landing_cmd.response.success)
				{
					ROS_INFO("Landing started");
					ROS_INFO("Altitude = %f Longitude = %f Latitude = %f ",altitude, longitude, latitude);
					flying = false;
					pose.pose.position.x = 0;
					pose.pose.position.y = 0;
					pose.pose.position.z = 0;
				}
				last_request = ros::Time::now();
			}
        }
		
        ros::spinOnce();
        rate.sleep();
    }*/

    return 0;
}



/*******************************************************************
 * imu_callback
 * Author : EM 
 * @param imu_msg, UAV GPS position from topic listened
 * 
 * Callback function to get IMU data
*******************************************************************/
void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg)
{
    printf("\nSeq: [%d]", imu_msg->header.seq);
    printf("\nOrientation-> x: [%f], y: [%f], z: [%f], w: [%f]",
			 imu_msg->orientation.x, imu_msg->orientation.y, 
			 imu_msg->orientation.z, imu_msg->orientation.w);

   	double quatx= imu_msg->orientation.x;
   	double quaty= imu_msg->orientation.y;
   	double quatz= imu_msg->orientation.z;
   	double quatw= imu_msg->orientation.w;

    tf::Quaternion q(quatx, quaty, quatz, quatw);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    printf("\nRoll: [%f],Pitch: [%f],Yaw: [%f]",roll,pitch,yaw);
    return ;

}

/*******************************************************************
 * gps_callback
 * Author : EM 
 * @param position, UAV GPS position from topic listened
 * 
 * Callback function to get GPS position
 * Here, only altitude is used but there are other data on position
 * (i) Sample code, you can erase if useless
*******************************************************************/
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position)
{
   current_altitude  = position->altitude;
   current_latitude  = position->latitude;
   current_longitude = position->longitude;
}

