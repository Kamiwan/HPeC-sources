#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>

#include <ros/callback_queue.h>
#include <boost/thread.hpp>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"




/*****************************************************************************
 * 				Global Variables and Struct Definition
*****************************************************************************/
struct timeval beginning, current, end;
struct timeval start, ends;

bool run = false;
int current_ver = 0;

boost::shared_ptr<boost::thread> thread_task;
boost::shared_ptr<ros::NodeHandle> nodehandler_task;

/*****************************************************************************
 * 				Prototype DECLARATION
*****************************************************************************/
void stop ( );
void mgt_callback (const std_msgs::Int32::ConstPtr& value);
void version_1 (const boost::shared_ptr<ros::NodeHandle> & nodehandler_task );

long elapse_time_us (struct timeval *end, struct timeval *start ) 
{
	long seconds = end->tv_sec - start->tv_sec ;
	long micro_seconds = end->tv_usec - start->tv_usec ;
	if (micro_seconds < 0) {
		seconds -= 1;
	} return (seconds * 1000000) + abs (micro_seconds) ;
}

long time_micros (struct timeval *end, struct timeval *start ) {
	if (end != NULL && start != NULL )
		return  (((double) elapse_time_us(end, start ) ) / 1000 ) ;
	return -1;
}


//boost::shared_ptr<ros::Publisher> search_land_pub = NULL;



void stop ( ) 
{
  if(nodehandler_task == NULL && thread_task == NULL) return ;
  if(nodehandler_task != NULL) nodehandler_task->shutdown ();
  if(thread_task !=  NULL) {
    thread_task->timed_join(boost::posix_time::seconds(3));
    if (run){ 
    	pthread_cancel(thread_task->native_handle());		
    }
  }
  //nodehandler_task = NULL;
  //thread_task = NULL;    
}



void mgt_callback (const std_msgs::Int32::ConstPtr& value)
{
  gettimeofday (&current , NULL);	
  switch (value->data){
  case 0 : // stop the task
    stop ( ) ;
    break;
    
  case 1 : //starting the Software version  
    stop ( ) ;
    nodehandler_task = boost::make_shared<ros::NodeHandle>();
    thread_task =  boost::make_shared<boost::thread>(&version_1, nodehandler_task);
    break;
    
    
  case 2 : //starting the HW version 
    stop ( ) ;
    nodehandler_task = boost::make_shared<ros::NodeHandle>();
    //thread_task =  boost::make_shared<boost::thread>(&version_2, nodehandler_task);
    break; 
    
  default:
    break;
  }
}



void version_1 (const boost::shared_ptr<ros::NodeHandle> & nodehandler_task )
{      
  	run = true;
  	ros::CallbackQueue queue;
  	nodehandler_task->setCallbackQueue(&queue);

	ros::Subscriber sub = nodehandler_task->subscribe("topic", 1, mgt_callback);
	ros::Publisher pub = nodehandler_task->advertise<std_msgs::Float32>("/topic", 1);

	std_msgs::Float32 elapsed_time2;

	ros::Rate rate(1);
	long elapsed_time;
	while(nodehandler_task->ok())
	{	   
	   queue.callAvailable(); 
	   start.tv_sec = clock(); 
	   
	  
	   ends.tv_sec = clock();
	   elapsed_time = ((double) (ends.tv_sec - start.tv_sec)) * 1000 / CLOCKS_PER_SEC ;
	   elapsed_time2.data = ((double) time_micros(&ends, &start));
	   pub.publish ( elapsed_time2 );	   
	   rate.sleep();	   
    	} 
	run = false ;
}


int main (int argc, char ** argv)
{
	ros::init(argc, argv, "generic_task_wrapper_node");
	ros::NodeHandle nh;
	ros::Subscriber mgt_topic;
	
  	gettimeofday (&beginning , NULL);
	mgt_topic = nh.subscribe("/task_mgt_topic", 1, mgt_callback);	
  	gettimeofday (&current , NULL); 	

       ros::spin();
}
