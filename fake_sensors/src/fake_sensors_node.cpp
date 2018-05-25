//include classes
#include "fake_sensors_class.h"

//include messages

//include libraries
#include <ros/ros.h>
#include <vector>
# include "communication/fake_sensors_msg.h"

#include "sensor_msgs/LaserScan.h"

//declare instance of classes
fake_sensors_class my_fake_sensors;
float dist_irs_max;
float dist_irl_max;
float dist_us_max;
//declare callbacks
void us_callback(const sensor_msgs::LaserScanConstPtr& data, int sector);
void ir_callback(const sensor_msgs::LaserScanConstPtr& data, int sector);

int main(int argc, char** argv)
{
	ros::init(argc, argv, "fake_sensors");
	ros::NodeHandle nh;
	
	//declare subscribers
	ros::Subscriber us_sub1 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/US/US1", 1, boost::bind(us_callback, _1, 1));
	ros::Subscriber us_sub2 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/US/US2", 1, boost::bind(us_callback, _1, 2));
	ros::Subscriber us_sub3 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/US/US3", 1, boost::bind(us_callback, _1, 3));
	ros::Subscriber us_sub4 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/US/US4", 1, boost::bind(us_callback, _1, 4));
	ros::Subscriber us_sub5 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/US/US5", 1, boost::bind(us_callback, _1, 5));
	ros::Subscriber us_sub6 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/US/US6", 1, boost::bind(us_callback, _1, 6));
	ros::Subscriber irl_sub1 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRL/IRL1", 1, boost::bind(ir_callback, _1, 1));
	ros::Subscriber irl_sub2 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRL/IRL2", 1, boost::bind(ir_callback, _1, 2));
	ros::Subscriber irl_sub3 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRL/IRL3", 1, boost::bind(ir_callback, _1, 3));
	ros::Subscriber irl_sub4 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRL/IRL4", 1, boost::bind(ir_callback, _1, 4));
	ros::Subscriber irl_sub5 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRL/IRL5", 1, boost::bind(ir_callback, _1, 5));
	ros::Subscriber irl_sub6 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRL/IRL6", 1, boost::bind(ir_callback, _1, 6));
	ros::Subscriber irs_sub1 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRS/IRS1", 1, boost::bind(ir_callback, _1, -1));
	ros::Subscriber irs_sub2 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRS/IRS2", 1, boost::bind(ir_callback, _1, -2));
	ros::Subscriber irs_sub3 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRS/IRS3", 1, boost::bind(ir_callback, _1, -3));
	ros::Subscriber irs_sub4 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRS/IRS4", 1, boost::bind(ir_callback, _1, -4));
	ros::Subscriber irs_sub5 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRS/IRS5", 1, boost::bind(ir_callback, _1, -5));
	ros::Subscriber irs_sub6 = nh.subscribe<sensor_msgs::LaserScan> ("/iris/IRS/IRS6", 1, boost::bind(ir_callback, _1, -6));
	//declare publishers
	ros::Publisher fake_sensors_pub = nh.advertise<communication::fake_sensors_msg>("fake_sensors_topic", 1);
	
	//load parameters
	double rate_double;
	if (!nh.getParam("/node_activation_rates/fake_sensors", rate_double))
	{
		ROS_ERROR("Could not read obstacle detection's activation rate. Setting 10 Hz");
		rate_double = 10;
	}
	
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
	
	if (!nh.getParam("/drone_features/us_max", dist_us_max))
	{
		ROS_ERROR("Could not read us max range. Setting 6m");
		dist_irs_max = 6.00;
	}
	
	
	
	ros::Rate rate(rate_double);
	
	while (ros::ok())
	{
		ros::spinOnce();
		if (my_fake_sensors.hasChanged())
		{
			fake_sensors_pub.publish(my_fake_sensors.toPublish());
		}
		//obs_det_pub.publish(to_publish);
		//ROS_ERROR("spinning");
		rate.sleep();
	}
}


void us_callback(const sensor_msgs::LaserScanConstPtr& data, int sector)
{
	std::vector<float> distances = data->ranges;
	float value = distances[0];
	for (int i=1; i<61; i++)
	{
		if (distances[i]<value)
		{
			value = distances[i];
		}
	}
	(value<dist_us_max)||(value=dist_us_max);
	my_fake_sensors.setUS(value, sector);
	
}

void ir_callback(const sensor_msgs::LaserScanConstPtr& data, int sector)
{
	std::vector<float> distances = data->ranges;
	float value = distances[1];
	if (sector<0)
	{
		(value<dist_irs_max)||(value=dist_irs_max);
		my_fake_sensors.setIRS(value, -sector);
	}
	else
	{
		(value<dist_irl_max)||(value=dist_irl_max);
		my_fake_sensors.setIRL(value, sector);
	}
}
