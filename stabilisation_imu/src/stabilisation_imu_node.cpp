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
 * File   : stabilisation_imu_node.cpp, 
 * Copyright (C) 2018 Lab-STICC Laboratory
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 *
 * Perform picture stabilisation using IUM data.
 * Can be executed in SW or HW
 * 
 * You can test this program:
 * 	1. run on a terminal "roscore" to get a ROS master
 *  2. on another terminal, run the command "rosrun stabilisation_imu stabilisation_imu_node"
 *  3. on another terminal, run the command 
 *     "rostopic pub -1 /stabilisation_imu_mgt_topic std_msgs/Int32 "0" or "1" or "2""
 *************************************************************************************/

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <boost/thread.hpp>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

extern "C" {
#include "debug.h"
}

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"

#include "sensor_msgs/Imu.h"
#include <tf/transform_datatypes.h>
#include "stabilisation_imu_node.h"


/***********************************************
 * EM, Global Variables  
************************************************/
cv::Mat * picture;
double roll, pitch, yaw;
double prev_roll, prev_pitch, prev_yaw;
bool first_time_imu;
bool img_acquired;


time_t start, ends;
time_t imcpy_start,imcpy_end;
struct timeval beginning, current, end;

bool run = false;
int current_ver = 0;
int hardware = 0;
double altitude;

boost::shared_ptr<boost::thread> worker_thread;
boost::shared_ptr<ros::NodeHandle> workerHandle_ptr;
boost::shared_ptr<ros::Publisher> search_land_pub;


/*************************************************************
 * stabilisation_imu_hwsw
 * Author : EM 
 * @param workerHandle_ptr allow the function to get ROS 
 * master events, to handle topics for example
 * 
**************************************************************/
void stabilisation_imu_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_INFO("[THREAD][RUNNING][HW]: stabilisation_imu HARDWARE VERSION \r\n");

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(STAB_IMU_INPUT_TOPIC, 1, image_callback);
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);
	ros::Subscriber imu_sub = workerHandle_ptr->subscribe<sensor_msgs::Imu>("mavros/imu/data", 1000, imu_callback);

	//EM, App parameters reading
	// /!\ Node activation rate is mandatory, add it in parameters folder
	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/stabilisation_imu", rate_double))
	{
		ROS_INFO("Could not read stabilisation_imu activation rate. Setting 10 Hz");
		rate_double = 10;
	}
	ros::Rate rate(rate_double);

	/* EM, create acquire function for Hardware version
	if (acquire() != 0) 
	{
		ROS_INFO("Could not INIT HARDWARE");
	}*/

	while (workerHandle_ptr->ok()) //Main processing loop
	{
		//EM, This call checks if a something from a topic has been received 
		//	  and run callback functions if yes.
		queue.callAvailable();

		ROS_INFO("HARDWARE STAB_IMU!");

		rate.sleep(); //EM, sleep time computed to respect the node_activation_rate of the app
		
	}	 // end while
	run = false;
	ROS_INFO("[THREAD][STOPPED]");
}


/*************************************************************
 * stabilisation_imu_sw
 * Author : EM 
 * @param workerHandle_ptr allow the function to get ROS 
 * master events, to handle topics for example
 * 
**************************************************************/
void stabilisation_imu_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_INFO("[THREAD][RUNNING][SW]: stabilisation_imu SOFTWARE VERSION \r\n");

	//EM, initalize booleans for imu and image
	first_time_imu = true;
	img_acquired = false;

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//TOPICS SUBSCRIPTION
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(STAB_IMU_INPUT_TOPIC, 1, image_callback);
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);
	ros::Subscriber imu_sub = workerHandle_ptr->subscribe<sensor_msgs::Imu>("mavros/imu/data", 1000, imu_callback);
	//TOPICS ADVERTISING
	image_transport::Publisher img_stab_pub = it.advertise(STAB_IMU_OUTPUT_TOPIC, 1);
	sensor_msgs::ImagePtr msg;

	//EM, App parameters reading
	// /!\ Node activation rate is mandatory, add it in parameters folder
	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/stabilisation_imu", rate_double))
	{
		ROS_INFO("Could not read stabilisation_imu activation rate. Setting 10 Hz");
		rate_double = 10;
	}
	ros::Rate rate(rate_double);

	bool first_time_rtz = true;
	double theta, x, y, last_theta, last_x, last_y;
	cv::Mat rtz_picture;
	int count = 0;

	std_msgs::Float32 elapsed_time;
	while (workerHandle_ptr->ok()) //Main processing loop
	{
		//EM, This call checks if a something from a topic has been received 
		//	  and run callback functions if yes.
		queue.callAvailable();

		if(!first_time_imu && img_acquired)
		{
			//EM, Start app execution time
			start = clock();

			ROS_INFO("\n IMAGE WIDTH = %d HEIGHT = %d \n", picture->cols, picture->rows);
			get_rtz_param_from_ins_values(*picture, yaw, pitch,
										roll, prev_yaw, prev_pitch,
										prev_roll, &theta, &x, &y);

			rtz_picture = rotozoom_ins(*picture, first_time_rtz,
										theta, x, y,
										last_theta, last_x, last_y);

			if (first_time_rtz)
				first_time_rtz = false;

			if(count>REFRESH_RTZ_PARAM)
			{
				last_theta = theta;
				last_x = x; 
				last_y = y;
				count = 0;
			} else {
				count++;
			}

			//EM, Compute execution time
			ends = clock();
			elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "SOFTWARE stabilisation_imu Processing time : " << elapsed_time.data << std::endl;

			msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", rtz_picture).toImageMsg();
			cv::waitKey(30);
			img_stab_pub.publish(msg);

			#ifdef HIL
				if(img_acquired)
				{
					delete picture; 	//Free memory of the allocated current picture
					img_acquired = false;
				}
			#endif
		}
		rate.sleep(); //EM, sleep time computed to respect the node_activation_rate of the app
	}
	run = false;
	ROS_INFO("[THREAD][STOPPED]");
}


