//TODO Insert header comment

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "am_defines.h"

long    elapse_time_u (struct timeval *end, struct timeval *start );
long    time_micros (struct timeval *end, struct timeval *start );
double  cpuload ( );

std::vector<std::string>    readfile(const char* path);
inline bool                 isInteger(const std::string & s);
void                        write_value_file(const char* path, 
                                             const std::string& app_name, 
                                             const int& value);

#endif