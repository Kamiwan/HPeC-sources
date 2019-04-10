/* --- Generated the 5/6/2018 at 19:47 --- */
/* --- heptagon compiler, version 1.04.00 (compiled tue. may. 8 0:14:56 CET 2018) --- */
/* --- Command line: /home/gueyes/.opam/4.04.0/bin/heptc -target c -hepts -s main -target ctrln main.ept --- */

#ifndef CALL_H
#define CALL_H


#include "main.h"

extern int ncc_heptagon;

typedef struct Task_in Task_in;
struct Task_in
{
    int priority;
    int req; // 1 : activation, 0 : arrêt

    int texec; // texec between [mintexec, maxtexec]
    int mintexec;
    int maxtexec;

    int qos;      //qos between [minqos, maxqos]
    int minqos;
    int maxqos;
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



typedef struct Task_out Task_out;
struct Task_out
{
    int act; // 1 : active, 0: stop or attente ressource
    int size;
    int version; // -1, (SW =>) 0,  (HW/tile =>) 1, 2, 3
    int code;

    int achievable; // 0 (NO), 1 (YES)

    int up_pos;   //  0 (NO), 1 (YES)
    int down_pos; //  0 (NO), 1 (YES)
    int keep_pos; //  0 (NO), 1 (YES)

    int qos_pos;  //  0 (NO), 1 (YES)
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


void doStep ( Step_in* in, Step_out* out);
void reset ();

#endif // CALL_H
