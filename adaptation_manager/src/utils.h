//TODO Insert header comment

#ifndef UTILS_H
#define UTILS_H

#include "adaptation_manager_node.h"

long elapse_time_u (struct timeval *end, struct timeval *start );
long time_micros (struct timeval *end, struct timeval *start );
double cpuload ( );
vector<string> readfile1(const char* path);

#endif