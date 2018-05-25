//include classes
#include "obstacle_detection_class.h"

//include messages
# include "communication/fake_sensors_msg.h"
# include "communication/obstacle_detection_msg.h"

//include libraries
#include <ros/ros.h>
#include <vector>

//declare callbacks
void fake_sensors_callback(const communication::fake_sensors_msg::ConstPtr& data);

//declare instance of classes
obstacle_detection_class my_obstacle_detection;

bool is_ok = false;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "obstacle_detection");
	ros::NodeHandle nh;
	
	std::cout << "COUCOU C'EST MOI DETECT OBSTACLE!!!!!!!!" << std::endl;
	
	//declare subscribers
	ros::Subscriber sensor_sub = nh.subscribe("/fake_sensors_topic", 1, &fake_sensors_callback);
	
	//declare publishers
	ros::Publisher obs_det_pub = nh.advertise<communication::obstacle_detection_msg>("obstacle_detection_topic", 1);
	
	//load parameters
	double rate_double;
	if (!nh.getParam("/node_activation_rates/obstacle_detection", rate_double))
	{
		ROS_ERROR("Could not read obstacle detection's activation rate. Setting 10 Hz");
		rate_double = 10;
	}
	float a, b, c, d, e, q, t;
	bool to_set = true;
	if (!nh.getParam("/obstacle_detection_parameters/a", a))
	{
		ROS_ERROR("Could not read obstacle detection's parameter a. Node shutdown");
		to_set=false;
	}
	if (!nh.getParam("/obstacle_detection_parameters/b", b))
	{
		ROS_ERROR("Could not read obstacle detection's parameter b. Node shutdown");
		to_set=false;
	}if (!nh.getParam("/obstacle_detection_parameters/c", c))
	{
		ROS_ERROR("Could not read obstacle detection's parameter c. Node shutdown");
		to_set=false;
	}if (!nh.getParam("/obstacle_detection_parameters/d", d))
	{
		ROS_ERROR("Could not read obstacle detection's parameter d. Node shutdown");
		to_set=false;
	}if (!nh.getParam("/obstacle_detection_parameters/e", e))
	{
		ROS_ERROR("Could not read obstacle detection's parameter e. Node shutdown");
		to_set=false;
	}if (!nh.getParam("/obstacle_detection_parameters/q", q))
	{
		ROS_ERROR("Could not read obstacle detection's parameter q. Node shutdown");
		to_set=false;
	}if (!nh.getParam("/obstacle_detection_parameters/t", t))
	{
		ROS_ERROR("Could not read obstacle detection's parameter t. Node shutdown");
		to_set=false;
	}
	
	
	float dist_irs_max;
	float dist_irl_max;
	float dist_irl_min;
	if (!nh.getParam("/drone_features/irs_max", dist_irs_max))
	{
		ROS_ERROR("Could not read irs max range. Setting 1.5m");
		dist_irs_max = 1.5;
	}
	
	if (!nh.getParam("/drone_features/irl_max", dist_irl_max))
	{
		ROS_ERROR("Could not read irl max range. Setting 5.5m");
		dist_irs_max = 5.5;
	}
	
	if (!nh.getParam("/drone_features/irl_min", dist_irl_min))
	{
		ROS_ERROR("Could not read irl min range. Setting 1.0m");
		dist_irs_max = 1.0;
	}
	
	
	//set parameters
	ros::Rate rate(rate_double);
	if (to_set)
	{
		my_obstacle_detection.set_parameters(a,b,c,d,e,q,t);
		my_obstacle_detection.setSeuil(5.0);
		my_obstacle_detection.setMaxs(dist_irs_max,dist_irl_max, dist_irl_min);
	}
	else
	{
		ros::shutdown();
	}
	
	while (ros::ok())
	{
		ros::spinOnce();
		if(is_ok)
		{
			obs_det_pub.publish(my_obstacle_detection.publish());
		}
		rate.sleep();
	}
}

void  fake_sensors_callback(const communication::fake_sensors_msg::ConstPtr& data)
{
	my_obstacle_detection.setIRS(data->IRS);
	my_obstacle_detection.setIRL(data->IRL);
	my_obstacle_detection.setUS(data->US);
	is_ok = true;
}



