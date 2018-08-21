//TODO Insert header comment

#ifndef UTILS_H
#define UTILS_H

#include "adaptation_manager_node.h"

long elapse_time_u (struct timeval *end, struct timeval *start );
long time_micros (struct timeval *end, struct timeval *start );

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


#endif