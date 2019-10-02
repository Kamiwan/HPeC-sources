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

#include "stabilisation_imu_node.hpp"


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
boost::shared_ptr<ros::NodeHandle> main_node_handle_ptr;
boost::shared_ptr<ros::Publisher> search_land_pub;

void 		 		*virtual_base_sdram;
volatile int 		*virtual_dmareg_addr;
volatile char 		*pio_reset_iomem;
volatile uint32_t 	*mc_addr;
int     fd;
FILE*   cma_dev_file;
unsigned int cma_base_addr=0;
volatile unsigned char * y_mem_to_mc_input_buffer;
volatile unsigned char * uv_mem_to_mc_input_buffer;
volatile unsigned char * output_stream_to_mem_dma_buffer;
mc422_device_t mc_device_ip;

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

	//EM, initalize booleans for imu and image
	first_time_imu = true;
	img_acquired = false;

	// TOPICS SUBSCRIPTION
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(STAB_IMU_INPUT_TOPIC, 1, image_callback);
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);
	ros::Subscriber imu_sub = workerHandle_ptr->subscribe<sensor_msgs::Imu>("mavros/imu/data", 1000, imu_callback);
	// TOPICS ADVERTISING
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

	//* EM, create acquire function for Hardware version
	if (acquire() != 0) 
	{
		ROS_ERROR("Could not INIT HARDWARE");
	}

	bool first_time_rtz = true;
	double theta = 0.0, x = 0.0, y = 0.0, last_theta = 0.0, last_x = 0.0, last_y = 0.0;
	double cumul_theta = 0.0, cumul_x = 0.0, cumul_y = 0.0;
	std_msgs::Float32 elapsed_time;
	int count = 0;

	while (workerHandle_ptr->ok()) //Main processing loop
	{
		//EM, This call checks if a something from a topic has been received 
		//	  and run callback functions if yes.
		queue.callAvailable();

		if(!first_time_imu && img_acquired)
		{
			// Start measuring time
        	start = clock();

			get_rtz_param_from_ins_values(*picture, yaw, pitch,
										roll, prev_yaw, prev_pitch,
										prev_roll, &theta, &x, &y);

			rotozoom_compute_params(*picture, first_time_rtz,
									theta, x, y,
									last_theta, last_x, last_y,
									cumul_theta, cumul_x, cumul_y);										

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

			mc422_set_params( &mc_device_ip, 1, cumul_theta, cumul_x, cumul_y, 1, 1, 
						(uint32_t) cma_base_addr,
						(uint32_t) (cma_base_addr + 0x3000000)
						);
			printf("MC422 configured\n");
			

			mc_input_grey_422_setup_no_split(*picture, 
											y_mem_to_mc_input_buffer,
                       						uv_mem_to_mc_input_buffer);

			launch_custom_dma();
			        
			printf("Start motion compensation \n");
        	mc422_start( &mc_device_ip );

			// wait until the dma transfer complete
        	while (*(virtual_dmareg_addr+MATCHING_STATUS_REG_OFFSET)!=1); 
        	printf("status maching %x \n",*(virtual_dmareg_addr+MATCHING_STATUS_REG_OFFSET));
        	*(virtual_dmareg_addr+MATCHING_CTRL_REG_OFFSET)=0;

			cv::Mat rtz_picture(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC1, (unsigned char *)output_stream_to_mem_dma_buffer ); //zero copy

			msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", rtz_picture).toImageMsg();
			cv::waitKey(1);
			img_stab_pub.publish(msg);

			delete picture; 	//Free memory of the allocated current picture
			img_acquired = false;

			//EM, Compute execution time
			ends = clock();
			elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "HARDWARE stabilisation_imu Processing time : " << elapsed_time.data << std::endl;
		}
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

	// TOPICS SUBSCRIPTION
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(STAB_IMU_INPUT_TOPIC, 1, image_callback);
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);
	ros::Subscriber imu_sub = workerHandle_ptr->subscribe<sensor_msgs::Imu>("mavros/imu/data", 1000, imu_callback);
	// TOPICS ADVERTISING
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
	double theta = 0.0, x = 0.0, y = 0.0, last_theta = 0.0, last_x = 0.0, last_y = 0.0;
	double cumul_theta = 0.0, cumul_x = 0.0, cumul_y = 0.0;
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
										last_theta, last_x, last_y,
										cumul_theta, cumul_x, cumul_y);

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

			msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", rtz_picture).toImageMsg();
			cv::waitKey(1);
			img_stab_pub.publish(msg);

			#ifdef HIL
					delete picture; 	//Free memory of the allocated current picture
			#endif
			img_acquired = false;

			//EM, Compute execution time
			ends = clock();
			elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "SOFTWARE stabilisation_imu Processing time : " << elapsed_time.data << std::endl;
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
				release(); // EM, function to release mem allocated for the HW
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

	// Video flow MUX Service setup
	// EM, this topic is only used when we turn on/off the image stabilisation
	ros::ServiceClient mux_select_client = main_node_handle_ptr->serviceClient<topic_tools::MuxSelect>("video_mux/select");
	topic_tools::MuxSelect mux_select_cmd;

	switch (value->data)
	{
	case 0: // stop the task
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);

		// EM, switch to camera topic
		mux_select_cmd.request.topic = CAMERA_MUX_TOPIC;
		if( mux_select_client.call(mux_select_cmd))
			ROS_INFO_STREAM("Switch video source to " << mux_select_cmd.request.topic << std::endl);

		stop();
		hardware = 0;
		break;

	case 1: //starting the Software version
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 0;
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();

		// EM, switch to stab_imu video topic
		mux_select_cmd.request.topic = STAB_IMU_OUTPUT_TOPIC;
		if( mux_select_client.call(mux_select_cmd))
			ROS_INFO_STREAM("Switch video source to " << mux_select_cmd.request.topic << std::endl);

		worker_thread = boost::make_shared<boost::thread>(&stabilisation_imu_sw, workerHandle_ptr);
		break;
		
	case 2: //starting the HW version
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 1;
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();

		// EM, switch to stab_imu video topic
		mux_select_cmd.request.topic = STAB_IMU_OUTPUT_TOPIC;
		if( mux_select_client.call(mux_select_cmd))
			ROS_INFO_STREAM("Switch video source to " << mux_select_cmd.request.topic << std::endl);

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
	main_node_handle_ptr = boost::make_shared<ros::NodeHandle>();
	
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
		cv_bridge::CvImagePtr image_DATA = cv_bridge::toCvCopy(image_cam, "mono8");
		std_msgs::Float32 elapsed_time;
		img_acquired = true;

		#ifdef HIL //mandatory malloc to save image from a remote computer
			imcpy_start = clock();
			picture = new cv::Mat(image_DATA->image);
			imcpy_end = clock();

			elapsed_time.data = ((double)(imcpy_end - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "HIL Image COPY time : " << elapsed_time.data << std::endl;
			dbprintf("\n IMAGE WIDTH = %d HEIGHT = %d \n", picture->cols, picture->rows);

		#else //Zero-copy transfer
			imcpy_start = clock();
			*picture = image_DATA->image;
			imcpy_end = clock();
			
			ROS_INFO("\n IMAGE WIDTH = %d HEIGHT = %d \n", picture->cols, picture->rows);
			elapsed_time.data = ((double)(imcpy_end - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "SOFTWARE Image COPY time : " << elapsed_time.data << std::endl;
		#endif

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
						const double last_theta, const double last_x, const double last_y,
						double & theta_cumul, double & x_cumul, double & y_cumul)
{
	if (FILTERING) 
	{
		double delta_x_limit = 180 * (pic.cols / HFOV);
		double delta_y_limit = 180 * (pic.rows / VFOV);
		if (first_time)
		{
			x_cumul		= x;
			y_cumul		= y;
			theta_cumul	= theta;	
		} else {
			double delta_theta = theta - last_theta;
			if (delta_theta > M_PI) 
			{
				delta_theta = delta_theta - 2*M_PI;
				std::cout << "delta_theta > M_PI =" << delta_theta << std::endl;
			}
			if (delta_theta < -M_PI)
			{
				delta_theta = delta_theta + 2*M_PI;
				std::cout << "delta_theta < -M_PI =" << delta_theta << std::endl;
			}
			theta_cumul = LEARN_RATE * ( theta_cumul + delta_theta);

			double delta_x = x - last_x;
			if (delta_x > delta_x_limit) 
			{
				delta_x = delta_x - 2*delta_x_limit;
				std::cout << "delta_x > delta_x_limit =" << delta_x << std::endl;
			}
			if (delta_x < -delta_x_limit)
			{
				delta_x = delta_x + 2*delta_x_limit;
				std::cout << "delta_x < -delta_x_limit =" << delta_x << std::endl;
			}
			x_cumul = LEARN_RATE * ( x_cumul + delta_x);

			double delta_y = y - last_y;
			if (delta_y > delta_y_limit) 
			{
				delta_y = delta_y - 2*delta_y_limit;
				std::cout << "delta_y > delta_y_limit =" << delta_y << std::endl;
			}
			if (delta_y < -delta_y_limit)
			{
				delta_y = delta_y + 2*delta_y_limit;
				std::cout << "delta_y < -delta_y_limit =" << delta_y << std::endl;
			}
			y_cumul = LEARN_RATE * (y_cumul + delta_y);
			
			std::cout << "delta theta=" << delta_theta << " x=" << delta_x << " y=" << delta_y << std::endl;
			std::cout << "IN theta_cumul=" << theta_cumul << " x_cumul=" << x_cumul << " y_cumul=" << y_cumul << std::endl;
		} // end first_time
	} // end filtering

	cv::Mat T(2, 3, CV_32FC1); //EM, use CV_32FC1 to get double data type

	T.at<float>(0,0) = (float)cos(theta_cumul);
    T.at<float>(0,1) = (float)-sin(theta_cumul);
    T.at<float>(1,0) = (float)sin(theta_cumul);
    T.at<float>(1,1) = (float)cos(theta_cumul);

    T.at<float>(0,2) = (float)x_cumul;
    T.at<float>(1,2) = (float)y_cumul;

	std::cout << "cumul theta=" << theta_cumul << " x_cumul=" << x_cumul << " y_cumul=" << y_cumul << std::endl;

	cv::Mat rtz_picture; 
	cv::warpAffine(pic, rtz_picture, T, pic.size());
	
	return rtz_picture;
}// end rotozoom_ins



void rotozoom_compute_params(const cv::Mat & pic, bool first_time,
                        const double theta, const double x, const double y,
						const double last_theta, const double last_x, const double last_y,
						double & theta_cumul, double &  x_cumul, double &  y_cumul)
{
	if (FILTERING) 
	{
		double delta_x_limit = 180 * (pic.cols / HFOV);
		double delta_y_limit = 180 * (pic.rows / VFOV);
		if (first_time)
		{
			x_cumul		= x;
			y_cumul		= y;
			theta_cumul	= theta;	
		} else {
			double delta_theta = theta - last_theta;
			if (delta_theta > M_PI) 
			{
				delta_theta = delta_theta - 2*M_PI;
				std::cout << "delta_theta > M_PI =" << delta_theta << std::endl;
			}
			if (delta_theta < -M_PI)
			{
				delta_theta = delta_theta + 2*M_PI;
				std::cout << "delta_theta < -M_PI =" << delta_theta << std::endl;
			}
			theta_cumul = LEARN_RATE * ( theta_cumul + delta_theta);

			double delta_x = x - last_x;
			if (delta_x > delta_x_limit) 
			{
				delta_x = delta_x - 2*delta_x_limit;
				std::cout << "delta_x > delta_x_limit =" << delta_x << std::endl;
			}
			if (delta_x < -delta_x_limit)
			{
				delta_x = delta_x + 2*delta_x_limit;
				std::cout << "delta_x < -delta_x_limit =" << delta_x << std::endl;
			}
			x_cumul = LEARN_RATE * ( x_cumul + delta_x);

			double delta_y = y - last_y;
			if (delta_y > delta_y_limit) 
			{
				delta_y = delta_y - 2*delta_y_limit;
				std::cout << "delta_y > delta_y_limit =" << delta_y << std::endl;
			}
			if (delta_y < -delta_y_limit)
			{
				delta_y = delta_y + 2*delta_y_limit;
				std::cout << "delta_y < -delta_y_limit =" << delta_y << std::endl;
			}
			y_cumul = LEARN_RATE * (y_cumul + delta_y);
			
			std::cout << "delta theta=" << delta_theta << " x=" << delta_x << " y=" << delta_y << std::endl;
			std::cout << "IN theta_cumul=" << theta_cumul << " x_cumul=" << x_cumul << " y_cumul=" << y_cumul << std::endl;
		} // end first_time
	} // end filtering
}// end rotozoom_compute_params



/*******************************************************************
 * mc_input_422_setup
 * Author : EM 
 * @param YUV_in, image received from a camera by a topic in YUV 
 * 
 * Transform an YUV picture in YUV:422 and write results 
 * on pointer location
*******************************************************************/
void mc_input_422_setup(const cv::Mat & YUV_in, 
                        volatile unsigned char * mem_to_mc_input_buffer,
                        volatile unsigned char * uv_mem_to_mc_input_buffer)
{
    int width  = YUV_in.cols;
    int height = YUV_in.rows;
    printf("width = %d, height = %d \n",width, height);

    // Y 422 part setup
    printf("Image spliting \n");
    cv::Mat splitted_yuv[3];
    cv::split(YUV_in,splitted_yuv);
    unsigned char * ptr_tmp = (unsigned char *)mem_to_mc_input_buffer;
    int cpt2=0;
    for(int row = 0; row < height; row++)
    {
        for(int col = 0; col < width; col++)
        {
            *ptr_tmp = splitted_yuv[0].at<uchar>(row,col);
            cpt2++;
            ptr_tmp++;
        }
    }
    printf ("\n nb points y %d \n", cpt2);

    // UV 422 part setup
    cv::Mat cb = splitted_yuv[1];
    cv::Mat cr = splitted_yuv[2];
    printf("CB cols = %d and CB rows = %d.\r\n",cb.cols, cb.rows);
    printf("CR cols = %d and CR rows = %d.\r\n",cr.cols, cr.rows);

    cv::Mat cb422(height, width/2, CV_8UC1);
    cv::Mat cr422(height, width/2, CV_8UC1);
    for(int row = 0; row < height; row++)
    {
        for(int col = 0; col < width/2; col++)
        {
          cb422.at<uchar>(row,col) = cb.at<uchar>(row,col*2);
          cr422.at<uchar>(row,col) = cr.at<uchar>(row,col*2);
        }
    }

    ptr_tmp = (unsigned char *)uv_mem_to_mc_input_buffer;
    for(int row = 0; row < height; row++)
    {
        for(int col = 0; col < width/2; col++)
        {
        *ptr_tmp = cb422.at<uchar>(row,col);
        ptr_tmp++;
        *ptr_tmp = cr422.at<uchar>(row,col);
        ptr_tmp++;
        }
    }
}



/*******************************************************************
 * acquire
 * Author : EM 
 * 
 * Memory allocation setup to use hardware accelerator
 * Return 0 if everything is ok, 1 if there is a problem
*******************************************************************/
int  acquire()
{
	//init hardware
   	// Open /dev/mem
   	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
       	printf( "ERROR: could not open \"/dev/mem\"...\n" );
       	return 1 ;
   	}

   	// Open /dev/cma_block
   	if( ( cma_dev_file = fopen("/dev/cma_block", "r") ) == NULL ) {
       	printf( "ERROR: could not open \"/dev/cma_block\"...\n" );
       	close( fd );
       	return 1 ;
   	}

    char cma_addr_string[10];

    // Determine the CMA block physical address
    fgets(cma_addr_string, sizeof(cma_addr_string), cma_dev_file);
    cma_base_addr = strtol(cma_addr_string, NULL, 16);
    printf("Use CMA memory block at phys addr: 0x%x\n",cma_base_addr);
    fclose(cma_dev_file);

    // get virtual addr that maps to the sdram region (through HPS-FPGA non-lightweight bridge)
    virtual_base_sdram = mmap( NULL, SDRAM_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, cma_base_addr /* FPGA_DDR_BASEADDR*/ );
        if( virtual_base_sdram == MAP_FAILED ) {
      	printf( "ERROR: mmap() failed...\n" );
       	close( fd );
       	return 1;
    }

	//get virtual addr that maps to the matching component (through HPS-FPGA lightweight bridge)
	virtual_dmareg_addr = (volatile int *)mmap( NULL, 4096, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, DMA_WRITE_CTRL_REG );
    if( virtual_dmareg_addr == MAP_FAILED ) 
    {
        printf( "ERROR: matching mmap() failed...\n" );
        close( fd );
    	return 1;
    }

	// EM, To try Dynamic reconfiguration
    //pio reset setup
	//pio_reset_iomem = (volatile char *)mmap( NULL, 4096, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, PIO_PR_RESET_BASEADDR);
    //if( pio_reset_iomem == MAP_FAILED ) {
    //	printf( "ERROR: mmap() failed...\n" );
    //	close( fd );
    //	return 1;
    //}

    // reset pr_region 
	//*pio_reset_iomem=0x0;
    //    printf( "RESET Region\n" );
    //    FPGA_reconfiguration(fd,"/home/ubuntu/reconfig_masks/mc_inpixal_mask.rbf");
    //*pio_reset_iomem=0x1;

    y_mem_to_mc_input_buffer        = (volatile unsigned char *)(virtual_base_sdram); // EM, MC Y input
    uv_mem_to_mc_input_buffer       = (volatile unsigned char *)(virtual_base_sdram + 0x3000000); // EM, MC UV input
    output_stream_to_mem_dma_buffer = (volatile unsigned char *)(virtual_base_sdram + 0x1000000); // EM, custom DMA output


    //############### InPixal IP INIT configuration ###############
	#ifdef PLATFORM_32_BITS // EM, only works on a 32-bit platform
		mc_addr = (volatile uint32_t *)mmap( 0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, MC_CTRL_BASEADDR);
		mc422_init( &mc_device_ip,      // device output structure
					(uint32_t)mc_addr,           // pointer value to mc422 register @s
					IMAGE_WIDTH, IMAGE_HEIGHT,      // input img size
					IMAGE_WIDTH, IMAGE_HEIGHT,      // output img size
					MC_OUTPUT_DEINTERLACING_DONTCARE,   // deinterlacing_mode
					MC_OUTPUT_PROGRESSIVE               // init parity
					);
		printf("MC422 INIT complete\n");

		// Wait until init ok
		while(mc422_ready_to_send(&mc_device_ip) == 0 );
		
		printf("MC422 INIT ready\n");
	#endif
    //############### END InPixal IP INIT configuration ############



	// All succeed
	return 0;
}


/*******************************************************************
 * release
 * Author : EM 
 * 
 * Realease allocated memory to use hardware accelerator
*******************************************************************/
void release()
{
	if( munmap( virtual_base_sdram, SDRAM_SPAN ) != 0 ) {
        	printf( "ERROR: virtual_base_sdram munmap() failed...\n" );
	}
	if( munmap( (void*)virtual_dmareg_addr, 4096 ) != 0 ) {
        	printf( "ERROR: virtual_dmareg_addr munmap() failed...\n" );
	}
	// EM, To try Dynamic reconfiguration
	//if( munmap( (void*)pio_reset_iomem, 4096 ) != 0 ) {
    //    	printf( "ERROR: pio_reset_iomem munmap() failed...\n" );
	//}
	#ifdef PLATFORM_32_BITS // EM, only works on a 32-bit platform
		if( munmap( (void*)mc_addr, 4096 ) != 0 ) {
				printf( "ERROR: mc_addr munmap() failed...\n" );
		}
	#endif
    close( fd );
}



/*******************************************************************
 * launch_custom_dma
 * Author : EM 
 * 
 * Configure and launch the DMA
*******************************************************************/
void launch_custom_dma()
{
		printf("Turn on output custom DMA\n");
        memset((void*)output_stream_to_mem_dma_buffer,0,sizeof(char)*IMAGE_WIDTH*IMAGE_HEIGHT);
        
        *(virtual_dmareg_addr+MATCHING_CTRL_REG_OFFSET)=0x1; //reset soft 
        *(virtual_dmareg_addr+MATCHING_STATUS_REG_OFFSET)=0; //clear go  on write
        *(virtual_dmareg_addr+MATCHING_CTRL_REG_OFFSET)=0x0; //clear reset

        //addresss in word (4 symbols for word) !!!
        *(virtual_dmareg_addr+MATCHING_WRITE_ADDR_REG_OFFSET)=cma_base_addr+0x1000000;
        //length in bytes !!!
        *(virtual_dmareg_addr+MATCHING_WRITE_LENGTH_REG_OFFSET)= sizeof(char)*IMAGE_WIDTH*IMAGE_HEIGHT;
        //START IP Matching
        *(virtual_dmareg_addr+MATCHING_CTRL_REG_OFFSET)=8;
}		



/*******************************************************************
 * mc_input_grey_422_setup_no_split
 * Author : EM 
 * 
 * Configure motion compensation input
*******************************************************************/
void mc_input_grey_422_setup_no_split(const cv::Mat & Y_in, 
                        volatile unsigned char * mem_to_mc_input_buffer,
                        volatile unsigned char * uv_mem_to_mc_input_buffer)
{
    int width  = Y_in.cols;
    int height = Y_in.rows;
    // Y 422 part setup
    std::memcpy((unsigned char *)mem_to_mc_input_buffer, Y_in.data , width*height);

    // UV 422 part setup
    memset((void*)uv_mem_to_mc_input_buffer,0x80,sizeof(char)*width*height);
}