/*************************************************************
 * stop
 * Author : Soguy, EM 
 * 
 * Stop the currently running version of the application
**************************************************************/
void stop()
{
	if (workerHandle_ptr == NULL && worker_thread == NULL)
		return;
	if (workerHandle_ptr != NULL)
		workerHandle_ptr->shutdown();
	if (worker_thread != NULL)
	{
		worker_thread->timed_join(boost::posix_time::seconds(3));
		if (run)
		{
			pthread_cancel(worker_thread->native_handle());

			if (hardware == 1)
			{
				//release(); EM, create a function to release mem allocated for the HW
				hardware = 0;
			}
		}
	}
	// RESET
	//workerHandle_ptr = NULL;
	//worker_thread = NULL;
}



/*******************************************************************
 * managing_controller_request
 * Author : Soguy, EM 
 * @param value, value of the message sent on the topic listened
 * 				0 stop the application
 * 				1 start the software version of the application
 * 				2 start the hardware version of the application
 * 
 * Callback function to receive orders from adaptation manager node
*******************************************************************/
void managing_controller_request(const std_msgs::Int32::ConstPtr &value)
{
	gettimeofday(&current, NULL);

	dbprintf("\n Hardware = %d\n",hardware);
	if(hardware==1){ 
		
		/**********************************************************************
		 * EM, Insert here soft reset on the IP before to switch to another 
		 * mode than HW (For Hardware version only)
		**********************************************************************/		
	}


	switch (value->data)
	{
	case 0: // stop the task
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 0;
		break;

	case 1: //starting the Software version
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 0;
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();
		worker_thread = boost::make_shared<boost::thread>(&stabilisation_imu_sw, workerHandle_ptr);
		break;
		
	case 2: //starting the HW version
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 1;
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();
		worker_thread = boost::make_shared<boost::thread>(&stabilisation_imu_hwsw, workerHandle_ptr);
		break;

	default:
		break;
	}
}


/**##################################################################
 * MAIN
 * Author : Valentin, Soguy, EM 
 * @param argc, number of argument of the execution
 * @param argv, execution arguments
 * 
 * Launch the ROS node and subscribe to topics in order to wait 
 * for orders.
##################################################################**/
int main(int argc, char **argv)
{
	ros::init(argc, argv, "stabilisation_imu_node");
	ros::NodeHandle nh;
	
	gettimeofday(&beginning, NULL);
	ros::Subscriber mgt_topic;
	mgt_topic = nh.subscribe("stabilisation_imu_mgt_topic", 1, managing_controller_request);

	search_land_pub = boost::make_shared<ros::Publisher>(
	nh.advertise<std_msgs::Float32>("/stabilisation_imu_notification_topic", 1));
	gettimeofday(&current, NULL);

	dbprintf("wrapper_ver %.0f 0\n", ((double)time_micros(&current, &beginning)));

	#ifdef HIL
	#else
		picture = new cv::Mat(); //EM, Must be done only once to allow zero-copy transfer
	#endif

	ROS_INFO("[TASK WRAPPER][RUNNING]");
	ros::spin();
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
	
	dbprintf("\nSeq: [%d]", imu_msg->header.seq);
    dbprintf("\nRoll: [%f],Pitch: [%f],Yaw: [%f]",roll,pitch,yaw);
}

/*******************************************************************
 * gps_callback
 * Author : Valentin, EM 
 * @param position, UAV GPS position from topic listened
 * 
 * Callback function to get GPS position
 * Here, only altitude is used but there are other data on position
*******************************************************************/
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position)
{
	altitude = position->altitude;
}

