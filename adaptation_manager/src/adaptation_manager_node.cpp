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
using namespace std; 

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "initimg.cpp"
#include "fpga_header.h"

using namespace cv;

extern"C"{
	#include "main.h"
	//#include "call.h"
}

boost::shared_ptr<ros::Publisher> search_land_pub = NULL;
boost::shared_ptr<ros::Publisher> contrast_img_pub = NULL;
boost::shared_ptr<ros::Publisher> motion_estim_imu_pub = NULL;
boost::shared_ptr<ros::Publisher> motion_estim_img_pub = NULL;
boost::shared_ptr<ros::Publisher> obstacle_avoidance_pub = NULL;
boost::shared_ptr<ros::Publisher> t_landing_pub = NULL;
boost::shared_ptr<ros::Publisher> rotoz_s_pub = NULL;
boost::shared_ptr<ros::Publisher> rotoz_b_pub = NULL;
boost::shared_ptr<ros::Publisher> replanning_pub = NULL;
boost::shared_ptr<ros::Publisher> detection_pub = NULL;
boost::shared_ptr<ros::Publisher> tracking_pub = NULL;

boost::shared_ptr<ros::Publisher> ctr_img_m_e_imu_pub= NULL;
boost::shared_ptr<ros::Publisher> ctr_img_m_e_img_pub= NULL;
boost::shared_ptr<ros::Publisher> ctr_img_s_land_pub= NULL;
boost::shared_ptr<ros::Publisher> ctr_img_rotoz_s_pub= NULL;
boost::shared_ptr<ros::Publisher> m_e_img_t_landing_pub= NULL;
boost::shared_ptr<ros::Publisher> m_e_img_rotoz_s_pub= NULL;
boost::shared_ptr<ros::Publisher> m_e_img_s_land_pub= NULL;
boost::shared_ptr<ros::Publisher> rotoz_s_t_landing_pub= NULL;
boost::shared_ptr<ros::Publisher> t_landing_s_land_pub= NULL;
boost::shared_ptr<ros::Publisher> rotoz_s_s_land_pub= NULL;

//******************
boost::shared_ptr<ros::Publisher> achievable_pub=NULL;

//time_t beginning, current1, current2, current3, current4, current5;
struct timeval  beginning, current1,  current2, current3, current4, current5;
//Main__main_mem mem;


int r = 0;
int e = 0;
int time_tk = 0;
int min_thres = 0;
int max_thres = 2000;
int f = 0;
int rp = 0;
//Main__main_out _res;

int time_notif = 0 ;
  
std_msgs::Int32 search_landing_task_vers;


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

/*void reset ( ) 
{
    Main__main_reset(&mem);
    search_landing_task_vers.data = 0;
}*/


/*void do_step ( ) 
{
    Main__main_step(r, e, time_tk, min_thres, max_thres, f, rp, &_res, &mem);
    
    gettimeofday (&current1 , NULL);
    		
    printf("adaptation_manager_ver %.0f %d\n",((double) time_micros(&current1, &beginning)), _res.res);
    if(search_landing_task_vers.data != _res.res ) 
    {
    	if( e == 1){
		time_tk = 0;
		min_thres = 0;
		max_thres = 2000;
	}
	r = 0;
	e = 0;
    	
    	ROS_ERROR("[COMPUTE][STEP] : %.0f execution time = [%d ms] min = [%d ms] max = [%d ms]\n",
    			((double)time_micros (&current1, &beginning)),  time_tk, min_thres, max_thres);
   	search_landing_task_vers.data = _res.res;
    	search_land_pub -> publish ( search_landing_task_vers );
    }
}*/

void handle_notification_from_search_landing_wrapper (const std_msgs::Float32::ConstPtr& value)
{
	
	//current5 = clock();  
	gettimeofday (&current5 , NULL);
	time_tk = (int) value -> data;	
	printf("adaptation_manager_time %.0f %d\n", ((double) time_micros (&current5, &beginning)), time_tk);
	
	time_notif = 1 ;
}

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
//*****************
/* author : El Mehdi ABDALI    Laboratory: Institut Pascal, CNRS UMR 6602, FRANCE        */
/* contact information : el_mehdi.abdali@uca.fr  , elmehdi.abdali@gmail.com              */

/* start trigger module */
void start_reconfiguration(int fd_mem, int bitstream_address, int region_id)
{
   int param_address;
   int start_done_word = 0x80000000;

   start_done_word = start_done_word + (0x30000000 & (region_id<<28)) + (0x0FFFFFFF & bitstream_address);
   int *pointer_param= &start_done_word;

   setimg((uchar*)pointer_param, fd_mem, 0, LW_HPS2FPGA_AXI_MASTER, 1, 4);
}

/* read done module */
int reconfiguration_done(int fd_mem)
{
   int param_address;
   int start_done_word;
   int *pointer_param= &start_done_word;
   int value;

   getimg((uchar*)pointer_param, fd_mem, 0, LW_HPS2FPGA_AXI_MASTER, 1, 4);
   
   if ((*pointer_param & 0x40000000) == 2)
         value=1;
   else
         value=0;

   return value;
}
//******************

vector<string> readfile1(const char* path){
      std::ifstream fichier(path); 
    vector<string> lignes;
   int i(0); 
   string temp;
    if ( fichier ) 
    { 
        while ( std::getline( fichier, temp) ) 
        { 
		 lignes.push_back(temp);
        }
	}
	else
   {
      cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;

   }  fichier.close();
   //cout << "il y a "<< lignes.size() << " lignes dans le fichier" << endl;

   /*for (size_t i = 0; i < lignes.size(); ++i)
    {   
       cout << "Ligne " << i << " : " << lignes[i] <<  endl;
	}*/
	return lignes;
        }
	  //*************** entree automate :
typedef struct Task_in Task_in;
struct Task_in
{
    int req; // 1 : activation, 0 : arrêt

    int texec; // texec , [mintexec, maxtexec]
    int mintexec;
    int maxtexec;

    int qos;      //qos , [minqos, maxqos]
    int minqos;
    int maxqos;

	int priority; //priorite de tache  
};
typedef struct Hw_st Hw_st;
struct Hw_st 
{
	int av; // 1 (YES : Available) , 0 (NO)
};

typedef struct Step_in Step_in;
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
};
 //*********** sortie automate
	  typedef struct Task_out Task_out;
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

typedef struct Step_out Step_out;
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
//********** initialiser l'entree du step
struct Step_in entree0(vector<string> C3){
    struct Step_in e;  //std::string::size_type sz;
	e.contrast_img.req =stoi(C3[1]);
    e.contrast_img.texec =0;
    e.contrast_img.mintexec =stoi(C3[3]);
    e.contrast_img.maxtexec =stoi(C3[4]);
    e.contrast_img.qos =0;
    e.contrast_img.minqos =stoi(C3[6]);
    e.contrast_img.maxqos =stoi(C3[7]);
    e.contrast_img.priority =stoi(C3[8]);


	e.motion_estim_imu.req =stoi(C3[10]);
    e.motion_estim_imu.texec =0;
    e.motion_estim_imu.mintexec =stoi(C3[12]);
    e.motion_estim_imu.maxtexec =stoi(C3[13]);
    e.motion_estim_imu.qos =0;
    e.motion_estim_imu.minqos =stoi(C3[15]);
    e.motion_estim_imu.maxqos =stoi(C3[16]);
    e.motion_estim_imu.priority =stoi(C3[17]);

	e.motion_estim_img.req =stoi(C3[19]);
    e.motion_estim_img.texec =0;
    e.motion_estim_img.mintexec =stoi(C3[21]);
    e.motion_estim_img.maxtexec =stoi(C3[22]);
    e.motion_estim_img.qos =0;
    e.motion_estim_img.minqos =stoi(C3[24]);
    e.motion_estim_img.maxqos =stoi(C3[25]);
    e.motion_estim_img.priority =stoi(C3[26]);

	
	e.search_landing.req =stoi(C3[28]);
    e.search_landing.texec =0;
    e.search_landing.mintexec =stoi(C3[30]);
    e.search_landing.maxtexec =stoi(C3[31]);
    e.search_landing.qos =0;
    e.search_landing.minqos =stoi(C3[33]);
    e.search_landing.maxqos =stoi(C3[34]);
    e.search_landing.priority =stoi(C3[35]);

