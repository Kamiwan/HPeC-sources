/*************************************************************************************
 * File   : ipc_node_test.cpp, file to create easily ros nodes for HPeC
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
 *  2. on another terminal, run the command "rosrun ipc_node_test ipc_node_test_node"
 *  3. on another terminal, run the command 
 *     "rostopic pub -1 /ipc_node_test_mgt_topic std_msgs/Int32 "0" or "1" or "2""
 *************************************************************************************/
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <boost/thread.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

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

//Erwan Moréac, 05/03/18 : Setup #define
#define HIL				 //Code modifications for Hardware In the Loop

/************************************************************************
 * EM, Insert here:
 * 	- Defines for the app
 *  - Topic names to subscribe or publish
**********************************************************************/

#define SL_INPUT_TOPIC "/iris/camera2/image_raw" //example of Topic name def

/*******************************************
 * EM, Sample code for image topic
*******************************************/
/* color format image : pixmap */
typedef struct{
    int w;
    int h;
    unsigned char * img;
} PPM_IMG;

PPM_IMG img_ibuf_color;
/*******************************************
 * EM, Sample code for image topic
 * /!\	USE image.h 
 * to really use PPM_IMG struct!
*******************************************/


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

/************************************************************************
 * EM, Insert here global variables if needed
**********************************************************************/


void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam);
void imu_callback(const sensor_msgs::Imu::ConstPtr &imu_msg);


long elapse_time_u(struct timeval *end, struct timeval *start)
{
	long seconds = end->tv_sec - start->tv_sec;
	long micro_seconds = end->tv_usec - start->tv_usec;
	if (micro_seconds < 0)
	{
		seconds -= 1;
	}
	return (seconds * 1000000) + abs(micro_seconds);
}

long time_micros(struct timeval *end, struct timeval *start)
{
	if (end != NULL && start != NULL)
		return (((double)elapse_time_u(end, start)) / 1000);
	return -1;
}


/*************************************************************
 * appname_hwsw
 * Author : EM 
 * @param workerHandle_ptr allow the function to get ROS 
 * master events, to handle topics for example
 * 
 * Write in this function the Hardware version of your app
 * /!\ RENAME IT
**************************************************************/
void appname_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_INFO("[THREAD][RUNNING][HW]: ipc_node_test HARDWARE VERSION \r\n");

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//EM 22/02/18, Topic Subscribtion (Input) 
	//I let the gps and camera subscriptions and callbacks as an example of ROS topic subscription
	//Erase them if you do not need it.
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(SL_INPUT_TOPIC, 1, image_callback);
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);

	ros::Subscriber imu_sub = workerHandle_ptr->subscribe<sensor_msgs::Imu>("mavros/imu/data", 1000, imu_callback);

	/**********************************************************************
	* EM, Insert here Topic Publication and Subscription
	**********************************************************************/
	
	//EM, App parameters reading
	// /!\ Node activation rate is mandatory, add it in parameters folder
	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/ipc_node_test", rate_double))
	{
		ROS_INFO("Could not read ipc_node_test activation rate. Setting 1 Hz");
		rate_double = 1;
	}
	ros::Rate rate(rate_double);

	/**********************************************************************
	* EM, Insert here other parameters loading
	**********************************************************************/

	/* EM, create acquire function for Hardware version
	if (acquire() != 0) 
	{
		ROS_INFO("Could not INIT HARDWARE");
	}*/

	std_msgs::Float32 elapsed_time;
	while (workerHandle_ptr->ok()) //Main processing loop
	{
		//EM, This call checks if a somethubg from a topic has been received 
		//	  and run callback functions if yes.
		queue.callAvailable();

		//EM, Start app execution time
		start = clock();


		/**********************************************************************
		 * EM, Insert here some application functions
		**********************************************************************/
		std::cout << "HARD Hello World!" << std::endl;



		//EM, Compute execution time
		ends = clock();
		elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
		std::cout << "HARDWARE ipc_node_test Processing time : " << elapsed_time.data << std::endl;

		rate.sleep(); //EM, sleep time computed to respect the node_activation_rate of the app
		
	}	 // end while
	run = false;
	ROS_INFO("[THREAD][STOPPED]");
}


