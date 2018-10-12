//TODO Insert header comment

#ifndef UTILS_H
#define UTILS_H

#include "adaptation_manager_node.h"

long elapse_time_u (struct timeval *end, struct timeval *start );
long time_micros (struct timeval *end, struct timeval *start );
double cpuload ( );
vector<string> readfile(const char* path);
inline bool isInteger(const std::string & s);

#endif