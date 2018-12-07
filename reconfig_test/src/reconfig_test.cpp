/*************************************************************************************
 * File   : reconfig_test.cpp, file to create easily ros nodes for HPeC
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
 *  2. on another terminal, run the command "rosrun reconfig_test reconfig_test_node"
 *  3. on another terminal, run the command 
 *     "rostopic pub -1 /reconfig_test_mgt_topic std_msgs/Int32 "0" or "1" or "2""
 *************************************************************************************/
#include "reconfig_test.h"

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
	ROS_INFO("[THREAD][RUNNING][HW]: reconfig_test HARDWARE VERSION \r\n");

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//EM 22/02/18, Topic Subscribtion (Input) 
	//I let the gps and camera subscriptions and callbacks as an example of ROS topic subscription
	//Erase them if you do not need it.
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(NM_INPUT_TOPIC, 1, image_callback);
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);

	ros::Subscriber imu_sub = workerHandle_ptr->subscribe<sensor_msgs::Imu>("mavros/imu/data", 1000, imu_callback);

	/**********************************************************************
	* EM, Insert here Topic Publication and Subscription
	**********************************************************************/
	
	//EM, App parameters reading
	// /!\ Node activation rate is mandatory, add it in parameters folder
	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/reconfig_test", rate_double))
	{
		ROS_INFO("Could not read reconfig_test activation rate. Setting 1 Hz");
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
		std::cout << "HARDWARE reconfig_test Processing time : " << elapsed_time.data << std::endl;

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
	ROS_INFO("[THREAD][RUNNING][SW]: reconfig_test SOFTWARE VERSION \r\n");

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//EM 22/02/18, Topic Subscribtion (Input) 
	//I let the gps and camera subscriptions and callbacks as an example of ROS topic subscription
	//Erase them if you do not need it.
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(NM_INPUT_TOPIC, 1, image_callback);
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);

	/**********************************************************************
	* EM, Insert here Topic Publication and Subscription
	**********************************************************************/
	
	//EM, App parameters reading
	//Node activation rate is mandatory
	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/reconfig_test", rate_double))
	{
		ROS_INFO("Could not read reconfig_test activation rate. Setting 1 Hz");
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



		//EM, Compute execution time
		ends = clock();

		elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
		std::cout << "SOFTWARE reconfig_test Processing time : " << elapsed_time.data << std::endl;
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

	case 3: //starting FPGA RECONFIGURATION TEST
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 0;
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();
		worker_thread = boost::make_shared<boost::thread>(&test_reconfiguration, workerHandle_ptr);
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
	ros::init(argc, argv, "reconfig_test_node");
	ros::NodeHandle nh;
	
	gettimeofday(&beginning, NULL);
	ros::Subscriber mgt_topic;
	mgt_topic = nh.subscribe("reconfig_test_mgt_topic", 1, managing_controller_request);

	node_model_pub = boost::make_shared<ros::Publisher>(
	nh.advertise<std_msgs::Float32>("/reconfig_test_notification_topic", 1));
	gettimeofday(&current, NULL);

	dbprintf("wrapper_ver %.0f 0\n", ((double)time_micros(&current, &beginning)));


	init_FPGA_reconfiguration(); //EM, TEST for reconfiguration


	ROS_INFO("[TASK WRAPPER][RUNNING]");
	ros::spin();

	ROS_INFO("[TASK WRAPPER][RELEASE INCOMING!]");
	release(); //EM, called at the end to release memory reserved for FPGA DPR
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


/*int load_bitstream(std::string bitstream_path, int fd_mem,int mem_str_adrss,int offset_addr)
{
    FILE* file = fopen(bitstream_path.c_str(), "rb");
    if(file == NULL)
    {
        printf("ERROR: could not open %s file\n",bitstream_path.c_str());
		exit (1);
    }

    long fileSize = fsize(file);
	printf("\n FileSize=%ld",fileSize);

    void *bridge_map;
    bridge_map = mmap(NULL, DDR_PERSONNA_SPAN, PROT_WRITE, MAP_SHARED, fd_mem, mem_str_adrss+offset_addr);
    if (bridge_map == MAP_FAILED)
	{
		printf("ERROR: mmap() failed...\n");
		exit (1);
	}

    long buffer_size = fileSize>>2;
    char *buffer = (char *)malloc(fileSize + 1);
    size_t test = fread(buffer, 1,fileSize,file);
	printf("\n nb read=%ld\n",test);
	printf("\n Buffer_size=%ld\n",buffer_size);
	
    memcpy(bridge_map, buffer, fileSize);

	printf("\n FileSize=%x\n",fileSize);
	for(int i=0; i<5;i++)
		printf("%x ",buffer[i]);

	printf("\n");

	int *buff = (int *)buffer;
	for(int i=(buffer_size-1); i>(buffer_size-40);i--)
		printf("addr:%x value=%x \n",i,buff[i]);
	printf("\n");
    fclose(file);
    free(buffer);

	if (munmap(bridge_map, DDR_PERSONNA_SPAN) != 0)
	{
		ROS_ERROR("ERROR: munmap() failed...\n");
	}

	printf("\n BITSTREAM LOADING COMPLETE!");
    return 0;
}*/

/* start trigger module */
void start_reconfiguration(int bitstream_address, int region_id)
{
   printf("I received the reconf order\n");
   int param_address,i;
   int start_done_word[2] = {769 ,0};
   //start_done_word = start_done_word + start_selector*256;

   start_done_word[1] = start_done_word[0] + region_id*16;
   start_done_word[0] = bitstream_address;
   //int *pointer_param= &start_done_word;

   char * tmp = (char *)virtual_reconfig_ctrl;
   char * tmp2 = (char *)start_done_word;
   
   for (i=0;i<8;i++)
    *(tmp+i) = *(tmp2+i);
   
	//memcpy(virtual_reconfig_ctrl,start_done_word,8);
}

int reconfiguration_done()
{
   int *pointer_param= (int*)virtual_reconfig_ctrl;
   int value;
   
   if ((*pointer_param & 0x40000000) == 2)
         value=1;
   else
         value=0;

	printf("Pointer param = %x",*pointer_param);
   return value;
}


void release()
{
	if (munmap(virtual_reconfig_ctrl, 4096) != 0)
	{
		ROS_ERROR("ERROR: munmap() failed...\n");
	}
    close( fd );
}


int init_FPGA_reconfiguration()
{
	//init hardware
	// Open /dev/mem
	if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
	{
		dbprintf("ERROR: could not open \"/dev/mem\"...\n");
		exit (1);
	}

	virtual_reconfig_ctrl = mmap(NULL, 4096, (PROT_READ | PROT_WRITE), 
							MAP_SHARED, fd, LW_HPS2FPGA_AXI_MASTER+0x00080000); 

	/* write partial bitstream file in DDR memory */
    //int initconf_state=load_bitstream("/home/ubuntu/personna/personna_1.rbf", 
    //                        		fd, 
    //                        		PERSONNA_1,
    //                        		HPS2FPGA_AXI_MASTER+HARD_DDR3_CONTROLLER);
	return 0;
}

void test_reconfiguration(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//EM, C writing style because reconfiguration have been designed with C
	char * rbf_path_app1, * rbf_path_app2;
	char * bitstream_app1, * bitstream_app2;
	unsigned int *pr_base_iomem;
	long size_bts_app1, size_bts_app2;
	int fd_mem;
	
	boost::shared_ptr<ros::Publisher> search_land_pub = boost::make_shared<ros::Publisher>( 
        workerHandle_ptr->advertise<std_msgs::Int32>("/search_landing_area_mgt_topic", 1));
	boost::shared_ptr<ros::Publisher> harris_pub = boost::make_shared<ros::Publisher>( 
        workerHandle_ptr->advertise<std_msgs::Int32>("/harris_detector_mgt_topic", 1));
	std_msgs::Int32 msg;	


	ROS_INFO("[RECONFIG_TEST]: Start FPGA reconfiguration test!");

    /* map pr_controler to user address space  */
	pr_base_iomem = pr_controler_init(fd_mem);
	/* write partial bitstream file in HPS DDR memory */
    bitstream_app1 = load_bitstream(rbf_path_app1, &size_bts_app1);
	bitstream_app2 = load_bitstream(rbf_path_app2, &size_bts_app2);
	
	if (pr_controler_start(pr_base_iomem)==0)
	{
		pr_controler_write(pr_base_iomem,bitstream_app1,size_bts_app1);
		pr_controler_write_complete(pr_base_iomem);
	}
	ROS_INFO("[RECONFIG_TEST]: Reconfiguration succeed!");
	
	//Start app1
	msg.data = 2;
	search_land_pub->publish(msg);

	ROS_INFO("[RECONFIG_TEST]: Appli 1 launched for 30 sec!");
	//Sleep 30s
	std::this_thread::sleep_for(std::chrono::milliseconds(30000));

	//Stop app1
	msg.data = 0;
	search_land_pub->publish(msg);
	//Wait 3 sec end app1
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	ROS_INFO("[RECONFIG_TEST]: Start second FPGA reconfiguration test!");

	if (pr_controler_start(pr_base_iomem)==0)
	{
		pr_controler_write(pr_base_iomem,bitstream_app2,size_bts_app2);
		pr_controler_write_complete(pr_base_iomem);
	}
	ROS_INFO("[RECONFIG_TEST]: Reconfiguration succeed!");

	//Start app2
	msg.data = 2;
	harris_pub->publish(msg);
	
	ROS_INFO("[RECONFIG_TEST]: Appli 2 launched for 30 sec!");
	//Sleep 30s
	std::this_thread::sleep_for(std::chrono::milliseconds(30000));

	//Stop app2
	msg.data = 0;
	harris_pub->publish(msg);
	//Wait 3 sec end app2
	std::this_thread::sleep_for(std::chrono::milliseconds(30000));

	pr_controler_release(pr_base_iomem);

	ROS_INFO("RECONFIGURATION COMPLETE!");

	/* start the detector 
    start_reconfiguration(PERSONNA_1, 2);      
    // check the detection computation is done 
	//reconfiguration_done();
    while(reconfiguration_done()==0); */
}