	e.obstacle_avoidance.req =stoi(C3[37]);
    e.obstacle_avoidance.texec =0;
    e.obstacle_avoidance.mintexec =stoi(C3[39]);
    e.obstacle_avoidance.maxtexec =stoi(C3[40]);
    e.obstacle_avoidance.qos =0;
    e.obstacle_avoidance.minqos =stoi(C3[42]);
    e.obstacle_avoidance.maxqos =stoi(C3[43]);
    e.obstacle_avoidance.priority =stoi(C3[44]);

	e.t_landing.req =stoi(C3[46]);
    e.t_landing.texec =0;
    e.t_landing.mintexec =stoi(C3[48]);
    e.t_landing.maxtexec =stoi(C3[49]);
    e.t_landing.qos =0;
    e.t_landing.minqos =stoi(C3[51]);
    e.t_landing.maxqos =stoi(C3[52]);
    e.t_landing.priority =stoi(C3[53]);


	e.rotoz_s.req =stoi(C3[55]);
    e.rotoz_s.texec =0;
    e.rotoz_s.mintexec =stoi(C3[57]);
    e.rotoz_s.maxtexec =stoi(C3[58]);
    e.rotoz_s.qos =0;
    e.rotoz_s.minqos =stoi(C3[60]);
    e.rotoz_s.maxqos =stoi(C3[61]);
    e.rotoz_s.priority =stoi(C3[62]);
	
	e.rotoz_b.req =stoi(C3[64]);
    e.rotoz_b.texec =0;
    e.rotoz_b.mintexec =stoi(C3[66]);
    e.rotoz_b.maxtexec =stoi(C3[67]);
    e.rotoz_b.qos =0;
    e.rotoz_b.minqos =stoi(C3[69]);
    e.rotoz_b.maxqos =stoi(C3[70]);
    e.rotoz_b.priority =stoi(C3[71]);

	e.replanning.req =stoi(C3[73]);
    e.replanning.texec =0;
    e.replanning.mintexec =stoi(C3[75]);
    e.replanning.maxtexec =stoi(C3[76]);
    e.replanning.qos =0;
    e.replanning.minqos =stoi(C3[78]);
    e.replanning.maxqos =stoi(C3[79]);
    e.replanning.priority =stoi(C3[80]);
	

	e.detection.req =stoi(C3[82]);
    e.detection.texec =0;
    e.detection.mintexec =stoi(C3[84]);
    e.detection.maxtexec =stoi(C3[85]);
    e.detection.qos =0;
    e.detection.minqos =stoi(C3[87]);
    e.detection.maxqos =stoi(C3[88]);
    e.detection.priority =stoi(C3[89]);

	e.tracking.req =stoi(C3[91]);
    e.tracking.texec =0;
    e.tracking.mintexec =stoi(C3[93]);
    e.tracking.maxtexec =stoi(C3[94]);
    e.tracking.qos =0;
    e.tracking.minqos =stoi(C3[96]);
    e.tracking.maxqos =stoi(C3[97]);
    e.tracking.priority =stoi(C3[98]);
	
   return e;
}
struct Step_in entree1(vector<string> C3){
    struct Step_in e;  //std::string::size_type sz;
	e.contrast_img.req =stoi(C3[1]);
    e.contrast_img.texec =stoi(C3[2]);
    e.contrast_img.mintexec =stoi(C3[3]);
    e.contrast_img.maxtexec =stoi(C3[4]);
    e.contrast_img.qos =stoi(C3[5]);
    e.contrast_img.minqos =stoi(C3[6]);
    e.contrast_img.maxqos =stoi(C3[7]);
    e.contrast_img.priority =stoi(C3[8]);


	e.motion_estim_imu.req =stoi(C3[10]);
    e.motion_estim_imu.texec =stoi(C3[11]);
    e.motion_estim_imu.mintexec =stoi(C3[12]);
    e.motion_estim_imu.maxtexec =stoi(C3[13]);
    e.motion_estim_imu.qos =stoi(C3[14]);
    e.motion_estim_imu.minqos =stoi(C3[15]);
    e.motion_estim_imu.maxqos =stoi(C3[16]);
    e.motion_estim_imu.priority =stoi(C3[17]);

	e.motion_estim_img.req =stoi(C3[19]);
    e.motion_estim_img.texec =stoi(C3[20]);
    e.motion_estim_img.mintexec =stoi(C3[21]);
    e.motion_estim_img.maxtexec =stoi(C3[22]);
    e.motion_estim_img.qos =stoi(C3[23]);
    e.motion_estim_img.minqos =stoi(C3[24]);
    e.motion_estim_img.maxqos =stoi(C3[25]);
    e.motion_estim_img.priority =stoi(C3[26]);

	
	e.search_landing.req =stoi(C3[28]);
    e.search_landing.texec =stoi(C3[29]);
    e.search_landing.mintexec =stoi(C3[30]);
    e.search_landing.maxtexec =stoi(C3[31]);
    e.search_landing.qos =stoi(C3[32]);
    e.search_landing.minqos =stoi(C3[33]);
    e.search_landing.maxqos =stoi(C3[34]);
    e.search_landing.priority =stoi(C3[35]);

	e.obstacle_avoidance.req =stoi(C3[37]);
    e.obstacle_avoidance.texec =stoi(C3[38]);
    e.obstacle_avoidance.mintexec =stoi(C3[39]);
    e.obstacle_avoidance.maxtexec =stoi(C3[40]);
    e.obstacle_avoidance.qos =stoi(C3[41]);
    e.obstacle_avoidance.minqos =stoi(C3[42]);
    e.obstacle_avoidance.maxqos =stoi(C3[43]);
    e.obstacle_avoidance.priority =stoi(C3[44]);

	e.t_landing.req =stoi(C3[46]);
    e.t_landing.texec =stoi(C3[47]);
    e.t_landing.mintexec =stoi(C3[48]);
    e.t_landing.maxtexec =stoi(C3[49]);
    e.t_landing.qos =stoi(C3[50]);
    e.t_landing.minqos =stoi(C3[51]);
    e.t_landing.maxqos =stoi(C3[52]);
    e.t_landing.priority =stoi(C3[53]);


	e.rotoz_s.req =stoi(C3[55]);
    e.rotoz_s.texec =stoi(C3[56]);
    e.rotoz_s.mintexec =stoi(C3[57]);
    e.rotoz_s.maxtexec =stoi(C3[58]);
    e.rotoz_s.qos =stoi(C3[59]);
    e.rotoz_s.minqos =stoi(C3[60]);
    e.rotoz_s.maxqos =stoi(C3[61]);
    e.rotoz_s.priority =stoi(C3[62]);
	
	e.rotoz_b.req =stoi(C3[64]);
    e.rotoz_b.texec =stoi(C3[65]);
    e.rotoz_b.mintexec =stoi(C3[66]);
    e.rotoz_b.maxtexec =stoi(C3[67]);
    e.rotoz_b.qos =stoi(C3[68]);
    e.rotoz_b.minqos =stoi(C3[69]);
    e.rotoz_b.maxqos =stoi(C3[70]);
    e.rotoz_b.priority =stoi(C3[71]);

	e.replanning.req =stoi(C3[73]);
    e.replanning.texec =stoi(C3[74]);
    e.replanning.mintexec =stoi(C3[75]);
    e.replanning.maxtexec =stoi(C3[76]);
    e.replanning.qos =stoi(C3[77]);
    e.replanning.minqos =stoi(C3[78]);
    e.replanning.maxqos =stoi(C3[79]);
    e.replanning.priority =stoi(C3[80]);
	

	e.detection.req =stoi(C3[82]);
    e.detection.texec =stoi(C3[83]);
    e.detection.mintexec =stoi(C3[84]);
    e.detection.maxtexec =stoi(C3[85]);
    e.detection.qos =stoi(C3[86]);
    e.detection.minqos =stoi(C3[87]);
    e.detection.maxqos =stoi(C3[88]);
    e.detection.priority =stoi(C3[89]);

	e.tracking.req =stoi(C3[91]);
    e.tracking.texec =stoi(C3[92]);
    e.tracking.mintexec =stoi(C3[93]);
    e.tracking.maxtexec =stoi(C3[94]);
    e.tracking.qos =stoi(C3[95]);
    e.tracking.minqos =stoi(C3[96]);
    e.tracking.maxqos =stoi(C3[97]);
    e.tracking.priority =stoi(C3[98]);
	