/*************************************************************
 * appname_sw
 * Author : EM 
 * @param workerHandle_ptr allow the function to get ROS 
 * master events, to handle topics for example
 * 
 * Write in this function the Software version of your app
 * /!\ RENAME IT
**************************************************************/
void appname_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_INFO("[THREAD][RUNNING][SW]: ipc_node_test SOFTWARE VERSION \r\n");

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//EM 22/02/18, Topic Subscribtion (Input) 
	//I let the gps and camera subscriptions and callbacks as an example of ROS topic subscription
	//Erase them if you do not need it.
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(SL_INPUT_TOPIC, 1, image_callback);
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);

	/**********************************************************************
	* EM, Insert here Topic Publication and Subscription
	**********************************************************************/
	
	//EM, App parameters reading
	//Node activation rate is mandatory
	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/ipc_node_test", rate_double))
	{
		ROS_INFO("Could not read ipc_node_test activation rate. Setting 1 Hz");
		rate_double = 1;
	}
	ros::Rate rate(rate_double);

	/**********************************************************************
	* EM, Insert here Other parameters loading
	**********************************************************************/

	std_msgs::Float32 elapsed_time;
	while (workerHandle_ptr->ok()) //Main processing loop
	{
		//EM, This call checks if a somethubg from a topic has been received 
		//	  and run callback functions if yes.
		queue.callAvailable();

		//EM, Start app execution time
		start = clock();

		/**********************************************************************
		 * EM, Insert here some application functions
		**********************************************************************/
		std::cout << "Hello World!" << std::endl;

		//Open already created shared memory object.
      	boost::interprocess::shared_memory_object shm (boost::interprocess::open_only
														,"shared_memory"
														, boost::interprocess::read_only);

      	//Map the whole shared memory in this process
     	boost::interprocess::mapped_region region(shm, boost::interprocess::read_only);
		//Get the address of the region
		std::cout << "Address of the region = " << region.get_address() << std::endl;
		//Get the size of the region
		std::cout << "Size of the region = " << region.get_size() << std::endl;	

     	//Check that memory was initialized to 1
     	char *mem = static_cast<char*>(region.get_address());
     	for(std::size_t i = 0; i < region.get_size(); ++i)
		{
			//std::cout << "Memory index " << i << " *mem++ == 1" << std::endl;	
     	   	if(*mem++ != 0)
			{
				std::cout << "ERROR " << i << " *mem++ != 1" << std::endl;	
     	    	exit(1);   //Error checking memory
			}
		}

		/*############# Vector in shared memory #############*/
		boost::interprocess::managed_shared_memory segment(boost::interprocess::open_only,"SharedMemVector");  //segment name
		typedef boost::interprocess::allocator<int, boost::interprocess::managed_shared_memory::segment_manager> ShmemAllocator;
		//Alias a vector that uses the previous STL-like allocator
      	typedef std::vector<int, ShmemAllocator> MyVector;
      	//Find the vector using the c-string name
      	MyVector *myvector = segment.find<MyVector>("MyVector").first;
    	int* ptr = myvector->data();

		//Insert data in the vector
    	for(int i = 0; i < 100; ++i){
    	   std::cout << "Value of shared memory = " << ptr[i] << std::endl;	
    	}
		


		/*############# Vector in shared memory #############*/






		//EM, Compute execution time
		ends = clock();

		elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
		std::cout << "SOFTWARE ipc_node_test Processing time : " << elapsed_time.data << std::endl;
		rate.sleep(); //EM, sleep time computed to respect the node_activation_rate of the app
		
	}	 // end while
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
		worker_thread = boost::make_shared<boost::thread>(&appname_sw, workerHandle_ptr);
		break;
		
	case 2: //starting the HW version
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 1;
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();
		worker_thread = boost::make_shared<boost::thread>(&appname_hwsw, workerHandle_ptr);
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
	ros::init(argc, argv, "ipc_node_test_node");
	ros::NodeHandle nh;
	
	gettimeofday(&beginning, NULL);
	ros::Subscriber mgt_topic;
	mgt_topic = nh.subscribe("ipc_node_test_mgt_topic", 1, managing_controller_request);

	search_land_pub = boost::make_shared<ros::Publisher>(
	nh.advertise<std_msgs::Float32>("/ipc_node_test_notification_topic", 1));
	gettimeofday(&current, NULL);

	dbprintf("wrapper_ver %.0f 0\n", ((double)time_micros(&current, &beginning)));

	ROS_INFO("[TASK WRAPPER][RUNNING]");
	ros::spin();
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
 * Author : Valentin, EM 
 * @param position, UAV GPS position from topic listened
 * 
 * Callback function to get GPS position
 * Here, only altitude is used but there are other data on position
 * (i) Sample code, you can erase if useless
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
 * (i) Sample code, you can erase if useless
 * (i) Use img_ibuf_color global variable then in the app function
 * /!\ In HIL soft, 
 * 	   FREE img_ibuf_color AT THE END OF EACH app function!
*******************************************************************/
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam)
{
	// Erwan Moréac 22/02/18, Image size update
	img_ibuf_color.h = image_cam->height;
	img_ibuf_color.w = image_cam->width;
	dbprintf("\n IMAGE WIDTH = %d HEIGHT = %d \n", img_ibuf_color.w, img_ibuf_color.h);

	// Erwan Moréac 22/02/18, use of cv_bridge to get the opencv::Mat of the picture
	try
	{
		cv_bridge::CvImagePtr image_DATA = cv_bridge::toCvCopy(image_cam, "bgr8");
		std_msgs::Float32 elapsed_time;
		
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
				img_ibuf_color.img = (unsigned char *)malloc(3*img_ibuf_color.w * img_ibuf_color.h * sizeof(unsigned char));
				
				//Use of uint32_t to copy data 4x faster instead of copying byte by byte
				// /!\ The image size in bytes MUST BE a multiple of 32
				uint32_t * ptrRes;
				uint32_t * ptrIn;
				ptrIn  = (uint32_t *)image_DATA->image.data;
				ptrRes = (uint32_t *)img_ibuf_color.img;
				int image_size=img_ibuf_color.w*img_ibuf_color.h;
				int int_img_size=(image_size*3)>>2; //int_img_size = image_size * 3 bytes (BGR) / 4 bytes (int)

				for(int i=0;i<int_img_size;i++)
					ptrRes[i] = ptrIn[i];
				imcpy_end = clock();

				elapsed_time.data = ((double)(imcpy_end - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
				std::cout << "SOFTWARE Image COPY time : " << elapsed_time.data << std::endl;

			#else //Zero-copy transfer
				imcpy_start = clock();
				img_ibuf_color.img = image_DATA->image.data;
				imcpy_end = clock();

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
