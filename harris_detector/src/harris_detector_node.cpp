#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <image_transport/image_transport.h>
#include <vector>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>

extern "C" {
#include "image.h"
#include "debug.h"
#include "harris.h"
#include "freak.h"
}

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>

#include <ros/callback_queue.h>
#include <boost/thread.hpp>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"

#include "std_msgs/String.h"
#include <sstream>
#include "communication/area_location.h"
#include "sgdma_dispatcher.h"
#include "sgdma_dispatcher_regs.h"



#define SDRAM_SPAN (0x04000000)
#define HW_REGS_SPAN (0x00020000)
#define HWREG_BASE (0xff200000)

//EM, 11/04/18, Hardware version preparation
#define DMA_READ_CSR_BASEADDR 0xFF200000
#define DMA_READ_DESC_BASEADDR 0xFF201000
#define DMA_WRITE_CSR_BASEADDR 0xFF202000
#define DMA_WRITE_DESC_BASEADDR 0xFF203000
#define OCRAM_SBASE	0xC0000000	/* Rootport onchip RAM base */
#define OCRAM_SIZE	0x8000	/* Rootport onchip RAM size */

//Erwan Moréac, 05/03/18 : Setup #define
#define HIL				 //Code modifications for Hardware In the Loop
#define AREA_MIN 400	 //TODO : create a function to make it dependent of the UAV height and FOV
#define AREA_MAX 2073601 //This value exceeds the area of an HD picture 1920 x 1080 in pixels
#define HARRIS_INPUT_TOPIC "/iris/camera2/image_raw"
#define HARRIS_OUTPUT_IMAGE_TOPIC "search_output/image"
//#define HARRIS_OUTPUT_AREA_LOCATION_TOPIC "search_output/areas"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
//DEGUG only #define WRITE_IMG

boost::shared_ptr<ros::Publisher> search_land_pub;

PPM_IMG img_ibuf_color;

int hardware = 0;
int fd;

FILE *cma_dev_file;
char cma_addr_string[10];

time_t start, ends,t1;
time_t imcpy_start,imcpy_end;
struct timeval beginning, current, end;

unsigned int cma_base_addr;
void *virtual_base_sdram;

volatile unsigned int *mem_to_stream_dma_buffer = NULL;
volatile unsigned char *stream_to_mem_dma_buffer = NULL;

// Create  Dispatcher
tcSGDMADispatcher     dispatcher_read;
tcSGDMADispatcher     dispatcher_write;
// Create  descriptor
tsSGDMADescriptor descriptor_read;
tsSGDMADescriptor descriptor_write;

struct pixel *data;
unsigned char *header;

double diameter;
double altitude;
double camera_angle;
double image_height;

bool run = false;
int current_ver = 0;
boost::shared_ptr<boost::thread> worker_thread;
boost::shared_ptr<ros::NodeHandle> workerHandle_ptr;

void gps_callback(const sensor_msgs::NavSatFix::ConstPtr &position);
void image_callback(const sensor_msgs::Image::ConstPtr &image_cam);

extern void RST_RANSAC(Point *point1,Point *point2,int N_SAMPLES,int threshold,float *theta,int *tx,int *ty );
static void save_keypoints( const char* filename, keyPoint* keypoints, size_t count   );
PGM_IMG harriscornerhw(PPM_IMG img_ibuf_color,int threshold,keyPoint *keyPoints,int *kpcount,int sort);
static void save_descriptor( const  char *filename, descriptor* descriptors, size_t desc_count );


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
	virtual_base_sdram = mmap(NULL, SDRAM_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, cma_base_addr);
	if (virtual_base_sdram == MAP_FAILED)
	{
		printf("ERROR: mmap() failed...\n");
		close(fd);
		exit (1);
	}

	mem_to_stream_dma_buffer = (volatile unsigned int *)(virtual_base_sdram);
	stream_to_mem_dma_buffer = (volatile unsigned char *)(virtual_base_sdram + 0x2000000);

	// Create  Dispatcher
	dispatcher_read.init(DMA_READ_CSR_BASEADDR, DMA_READ_DESC_BASEADDR, 0);
	dispatcher_write.init(DMA_WRITE_CSR_BASEADDR, DMA_WRITE_DESC_BASEADDR, 0);
	
	// EM, HW version update, Configure DMAs to the correct addresses
	// Set the HPS Memory start address
	descriptor_read.read_addr = cma_base_addr;
	descriptor_read.write_addr = cma_base_addr;
	descriptor_write.read_addr = cma_base_addr+0x2000000;
	descriptor_write.write_addr = cma_base_addr+0x2000000;
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
    close( fd );
}