   return e;
}
struct Step_in entree(vector<string> C3,vector<string> lignes){
    struct Step_in e;  //std::string::size_type sz;
	e.contrast_img.req =stoi(C3[1]);
    e.contrast_img.texec =stoi(lignes[1]);
    e.contrast_img.mintexec =stoi(C3[3]);
    e.contrast_img.maxtexec =stoi(C3[4]);
    e.contrast_img.qos =stoi(lignes[2]);
    e.contrast_img.minqos =stoi(C3[6]);
    e.contrast_img.maxqos =stoi(C3[7]);
    e.contrast_img.priority =stoi(C3[8]);


	e.motion_estim_imu.req =stoi(C3[10]);
    e.motion_estim_imu.texec =stoi(lignes[4]);
    e.motion_estim_imu.mintexec =stoi(C3[12]);
    e.motion_estim_imu.maxtexec =stoi(C3[13]);
    e.motion_estim_imu.qos =stoi(lignes[5]);
    e.motion_estim_imu.minqos =stoi(C3[15]);
    e.motion_estim_imu.maxqos =stoi(C3[16]);
    e.motion_estim_imu.priority =stoi(C3[17]);

	e.motion_estim_img.req =stoi(C3[19]);
    e.motion_estim_img.texec =stoi(lignes[7]);
    e.motion_estim_img.mintexec =stoi(C3[21]);
    e.motion_estim_img.maxtexec =stoi(C3[22]);
    e.motion_estim_img.qos =stoi(lignes[8]);
    e.motion_estim_img.minqos =stoi(C3[24]);
    e.motion_estim_img.maxqos =stoi(C3[25]);
    e.motion_estim_img.priority =stoi(C3[26]);

	
	e.search_landing.req =stoi(C3[28]);
    e.search_landing.texec =stoi(lignes[10]);
    e.search_landing.mintexec =stoi(C3[30]);
    e.search_landing.maxtexec =stoi(C3[31]);
    e.search_landing.qos =stoi(lignes[11]);
    e.search_landing.minqos =stoi(C3[33]);
    e.search_landing.maxqos =stoi(C3[34]);
    e.search_landing.priority =stoi(C3[35]);

	cout<<"s_l :"<<e.search_landing.texec<<endl;

	e.obstacle_avoidance.req =stoi(C3[37]);
    e.obstacle_avoidance.texec =stoi(lignes[13]);
    e.obstacle_avoidance.mintexec =stoi(C3[39]);
    e.obstacle_avoidance.maxtexec =stoi(C3[40]);
    e.obstacle_avoidance.qos =stoi(lignes[14]);
    e.obstacle_avoidance.minqos =stoi(C3[42]);
    e.obstacle_avoidance.maxqos =stoi(C3[43]);
    e.obstacle_avoidance.priority =stoi(C3[44]);

	e.t_landing.req =stoi(C3[46]);
    e.t_landing.texec =stoi(lignes[16]);
    e.t_landing.mintexec =stoi(C3[48]);
    e.t_landing.maxtexec =stoi(C3[49]);
    e.t_landing.qos =stoi(lignes[17]);
    e.t_landing.minqos =stoi(C3[51]);
    e.t_landing.maxqos =stoi(C3[52]);
    e.t_landing.priority =stoi(C3[53]);


	e.rotoz_s.req =stoi(C3[55]);
    e.rotoz_s.texec =stoi(lignes[19]);
    e.rotoz_s.mintexec =stoi(C3[57]);
    e.rotoz_s.maxtexec =stoi(C3[58]);
    e.rotoz_s.qos =stoi(lignes[20]);
    e.rotoz_s.minqos =stoi(C3[60]);
    e.rotoz_s.maxqos =stoi(C3[61]);
    e.rotoz_s.priority =stoi(C3[62]);
	
	e.rotoz_b.req =stoi(C3[64]);
    e.rotoz_b.texec =stoi(lignes[22]);
    e.rotoz_b.mintexec =stoi(C3[66]);
    e.rotoz_b.maxtexec =stoi(C3[67]);
    e.rotoz_b.qos =stoi(lignes[23]);
    e.rotoz_b.minqos =stoi(C3[69]);
    e.rotoz_b.maxqos =stoi(C3[70]);
    e.rotoz_b.priority =stoi(C3[71]);

	e.replanning.req =stoi(C3[73]);
    e.replanning.texec =stoi(lignes[25]);
    e.replanning.mintexec =stoi(C3[75]);
    e.replanning.maxtexec =stoi(C3[76]);
    e.replanning.qos =stoi(lignes[26]);
    e.replanning.minqos =stoi(C3[78]);
    e.replanning.maxqos =stoi(C3[79]);
    e.replanning.priority =stoi(C3[80]);
	

	e.detection.req =stoi(C3[82]);
    e.detection.texec =stoi(lignes[28]);
    e.detection.mintexec =stoi(C3[84]);
    e.detection.maxtexec =stoi(C3[85]);
    e.detection.qos =stoi(lignes[29]);
    e.detection.minqos =stoi(C3[87]);
    e.detection.maxqos =stoi(C3[88]);
    e.detection.priority =stoi(C3[89]);

	e.tracking.req =stoi(C3[91]);
    e.tracking.texec =stoi(lignes[31]);
    e.tracking.mintexec =stoi(C3[93]);
    e.tracking.maxtexec =stoi(C3[94]);
    e.tracking.qos =stoi(lignes[32]);
    e.tracking.minqos =stoi(C3[96]);
    e.tracking.maxqos =stoi(C3[97]);
    e.tracking.priority =stoi(C3[98]);
	cout<<"track :"<<e.tracking.texec<<endl;
	cout<<"track :"<<e.tracking.qos<<endl;
   return e;
}
//************initialiser sortie automate // appartient au step fantome //a effacer apres
struct Step_out sortie(){
	struct Step_out s;
	 
     s.contrast_img.act = 1;
	 s.contrast_img.code = 142; 
     s.contrast_img.achievable = 1;
	 
	 cout<<s.contrast_img.act<<endl;
	 cout<<s.contrast_img.code<<endl; 
     cout<<s.contrast_img.achievable<<endl;

     
	 s.motion_estim_imu.act = 0;
	 s.motion_estim_imu.code = 122; 
     s.motion_estim_imu.achievable = 1;
    
	
	 s.motion_estim_img.act = 1;
	 s.motion_estim_img.code = 182; 
     s.motion_estim_img.achievable = 1;

	 s.search_landing.act = 1;
	 s.search_landing.code = 13; 
     s.search_landing.achievable = 1;

	 s.obstacle_avoidance.act = 0;
	 s.obstacle_avoidance.code = 50; 
     s.obstacle_avoidance.achievable = 1;

	 s.t_landing.act = 0;
	 s.t_landing.code = 22; 
     s.t_landing.achievable = 1;

	 s.rotoz_s.act = 1;
	 s.rotoz_s.code = 170; 
     s.rotoz_s.achievable = 1;

	 s.rotoz_b.act = 0;
	 s.rotoz_b.code = 112; 
     s.rotoz_b.achievable = 1;

	 s.replanning.act = 1;
	 s.replanning.code = 20; 
     s.replanning.achievable = 1;

	 s.detection.act = 1;
	 s.detection.code = 121; 
     s.detection.achievable = 1;

	 s.tracking.act = 1;
	 s.tracking.code = 131; 
     s.tracking.achievable = 1;

	return s;
}
//*************** step()
struct Step_out doStep (struct Step_in e){  
    struct Step_out s; 
     cout << "[TRAITEMENT][ENTREES][AUTOMATE]...."<<endl;
     cout << "..................................."<<endl;
     cout<< "[PRISE DES DECISIONS PAR AUTOMATE......]"<< endl;
     cout << "[SORTIE][AUTOMATE]..." <<endl;
   s= sortie();
   return s;
    }

