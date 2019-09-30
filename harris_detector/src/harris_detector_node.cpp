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
 * Main file of harris_detector node
 *************************************************************************************/
#include "harris_detector_node.hpp"

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
	stream_to_mem_dma_buffer = (volatile unsigned char *)(virtual_base_sdram + 0x200000);

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



void harris_detector_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_ERROR("[THREAD][RUNNING][HW]: Vision Based Autonomous Landing HARDWARE VERSION \r\n");
	//ROS_ERROR("Rgb2gray, median filter, canny edge detector, morphological closing in hw...\r\n");

    PGM_IMG img_ibuf_ha;
    PGM_IMG img_ibuf_corr;
    keyPoint *refKeyPoints,*keyPoints;
    descriptor *refdescriptor,*descriptor;
    Point *point1,*point2;
    int N_SAMPLES;
    int refKpCount=0;
    int refDescriptorCount=0;
    int kpCount=0;
    int descriptorCount=0;
    float theta;
    int tx,ty;
    int hardware = 0;
    int fd;
    FILE* cma_dev_file;
    char cma_addr_string[10];


	run = true;
	img_acquired = false;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//Erwan Moréac 22/02/18, Topic Subscribtion (Input) and Publication (Output)
	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(HARRIS_INPUT_TOPIC, 1, image_callback);
	image_transport::Publisher pub = it.advertise(HARRIS_OUTPUT_IMAGE_TOPIC, 1);
	sensor_msgs::ImagePtr msg;
	cv::Mat imOutput;


	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/harris_detector", rate_double))
	{
		//ROS_ERROR("Could not read obstacle detection's activation rate. Setting 1 Hz");
		rate_double = 1;
	}
	ros::Rate rate(rate_double);

	if (!workerHandle_ptr->getParam("/drone_features/diameter", diameter))
	{
		//ROS_ERROR("Could not read drone's diameter. Setting to 0.6 meters");
		diameter = 0.6;
	}

	if (!workerHandle_ptr->getParam("/camera_features/camera_angle", camera_angle))
	{
		//ROS_ERROR("Could not read camera's angle. Setting to 0.4 radian");
		camera_angle = 0.4;
	}

	if (!workerHandle_ptr->getParam("/camera_features/image_height", image_height))
	{
		//ROS_ERROR("Could not read image's height. Setting to 480");
		image_height = 480;
	}
	altitude = 60;

	if (acquire() != 0)
	{
		ROS_ERROR("Could not INIT");
	}

	std_msgs::Float32 elapsed_time;
	while (workerHandle_ptr->ok())
	{
		queue.callAvailable();

		//EM, Add a test in the case the camera topic is not activated
		if (img_acquired)
		{
			// Start measuring time
			start = clock();

			img_ibuf_ha.h 	= picture->rows;
			img_ibuf_ha.w 	= picture->cols;
			img_ibuf_ha.img = picture->data;


			t1=clock();
			
			refKeyPoints=(keyPoint *)malloc(MAX_KEYPOINT* sizeof(keyPoint));
			
			//memset(virtual_base_bram,0,MAX_KEYPOINT*8);
			img_ibuf_ha=greyharriscornerhw(img_ibuf_ha,32,refKeyPoints,&refKpCount,0);
				#ifdef WRITE_IMG
				write_pgm(img_ibuf_ha, "harrisref.pgm");
				#endif
			//no free : no allocation for img_ibuf_ha.img : zerocopy

			printf("Reference image keypoints count %d\n",refKpCount);
			//debug 
			//save_keypoints( "keypoints.ref", refKeyPoints,refKpCount);

			ends = clock();

			elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "HARDWARE Image Processing time : " << elapsed_time.data << std::endl;
			elapsed_time.data = ((double)(ends - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "HARDWARE TOTAL time : " << elapsed_time.data << std::endl;

			//Erwan Moréac 16/04/18, Image result publishing in "search_output/image" topic
			imOutput = cv::Mat(img_ibuf_ha.h, img_ibuf_ha.w, CV_8U, img_ibuf_ha.img);
			msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", imOutput).toImageMsg();
			cv::waitKey(10);
			pub.publish(msg);

			delete picture; 	//Free memory of the allocated current picture
			img_acquired = false;

			free(refKeyPoints);
			dbprintf("wrapper_time %.0f %.0f\n", ((double)time_micros(&end, &beginning)), elapsed_time.data);
			search_land_pub->publish(elapsed_time);
			rate.sleep();
		} // end if test Image size
	}	 // end while

	// STOPPING THE HARDWARE VERSION
	release();

	run = false;
	ROS_ERROR("[THREAD][STOPPED]");
}




void harris_detector_sw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_ERROR("[THREAD][RUNNING][SW]: Vision Based Autonomous Landing for ppm color images \r\n");

    PGM_IMG img_ibuf_ha;
    PGM_IMG img_ibuf_corr;
    keyPoint *refKeyPoints,*keyPoints;
    descriptor *refdescriptor,*descriptor;
    Point *point1,*point2;
    int N_SAMPLES;
    int refKpCount=0;
    int refDescriptorCount=0;
    int kpCount=0;
    int descriptorCount=0;
    float theta;
    int tx,ty;
    int fd;
    FILE* cma_dev_file;
    char cma_addr_string[10];

	run = true;
	img_acquired = false;
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);

	//Erwan Moréac 22/02/18, Topic Subscribtion (Input) and Publication (Output)
	image_transport::ImageTransport it(*workerHandle_ptr);
	image_transport::Subscriber cam_sub = it.subscribe(HARRIS_INPUT_TOPIC, 1, image_callback);
	image_transport::Publisher pub = it.advertise(HARRIS_OUTPUT_IMAGE_TOPIC, 1);
	sensor_msgs::ImagePtr msg;
	cv::Mat imOutput;


	ROS_ERROR("[ENTER IN WHILE WORK_HANDLE]");

	std_msgs::Float32 elapsed_time;

	double rate_double;
	if (!workerHandle_ptr->getParam("/node_activation_rates/harris_detector", rate_double))
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

	ROS_ERROR("[BEGINNING IMAGE PROCESSING]");
	while (workerHandle_ptr->ok())
	{
		queue.callAvailable();

		//EM, Add a test in the case the camera topic is not activated
		if (img_acquired)
		{
 			refKeyPoints=(keyPoint *)malloc(MAX_KEYPOINT* sizeof(keyPoint));
    		printf("Reference image Harris Corner...\r\n");

			// Start measuring time
			start = clock();

			img_ibuf_ha.h 	= picture->rows;
			img_ibuf_ha.w 	= picture->cols;
			img_ibuf_ha.img = picture->data;

			#ifdef WRITE_IMG
				write_pgm(img_ibuf_ha, "grayref.pgm");
			#endif
    		img_ibuf_ha = harriscorner(img_ibuf_ha,32,refKeyPoints,&refKpCount,0);
			#ifdef WRITE_IMG
				write_pgm(img_ibuf_ha, "harrisref.pgm");
			#endif

			ends = clock();

			//Erwan Moréac 16/04/18, Image result publishing in "search_output/image" topic
			imOutput = cv::Mat(img_ibuf_ha.h, img_ibuf_ha.w, CV_8U, img_ibuf_ha.img);
			msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", imOutput).toImageMsg();
			cv::waitKey(10);
			pub.publish(msg);

			elapsed_time.data = ((double)(ends - start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "SOFTWARE Image Processing time : " << elapsed_time.data << std::endl;
			elapsed_time.data = ((double)(ends - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "SOFTWARE TOTAL time : " << elapsed_time.data << std::endl;
			dbprintf("wrapper_time %.0f %.0f\n", ((double)time_micros(&end, &beginning)), elapsed_time.data);

   			free_pgm(img_ibuf_ha);
			free(refKeyPoints);

			#ifdef HIL
				if(img_acquired)
				{
					delete picture; 	//Free memory of the allocated current picture
					img_acquired = false;
				}
			#endif	

			search_land_pub->publish(elapsed_time);
			rate.sleep();
		} // end if test Image size
	}	 // end while
	run = false;
	ROS_ERROR("[THREAD][STOPPED]");
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
	if(hardware==1){ 
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
		worker_thread = boost::make_shared<boost::thread>(&harris_detector_sw, workerHandle_ptr);
		break;
		
	case 2: //starting the HW version
		dbprintf("wrapper_ver %.0f %d\n", ((double)time_micros(&current, &beginning)), value->data);
		stop();
		hardware = 1;
		workerHandle_ptr = boost::make_shared<ros::NodeHandle>();
		worker_thread = boost::make_shared<boost::thread>(&harris_detector_hwsw, workerHandle_ptr);
		break;

	default:
		break;
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "harris_detector_node");
	ros::NodeHandle nh;
	gettimeofday(&beginning, NULL);

	ros::Subscriber mgt_topic;
	mgt_topic = nh.subscribe("/harris_detector_mgt_topic", 1, managing_controller_request);
	search_land_pub = boost::make_shared<ros::Publisher>(
		nh.advertise<std_msgs::Float32>("/harris_detector_notification_topic", 1));

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

		#ifdef HIL // mandatory new to save image from a remote computer
			imcpy_start = clock();
			picture = new cv::Mat(image_DATA->image);
			imcpy_end = clock();

			elapsed_time.data = ((double)(imcpy_end - imcpy_start)) * 1000 / CLOCKS_PER_SEC;
			std::cout << "SOFTWARE HIL Image COPY time : " << elapsed_time.data << std::endl;
			dbprintf("\n IMAGE WIDTH = %d HEIGHT = %d \n", picture->cols, picture->rows);

		#else // Zero-copy transfer
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

static void save_descriptor( const  char *filename, descriptor* descriptors, size_t desc_count )
{
    unsigned int i;
    FILE* fp = fopen( filename, "wb+");
    if (fp) {
        fwrite(&desc_count, sizeof(int), 1, fp);
        for (i = 0; i < desc_count; ++i) {
            fwrite(descriptors[i].data, sizeof(*descriptors[i].data), FREAK_NB_PAIRS / WORD_SIZE, fp);
            fwrite(&descriptors[i].x, sizeof(descriptors[i].x), 1, fp);
            fwrite(&descriptors[i].y, sizeof(descriptors[i].y), 1, fp);
        }
        fclose(fp);
    }
    else fprintf(stderr, "Can't create descriptor file\n");
}


PGM_IMG greyharriscornerhw(PGM_IMG img_ibuf_g,int threshold,keyPoint *keyPoints,int *kpcount,int sort)
{
    PGM_IMG result;
    time_t start, t1,t2;
    // Image dimensions
    int i, j,incr;
    result.w 	= img_ibuf_g.w;
    result.h 	= img_ibuf_g.h;
    int width  	= img_ibuf_g.w;
    int height  = img_ibuf_g.h;

    start  =clock();

	//stop soft the DMAs if busy
    stop_dma(simple_dma_read_addr);
    //reset soft the DMAs if busy
    reset_dma(simple_dma_read_addr);
    	
	std::memcpy((unsigned char *)mem_to_stream_dma_buffer, img_ibuf_g.img , width*height);

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

	// wait until the dma write transfer complete
  	while (*(simple_dma_write_addr+DMAWRITE_STATUS_REG_OFFSET)!=1); 
	*(simple_dma_write_addr+DMAWRITE_CTRL_REG_OFFSET)=0x0; //clear reset    

	t2 = clock();
 	ROS_INFO("write memory transfer done... %.0f ms \r\n",((double) (t2 - t1)) * 1000 / CLOCKS_PER_SEC); 

    // Copy the harris detected image from the stream-to-mem buffer
    result.w = width;
    result.h = height;
    result.img=(unsigned char *)stream_to_mem_dma_buffer;//zero copy
    //extract keypoints from harris image result
    start  =clock();
    incr=0;
    *kpcount=0;
    for(i=0;i < height;i++)
	for(j =0;j < width;j++)
	{
		unsigned char cim=result.img[incr];
		//printf("cim = %d\n",cim);
               	if ((cim!=0) && (*kpcount<MAX_KEYPOINT))
               	{
			//printf("cim = %d\n",cim);
                    	keyPoints[*kpcount].x=j;
                    	keyPoints[*kpcount].y=i;
                    	keyPoints[*kpcount].size=1.0;
                    	keyPoints[*kpcount].cim=cim;
                    	*kpcount=*kpcount+1;
		}
		incr++;
    }

    t2 = clock();
 	printf("extract keypoints... %.0f ms  \r\n",((double) (t2 - start)) * 1000 / CLOCKS_PER_SEC); 
    if (sort!=0)						 			
    { 
   	qsort(keyPoints,sizeof(keyPoints)/sizeof(keyPoint),sizeof(keyPoint),compare2);
   	*kpcount=1024; //keep only highest first points
    }		  
    return result;
}


static void save_keypoints( const char* filename, keyPoint* keypoints, size_t count   )
{
    unsigned int i;
    int filenamelen = strlen(filename)+strlen(".kpts")+1;
    char name[filenamelen];
    snprintf( name, filenamelen, "%s.kpts", filename );
    FILE* f = fopen( name, "w" );

    for(i = 0; i < count; i++ )
    {
        int x = keypoints[i].x;
        int y = keypoints[i].y;
        int cim = keypoints[i].cim;
        fprintf( f, "%d,%d,%d\n",x,y,cim);
    }
    fclose( f );
}