void harris_detector_hwsw(const boost::shared_ptr<ros::NodeHandle> &workerHandle_ptr)
{
	ROS_ERROR("[THREAD][RUNNING][HW]: Vision Based Autonomous Landing HARDWARE VERSION \r\n");
	//ROS_ERROR("Rgb2gray, median filter, canny edge detector, morphological closing in hw...\r\n");

    PGM_IMG img_ibuf_g;
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
	ros::CallbackQueue queue;
	workerHandle_ptr->setCallbackQueue(&queue);

	//Erwan Moréac 22/02/18, Topic Subscribtion (Input) and Publication (Output)
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


	ros::Subscriber gps_sub = workerHandle_ptr->subscribe("mavros/global_position/global", 1, gps_callback);
	std_msgs::Float32 elapsed_time;
	while (workerHandle_ptr->ok())
	{
		queue.callAvailable();

		//EM, Add a test in the case the camera topic is not activated
		if (img_ibuf_color.h != 0 && img_ibuf_color.w != 0)
		{
			// Start measuring time
			start = clock();
			t1=clock();
			
			refKeyPoints=(keyPoint *)malloc(MAX_KEYPOINT* sizeof(keyPoint));
			
			//memset(virtual_base_bram,0,MAX_KEYPOINT*8);
			img_ibuf_ha=harriscornerhw(img_ibuf_color,32,refKeyPoints,&refKpCount,0);
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

			free_ppm(img_ibuf_color);
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

    PGM_IMG img_ibuf_g;
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
		if (img_ibuf_color.h != 0 && img_ibuf_color.w != 0 && img_ibuf_color.img != NULL)
		{
 			refKeyPoints=(keyPoint *)malloc(MAX_KEYPOINT* sizeof(keyPoint));
    		printf("Reference image Harris Corner...\r\n");

			// Start measuring time
			start = clock();

			printf("Reference image Rgb2gray...\r\n");
         	img_ibuf_g = rgb2gray(img_ibuf_color);
			#ifdef WRITE_IMG
				write_pgm(img_ibuf_g, "grayref.pgm");
			#endif
    		img_ibuf_ha=harriscorner(img_ibuf_g,32,refKeyPoints,&refKpCount,0);
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
       		free_pgm(img_ibuf_g);
			free(refKeyPoints);

			#ifdef HIL //Erwan Moréac, Mandatory free since we have to use malloc
				if(img_ibuf_color.img != NULL)
				{
					free_ppm(img_ibuf_color); //Erwan Moréac 22/02/18, pointer adress is given so zero copy, no free
					img_ibuf_color.img = NULL;
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
	
	//beginning = clock();
	gettimeofday(&beginning, NULL);
	ros::Subscriber mgt_topic;
	mgt_topic = nh.subscribe("/harris_detector_mgt_topic", 1, managing_controller_request);

	search_land_pub = boost::make_shared<ros::Publisher>(
		nh.advertise<std_msgs::Float32>("/harris_detector_notification_topic", 1));
	//current = clock();
	gettimeofday(&current, NULL);

	dbprintf("wrapper_ver %.0f 0\n", ((double)time_micros(&current, &beginning)));

	ROS_ERROR("[TASK WRAPPER][RUNNING]");
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

		ROS_ERROR("IMAGE RECOVERY SUCCEED");
		//std::cout << "ORIGINAL PICTURE : " << image_DATA->image << std::endl;
		//std::cout << "PICTURE PIXELS : " << img_ibuf_color.img << std::endl;
	}
	catch (cv_bridge::Exception &e)
	{
		ROS_ERROR("Could not convert from '%s' to 'bgr'.", image_cam->encoding.c_str());
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

PGM_IMG harriscornerhw(PPM_IMG img_ibuf_color,int threshold,keyPoint *keyPoints,int *kpcount,int sort)
{
    PGM_IMG result;
    time_t start, t1,t2;
    // Image dimensions
    int i, j,incr;
    result.w = img_ibuf_color.w;
    result.h = img_ibuf_color.h;
    int width =img_ibuf_color.w;
    int height = img_ibuf_color.h;

    start  =clock();

    // Tell the dispatcher to start
    descriptor_read.control.msBits.go = 1;
    descriptor_write.control.msBits.go = 1;

    // Turn on the DMAs
    dispatcher_read.WriteDescriptor(descriptor_read);
    dispatcher_write.WriteDescriptor(descriptor_write);
    // wait until the dma transfer complete
    //start = clock();
    /* while( dispatcher_read.GetStatusReg().msBits.busy)
    {
	printf("busy : %d\n",dispatcher_read.GetStatusReg().msBits.busy);
	printf("desc_buf_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_empty);
	printf("desc_buf_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_buf_full);
	printf("desc_res_empty : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_empty);
	printf("desc_res_full : %d\n",dispatcher_read.GetStatusReg().msBits.desc_res_full);
	printf("stopped : %d\n",dispatcher_read.GetStatusReg().msBits.stopped);
	printf("resetting  : %d\n",dispatcher_read.GetStatusReg().msBits.resetting );
	printf("stop_on_err  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_err );
	printf("stop_on_early  : %d\n",dispatcher_read.GetStatusReg().msBits.stop_on_early );
	printf("irq  : %d\n",dispatcher_read.GetStatusReg().msBits.irq );
    };
    */
    /* t1 = clock();
 	printf("read memory transfer done... %.0f ms \r\n",((double) (t1 - start)) * 1000 / CLOCKS_PER_SEC); */
    while( dispatcher_write.GetStatusReg().msBits.busy)
    {
	/* printf("busy : %d\n",dispatcher_write.GetStatusReg().msBits.busy);
	printf("desc_buf_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_empty);
	printf("desc_buf_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_buf_full);
	printf("desc_res_empty : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_empty);
	printf("desc_res_full : %d\n",dispatcher_write.GetStatusReg().msBits.desc_res_full);
	printf("stopped : %d\n",dispatcher_write.GetStatusReg().msBits.stopped);
	printf("resetting  : %d\n",dispatcher_write.GetStatusReg().msBits.resetting );
	printf("stop_on_err  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_err );
	printf("stop_on_early  : %d\n",dispatcher_write.GetStatusReg().msBits.stop_on_early );
	printf("irq  : %d\n",dispatcher_write.GetStatusReg().msBits.irq ); */
    };
    t2 = clock();
 	printf("write memory transfer done... %.0f ms \r\n",((double) (t2 - start)) * 1000 / CLOCKS_PER_SEC); 

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



