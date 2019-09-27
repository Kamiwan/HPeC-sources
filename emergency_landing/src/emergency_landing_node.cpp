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
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: January, 2018
 * 
 * Main file of emergency_landing node
 *************************************************************************************/
#include "emergency_landing_node.h"


int acquire()
{
	//init hardware
	// Open /dev/mem
	if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
	{
		ROS_ERROR("ERROR: could not open \"/dev/mem\"...\n");
		exit (1);
	}
	// Open /dev/cma_block
	if ((cma_dev_file = fopen("/dev/cma_block", "r")) == NULL)
	{
		ROS_ERROR("ERROR: could not open \"/dev/cma_block\"...\n");
		close(fd);
		exit (1);
	}

	// Determine the CMA block physical address
	fgets(cma_addr_string, sizeof(cma_addr_string), cma_dev_file);
	cma_base_addr = strtol(cma_addr_string, NULL, 16);
	dbprintf("Use CMA memory block at phys addr: 0x%x\n", cma_base_addr);
	fclose(cma_dev_file);

	// get virtual addr that maps to the sdram region (through HPS-FPGA non-lightweight bridge)
   	virtual_base_sdram = mmap( NULL, SDRAM_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, cma_base_addr /* FPGA_DDR_BASEADDR*/ );
   	if( virtual_base_sdram == MAP_FAILED ) {
       	ROS_ERROR( "ERROR: mmap() failed...\n" );
       	close( fd );
       	return(1);
    }

	mem_to_stream_dma_buffer = (volatile unsigned int *)(virtual_base_sdram);
	stream_to_mem_dma_buffer = (volatile unsigned char *)(virtual_base_sdram + 0x2000000);

	//get virtual addr that maps to the simple_dma_read component (through HPS-FPGA lightweight bridge)
	simple_dma_read_addr = (volatile unsigned int *)mmap( NULL, 4096, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, DMA_READ_CSR_BASEADDR );
    if( simple_dma_read_addr == MAP_FAILED ) {
       	ROS_ERROR( "ERROR: simple_dma_read mmap() failed...\n" );
       	close( fd );
       	return(1);
    }

	//get virtual addr that maps to the simple_dma_write component (through HPS-FPGA lightweight bridge)
	simple_dma_write_addr = (volatile unsigned int *)mmap( NULL, 4096, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, DMA_WRITE_CSR_BASEADDR );
    if( simple_dma_write_addr == MAP_FAILED ) {
       	ROS_ERROR( "ERROR: simple_dma_write mmap() failed...\n" );
       	close( fd );
       	return(1);
    }

	// ALL SUCCEDED 
	return 0;
}

void release()
{
	if( munmap(virtual_base_sdram, SDRAM_SPAN) != 0 )
	{
		ROS_ERROR("ERROR: munmap() failed...\n");
	}
    close( fd );
}