//***************
void achievable_tab(struct Step_out s){
	 int i;
	vector< vector<string> > M(11); 
   for(i=0; i < 11; ++i) {
     M[i] = vector<string>(2);
   }
   M[0][0] = "contrast_img";
   M[1][0] = "motion_estim_imu";
   M[2][0] = "motion_estim_img";
   M[3][0] = "search_landing";
   M[4][0] = "obstacle_avoidance";
   M[5][0] = "t_landing";
   M[6][0] = "rotoz_s";
   M[7][0] = "rotoz_b";
   M[8][0] = "replanning";
   M[9][0] = "detection";
   M[10][0] = "tracking";

   M[0][1] = to_string(s.contrast_img.achievable);
   M[1][1] = to_string(s.motion_estim_imu.achievable);
   M[2][1] = to_string(s.motion_estim_img.achievable);
   M[3][1] = to_string(s.search_landing.achievable);
   M[4][1] = to_string(s.obstacle_avoidance.achievable);
   M[5][1] = to_string(s.t_landing.achievable);
   M[6][1] = to_string(s.rotoz_s.achievable);
   M[7][1] = to_string(s.rotoz_b.achievable);
   M[8][1] = to_string(s.replanning.achievable);
   M[9][1] = to_string(s.detection.achievable);
   M[10][1] = to_string(s.tracking.achievable);

	ofstream file("./src/parameters/achievable_tab.txt", ios::out); 
   if ( file ) 
    {       
		for (int i = 0; i < 11; ++i){
            if(M[i][1]=="0"){
              file <<M[i][0]<<endl;
			  file <<M[i][1]<<endl; 
			}
	    }
         file.close();  
    }  else{
                cerr << "Impossible d'ouvrir le fichier !" << endl;
                }

}
//******************BOUDABZA Ghizlane; verification de la realisation de toutes les APPs..
int verify(struct Step_out s){
    int a =1; 
   a = a & (s.contrast_img.achievable) & (s.motion_estim_imu.achievable)& (s.motion_estim_img.achievable)& (s.search_landing.achievable)& (s.obstacle_avoidance.achievable)& (s.t_landing.achievable)& (s.rotoz_s.achievable)& (s.rotoz_b.achievable)& (s.replanning.achievable)& (s.detection.achievable)& (s.tracking.achievable);
			  return a;
   }
  //********************* Publier l'alerte à Mission Manager
void publish(int a,struct Step_out s)
    {   ros::Rate loop_rate(0.5); 
	std_msgs::Int32 msg1;
	//remplissage de liste des taches nn realisables
   achievable_tab(s);
   if(a==1){
         ROS_INFO("[CHARGEMENT][BITSTREAM][SCHEDULING] , Achievable : [%d]", a);
		  }
		  else{
        ROS_ERROR("[ALTERTE][MISSION_MANAGER]");
         msg1.data =1;      
         achievable_pub->publish(msg1);   
        loop_rate.sleep();   	
		ROS_ERROR("LISTE DES TACHES NN REALISABLES ENVOYEE, Total_Achievable : [%d]", a);
		ROS_INFO("[CHARGEMENT][BITSTREAM][DERNIERE_SORTIE_AUTOMATE]");
		  }
     }
