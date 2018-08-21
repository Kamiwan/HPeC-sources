//TODO Insert header comment

#ifndef ADAPTATION_MANAGER_NODE_H
#define ADAPTATION_MANAGER_NODE_H

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
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/image_encodings.h>
#include <ros/callback_queue.h>
#include <boost/thread.hpp>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include <sys/wait.h>

#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include <iosfwd>

#include <iostream>
#include <fstream>
#include <string>


extern struct timeval  beginning, current1,  current2, current3, current4, current5;
extern int time_tk;
extern int time_notif;


#endif