void search_landing_area_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_INFO("[THREAD][RUNNING][HW]: Vision Based Autonomous Landing HARDWARE VERSION \r\n");
	//ROS_INFO("Rgb2gray, median filter, canny edge detector, morphological closing in hw...\r\n");

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);
	img_acquired = false;

	//Erwan Moréac 22/02/18, Topic Subscribtion (Input) and Publication (Output)
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(SL_INPUT_TOPIC, 1, image_callback);
	image_transport::Publisher pub = it.advertise(SL_OUTPUT_IMAGE_TOPIC, 1);
	sensor_msgs::ImagePtr msg;
	cv::Mat imOutput;

	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/emergency_landing", rate_double))
	{
		ROS_INFO("Could not read obstacle detection's activation rate. Setting 1 Hz");
		rate_double = 1;
	}
	ros::Rate rate(rate_double);
	if (!workerHandle_ptr->getParam("/drone_features/diameter", diameter))
	{
		ROS_INFO("Could not read drone's diameter. Setting to 0.6 meters");
		diameter = 0.6;
	}
	if (!workerHandle_ptr->getParam("/camera_features/camera_angle", camera_angle))
	{
		ROS_INFO("Could not read camera's angle. Setting to 0.4 radian");
		camera_angle = 0.4;
	}
	if (!workerHandle_ptr->getParam("/camera_features/image_height", image_height))
	{
		ROS_INFO("Could not read image's height. Setting to 480");
		image_height = 480;
	}
	altitude = 60;
	if (acquire() != 0)
	{
		ROS_INFO("Could not INIT");
	}

	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);
	std_msgs::Float32 elapsed_time;
	while (workerHandle_ptr->ok())
	{
		queue.callAvailable();

		//EM, Add a test in the case the camera topic is not activated
		if (img_acquired)
		{
			// Start measuring time
			start = clock();
			img_ibuf_g.h 	= picture->rows;
			img_ibuf_g.w 	= picture->cols;
			img_ibuf_g.img 	= picture->data;
			int width  = img_ibuf_g.w;
    		int height = img_ibuf_g.h;

			//stop soft the DMAs if busy
    		stop_dma(simple_dma_read_addr);
    		//reset soft the DMAs if busy
    		reset_dma(simple_dma_read_addr);
    
			memcpy((unsigned char *)mem_to_stream_dma_buffer, img_ibuf_g.img , width*height);

    		// Configure DMAs to the correct addresses 
    		*(simple_dma_write_addr+DMAWRITE_CTRL_REG_OFFSET)=0x1; //reset soft 
    		*(simple_dma_write_addr+DMAWRITE_STATUS_REG_OFFSET)=0; //clear go  on write
    		*(simple_dma_write_addr+DMAWRITE_CTRL_REG_OFFSET)=0x0; //clear reset

    		//addresss in word (4 symbols for word) !!!
    		*(simple_dma_write_addr+DMAWRITE_ADDR_REG_OFFSET)=cma_base_addr+0x200000;
    		//length in bytes !!!
    		*(simple_dma_write_addr+DMAWRITE_LENGTH_REG_OFFSET)= width*height*sizeof(unsigned char);
    		t1=clock();
    		start_dma(simple_dma_read_addr,cma_base_addr,width*height*sizeof(unsigned char));	
    		ROS_INFO("length = %x\n", *(simple_dma_read_addr+SIMPLE_DMA_LENGTH_OFST));
    		ROS_INFO("base addr = %x\n", *(simple_dma_read_addr+SIMPLE_DMA_ADDRESS_OFST));
			
    		//START IP Matching
    		*(simple_dma_write_addr+DMAWRITE_CTRL_REG_OFFSET)=8;
    		while (transfer_done(simple_dma_read_addr)==0);
			time_t t2 = clock();
 			ROS_INFO("read memory transfer done... %.0f ms \r\n",((double) (t2 - t1)) * 1000 / CLOCKS_PER_SEC); 
			t1=clock();
			// wait until the dma write transfer complete
  			while (*(simple_dma_write_addr+DMAWRITE_STATUS_REG_OFFSET)!=1); 
			*(simple_dma_write_addr+DMAWRITE_CTRL_REG_OFFSET)=0x0; //clear reset    
			t2 = clock();
 			ROS_INFO("write memory transfer done... %.0f ms \r\n",((double) (t2 - t1)) * 1000 / CLOCKS_PER_SEC); 

			// Copy the edge detected image from the stream-to-mem buffer
			img_ibuf_ero.w = img_ibuf_g.w;
			img_ibuf_ero.h = img_ibuf_g.h;

			img_ibuf_ero.img = (unsigned char *)stream_to_mem_dma_buffer; //zero copy
			ends = clock();
			gettimeofday(&end, NULL);
			dbprintf("input padding time %.0f ms \r\n",((double) (t1 - start)) * 1000 / CLOCKS_PER_SEC);

	#ifdef WRITE_IMG
			write_pgm(img_ibuf_ero, "/home/ubuntu/search_land_are/erode.pgm");
	#endif

			dbprintf("Connected components labeling...\r\n");
			img_ibuf_ccl = connected_component_labeling(img_ibuf_ero, &nb_labels, &ccl, AREA_MIN, AREA_MAX);
			dbprintf("Nb connected components = %d \r\n", nb_labels);

	#ifdef WRITE_IMG
			write_pgm(img_ibuf_ccl, "/home/ubuntu/search_land_are/ccl.pgm");
	#endif

			//Erwan Moréac 16/04/18, Image result publishing in "search_output/image" topic
			imOutput = cv::Mat(img_ibuf_ccl.h, img_ibuf_ccl.w, CV_8U, img_ibuf_ccl.img);
			msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", imOutput).toImageMsg();
			cv::waitKey(30);
			pub.publish(msg);

			free_pgm(img_ibuf_ccl);

			elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "HARDWARE Image Processing time : " << elapsed_time.data << std::endl;
			elapsed_time.data = ((double)(ends - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "HARDWARE TOTAL time : " << elapsed_time.data << std::endl;

			dbprintf("wrapper_time %.0f %.0f\n", ((double)time_micros(&end, &beginning)), elapsed_time.data);
			search_land_pub->publish(elapsed_time);

			if(img_acquired)
			{
				delete picture; 	//Free memory of the allocated current picture
				img_acquired = false;
			}
			rate.sleep();
		} // end if test Image size
	}	 // end while

	// STOPPING THE HARDWARE VERSION
	release();

	run = false;
	ROS_INFO("[THREAD][STOPPED]");
}




void search_landing_area_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_INFO("[THREAD][RUNNING][SW]: Vision Based Autonomous Landing for ppm color images \r\n");
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//EM, initalize booleans for imu and image
	img_acquired = false;
	run = true;

	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);

	//Erwan Moréac 22/02/18, Topic Subscribtion (Input) and Publication (Output)
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(SL_INPUT_TOPIC, 1, image_callback);
	image_transport::Publisher pub = it.advertise(SL_OUTPUT_IMAGE_TOPIC, 1);
	sensor_msgs::ImagePtr msg;
	cv::Mat imOutput;
	//Labels area publication, this is a topic test, it will not be used in the final program
	ros::Publisher my_msg_pub = workerHandle_ptr->advertise<communication::area_location>("/search_landing/THE_TOPIC_SHOW", 1);
	communication::area_location area_msg;

	ROS_INFO("[ENTER IN WHILE WORK_HANDLE]");

	std_msgs::Float32 elapsed_time;

	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/emergency_landing", rate_double))
	{
		ROS_ERROR("Could not read obstacle detection's activation rate. Setting 1 Hz");
		rate_double = 1;
	}
	ros::Rate rate(rate_double);

	if (!workerHandle_ptr->getParam("/drone_features/diameter", diameter))
	{
		ROS_ERROR("Could not read drone's diameter. Setting to 0.6 meters");
		diameter = 0.6;
	}

	if (!workerHandle_ptr->getParam("/camera_features/camera_angle", camera_angle))
	{
		ROS_ERROR("Could not read camera's angle. Setting to 0.4 radian");
		camera_angle = 0.4;
	}

	if (!workerHandle_ptr->getParam("/camera_features/image_height", image_height))
	{
		ROS_ERROR("Could not read image's height. Setting to 480");
		image_height = 480;
	}

	altitude = 60;

	ROS_INFO("[BEGINNING IMAGE PROCESSING]");
	while (workerHandle_ptr->ok())
	{
		queue.callAvailable();

		//EM, Add a test in the case the camera topic is not activated
		if (img_acquired)
		{
			// Start measuring time
			start = clock();

			img_ibuf_g.h 	= picture->rows;
			img_ibuf_g.w 	= picture->cols;
			img_ibuf_g.img 	= picture->data;

		#ifdef WRITE_IMG
			write_pgm(img_ibuf_g, "/home/labsticc/Bureau/search_res/gray.pgm");
		#endif

		#ifdef MEDIAN_FILTER
			img_ibuf_me = medianfilter(img_ibuf_g);
			img_ibuf_gs = gaussianfilter(img_ibuf_me);
		#ifdef WRITE_IMG
					write_pgm(img_ibuf_me, "/home/labsticc/Bureau/search_res/median.pgm");
		#endif
		#else
			img_ibuf_gs = gaussianfilter(img_ibuf_g);
		#endif

		#ifdef WRITE_IMG
					write_pgm(img_ibuf_gs, "/home/labsticc/Bureau/search_res/gauss.pgm");
		#endif
			#ifdef DEBUG
				ROS_INFO("[AFTER GAUSSIAN FILTER]");
			#endif

			img_ibuf_sp.w = img_ibuf_gs.w;
			img_ibuf_sp.h = img_ibuf_gs.h;
			img_ibuf_sp.img = (unsigned char *)malloc(img_ibuf_sp.w * img_ibuf_sp.h * sizeof(unsigned char));
			img_ibuf_se.w = img_ibuf_gs.w;
			img_ibuf_se.h = img_ibuf_gs.h;
			img_ibuf_se.img = (unsigned char *)malloc(img_ibuf_se.w * img_ibuf_se.h * sizeof(unsigned char));

			sobel(img_ibuf_gs, img_ibuf_sp, img_ibuf_se);

			#ifdef DEBUG
				ROS_INFO("[AFTER SOBEL]");
			#endif

#ifdef WRITE_IMG
			write_pgm(img_ibuf_sp, "/home/labsticc/Bureau/search_res/sobel.pgm");
			write_pgm(img_ibuf_se, "/home/labsticc/Bureau/search_res/edge.pgm");
#endif

			img_ibuf_ms = canny(img_ibuf_sp, img_ibuf_se, 20, 80);
#ifdef WRITE_IMG
			write_pgm(img_ibuf_ms, "/home/labsticc/Bureau/search_res/canny.pgm");
#endif

			img_ibuf_mop = dilate(img_ibuf_ms);
#ifdef WRITE_IMG
			write_pgm(img_ibuf_mop, "/home/labsticc/Bureau/search_res/dilate.pgm");
#endif

			img_ibuf_mop2 = dilate(img_ibuf_mop);
#ifdef WRITE_IMG
			write_pgm(img_ibuf_mop2, "/home/labsticc/Bureau/search_res/dilate2.pgm");
#endif

			img_ibuf_mop3 = dilate(img_ibuf_mop2);
#ifdef WRITE_IMG
			write_pgm(img_ibuf_mop3, "/home/labsticc/Bureau/search_res/dilate3.pgm");
#endif
			//erode
			img_ibuf_ero = erode(img_ibuf_mop3);
			ends = clock();

#ifdef WRITE_IMG
			write_pgm(img_ibuf_ero, "/home/labsticc/Bureau/search_res/erode.pgm");
#endif

			#ifdef HIL
				if(img_acquired)
				{
					delete picture; 	//Free memory of the allocated current picture
					img_acquired = false;
				}
			#endif

			free_pgm(img_ibuf_gs);
			free_pgm(img_ibuf_mop);
			free_pgm(img_ibuf_mop2);
			free_pgm(img_ibuf_mop3);
			free_pgm(img_ibuf_sp);
			free_pgm(img_ibuf_se);
			free_pgm(img_ibuf_ms);
#ifdef MEDIAN_FILTER
			free_pgm(img_ibuf_me);
#endif
			dbprintf("Connected components labeling...\r\n");
			img_ibuf_ccl = connected_component_labeling(img_ibuf_ero, &nb_labels, &ccl, AREA_MIN, AREA_MAX);
			dbprintf("Nb connected components = %d \r\n", nb_labels);
#ifdef WRITE_IMG
			write_pgm(img_ibuf_ccl, "/home/labsticc/Bureau/search_res/ccl.pgm");
#endif
			int labels_count = nb_labels;
			/* info */
			#ifdef DEBUG
			dbprintf("ENTER IN THE LAST COMPONENT RECOVERY : \r\n");
			for (int i = 0; i < labels_count; i++)
			{
				if (ccl[i].skip == 0)
					dbprintf("component EMERGENCY_LANDING %d, area %d, rect[%d,%d,%d,%d]\r\n", i, ccl[i].area, ccl[i].xmin,
							 ccl[i].ymin, ccl[i].xmax, ccl[i].ymax);
			}
			#endif

			//Erwan Moréac 22/02/18, Image result publishing in "search_output/image" topic
			imOutput = cv::Mat(img_ibuf_ccl.h, img_ibuf_ccl.w, CV_8U, img_ibuf_ccl.img);
			msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", imOutput).toImageMsg();
			cv::waitKey(30);
			pub.publish(msg);

			if (workerHandle_ptr->ok())
			{
				if (sizeof ccl != 0)
				{
					for (int i = 0; i < labels_count; i++)
					{
						area_msg.xmin = ccl[i].xmin;
						area_msg.xmax = ccl[i].xmax;
						area_msg.ymin = ccl[i].ymin;
						area_msg.ymax = ccl[i].ymax;
						area_msg.area = ccl[i].area;
						area_msg.skip = ccl[i].skip;
					}

					#ifdef DEBUG
					std::cout << "LABELS OUTPUT : " << area_msg.xmin << " : " << area_msg.xmax
							  << " : " << area_msg.ymin << " : " << area_msg.ymax
							  << " : " << area_msg.area << " : " << area_msg.skip << std::endl;
					#endif
					my_msg_pub.publish(area_msg);
				}
			}

			free_pgm(img_ibuf_ccl);
			free_pgm(img_ibuf_ero);

			elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "SOFTWARE Image Processing time : " << elapsed_time.data << std::endl;
			elapsed_time.data = ((double)(ends - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "SOFTWARE TOTAL time : " << elapsed_time.data << std::endl;
			dbprintf("wrapper_time %.0f %.0f\n", ((double)time_micros(&end, &beginning)), elapsed_time.data);

			search_land_pub->publish(elapsed_time);
			rate.sleep();
		} // end if test Image size
	}	 // end while
	run = false;
	ROS_INFO("[THREAD][STOPPED]");
}

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
				release();
				hardware = 0;
				//EM, tells to the Adaptation Manager the Tile is released
				//TODO USE SHARED MEMORY
			}
		}
	}
}