//********************BOUDABZA Ghizlane; la fonction qui permet de creer la table de sortie de l'automate..EXEMPLE: S_L ET DETECTION
vector < vector<string> > initilise_sortie(int n, int m, struct Step_out s){
   int i;
   	vector< vector<string> > M(n); 
   for(i=0; i < n; ++i) {
     M[i] = vector<string>(m);
   }
   M[0][0] = "                  ";
   M[0][1] = "ctr_img";
   M[0][2] = "m_e_imu";
   M[0][3] = "m_e_img";
   M[0][4] = "s_land";
   M[0][5] = "rotoz_b";
   M[0][6] = "tracking";
   M[0][7] = "rotoz_s";
   M[0][8] = "detection";
   M[0][9] = "obt_avoid";
   M[0][10]= "t_landing";
   M[0][11]= "replanning";
  
   M[1][0] = "active";
   M[2][0] = "version_code";
   M[3][0] = "region_id";
   M[4][0] = "fusion_sequence";
    i=0;
	cout << M[i][0] << " ";
    cout << M[i][1] << "   ";
	cout << M[i][2] << "   ";
	cout << M[i][3] << "   ";
	cout << M[i][4] << "   ";
	cout << M[i][5] << " ";
	cout << M[i][6] << " ";
	cout << M[i][7] << " ";
	cout << M[i][8] << " ";
	cout << M[i][9] << " ";
    cout << M[i][10] << " ";
	cout << M[i][11] << " ";
	cout << endl;
	i=1;                   
	cout << M[i][0] << "           ";
	   M[i][1]=to_string(s.contrast_img.act);
	   M[i][2]=to_string(s.motion_estim_imu.act);
	   M[i][3]=to_string(s.motion_estim_img.act);
	   M[i][4]=to_string(s.search_landing.act);
	   M[i][5]=to_string(s.rotoz_b.act);
	   M[i][6]=to_string(s.tracking.act);
	   M[i][7]=to_string(s.rotoz_s.act);
	   M[i][8]=to_string(s.detection.act);
	   M[i][9]=to_string(s.obstacle_avoidance.act);
	   M[i][10]=to_string(s.t_landing.act);
	   M[i][11]=to_string(s.replanning.act);
	   for(int j(1); j < m; ++j) { 
	  cout << "  "<<stoi(M[i][j]) << "       ";
          } 
	cout << endl; 
    i=2; 
	cout << M[i][0] << "      ";
	   M[i][1]=to_string(s.contrast_img.code);
	   M[i][2]=to_string(s.motion_estim_imu.code);
	   M[i][3]=to_string(s.motion_estim_img.code);
	   M[i][4]=to_string(s.search_landing.code);
	   M[i][5]=to_string(s.rotoz_b.code);
	   M[i][6]=to_string(s.tracking.code);
	   M[i][7]=to_string(s.rotoz_s.code);
	   M[i][8]=to_string(s.detection.code);
	   M[i][9]=to_string(s.obstacle_avoidance.code);
	   M[i][10]=to_string(s.t_landing.code);
	   M[i][11]=to_string(s.replanning.code);
	
	   for(int j(1); j < m; ++j) { 
	  cout << "  "<<stoi(M[i][j]) << "      ";
          } 
	cout << endl; 
	i=3; 
	cout << M[i][0] << "       ";
	   M[i][1]=to_string((s.contrast_img.code)%10);
	   M[i][2]=to_string((s.motion_estim_imu.code)%10);
	   M[i][3]=to_string((s.motion_estim_img.code)%10);
	   M[i][4]=to_string((s.search_landing.code)%10);
	   M[i][5]=to_string((s.rotoz_b.code)%10);
	   M[i][6]=to_string((s.tracking.code)%10);
	   M[i][7]=to_string((s.rotoz_s.code)%10);
	   M[i][8]=to_string((s.detection.code)%10);
	   M[i][9]=to_string((s.obstacle_avoidance.code)%10);
	   M[i][10]=to_string((s.t_landing.code)%10);
	   M[i][11]=to_string((s.replanning.code)%10);
	   for(int j(1); j < m; ++j) { 
	  cout << "  "<<stoi(M[i][j]) << "        ";
          } 
    cout << endl; 
	i=4;  vector<string> test; int l,j,p,t=1;
	cout << M[i][0] << "   ";
	   for(j=1;j < m;j++){
		   //*********TEST pour eviter la redondance de traitement de taches
		for(p=0;p<test.size();p++){
		   if(M[0][j]!=test[p]) {
	        t=1;
		     }
		   else{
			t=0;
			break;
		       }
		}
		   if(t==1){
		   if((M[1][j] != "0")&&(M[3][j] != "0")){ // active : 1 (!="0")
			  if(stoi(M[2][j]) < stoi("140") ){ // || M[2][j] < "99"
			   for(l=j+1 ;l<m+1;l++){
            if((M[3][j] == M[3][l])&&(M[1][l] != "0")&& (stoi(M[2][l]) < stoi("140"))){  //meme region_id
			    M[4][j]="s";
				M[4][l]="s";
				//test.push_back(M[0][l]);
			} 
			}
		    }
			 if(stoi(M[2][j]) >= stoi("140")){
             M[4][j] = "f";
		                        }
			 else{
				  if(M[4][j] != "s" || M[4][j] == "" ){
					  M[4][j]="-";
				  }
			     } 
	        }
	   else{
		    M[4][j]="-";
	   }
	   }
	   }
	   for(int j=1; j<m; j++) { 
	  cout << M[i][j] << "          ";
          }
	cout << endl; 
	  return M;       
}
//************************ Creation des tables de scheduling
vector < vector<string> > scheduler_tab(int n, int m){ 
   int i;
   	vector< vector<string> > A(n); 
   for(i=0; i < n; ++i) {
     A[i] = vector<string>(m);
   }
   i=0;
  A[i][0] = "func";
  A[i][1] = "Bitstream@";
  A[i][2] = "version_code";
  A[i][3] = "sequence";
  A[i][4] = "loaded";
  A[i][5] = "region_id";
  A[i][6] = "Done";
   return A;
}
//************** pour verifier la derniere tache excecutée dans les tuiles pour ne pas recharger le meme bts ou charger un bts au meme moment qu'une autre tache est entrain de s'excecuter
string verify1[3][7]={{"0","0","0"},{"1","2","3"}};
void v(string t[][7],int i){
	if(t[i][5]=="1"){
	verify1[0][0] = t[i][0];}
	else if(t[i][5]=="2"){
	verify1[1][0] = t[i][0];}
	else if(t[i][5]=="3"){
	verify1[2][0] = t[i][0];}
}
//******************fonction d'activation des taches en sw ou hw: en (-), ou en (s,f,-)
void activate_desactivate_task(string t[][7],int i,std_msgs::Int32 msg){

	if(t[i][0]=="ctr_img"){
		  contrast_img_pub->publish(msg);
		}
	else if(t[i][0]=="m_e_imu"){
		motion_estim_imu_pub->publish(msg);
		}

	else if(t[i][0]=="m_e_img"){ 
		motion_estim_img_pub->publish(msg);
		}

	else if(t[i][0]==" s_land"){
		search_land_pub->publish(msg);
		}

	else if(t[i][0]=="rotoz_b"){ 
		rotoz_b_pub->publish(msg);
		}

	else if(t[i][0]=="tracking"){  
		tracking_pub->publish(msg);
		
	}
    else if(t[i][0]==" rotoz_s"){
		rotoz_s_pub->publish(msg);
		
	}
	else if(t[i][0]=="detection"){ 
		detection_pub->publish(msg);
		
	}
	else if(t[i][0]=="obt_avoid"){ 
		obstacle_avoidance_pub->publish(msg);
		
	}
    else if(t[i][0]=="t_landing"){ 
		t_landing_pub->publish(msg);
	
	}

	else if(t[i][0]=="replanning"){ 
		replanning_pub->publish(msg);
		}

	else if(t[i][0]=="ctr_img&m_e_imu"){ 
		ctr_img_m_e_imu_pub->publish(msg);
		}

    else if(t[i][0]=="ctr_img&m_e_img"){ 
		ctr_img_m_e_img_pub->publish(msg);
		
	}
	else if(t[i][0]=="ctr_img&s_land"){ 
		ctr_img_s_land_pub->publish(msg);
		                  }                           

	else if(t[i][0]=="ctr_img&rotoz_s"){ 
		ctr_img_rotoz_s_pub->publish(msg);
		}

	else if(t[i][0]=="m_e_img&t_landing"){ 
		m_e_img_t_landing_pub->publish(msg);
		}

	else if(t[i][0]=="m_e_img&rotoz_s"){  
		m_e_img_rotoz_s_pub->publish(msg);
		}

	else if(t[i][0]=="m_e_img&s_land"){  
		m_e_img_s_land_pub->publish(msg);
		}

	else if(t[i][0]=="rotoz_s&t_landing"){ 
		rotoz_s_t_landing_pub->publish(msg);
		}

    else if(t[i][0]==" t_landing&s_land"){
		t_landing_s_land_pub->publish(msg);
		}

	else if(t[i][0]=="rotoz_s&s_land"){  
		rotoz_s_s_land_pub->publish(msg);
			}
}
//********************mapping :Remplissage des tableaux de scheduling
void mapping(vector< vector<string> > M){
      int nb=6; int t1=0, t2=0;
   string static_tab[nb][7]; string as[4][7]; string as2[4][7]; string as3[4][7]; std_msgs::Int32 msg;
   vector <string> map =readfile1("./src/parameters/map_tab.txt");
 
    /*string** static_tab = (string**)malloc(7*sizeof(string*)); //allocation des colonnes
	for(i=0;i<9;i++)
		static_tab[i] = (string*) malloc(9*sizeof(string)); //allocation du nombre de cases par colonnes*/
	int i=0;
  static_tab[i][0] = "func";
  static_tab[i][1] = "Bitstream@";
  static_tab[i][2] = "version_code";
  static_tab[i][3] = "sequence";
  static_tab[i][4] = "loaded";
  static_tab[i][5] = "region_id";
  static_tab[i][6] = "Done";
  as[i][0] = "func";
  as[i][1] = "Bitstream@";
  as[i][2] = "version_code";
  as[i][3] = "sequence";
  as[i][4] = "loaded";
  as[i][5] = "region_id";
  as[i][6] = "Done";
  as2[i][0] = "func2";
  as2[i][1] = "Bitstream@";
  as2[i][2] = "version_code";
  as2[i][3] = "sequence";
  as2[i][4] = "loaded";
  as2[i][5] = "region_id";
  as2[i][6] = "Done";
  as3[i][0] = "func3";
  as3[i][1] = "Bitstream@";
  as3[i][2] = "version_code";
  as3[i][3] = "sequence";
  as3[i][4] = "loaded";
  as3[i][5] = "region_id";
  as3[i][6] = "Done";
	  int j,k,l,m,m1,d,b,b1=0,indice=1,p,t=1; int l2,l3;
	  string n,n1,n2,c1,push,push2,push3; 
	  vector<string> test; vector < vector<string> > a,af;
     //std::vector<std::vector<string>> static_tab=scheduler_tab(9,6);
	for( j=1;j<12;j++){
		 //*********TEST pour eviter la redondance des traitement des taches
		       for(p=0;p<test.size();p++){
		          if(M[0][j]!=test[p]) {
	              t=1;
		            }
		          else{
			          t=0;
			          break;
		             }
		        }
				
    if((t==1)&&(M[1][j] != "0")){ 
	if(M[4][j]=="-"){     
                a= scheduler_tab(2,7); 
                a[1][0]=M[0][j];
				a[1][2]=M[2][j];
				a[1][3]=M[4][j]; 
				a[1][4]="0";   // a remplir
				a[1][6]="0";  //a remplir
				a[1][5]=M[3][j]; 
				for(k=0;k<map.size();k++){
                     if(a[1][2]==map[k]){
                        a[1][1]=map[k+1];
					 }
				}
	                 std::vector<string> temp;
	        	for(int i=1 ;i < 2; ++i) { 
	            	for(int j(0); j < 7; ++j) { 
		     	temp.push_back(a[i][j]);
		         }
		          	}
		        //remplissae de static_tab... 
		          if(indice==j){
		      	for(b1=0,b=0; b1 < 7, b<temp.size(); ++b1,++b) {
		         static_tab[indice][b1]=temp[b];
		          }
		         indice++;
	          	}
		         else{
		        for(b1=0,b=0; b1 < 7, b<temp.size(); ++b1,++b) {
	          	static_tab[indice][b1]=temp[b];
		                             }
									  indice++;
									  }
		}                    
	else if(M[4][j]=="f"){  
			    af= scheduler_tab(2,7);
				af[1][2]=M[2][j];
				af[1][3]=M[4][j];
				af[1][4]="0";  // a remplir
				af[1][6]="0";  //a remplir
				af[1][5]=M[3][j];
                 n1= M[0][j];
				for(l=j+1;l<M[i].size();l++){ 
					if((M[3][l]==M[3][j]) && (M[4][j]==M[4][l])){  //meme tuile , meme char f
						 n2 = M[0][l];
						 test.push_back(n2);
						 }
				}
				 n= n1+'&'+n2;
                af[1][0]= n;
                for(k=0;k<map.size();k++){
                     if(af[1][2]==map[k]){
                         af[1][1]=map[k+1];
		        }
				}		
	 	        for(int i=1 ;i < 2; ++i) { 
		        	std::vector<string> temp;
		        for(int j(0); j < 7; ++j) { 
			    temp.push_back(af[i][j]);
	             	}
		        //remplissae de static_tab... 
		        if(indice==j){
		     	for(b1=0,b=0; b1 < 7, b<temp.size(); ++b1,++b) {
		        static_tab[indice][b1]=temp[b];
		         }
	          	indice++;
		         }
		         else{
		        for(b1=0,b=0; b1 < 7, b<temp.size(); ++b1,++b) {
		       static_tab[indice][b1]=temp[b];
		                             }
									 	 indice ++;
										 }
		            } 
		} 
	else {
		if(M[4][j]=="s"){ 
			    //cas detect et track en seq : 2 bts dans meme tuile ; notion de "Done"
                as[1][0]=M[0][j];
				as[1][2]=M[2][j];
				as[1][3]=M[4][j];
				as[1][4]="0";  // a remplir
				as[1][6]="0";  //a remplir
				as[1][5]=M[3][j];
				for(k=0;k<map.size();k++){
                    if(as[1][2]==map[k]){
                         as[1][1]=map[k+1];
		        }
				}
				 d=M[i].size(); int indice=j;
				int l=2;
				for(m1=indice+1;m1<(d+1);m1++){ 
					if((M[3][m1]==M[3][j]) && (M[4][j]==M[4][m1])){  //meme tuile, meme char s
				      c1= M[0][m1];
					  as[l][0]=c1;
					  as[l][2]= M[2][m1];
				      as[l][3]= M[4][j];
				      as[l][4]="0";  // a remplir
				      as[l][6]="0";  //a remplir
				      as[l][5]=M[3][j];
					  indice=m1;
					 for(k=0;k<map.size();k++){
                     if(as[l][2]==map[k]){
                         as[l][1]=map[k+1];
						 cout<<as[l][2]<<endl;
					    }
				        }
				    push = as[l][0];
	                test.push_back(push);
					l++;
					}
					//test supp
                  else if((M[3][m1]!=M[3][j]) && (M[4][j]==M[4][m1])){  //#tuile, meme char s
                      l2=1;
					  if(as2[l2][0]==""){
					  string c2= M[0][m1]; 
					  as2[l2][0]=c2;
					  as2[l2][2]= M[2][m1];
				      as2[l2][3]= M[4][j];
				      as2[l2][4]="0";  // a remplir
				      as2[l2][6]="0";  //a remplir
				      as2[l2][5]=M[3][m1];
					  indice=m1;
                     for(k=0;k<map.size();k++){
                     if(as2[l2][2]==map[k]){
                         as2[l2][1]=map[k+1];
					    }
				        }
					push2 = as2[l2][0];
	                test.push_back(push2);
				     }
					 else {
						 if(as2[1][5]==M[3][m1] && as2[1][3]==M[4][m1]){
							 l2=2;
                      string c2= M[0][m1]; 
					  as2[l2][0]=c2;
					  as2[l2][2]= M[2][m1];
				      as2[l2][3]= M[4][j];
				      as2[l2][4]="0";  // a remplir
				      as2[l2][6]="0";  //a remplir
				      as2[l2][5]=M[3][m1];
					  indice=m1;
                     for(k=0;k<map.size();k++){
                     if(as2[l2][2]==map[k]){
                         as2[l2][1]=map[k+1];
					    }
				        }
					  push2 = as2[l2][0];
	                  test.push_back(push2);
					  l2++; 
					    }
						//test inf
					   else{
						 l3=1;
					  if(as3[l3][0]==""){
					  string c3= M[0][m1]; 
					  as3[l3][0]=c3;
					  as3[l3][2]= M[2][m1];
				      as3[l3][3]= M[4][j];
				      as3[l3][4]="0";  // a remplir
				      as3[l3][6]="0";  //a remplir
				      as3[l3][5]=M[3][m1];
					  indice=m1;
                    for(int k=0;k<map.size();k++){
                     if(as3[l3][2]==map[k]){
                        as3[l3][1]=map[k+1];
					    }
				    	}
					push3 = as3[l3][0];
	                test.push_back(push3);
				     } 
					else{
						 l3=2;
						string c3= M[0][m1]; 
					  as3[l3][0]=c3;
					  as3[l3][2]= M[2][m1];
				      as3[l3][3]= M[4][j];
				      as3[l3][4]="0";  // a remplir
				      as3[l3][6]="0";  //a remplir
				      as3[l3][5]=M[3][m1];
					  indice=m1;
                     for(int k=0;k<map.size();k++){
                     if(as3[l3][2]==map[k]){
                         as3[l3][1]=map[k+1];
					    }
				        }
					  push3 = as3[l3][0];
	                  test.push_back(push3);
					  l3++; 
					 }
					 }
					 //test inf
					 }
				     }
                  //test supp
				}     
				for(int i(0);i < 4; ++i) { 
	            for(int j(0); j < 7; ++j) { 
                cout <<as[i][j] << "        "; 
                 } 
                 cout << endl; 
                  } 
				  if(as2[1][0]!=""){
				  for(int i(0);i < 4; ++i) { 
	            for(int j(0); j < 7; ++j) { 
                cout <<as2[i][j] << "        "; 
                 } 
                 cout << endl; 
                  } }
				  if(as3[1][0]!=""){
				  for(int i(0);i < 4; ++i) { 
	            for(int j(0); j < 7; ++j) { 
                cout <<as3[i][j] << "        "; 
                 } 
                 cout << endl; 
                  } }
	   }
    }		  
	//**********
    }
	} 
    ROS_ERROR("STATIC_TAB_AFFICHAGE...");
		  for(int i(0);i < nb; ++i) { 
	for(int j(0); j < 6; ++j) { 
    cout << static_tab[i][j] << "       "; 
     } 
     cout << endl; 
     }   

  //chargement des Bitstreams en hw (f,s,-)
	    int fd_mem, ret = EXIT_FAILURE;
        /*fd_mem = open("/dev/mem", O_RDWR|O_SYNC);
        if (fd_mem < 0) 
        {
            perror("Failed to open /dev/mem");
            exit(EXIT_FAILURE);
        }
		*/
	    //charger bts de static table statique :en hw (f,-)
        for(int i=1;i< nb;i++){ 
            if(static_tab[i][5]=="1" || static_tab[i][5]=="2" || static_tab[i][5]=="3")
			{  //test pour eviter de recharger le meme bts ou charger un nv bts alors que la tuile contient une tache en cours d'exec.
				for(int i1=0;i1<3;i1++)
				{
					if(static_tab[i][0]==verify1[i1][0] && static_tab[i][5]==verify1[i1][1])
					{  //dernier etat (Bts) dans les 3 regions reconfigurables(nom,region_id)
						t1=1;   //bts est deja configuré
						break;
					}
					else
					{
						t1=0;
						msg.data=0;       
						activate_desactivate_task(verify1,i1,msg); //desactiver la tache en cours d'exec dans la tuile demandé pour la nv config
					}
				}
				if(t1==0){
					msg.data=2;
					int bitstream_address =std::stoi(static_tab[i][1]); 
					int loaded=0;
					int region_id =std::stoi(static_tab[i][5]); 
					/* EM 24/07/2018, comment code calling bitstreams
					start_reconfiguration(fd_mem, bitstream_address, region_id);
					do{
					loaded = reconfiguration_done(fd_mem);
					}while(loaded==1);
					*/
					static_tab[i][4]=loaded;
					v(static_tab,i); //sauvegarder le dernier etat configuré dans la region reconfigurable (non_tache, region_id)
					activate_desactivate_task(static_tab,i,msg);  //activer la tache
				} 
			}	
	    }
		vector<int> B; 
		b=1;
        //charger bts de la 1er tache en s et l'activer,de meme pour taches suivantes apres Done=1 de la tache precedante
		//TODO : normaliser les seuils du for
		for(int i=1,i3=1,i4=1;i< 4 ,i3< 4,i4< 4;i++,i3++,i4++){
		if(as[i][0]!=""||as2[i3][0]!=""||as3[i4][0]!=""){ 
            for(int i2=0;i2<3;i2++){
		        if(as[i][0]==verify1[i2][0] && as[i][5]==verify1[i2][1] || as2[i3][0]==verify1[i2][0] && as2[i3][5]==verify1[i2][1] ||as3[i4][0]==verify1[i2][0] && as3[i4][5]==verify1[i2][1]){  //dernier etat (Bts) dans les 3 regions reconfigurables
	            t2=1; 
				B.push_back(t2);  //bts est deja configuré
				break;
		        }
		        else{
			    t2=0;
				B.push_back(t2); 
				msg.data=0;       
				activate_desactivate_task(verify1,i2,msg); //desactiver la tache en cours d'exec dans la tuile demandé pour la nv config au cas d'une nouvelle seq s
		        }
		    }
			for(int i=0;i<B.size();i++){
				 b=b & B[i];
			}
			if(b==0){ 
				//test as
				if(as[i][0]!="" && as[i][4]!="1")
				{
					msg.data=2;                       //si on a une exec en s dans notre sequence produite par M_M
					int bitstream_address =stoi(as[i][1]); int loaded=0;
					int region_id =stoi(as[i][5]);
					/* EM 24/07/2018, comment code calling bitstreams
					start_reconfiguration(fd_mem, bitstream_address, region_id);
					do{ 
					loaded = reconfiguration_done(fd_mem);
					}while(loaded==1);
					*/
					as[i][4]=loaded;
					v(as,i);
					activate_desactivate_task(as,i,msg); //activer la tache
				} 
				//fin test as

				//test as2
				if(as2[i3][0]!="" && as2[i3][4]!="1")
				{
					msg.data=2;      //si on a une exec en s dans notre sequence produite par M_M
					int bitstream_address =stoi(as2[i3][1]); int loaded=0;
					int region_id =stoi(as2[i3][5]);
					/* EM 24/07/2018, comment code calling bitstreams
					start_reconfiguration(fd_mem, bitstream_address, region_id);
					do{ 
					loaded = reconfiguration_done(fd_mem);
					}while(loaded==1);
					*/
					as2[i3][4]=loaded;
					v(as2,i3);
					activate_desactivate_task(as2,i3,msg); //activer la tache
				} 
			 	//fin test as2

				//test as3
				if(as3[i4][0]!="" && as3[i4][4]!="1")
				{
					msg.data=2;                       //si on a une exec en s dans notre sequence produite par M_M
					int bitstream_address =stoi(as3[i4][1]); int loaded=0;
					int region_id =stoi(as3[i4][5]);
					/* EM 24/07/2018, comment code calling bitstreams
					start_reconfiguration(fd_mem, bitstream_address, region_id);
					do{ 
					loaded = reconfiguration_done(fd_mem);
					}while(loaded==1);
					*/
					as3[i4][4]=loaded;
					v(as3,i4);
					activate_desactivate_task(as3,i4,msg); //activer la tache
				} 
				//fin test as3

		    //attente des Done des taches actives en s 
			i=1; int i3=1,i4=1; bool done1 = false; bool done2= false; bool done3= false; 
		   if(as[i][0]!=""){
				do{ 
					const char* path ="./src/parameters/done.txt";
					vector<string> done = readfile1(path);
					for(int l=0;l<done.size();l++){
						if(done[l]==as[i][0]){
							as[i][6]=done[l+1];
							cout<<"as"<<endl;
							cout<<as[i][6]<<endl;
						}
					}
					if(as[i][6]=="1")
						done1= true;
				}while(!done1); 
			}
			if(as2[i3][0]!=""){
				do{ 
					const char* path ="./src/parameters/done.txt";
					vector<string> done = readfile1(path);
					for(int l=0;l<done.size();l++){
						if(done[l]==as2[i3][0]){
							as2[i3][6]=done[l+1];
							cout<<"as2"<<endl;
							cout<<as2[i3][6]<<endl;
						}
					}
					if(as2[i3][6]=="1")
						done2= true;
				}while(!done2); 
			}
			if(as3[i4][0]!=""){
				do{ 
					const char* path ="./src/parameters/done.txt";
					vector<string> done = readfile1(path);
					for(int l=0;l<done.size();l++){
						if(done[l]==as3[i4][0]){
							as3[i4][6]=done[l+1];
							cout<<"as"<<endl;
							cout<<as3[i4][6]<<endl;
						}
					}
					if(as3[i4][6]=="1")
						done3= true;
				}while(!done3); 
			}
		    }
		}
		}
    //activer les taches en sw 
		for(int i=1;i< nb;i++){
			if(static_tab[i][5]=="0"){
				msg.data=1;
		    activate_desactivate_task(static_tab,i,msg);  //activer en sw
	       }
		}
    //affichage des taches en s
	if(as[1][0]!=""){
	for(int i(0);i < 4; ++i) { 
	for(int j(0); j < 7; ++j) { 
        cout <<as[i][j] << "        "; 
    } 
     cout << endl; 
    }}
	if(as2[1][0]!=""){
		for(int i(0);i < 4; ++i) { 
	    for(int j(0); j < 7; ++j) { 
        cout <<as2[i][j] << "        "; 
          } 
         cout << endl; 
          } }
	 if(as3[1][0]!=""){
		for(int i(0);i < 4; ++i) { 
	    for(int j(0); j < 7; ++j) { 
         cout <<as3[i][j] << "        "; 
          } 
          cout << endl; 
        } }
    ROS_ERROR("STATIC_TAB_COMPLETE..");
	 for(int i(0);i < nb; ++i) { 
	for(int j(0); j < 6; ++j) { 
    cout << static_tab[i][j] << "       "; 
     } 
     cout << endl; 
     }   
}