/*******************************************************************
 * image_callback
 * Author : EM 
 * @param image_cam, image received from a camera by a topic
 * 
 * Callback function to get camera raw image
*******************************************************************/
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam)
{
	// Erwan Moréac 22/02/18, use of cv_bridge to get the opencv::Mat of the picture
	try
	{
		cv_bridge::CvImagePtr image_DATA = cv_bridge::toCvCopy(image_cam, "bgr8");
		std_msgs::Float32 elapsed_time;
		img_acquired = true;

		if(hardware==1) //Hardware version
		{
			/**********************************************************************
		 	* EM, Insert here hardware version of image copying 
			*     It highly depends of the HW configuration
			*	  a memcpy is mandatory.
			**********************************************************************/	

		} else { //Software version
			#ifdef HIL //mandatory malloc to save image from a remote computer
				imcpy_start = clock();
				picture = new cv::Mat(image_DATA->image);
				
				imcpy_end = clock();

				elapsed_time.data = ((double)(imcpy_end - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
				std::cout << "SOFTWARE HIL Image COPY time : " << elapsed_time.data << std::endl;
				dbprintf("\n IMAGE WIDTH = %d HEIGHT = %d \n", picture->cols, picture->rows);

			#else //Zero-copy transfer
				imcpy_start = clock();
				*picture = image_DATA->image;
				imcpy_end = clock();

				ROS_INFO("\n IMAGE WIDTH = %d HEIGHT = %d \n", picture->cols, picture->rows);
				elapsed_time.data = ((double)(imcpy_end - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
				std::cout << "SOFTWARE Image COPY time : " << elapsed_time.data << std::endl;
			#endif
		}

		ROS_INFO("IMAGE RECOVERY SUCCEED");
	}
	catch (cv_bridge::Exception &e)
	{
		ROS_INFO("Could not convert from '%s' to 'bgr'.", image_cam->encoding.c_str());
	}
}


void get_rtz_param_from_ins_values(const cv::Mat&  pic, double yawData, double pitchData,
                                    double rollData, double initialYaw, double initialPitch,
                                    double initialRoll, double * theta, double * x, double * y)
{
	// Model Parameters
	const double Y_OFFSET     = 0;
	const double THETA_OFFSET = 0;
	const double X_OFFSET     = 0;

	const double  THETA_COEF  = 1;
	const double  X_COEF      = 1;
	const double  Y_COEF      = 1;
	
	*x 		= (yawData - initialYaw) * (pic.cols / HFOV);
	*y		= (pitchData - initialPitch) * (pic.rows / VFOV) + Y_OFFSET;
  	*theta	= THETA_COEF * (rollData - initialRoll) - THETA_OFFSET;
	*theta	= -(*theta);
}


cv::Mat rotozoom_ins(const cv::Mat & pic, bool first_time,
                        const double theta, const double x, const double y,
						const double last_theta, const double last_x, const double last_y)
{
	double x_cumul, y_cumul, theta_cumul;
	if (FILTERING) 
	{
		double delta_x_limit = 180 * (pic.cols / HFOV);
		if (first_time)
		{
			x_cumul		= x;
			y_cumul		= y;
			theta_cumul	= theta;	
		} else {
			double delta_theta = theta - last_theta;
			if (delta_theta > M_PI) 
				delta_theta = delta_theta - 2*M_PI;
			if (delta_theta < -M_PI) 
				delta_theta = delta_theta + 2*M_PI;
			theta_cumul = LEARN_RATE * ( theta_cumul + delta_theta);

			double delta_x = x - last_x;
			if (delta_x > delta_x_limit) 
				delta_x = delta_x - 2*delta_x_limit;
			if (delta_x < -delta_x_limit)
				delta_x = delta_x + 2*delta_x_limit;
			x_cumul = LEARN_RATE * ( x_cumul + delta_x);

			double delta_y = y - last_y;
			y_cumul = LEARN_RATE * (y_cumul + delta_y);
			
			std::cout << "delta theta=" << delta_theta << " x=" << delta_x << " y=" << delta_y << std::endl;
		} // end first_time
	} // end filtering

	cv::Mat T(2, 3, CV_64FC1); //EM, use CV_64FC1 to get double data type

	T.at<double>(0,0) = cos(theta_cumul);
    T.at<double>(0,1) = -sin(theta_cumul);
    T.at<double>(1,0) = sin(theta_cumul);
    T.at<double>(1,1) = cos(theta_cumul);

    T.at<double>(0,2) = x_cumul;
    T.at<double>(1,2) = y_cumul;

	cv::Mat rtz_picture; 
	cv::warpAffine(pic, rtz_picture, T, pic.size());
	
	return rtz_picture;
}// end rotozoom_ins


