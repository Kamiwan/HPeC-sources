#include "emergency_landing_node.h"

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

// source ppm color image 24bits RGB (packed)
// The video IP cores used for edge detection require the RGB 24 bits of each pixel to be
// word aligned (aka 1 byte of padding per pixel). | unused 8 bits | red 8 bits | green 8 bits | blue 8 bits |
void memcpy_consecutive_to_padded(unsigned char *from, volatile unsigned int *to, int pixels)
{
	int i;
	for (i = 0; i < pixels; i++)
	{
		*(to + i) = (((unsigned int)*((unsigned char *)(from + i * 3 + 2))) & 0xff) |
					((((unsigned int)*((unsigned char *)(from + i * 3 + 1))) << 8) & 0xff00) |
					((((unsigned int)*((unsigned char *)(from + i * 3 + 0))) << 16) & 0xff0000);
	}
}

int acquire()
{
	//init hardware
	// Open /dev/mem
	if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
	{
		dbprintf("ERROR: could not open \"/dev/mem\"...\n");
		exit (1);
	}
	// Open /dev/cma_block
	if ((cma_dev_file = fopen("/dev/cma_block", "r")) == NULL)
	{
		dbprintf("ERROR: could not open \"/dev/cma_block\"...\n");
		close(fd);
		exit (1);
	}

	// Determine the CMA block physical address
	fgets(cma_addr_string, sizeof(cma_addr_string), cma_dev_file);
	cma_base_addr = strtol(cma_addr_string, NULL, 16);

	dbprintf("Use CMA memory block at phys addr: 0x%x\n", cma_base_addr);

	fclose(cma_dev_file);

	// get virtual addr that maps to the sdram region (through HPS-FPGA non-lightweight bridge)
	if(USE_DDR_FPGA)
	{
		virtual_base_sdram = mmap(NULL, SDRAM_SPAN, (PROT_READ | PROT_WRITE), 
							MAP_SHARED, fd, DDR_FPGA_CMA_BASE_ADDR); //EM, replace by cma_base_addr
	}
	else
	{
		virtual_base_sdram = mmap(NULL, SDRAM_SPAN, (PROT_READ | PROT_WRITE), 
							MAP_SHARED, fd, cma_base_addr); 
	}

	if (virtual_base_sdram == MAP_FAILED)
	{
		printf("ERROR: mmap() failed...\n");
		close(fd);
		exit (1);
	}

	mem_to_stream_dma_buffer = (volatile unsigned int *)(virtual_base_sdram);
	stream_to_mem_dma_buffer = (volatile unsigned char *)(virtual_base_sdram + 0x2000000);

	//virtual_reconfig_ctrl = mmap(NULL, 4096, (PROT_READ | PROT_WRITE), 
	//						MAP_SHARED, fd, LW_HPS2FPGA_AXI_MASTER+0x00080000); //EM, replace by cma_base_addr

	// Create  Dispatcher
	dispatcher_read.init(DMA_READ_CSR_BASEADDR, DMA_READ_DESC_BASEADDR, 0);
	dispatcher_write.init(DMA_WRITE_CSR_BASEADDR, DMA_WRITE_DESC_BASEADDR, 0);

	//dispatcher_write.SetControlReg(0x0); //EM, Unset Reset and Stop Dispatcher on write DMA
	//dispatcher_read.SetControlReg(0x0); //EM, Unset Reset and Stop Dispatcher on read DMA
	// Create  descriptor, new instance of descriptor objects
	//descriptor_read = {0};
	//descriptor_write = {0};
	
	// EM, HW version update, Configure DMAs to the correct addresses
	// Set the HPS Memory start address
	if(USE_DDR_FPGA)
	{
		descriptor_read.read_addr 	= DDR_FPGA_CMA_BASE_ADDR;
		descriptor_read.write_addr 	= DDR_FPGA_CMA_BASE_ADDR;
		descriptor_write.read_addr 	= DDR_FPGA_CMA_BASE_ADDR+0x2000000;
		descriptor_write.write_addr	= DDR_FPGA_CMA_BASE_ADDR+0x2000000;
	}
	else
	{
		descriptor_read.read_addr	= cma_base_addr; 
		descriptor_read.write_addr 	= cma_base_addr;
		descriptor_write.read_addr 	= cma_base_addr+0x2000000;
		descriptor_write.write_addr = cma_base_addr+0x2000000;
	}

	// We are using packetized streams so set length to max
	descriptor_read.length = 640*480*sizeof(unsigned int);
	descriptor_write.length = 640*480*sizeof(unsigned char);
	// The dispatcher will use the End of Packet flag to end the transfer
	descriptor_read.control.msBits.end_on_eop = 1;
	descriptor_read.control.msBits.gen_sop = 1;
	descriptor_read.control.msBits.gen_eop = 1;
	descriptor_write.control.msBits.end_on_eop = 1;
	descriptor_write.control.msBits.gen_sop = 1;
	descriptor_write.control.msBits.gen_eop = 1;
	descriptor_read.control.msBits.go = 0;
	descriptor_write.control.msBits.go = 0;
	// configure the DMAs
	dispatcher_read.WriteDescriptor(descriptor_read);
	dispatcher_write.WriteDescriptor(descriptor_write);

	// ALL SUCCEDED ;
	return 0;
}

