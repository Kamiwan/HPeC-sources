//TODO insert header comment here

#include "utils.h"


long elapse_time_u (struct timeval *end, struct timeval *start ) 
{
	long seconds = end->tv_sec - start->tv_sec ;
	long micro_seconds = end->tv_usec - start->tv_usec ;
	if (micro_seconds < 0)
	{
		seconds -= 1;
	}
	return (seconds * 1000000) + abs (micro_seconds) ;
}

long time_micros (struct timeval *end, struct timeval *start ) 
{
	if (end != NULL && start != NULL )
		return  (((double) elapse_time_u (end, start)) / 1000 ) ;
	return -1;
}