//*****************
void do1(struct Step_in e){
	
    vector < vector<string> > M;
      struct Step_out s = doStep (e);
          // struct Step_out s;
	      // doStep ( &e, &s);   //l'automate de config auto-adaptatif
		   int a = verify(s); //VERIFIER la variable Achievable des taches
		   publish(a,s);       //alerter le niveau mission si il exuste des taches nn réalisables
		   M= initilise_sortie(5, 12, s); //conversion sortie step -> table 
           mapping(M); //chargement des bITSTREAMS DANS LA CARTE 
    }
	 
	//**************************BOUDABZA Ghizlane; la fonction qui verifie est ce que texe appartient à l'intervalle [Tmin, Tmax]...pour chaque fonction;
void comparer(vector<string> lignes, vector<string> C3,struct Step_in e) 
{ 
	 vector<int> T; int t=0; int a=1;
  for(int j = 0; j < C3.size(); j++){
  for(int m=0;m<lignes.size();m=m+3){
    if(C3[j]==lignes[m]){
    if((lignes[m+1] >= C3[j+3] && lignes[m+1] <= C3[j+4]) && (lignes[m+2] > C3[j+6] && lignes[m+2] <= C3[j+7])){
	   t=1;
	   T.push_back(t);
    }
	else{
		t=0;
		T.push_back(t);
	}
  }
  }
  }
 for(int i=0;i<T.size();i++){
	 a= a & T[i];
	 cout<<T[i]<<" ";
 }
 cout<<endl;
 if(a==1){
 cout<<"[Texe,qos sont bien dans l'intervalle, on attend la prochaine lecture du texe_file]"<<endl;
	}
	else{
		  cout<<"[INTERVALLE TEMPS,QOS NON RESPECTEE ][RECHERCHE SOLUTION]..[APPEL A MY_STEP]"<<endl;
		    do1(e);
	}
}