void release()
{
	if (munmap(virtual_base_sdram, SDRAM_SPAN) != 0)
	{
		ROS_ERROR("ERROR: munmap() failed...\n");
	}
	/*if (munmap(virtual_reconfig_ctrl, 4096) != 0)
	{
		ROS_ERROR("ERROR: munmap() failed...\n");
	}*/
    close( fd );
}


void search_landing_area_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_INFO("[THREAD][RUNNING][HW]: Vision Based Autonomous Landing HARDWARE VERSION \r\n");
	//ROS_INFO("Rgb2gray, median filter, canny edge detector, morphological closing in hw...\r\n");

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//Erwan Moréac 22/02/18, Topic Subscribtion (Input) and Publication (Output)
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(SL_INPUT_TOPIC, 1, image_callback);
	image_transport::Publisher pub = it.advertise(SL_OUTPUT_IMAGE_TOPIC, 1);
	sensor_msgs::ImagePtr msg;
	cv::Mat imOutput;


	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/emergency_landing", rate_double))
	{
		//ROS_INFO("Could not read obstacle detection's activation rate. Setting 1 Hz");
		rate_double = 1;
	}
	ros::Rate rate(rate_double);

	if (!workerHandle_ptr->getParam("/drone_features/diameter", diameter))
	{
		//ROS_INFO("Could not read drone's diameter. Setting to 0.6 meters");
		diameter = 0.6;
	}

	if (!workerHandle_ptr->getParam("/camera_features/camera_angle", camera_angle))
	{
		//ROS_INFO("Could not read camera's angle. Setting to 0.4 radian");
		camera_angle = 0.4;
	}

	if (!workerHandle_ptr->getParam("/camera_features/image_height", image_height))
	{
		//ROS_INFO("Could not read image's height. Setting to 480");
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
		if (img_ibuf_color.h != 0 && img_ibuf_color.w != 0)
		{
			//img_ibuf_color = read_ppm("/home/ubuntu/search_land_are/sim_cam/australia.ppm");
			// Write the image to the mem-to-stream buffer
			//memcpy_consecutive_to_padded(img_ibuf_color.img, mem_to_stream_dma_buffer, img_ibuf_color.h * img_ibuf_color.w);

			// Start measuring time
			start = clock();
			// gettimeofday (&start , NULL);

			t1=clock();
			
			#ifdef DEBUG
			std::cout << "Descriptor READ parameters: " << std::endl;
			dispatcher_read.DisplayDescriptor(descriptor_read);
			std::cout << "\nDescriptor WRITE parameters: " << std::endl;
			dispatcher_write.DisplayDescriptor(descriptor_write);
			#endif

			// Tell the dispatcher to start
			descriptor_read.control.msBits.go = 1;
			descriptor_write.control.msBits.go = 1;
			// Turn on the DMAs
			dispatcher_read.WriteDescriptor(descriptor_read);
			dispatcher_write.WriteDescriptor(descriptor_write);

	        while(dispatcher_read.GetStatusReg().msBits.busy)
			{
				dbprintf("wait read busy : %d\n",dispatcher_read.GetStatusReg().msBits.busy);
				dbprintf("desc_buf_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_empty);
				dbprintf("desc_buf_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_full);
				dbprintf("desc_res_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_empty);
				dbprintf("desc_res_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_full);
				dbprintf("stopped : %d\n",dispatcher_read.GetStatusReg().msBits.stopped);
				dbprintf("resetting  : %d\n",dispatcher_read.GetStatusReg().msBits.resetting);
				dbprintf("stop_on_err  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_err);
				dbprintf("stop_on_early  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_early);
				dbprintf("irq  : %d\n",dispatcher_read.GetStatusReg().msBits.irq); 
			}

			// wait until the dma transfer complete
			while(dispatcher_write.GetStatusReg().msBits.busy)
			{
				dbprintf("busy : %d\n",dispatcher_write.GetStatusReg().msBits.busy);
				dbprintf("desc_buf_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_empty);
				dbprintf("desc_buf_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_full);
				dbprintf("desc_res_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_empty);
				dbprintf("desc_res_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_full);
				dbprintf("stopped : %d\n",dispatcher_write.GetStatusReg().msBits.stopped);
				dbprintf("resetting  : %d\n",dispatcher_write.GetStatusReg().msBits.resetting);
				dbprintf("stop_on_err  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_err);
				dbprintf("stop_on_early  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_early);
				dbprintf("irq  : %d\n",dispatcher_write.GetStatusReg().msBits.irq); 
			};

			dbprintf("TRANSFERT OK ############################################## \n");

			//EM, HW synchro, Tell the dispatcher to STOP
			descriptor_read.control.msBits.go = 0;
			descriptor_write.control.msBits.go = 0;
			// Turn off the DMAs
			dispatcher_read.WriteDescriptor(descriptor_read);
			dispatcher_write.WriteDescriptor(descriptor_write);

			// Copy the edge detected image from the stream-to-mem buffer
			img_ibuf_ero.w = img_ibuf_color.w;
			img_ibuf_ero.h = img_ibuf_color.h;

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

	run = true;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

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
		if (img_ibuf_color.h != 0 && img_ibuf_color.w != 0 && img_ibuf_color.img != NULL)
		{
			//img_ibuf_color = read_ppm("/home/labsticc/Bureau/australia.ppm");

			// Start measuring time
			start = clock();
			//gettimeofday (&start , NULL);

			dbprintf("\nSECOND IMAGE WIDTH = %d HEIGHT = %d \n", img_ibuf_color.w, img_ibuf_color.h);
			uchar newval[3];
			int i,j;
			int image_size=img_ibuf_color.w*img_ibuf_color.h;
			for(i = 0,j=0; i < image_size; i ++,j+=3)
			{
				newval[0]=img_ibuf_color.img[j];
				newval[1]=img_ibuf_color.img[j+1];
				newval[2]=img_ibuf_color.img[j+2];
			}
			#ifdef DEBUG
			std::cout << "SECOND IMAGE RECOVERY B=" << (int)newval[0] << " G=" << (int)newval[1] << " R=" << (int)newval[2] << std::endl;
			#endif

			img_ibuf_g = rgb2gray(img_ibuf_color);
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
			//gettimeofday(&end, NULL);

#ifdef WRITE_IMG
			write_pgm(img_ibuf_ero, "/home/labsticc/Bureau/search_res/erode.pgm");
#endif

			#ifdef HIL //Erwan Moréac, Mandatory free since we have to use malloc
				if(img_ibuf_color.img != NULL)
				{
					free_ppm(img_ibuf_color); 
					img_ibuf_color.img = NULL;
				}
			#endif

			free_pgm(img_ibuf_gs);
			free_pgm(img_ibuf_mop);
			free_pgm(img_ibuf_mop2);
			free_pgm(img_ibuf_mop3);
			free_pgm(img_ibuf_sp);
			free_pgm(img_ibuf_se);
			free_pgm(img_ibuf_ms);
			free_pgm(img_ibuf_g);
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
			}
		}
	}
	// RESET
	//workerHandle_ptr = NULL;
	//worker_thread = NULL;
}

void managing_controller_request(const std_msgs::Int32::ConstPtr &value)
{
	//current = clock();
	gettimeofday(&current, NULL);

	dbprintf("\n Hardware = %d\n",hardware);
	if(hardware==1){ //EM, Perform soft reset on the IP before to switch to another mode than HW

		//EM, Stops the current task to switch mode
		descriptor_read.control.msBits.go = 0;
		descriptor_write.control.msBits.go = 0;
		dispatcher_read.WriteDescriptor(descriptor_read);
		dispatcher_write.WriteDescriptor(descriptor_write);

		dbprintf("busy : %d\n",dispatcher_read.GetStatusReg().msBits.busy);
		dispatcher_read.SetControlReg(0x1); //EM, Stop Dispatcher on read DMA
		while(dispatcher_read.GetStatusReg().msBits.stopped)
		{
			dbprintf("busy : %d\n",dispatcher_read.GetStatusReg().msBits.busy);
			dbprintf("desc_buf_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_empty);
			dbprintf("desc_buf_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_full);
			dbprintf("desc_res_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_empty);
			dbprintf("desc_res_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_full);
			dbprintf("stopped : %d\n",dispatcher_read.GetStatusReg().msBits.stopped);
			dbprintf("resetting  : %d\n",dispatcher_read.GetStatusReg().msBits.resetting);
			dbprintf("stop_on_err  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_err);
			dbprintf("stop_on_early  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_early);
			dbprintf("irq  : %d\n",dispatcher_read.GetStatusReg().msBits.irq); 
		}

		dispatcher_read.SetControlReg(0x2); //EM, Reset Dispatcher on read DMA
		while(dispatcher_read.GetStatusReg().msBits.resetting)
		{
			dbprintf("busy : %d\n",dispatcher_read.GetStatusReg().msBits.busy);
			dbprintf("desc_buf_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_empty);
			dbprintf("desc_buf_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_full);
			dbprintf("desc_res_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_empty);
			dbprintf("desc_res_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_full);
			dbprintf("stopped : %d\n",dispatcher_read.GetStatusReg().msBits.stopped);
			dbprintf("resetting  : %d\n",dispatcher_read.GetStatusReg().msBits.resetting);
			dbprintf("stop_on_err  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_err);
			dbprintf("stop_on_early  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_early);
			dbprintf("irq  : %d\n",dispatcher_read.GetStatusReg().msBits.irq); 
		}

		dbprintf("busy : %d\n",dispatcher_write.GetStatusReg().msBits.busy);
		dispatcher_write.SetControlReg(0x1); //EM, Stop Dispatcher on write DMA
		while(dispatcher_write.GetStatusReg().msBits.stopped)
		{
			dbprintf("busy : %d\n",dispatcher_write.GetStatusReg().msBits.busy);
			dbprintf("desc_buf_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_empty);
			dbprintf("desc_buf_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_full);
			dbprintf("desc_res_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_empty);
			dbprintf("desc_res_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_full);
			dbprintf("stopped : %d\n",dispatcher_write.GetStatusReg().msBits.stopped);
			dbprintf("resetting  : %d\n",dispatcher_write.GetStatusReg().msBits.resetting);
			dbprintf("stop_on_err  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_err);
			dbprintf("stop_on_early  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_early);
			dbprintf("irq  : %d\n",dispatcher_write.GetStatusReg().msBits.irq); 
		}

		dispatcher_write.SetControlReg(0x2); //EM, Reset Dispatcher on write DMA
		while(dispatcher_write.GetStatusReg().msBits.resetting)
		{
			dbprintf("busy : %d\n",dispatcher_write.GetStatusReg().msBits.busy);
			dbprintf("desc_buf_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_empty);
			dbprintf("desc_buf_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_full);
			dbprintf("desc_res_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_empty);
			dbprintf("desc_res_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_full);
			dbprintf("stopped : %d\n",dispatcher_write.GetStatusReg().msBits.stopped);
			dbprintf("resetting  : %d\n",dispatcher_write.GetStatusReg().msBits.resetting);
			dbprintf("stop_on_err  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_err);
			dbprintf("stop_on_early  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_early);
			dbprintf("irq  : %d\n",dispatcher_write.GetStatusReg().msBits.irq); 
		}

		dispatcher_write.SetControlReg(0x20); //EM, Reset Descriptor on write DMA
		while(dispatcher_write.GetStatusReg().msBits.stopped)
		{
			dbprintf("busy : %d\n",dispatcher_write.GetStatusReg().msBits.busy);
			dbprintf("desc_buf_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_empty);
			dbprintf("desc_buf_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_full);
			dbprintf("desc_res_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_empty);
			dbprintf("desc_res_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_full);
			dbprintf("stopped : %d\n",dispatcher_write.GetStatusReg().msBits.stopped);
			dbprintf("resetting  : %d\n",dispatcher_write.GetStatusReg().msBits.resetting);
			dbprintf("stop_on_err  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_err);
			dbprintf("stop_on_early  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_early);
			dbprintf("irq  : %d\n",dispatcher_write.GetStatusReg().msBits.irq); 
		}

		dispatcher_read.SetControlReg(0x20); //EM, Reset Descriptor on read DMA
		while(dispatcher_read.GetStatusReg().msBits.stopped)
		{
			dbprintf("busy : %d\n",dispatcher_read.GetStatusReg().msBits.busy);
			dbprintf("desc_buf_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_empty);
			dbprintf("desc_buf_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_full);
			dbprintf("desc_res_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_empty);
			dbprintf("desc_res_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_full);
			dbprintf("stopped : %d\n",dispatcher_read.GetStatusReg().msBits.stopped);
			dbprintf("resetting  : %d\n",dispatcher_read.GetStatusReg().msBits.resetting);
			dbprintf("stop_on_err  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_err);
			dbprintf("stop_on_early  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_early);
			dbprintf("irq  : %d\n",dispatcher_read.GetStatusReg().msBits.irq); 
		}

		dispatcher_write.SetControlReg(0x0); //EM, Unset Reset and Stop Dispatcher on write DMA
		dispatcher_read.SetControlReg(0x0); //EM, Unset Reset and Stop Dispatcher on read DMA


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
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();
		worker_thread = boost::make_shared<boost::thread>(&search_landing_area_hwsw, workerHandle_ptr);
		break;

	default:
		break;
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "emergency_landing_node");
	ros::NodeHandle nh;
	
	//beginning = clock();
	gettimeofday(&beginning, NULL);
	ros::Subscriber mgt_topic;
	mgt_topic = nh.subscribe("/search_landing_area_mgt_topic", 1, managing_controller_request);

	search_land_pub = boost::make_shared<ros::Publisher>(
		nh.advertise<std_msgs::Float32>("/search_landing_notification_topic", 1));
	//current = clock();
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
			// Image dimensions
			int width = img_ibuf_color.w;
			int height = img_ibuf_color.h;
			// We are using packetized streams so set length to max
			descriptor_read.length = width*height*sizeof(unsigned int);
			descriptor_write.length = width*height*sizeof(unsigned char);

			imcpy_start = clock();
			//EM, No changes is HIL or not
			memcpy_consecutive_to_padded(image_DATA->image.data, mem_to_stream_dma_buffer,  width * height);
			imcpy_end = clock();

			elapsed_time.data = ((double)(imcpy_end - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "HARDWARE Image COPY time : " << elapsed_time.data << std::endl;

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
		//std::cout << "ORIGINAL PICTURE : " << image_DATA->image << std::endl;
		//std::cout << "PICTURE PIXELS : " << img_ibuf_color.img << std::endl;
	}
	catch (cv_bridge::Exception &e)
	{
		ROS_INFO("Could not convert from '%s' to 'bgr'.", image_cam->encoding.c_str());
	}
}





