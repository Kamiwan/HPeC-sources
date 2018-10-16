//TODO Insert header comment

#ifndef ADAPTATION_MANAGER_NODE_H
#define ADAPTATION_MANAGER_NODE_H

#include <ros/ros.h>
#include <ros/callback_queue.h>
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
#include <boost/thread.hpp>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include <sys/wait.h>

#include <iosfwd>
#include <iostream>
#include <fstream>
#include <string>

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "initimg.h"
#include "fpga_header.h"

extern"C"{
	#include "main.h"
	//#include "call.h"
}

#define PATH_TIME_QOS  		"./src/parameters/time_qos_task.txt"
#define PATH_TABLE_C3  		"./src/parameters/tableC3.txt"
#define PATH_ACHIEVABLE_TAB "./src/parameters/achievable_tab.txt"
#define PATH_MAP_TAB 		"./src/parameters/map_tab.txt"
#define PATH_DONE 			"./src/parameters/done.txt"

#define APPLICATION_NUMBER		 11 	//EM, the number of apps in C3 table
#define MULTI_APP_THRESHOLD_CODE 140 	//EM, every code above means a bitstream fusion of apps

using namespace std; 
using namespace cv;

//EM, new struct to allow timing and qos updates
struct App_timing_qos
{  
    int texec; // texec , [mintexec, maxtexec]
    int qos;   //qos , [minqos, maxqos]

	//EM, useful functions to use App_timing_qos easily
	void print();
};


struct Task_in
{
    int req; // 1 : activation, 0 : stop

    int texec; // texec , [mintexec, maxtexec]
    int mintexec;
    int maxtexec;

    int qos;      //qos , [minqos, maxqos]
    int minqos;
    int maxqos;

	int priority; //task priority  

	//EM, useful functions to use Task_in easily
	void raz_timing_qos();
	void raz_all();
	void print();
	Task_in& operator=(Task_in const& rhs);
	Task_in& operator=(App_timing_qos const& rhs);
};

struct Hw_st 
{
	int av; // 1 (YES : Available) , 0 (NO)
};

struct Step_in
{
	Task_in contrast_img;
	Task_in motion_estim_imu;
	Task_in motion_estim_img;
	Task_in search_landing; 
	Task_in obstacle_avoidance;
	Task_in t_landing;
	Task_in rotoz_s;
	Task_in rotoz_b;
	Task_in replanning;
	Task_in detection; 
	Task_in tracking;

	Hw_st h1;
	Hw_st h2;
	Hw_st h3;

	//EM, useful functions to use Step_in easily
	void init();
	void raz_timing_qos();
	void load_C3(const std::vector<Task_in> C3);
	void update_timing_qos(std::vector<App_timing_qos> time_qos);
};


//*********** sortie automate
struct Task_out
{  
    int act; // 1 : active, 0: stop or attente ressource
    int version; // -1, (SW =>) 0,  (HW/tile =>) 1, 2, 3

    int code;

    int achievable; // 0 (NO), 1 :tache realisable (YES)

    int up_pos;   // 1 si l'automate peut choisir une version plus rapide sinon 0
    int down_pos; // 1 si l'automate peut choisir une version moins rapide sinon 0
    int keep_pos; // 1 si l'automate peut conserver la version courante sinon 0
    int qos_pos;  // 1 si possible, sinon 0
};

struct Step_out
{
	Task_out contrast_img;
	Task_out motion_estim_imu;
	Task_out motion_estim_img;
	Task_out search_landing; 
	Task_out obstacle_avoidance;
	Task_out t_landing;
	Task_out rotoz_s;
	Task_out rotoz_b;
	Task_out replanning;
	Task_out detection; 
	Task_out tracking;
};

struct Map_app_out
{
	int 	active;
	int 	version_code;
	int 	region_id;
	string	fusion_sequence;

	//EM, useful functions to use Map_app_out easily
	void init();
	string set_fusion(Map_app_out const& config_app);
	Map_app_out& operator=(Task_out const& rhs);
};



void achievable_tab(Step_out s);

int verify(Step_out s);
void publish_to_MM(int a,Step_out s);
std::vector < std::vector<std::string> > scheduler_tab(int n, int m);
void v(std::string t[][7],int i);
void activate_desactivate_task(std::string t[][7],int i,std_msgs::Int32 msg);
void mapping(std::vector< std::vector<std::string> > M);
void notify_Callback(const std_msgs::Int32::ConstPtr& msg);


bool compare(std::vector<App_timing_qos> time_qos, std::vector<Task_in> C3, Step_in e);

Step_out 				fake_output();
vector<Task_in> 		read_C3(const char* path);
vector<App_timing_qos> 	read_time_qos(const char* path);
vector<Map_app_out>  	init_output(int n, int m, Step_out const& step_output);
void					check_sequence(vector<Map_app_out> & map_config_app);

/*********** Global variables ***********/ 
extern struct timeval  beginning, current1, current2, current3, current4, current5;
extern int time_tk;
extern int time_notif;

#endif