//***************************
void notify_Callback(const std_msgs::Int32::ConstPtr& msg){
  ROS_ERROR("[RECU][MISSION_M][CHANGEMENT D'INTERVALLES DANS TABLE C3..] \n");
  ROS_ERROR("%d", msg->data);
  float texe; vector<string> C3;
  C3 = readfile1("./src/parameters/tableC3(2).txt");
  struct Step_in e= entree1(C3); 
   do1(e);
}

//**************
double cpuload ( ) {
        FILE *fp;
        char buffer[100];
        fp = popen("top -b -d 1 -n5 | grep Cpu | awk '{printf \"%.2f:\", $8}'","r");
        fscanf(fp,"%s", buffer);
		auto closureReturn = pclose(fp);
		cout << "Processus properly killed ? " << closureReturn << endl;

        int len, i, j = 0, nb = 0 ;
        double value, sum = 0;
        char stock [7];

    len = strlen (buffer);
        for (i = 0; i < len ; i++){
                if (buffer [i] == '\0'|| buffer [i] == '\n')
                        break;
                else {
                        if (buffer [i] == ':'){
                                //printf ("%s\n", stock); 
								j = 0;
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



/************************************************************
*	EM 24/07/2018, MAIN
*	Adaptation manager main code
************************************************************/
int main (int argc, char ** argv)
{   
	
	ros::init(argc, argv, "adaptation_manager_node");
	ros::NodeHandle nh;
	
	
	//reset ();
	//beginning = clock();
	gettimeofday (&beginning , NULL);
	
//search_land_pub;
	search_land_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/search_landing_area_mgt_topic", 1));
 //contrast_img_pub;
	contrast_img_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/contrast_img_area_mgt_topic", 1));
 //motion_estim_imu_pub;
	motion_estim_imu_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/motion_estim_imu_area_mgt_topic", 1));
 //motion_estim_img_pub;
	motion_estim_img_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/motion_estim_img_area_mgt_topic", 1));
 //obstacle_avoidance_pub;
	obstacle_avoidance_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/obstacle_avoidance_area_mgt_topic", 1));
 //t_landing_pub;
	t_landing_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/t_landing_area_mgt_topic", 1));
 //rotoz_s_pub;
	rotoz_s_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/rotoz_s_area_mgt_topic", 1));
 //rotoz_b_pub;
	rotoz_b_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/rotoz_b_area_mgt_topic", 1));
 //detection_pub;
	detection_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/detection_area_mgt_topic", 1));
 //tracking_pub;
	tracking_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/tracking_area_mgt_topic", 1));
 //replanning_pub;
	replanning_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/replanning_area_mgt_topic", 1));   
 //Pour les noeuds ros de fusion					 
 //ctr_img&m_e_imu_pub;
 ctr_img_m_e_imu_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/ctr_img_m_e_img_area_mgt_topic", 1));
 //ctr_img&m_e_img_pub;
 ctr_img_m_e_img_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/ctr_img_m_e_img_area_mgt_topic", 1));
 //ctr_img&s_land_pub;
 ctr_img_s_land_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/ctr_img_s_land_area_mgt_topic", 1));
 //ctr_img&rotoz_s_pub;
 ctr_img_rotoz_s_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/ctr_img_rotoz_s_area_mgt_topic", 1));
 //m_e_img&t_landing_pub;
 m_e_img_t_landing_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/m_e_img_t_landing_area_mgt_topic", 1));
 //m_e_img&rotoz_s_pub;
 m_e_img_rotoz_s_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/m_e_img_rotoz_s_area_mgt_topic", 1));
 //m_e_img&s_land_pub;
 m_e_img_s_land_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/m_e_img_s_land_area_mgt_topic", 1));
 //rotoz_s&t_landing_pub;
 rotoz_s_t_landing_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/rotoz_s_t_landing_area_mgt_topic", 1));
 //t_landing&s_land_pub;
 t_landing_s_land_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/t_landing_s_land_area_mgt_topic", 1));
 //rotoz_s&s_land_pub;
 rotoz_s_s_land_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/rotoz_s_s_land_area_mgt_topic", 1));					 					 					 
//***************************envoie de liste des taches nn realisables
achievable_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/achievable_topic", 1000));

ros::Subscriber notify_from_MM_sub;
	notify_from_MM_sub= nh.subscribe("/notify_from_MM_topic", 1000, notify_Callback);
				  
	
ros::Subscriber mgt_topic;
	mgt_topic = nh.subscribe("/search_landing_notification_topic", 1, 		
				  handle_notification_from_search_landing_wrapper);
				  
ros::Publisher cpu_pub;
	cpu_pub = nh.advertise<std_msgs::Float32>("/cpu_load_topic", 1);				    
//*********************				  
	/*ros::Subscriber mgt_request_topic;
	mgt_request_topic = nh.subscribe("/activate_search_landing_topic", 1, 		
				  handle_search_landing_activation);			  
				  
	ros::Subscriber search_landing_set_max_topic;
	search_landing_set_max_topic = nh.subscribe("/search_landing_set_max_topic", 1, 		
				  handle_search_landing_max_threshold);
				  
	ros::Subscriber search_landing_set_min_topic;
	search_landing_set_min_topic = nh.subscribe("/search_landing_set_min_topic", 1, 		
				  handle_search_landing_min_threshold);*/
				  
	
	//current1 = clock();  
	gettimeofday (&current1 , NULL);
	printf("adaptation_manager_req %.0f 0\n", ((double) time_micros(&current1, &beginning)));	
	printf("adaptation_manager_max %.0f %d\n",((double) time_micros(&current1, &beginning)), max_thres);
	printf("adaptation_manager_min %.0f %d\n",((double) time_micros(&current1, &beginning)),min_thres);
	//printf("adaptation_manager_ver %.0f %d\n",((double) time_micros(&current1, &beginning)), _res.res);
	
//*****************Ghizlane BOUDABZA
//.
const char* path0 ="./src/parameters/map_tab.txt";
const char* path1 ="./src/parameters/time_qos_task.txt";
const char* path3 = "./src/parameters/tableC3.txt";
const char* path4 = "./src/parameters/tableC3(2).txt";

std_msgs::Float32 load;
vector<string> lignes, C3;
struct Step_in e0,e;
struct Step_out s;
struct Step_in* in;


ROS_ERROR("[ADAPTATION MANAGER] [RUNNING] \n");
C3 = readfile1(path4);
e0 = entree0(C3); // texe,qos observé =0; 1ere utilisation du step()

do1(e0); 


 ros::Rate loop_rate(10); //10hz = 100ms, 0.1hz=10s
 while(ros::ok()){ 
	ros::spinOnce();

  load.data = cpuload ( ) ;
   cpu_pub.publish ( load );

  lignes = readfile1(path1); 
  if(lignes.size()!=0){
	  e = entree(C3,lignes);
      comparer(lignes,C3, e);	
  }

	loop_rate.sleep();
	}

}


  



//1 er methode faut changer pid si j'ouvre un nv terminal(ou j'ouvre et je ferme un nv term et je relance de l'ancien term) ; voir top -i : pid nv des taches
	//system("top -b -n1 -p 7072,4043 > ./src/parameters/cpu.txt");
//2 eme methode faut ios:app et ligne1 : adap / ligne2 :m_m
	//system("top -i -b -n15 |grep mission_manager > ./src/parameters/cpu1.txt");//-n3 (1fois)sur terminal
   //system("top -i -b -n15|grep adaptation_mana > ./src/parameters/cpu1.txt");


//************************************************ 
	/*ROS_ERROR("[ADAPTATION MANAGER] [RUNNING] \n");
	ros::Rate rate(1); 
	while(ros::ok())
	{
		ros::spinOnce();			
		if (time_notif == 1 )
		{
			do_step ( );  		
			time_notif = 0;

		} 
	   	rate.sleep();	   	
	}*/
	

/************************************************************
* 
*	EM 24/07/2018, Useless code below
*	Code that seems to not be used anytime in the program
*	TODO : Delete everything below once every useless parts
*		   are identified
*
************************************************************/

// ----------->> FONCTION QUI RECUPERE LA SORTIE STANDARD D'UNE COMMANDE UNIX DANS UNE CHAINE DE CARACTERE <<----------- 
  char *redirection_sortie_standard();
  char buf, sortie1[10000];
char *redirection_sortie(char *commande, char *option)
{
    int tube[2];
    pid_t cpid;
	int i=0;

    if (pipe(tube) == -1) 
	{
        perror("Le pipe n'a pas fonctionné");
        exit(EXIT_FAILURE); // quitte le programme si le pipe n'est pas créé
    }
     
	cpid = fork();

    if (cpid == -1)
	{
        perror("Le fork n'a pas fonctionné");
        exit(EXIT_FAILURE); // quitte le programme si le fork n'as pas fonctionné
    }
     
	if (cpid != 0) /* Le pere est lecteur sur le tube */
	{    
        close(tube[1]);          /* Fermeture du coté écriture non utilisé par le pere */
  		/*cette boucle while permet de lire les caractères présent sur le coté lecture du tube et les stocker dans la chaine sortie */
        while (read(tube[0], &buf, 1) > 0)
		{
	   		sortie1[i]=buf; // rempli la chaine sortie caractère par caractère
    		i++;
    		sortie1[i]='\0'; // indique la fin de la chaine de caractère
       	}
		close(tube[0]); // fermeture du coté lecture du tube une fois la lecture une fois que la fonction read a renvoyé 0 : EOF, la lecture terminée
		return sortie1; // la fonction retourne la chaine qui contient le contenu de la sortie standard
    	exit(EXIT_SUCCESS);
 	} else {                             /* Le fils écrit dans le tube  */
		close(tube[0]);          /* Fermeture du coté lecture non utilisé par le fil */
		dup2(tube[1],1);     /* */
		execlp(commande,commande,option,(char *) NULL);
		close(tube[1]);          /* Le pere lecteur verra un "EOF" ce qui permet la synchronisation */
		wait(NULL);                /* Attendre que le fils se termine */
		exit(EXIT_SUCCESS);
	}
}