void managing_controller_request(const std_msgs::Int32::ConstPtr &value)
{
	//current = clock();
	gettimeofday(&current, NULL);

	dbprintf("\n Hardware = %d\n",hardware);
	if(hardware==1)  //EM, Perform soft reset on the IP before to switch to another mode than HW
	{
		/* MAYBE NEED TO STOP DMA */

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
		worker_thread = boost::make_shared<boost::thread>(&search_landing_area_sw, workerHandle_ptr);
		break;
		
	case 2: //starting the HW version
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 1;
		//EM, tells to the Adaptation Manager search_landing is using a HW Tile
		//TODO USE SHARED MEMORY
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();
		worker_thread = boost::make_shared<boost::thread>(&search_landing_area_hwsw, workerHandle_ptr);
		break;

	default:
		break;
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "search_landing_node");
	ros::NodeHandle nh;
	
	gettimeofday(&beginning, NULL);

	ros::Subscriber mgt_topic;
	mgt_topic = nh.subscribe("/search_landing_area_mgt_topic", 1, managing_controller_request);
	search_land_pub = boost::make_shared<ros::Publisher>(
		nh.advertise<std_msgs::Float32>("/search_landing_notification_topic", 1));


	#ifdef HIL
	#else
		picture = new cv::Mat(); //EM, Must be done only once to allow zero-copy transfer
	#endif


	gettimeofday(&current, NULL);
	dbprintf("wrapper_ver %.0f 0\n", ((double)time_micros(&current, &beginning)));
	ROS_INFO("[TASK WRAPPER][RUNNING]");
	ros::spin();
}

void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position)
{
	altitude = position->altitude;
}

void image_callback(const sensor_msgs::Image::ConstPtr &image_cam)
{
	// Erwan Moréac 22/02/18, use of cv_bridge to get the opencv::Mat of the picture
	try
	{
		cv_bridge::CvImagePtr image_DATA = cv_bridge::toCvCopy(image_cam, "mono8");
		std_msgs::Float32 elapsed_time;
		img_acquired = true;
		std::cout << "COPY DONE " << std::endl;

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

		ROS_INFO("IMAGE RECOVERY SUCCEED");
	}
	catch (cv_bridge::Exception &e)
	{
		ROS_INFO("Could not convert from '%s' to 'bgr'.", image_cam->encoding.c_str());
	}
}





