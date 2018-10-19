//TODO insert header comment here

#include "handle_applications.h"


/*void handle_notification_from_search_landing_wrapper (const std_msgs::Float32::ConstPtr& value)
{
	gettimeofday (&current5 , NULL);
	time_tk = (int) value -> data;	
	printf("adaptation_manager_time %.0f %d\n", ((double) time_micros (&current5, &beginning)), time_tk);
	time_notif = 1 ;
}
*/


//NOT USED FUNCTIONS BELOW
/*
void handle_search_landing_activation (const std_msgs::Int32::ConstPtr& value) 
{
	//current4 = clock(); 
	gettimeofday (&current4 , NULL);
	if (value -> data == 0) 
	{
		r = 0;
		e = 1;	
		time_notif = 1; 
		printf("adaptation_manager_req %.0f 0\n", ((double) time_micros(&current4, &beginning)));
		return ;
	}
	
	if (value -> data == 1)
	{	 
		r = 1;
		e = 0;
		time_notif = 1;
		printf("adaptation_manager_req %.0f 1\n", ((double) time_micros(&current4, &beginning)));
	}
}


void handle_search_landing_min_threshold (const std_msgs::Int32::ConstPtr& value) 
{

	//current3 = clock();  
	gettimeofday (&current3 , NULL);
	min_thres = value -> data;	
	printf("adaptation_manager_min %.0f %d\n", ((double) time_micros(&current3, &beginning)), min_thres);
}


void handle_search_landing_max_threshold (const std_msgs::Int32::ConstPtr& value) 
{
	//current2 = clock();  
	gettimeofday (&current2 , NULL);
	max_thres = value -> data;	
	printf("adaptation_manager_max %.0f %d\n", ((double) time_micros (&current2, &beginning)), max_thres);
}
*/

