#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "std_msgs/Float32.h"

double cpuload ( ) {	
  	FILE *fp;
	char buffer[100];
	fp = popen("top -b -d 1 -n5 | grep Cpu | awk '{printf \"%.2f:\", $8}'","r"); 
        fscanf(fp,"%s", buffer); 
 
	int len, i, j = 0, nb = 0 ;
	double value, sum = 0;
	char stock [7];

    len = strlen (buffer);
	for (i = 0; i < len ; i++){
		if (buffer [i] == '\0'|| buffer [i] == '\n')
			break; 
		else {
			if (buffer [i] == ':'){
				printf ("%s\n", stock); j = 0; 
				if(nb > 0) {
					value = atof ( stock ); 
					sum+= value;
				}	
				nb++;			
				continue;
			} 
			stock[j++] = buffer [i];
		}
	} nb -- ;
	return ((nb > 0)? sum / nb : -1);
}



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




int main (int argc, char ** argv)
{
	struct timeval beginning, current;

	ros::init(argc, argv, "os_sensor_node");
	ros::NodeHandle nh;


  	gettimeofday (&beginning , NULL);

	double rate_double = 1;
  	std_msgs::Float32 load; 

	ros::Publisher pub;
	pub = nh.advertise<std_msgs::Float32>("/cpu_load_topic", 1);

	ros::Rate rate(rate_double);
	while (ros::ok()) {

		load.data = cpuload ( ) ;
		pub.publish ( load );	  

		gettimeofday (&current , NULL); 
	   	ROS_INFO("wrapper_time %.0f %.0f\n",((double)time_micros (&current, &beginning)), load.data);

		rate.sleep();	   
    	}
}

