/* --- Generated the 5/6/2018 at 19:47 --- */
/* --- heptagon compiler, version 1.04.00 (compiled tue. may. 8 0:14:56 CET 2018) --- */
/* --- Command line: /home/gueyes/.opam/4.04.0/bin/heptc -target c -hepts -s main -target ctrln main.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "call.h"



/**
APP / id_version 
1) Contrast_Img (SW/HW)
2) Motion_Estim_Imu (SW)
3) Motion_Estim _Img (SW/HW)
4) Search_Landing (SW/HW)
5) Obstacle_Avoidance (SW / HW)
6) T_Landing (SW/HW)
7) Rotoz_s(SW/HW)
8) Rotoz_b (HW)
9) Replanning (SW/HW)
10) Detection (SW/HW)
11) Tracking (SW/HW)

14) Contrast_Img&ME_img (HW)
15) Contrast_Img&T_Landing (HW)
16) Contrast_Img&Rotz_s (HW)
17) Contrast_Img&Search_Landing (HW)
18) ME_Img&T_Landing (HW)
19) ME_Img&Rotz_s (HW)
20) ME_Img&Search_Landing (HW)
21) Rotz_s (HW) & T_Landing (HW)
22)T_Landing&Search_Landing (HW)
23) Rotz_s&Search_Landing (HW) 

*)
*/


// 1 1 1 1 1 1 1 1 1 1 1
// 0 0 0 0 0 0 0 0 0 0 0 

// rm -rf */main_c */main_c* */main.log */main.mls */main.epci 


Main__main_mem mem;


int f_1 = 0;
int rp_1 = 0;
int f_2 = 0;
int rp_2 = 0;
int f_3 = 0;
int rp_3 = 0;

void reset (){
    printf("Init Internal Memory \n");
    Main__main_reset(&mem);
}

void doStep ( Step_in* in, Step_out* out)
{


	/***********************************************************
	* BEFORE STEP
	***********************************************************/
  int prio_t1 = (in->contrast_img).priority;
  int prio_t2= (in->motion_estim_imu).priority;
  int prio_t3= (in->motion_estim_img).priority;
  int prio_t4= (in->search_landing).priority;
  int prio_t5= (in->obstacle_avoidance).priority;
  int prio_t6= (in->t_landing).priority;
  int prio_t7= (in->rotoz_s).priority;
  int prio_t8= (in->rotoz_b).priority;
  int prio_t9= (in->replanning).priority;
  int prio_t10= (in->detection).priority;
  int prio_t11= (in->tracking).priority;

  int vtexe1 = 1;
  int vtexe2 = 1;
  int vtexe3 = 1;
  int vtexe4 = 1;
  int vtexe5 = 1;
  int vtexe6 = 1;
  int vtexe7 = 1;
  int vtexe8 = 1;
  int vtexe9 = 1;
  int vtexe10 = 1;
  int vtexe11 = 1;

  int texe1 = (in->contrast_img).texec;
  int texe2 = (in->motion_estim_imu).texec;
  int texe3 = (in->motion_estim_img).texec;
  int texe4 = (in->search_landing).texec;
  int texe5 = (in->obstacle_avoidance).texec;
  int texe6 = (in->t_landing).texec;
  int texe7 = (in->rotoz_s).texec;
  int texe8 = (in->rotoz_b).texec;
  int texe9 = (in->replanning).texec;
  int texe10 = (in->detection).texec;
  int texe11 = (in->tracking).texec;

  int mintexe1  = (in->contrast_img).mintexec;
  int mintexe2  = (in->motion_estim_imu).mintexec;
  int mintexe3  = (in->motion_estim_img).mintexec;
  int mintexe4  = (in->search_landing).mintexec;
  int mintexe5  = (in->obstacle_avoidance).mintexec;
  int mintexe6  = (in->t_landing).mintexec;
  int mintexe7  = (in->rotoz_s).mintexec;
  int mintexe8  = (in->rotoz_b).mintexec;
  int mintexe9  = (in->replanning).mintexec;
  int mintexe10  = (in->detection).mintexec;
  int mintexe11  = (in->tracking).mintexec;


  int maxtexe1  = (in->contrast_img).maxtexec;
  int maxtexe2  = (in->motion_estim_imu).maxtexec;
  int maxtexe3  = (in->motion_estim_img).maxtexec;
  int maxtexe4  = (in->search_landing).maxtexec;
  int maxtexe5  = (in->obstacle_avoidance).maxtexec;
  int maxtexe6  = (in->t_landing).maxtexec;
  int maxtexe7  = (in->rotoz_s).maxtexec;
  int maxtexe8  = (in->rotoz_b).maxtexec;
  int maxtexe9  = (in->replanning).maxtexec;
  int maxtexe10  = (in->detection).maxtexec;
  int maxtexe11  = (in->tracking).maxtexec;

  int c_img_r  = (in->contrast_img).req;
  int c_img_e= !((in->contrast_img).req);
  int me_imu_r= (in->motion_estim_imu).req;
  int me_imu_e= !((in->motion_estim_imu).req);
  int me_img_r= (in->motion_estim_img).req;
  int me_img_e= !((in->motion_estim_img).req);
  int sl_r= (in->search_landing).req;
  int sl_e= !((in->search_landing).req);
  int oa_r= (in->obstacle_avoidance).req;
  int oa_e= !((in->obstacle_avoidance).req);
  int tl_r= (in->t_landing).req;
  int tl_e= !((in->t_landing).req);
  int rs_r= (in->rotoz_s).req;
  int rs_e= !((in->rotoz_s).req);
  int rb_r= (in->rotoz_b).req;
  int rb_e= !((in->rotoz_b).req);
  int rpl_r= (in->replanning).req;
  int rpl_e=!((in->replanning).req);
  int dt_r= (in->detection).req;
  int dt_e= !((in->detection).req);
  int trk_r= (in->tracking).req;
  int trk_e= !((in->tracking).req);


	/***********************************************************
	* CALLING STEP
	***********************************************************/
  Main__main_out _res;


 Main__main_step(/*prio_t1, prio_t2, prio_t3, prio_t4, prio_t5, prio_t6,
                    prio_t7, prio_t8, prio_t9, prio_t10, prio_t11, vtexe1,
                    vtexe2, vtexe3, vtexe4, vtexe5, vtexe6, vtexe7, vtexe8,
                    vtexe9, vtexe10, vtexe11,*/ texe1, texe2, texe3, texe4,
                    texe5, texe6, texe7, texe8, texe9, texe10, texe11,
                    mintexe1, mintexe2, mintexe3, mintexe4, mintexe5,
                    mintexe6, mintexe7, mintexe8, mintexe9, mintexe10,
                    mintexe11, maxtexe1, maxtexe2, maxtexe3, maxtexe4,
                    maxtexe5, maxtexe6, maxtexe7, maxtexe8, maxtexe9,
                    maxtexe10, maxtexe11, c_img_r, c_img_e, me_imu_r,
                    me_imu_e, me_img_r, me_img_e, sl_r, sl_e, oa_r, oa_e,
                    tl_r, tl_e, rs_r, rs_e, rb_r, rb_e, rpl_r, rpl_e, dt_r,
                    dt_e, trk_r, trk_e, f_1, rp_1, f_2, rp_2, f_3, rp_3,
                    &_res, &mem);
	/***********************************************************
	* AFTER STEP
	***********************************************************/

	/* printf("Objective : %d\n", (_res.obj)); */

	switch (_res.c_img_ver) {
		/* case Main__N:	 */
	    	/* 	(out->contrast_img).code = -1; */
		/* 	break; */
		case Main__S:
	    		(out->contrast_img).code = 10;
			break;
		case Main__H1:
	    		(out->contrast_img).code = 11;
			break;
		case Main__H2:
	    		(out->contrast_img).code = 12;
			break;
		case Main__H3 :
	    		(out->contrast_img).code = 13;
			break;
		default :
			break;
	}
	/***********************************************************/
	switch (_res.me_imu_ver) {
		/* case Main__N: */
	    	/* 	(out->motion_estim_imu).code = 0; */
		/* 	break; */
		case Main__S:
	    		(out->motion_estim_imu).code = 20;
			break;
		case Main__H1:
	    		(out->motion_estim_imu).code = 21;
			break;
		case Main__H2:
	    		(out->motion_estim_imu).code = 22;
			break;
		case Main__H3 :
	    		(out->motion_estim_imu).code = 0;
			break;
		default :
			break;
	}
	/***********************************************************/
	switch (_res.me_img_ver) {
		/* case Main__N: */
	    	/* 	(out->motion_estim_img).code = 0; */
		/* 	break; */
		case Main__S:
	    		(out->motion_estim_img).code = 30;
			break;
		case Main__H1:
	    		(out->motion_estim_img).code = 31;
			break;
		case Main__H2:
	    		(out->motion_estim_img).code = 32;
			break;
		case Main__H3 :
	    		(out->motion_estim_img).code = 0;
			break;
		default :
			break;
	}
	/***********************************************************/
	switch (_res.sl_ver) {
		/* case Main__N: */
	    	/* 	(out->search_landing).code = 0; */
		/* 	break; */
		case Main__S:
	    		(out->search_landing).code = 40;
			break;
		case Main__H1:
	    		(out->search_landing).code = 41;
			break;
		case Main__H2:
	    		(out->search_landing).code = 42;
			break;
		case Main__H3 :
	    		(out->search_landing).code = 43;
			break;
		default :
	    		(out->search_landing).code = 0;
			break;
	}
	/***********************************************************/
	switch (_res.oa_ver) {
		/* case Main__N: */
	    	/* 	(out->obstacle_avoidance).code = 0; */
		/* 	break; */
		case Main__S:
	    		(out->obstacle_avoidance).code = 50;
			break;
		case Main__H1:
	    		(out->obstacle_avoidance).code = 51;
			break;
		case Main__H2:
	    		(out->obstacle_avoidance).code = 52;
			break;
		case Main__H3 :
	    		(out->obstacle_avoidance).code = 53;
			break;
		default :
	    		(out->obstacle_avoidance).code = 0;
			break;
	}
	/***********************************************************/
	/***********************************************************/
	switch (_res.tl_ver) {
		/* case Main__N: */
	    	/* 	(out->t_landing).code = 0; */
		/* 	break; */
		case Main__S:
	    		(out->t_landing).code = 60;
			break;
		case Main__H1:
	    		(out->t_landing).code = 61;
			break;
		case Main__H2:
	    		(out->t_landing).code = 62;
			break;
		case Main__H3 :
	    		(out->t_landing).code = 63;
			break;
		default :
	    		(out->t_landing).code = 0;
			break;
	}
	/***********************************************************/
	/***********************************************************/
	switch (_res.rs_ver) {
		/* case Main__N: */
	    	/* 	(out->rotoz_s).code = 0; */
		/* 	break; */
		case Main__S:
	    		(out->rotoz_s).code = 70;
			break;
		case Main__H1:
	    		(out->rotoz_s).code = 71;
			break;
		case Main__H2:
	    		(out->rotoz_s).code = 72;
			break;
		case Main__H3 :
	    		(out->rotoz_s).code = 73;
			break;
		default :
	    		(out->rotoz_s).code = 0;
			break;
	}
	/***********************************************************/
	/***********************************************************/
	switch (_res.rb_ver) {
		/* case Main__N: */
	    	/* 	(out->rotoz_b).code = 0; */
		/* 	break; */
		case Main__S:
	    		(out->rotoz_b).code = 0;
			break;
		case Main__H1:
	    		(out->rotoz_b).code = 81;
			break;
		case Main__H2:
	    		(out->rotoz_b).code = 82;
			break;
		case Main__H3 :
	    		(out->rotoz_b).code = 83;
			break;
		default :
	    		(out->rotoz_b).code = 0;
			break;
	}
	/***********************************************************/
	/***********************************************************/
	switch (_res.rpl_ver) {
		/* case Main__N: */
	   	/* 	(out->replanning).code = 0; */
		/* 	break; */
		case Main__S:
	   		(out->replanning).code = 90;
			break;
		case Main__H1:
	   		(out->replanning).code = 91;
			break;
		case Main__H2:
	   		(out->replanning).code = 92;
			break;
		case Main__H3 :
	   		(out->replanning).code = 93;
			break;
		default :
	   		(out->replanning).code = 0;
			break;
	}
	/***********************************************************/
	/***********************************************************/
	switch (_res.dt_ver) {
		/* case Main__N: */
	    	/* 	(out->detection).code  = 0; */
		/* 	break; */
		case Main__S:
	    		(out->detection).code  = 120;
			break;
		case Main__H1:
	    		(out->detection).code  = 121;
			break;
		case Main__H2:
	    		(out->detection).code  = 122;
			break;
		case Main__H3 :
	    		(out->detection).code  = 0;
			break;
		default :
	    		(out->detection).code  = -1;
			break;
	}
	/***********************************************************/
	/***********************************************************/
	switch (_res.trk_ver) {
		/* case Main__N: */
	    	/* 	(out->tracking).code = 0; */
		/* 	break; */
		case Main__S:
	    		(out->tracking).code = 130;
			break;
		case Main__H1:
	    		(out->tracking).code = 131;
			break;
		case Main__H2:
	    		(out->tracking).code = 132;
			break;
		case Main__H3 :
	    		(out->tracking).code = 0;
			break;
		default :
	    		(out->tracking).code = 0;
			break;
	}

	if(_res.c_img_act && _res.me_img_act){
		if(_res.c_img_ver == Main__S &&  _res.me_img_ver == Main__S){
			(out->contrast_img).code = 140;
		}else if(_res.c_img_ver == Main__H1 &&  _res.me_img_ver == Main__H1){ //H1
			(out->contrast_img).code = 141;
		}else if(_res.c_img_ver == Main__H2 &&  _res.me_img_ver == Main__H2){ //H1
			(out->contrast_img).code = 142;
		}	
	}
	if(_res.c_img_act &&  _res.tl_act){
		if(_res.c_img_ver == Main__S && _res.tl_ver  == Main__S){
			(out->contrast_img).code = 150;
		}else if(_res.c_img_ver == Main__H1 && _res.tl_ver  == Main__H1){ //H1
			(out->contrast_img).code = 151;
		}else if(_res.c_img_ver == Main__H2 && _res.tl_ver  == Main__H2){ //H1
			(out->contrast_img).code = 152;
		}		
	}
	if(_res.c_img_act && _res.rs_act){
		if(_res.c_img_ver == Main__S && _res.rs_ver == Main__S){
			(out->rotoz_s).code = 160;
		}else if(_res.c_img_ver == Main__H1 && _res.rs_ver == Main__H1){ //H1
			(out->rotoz_s).code = 161;
		}else if(_res.c_img_ver == Main__H2 && _res.rs_ver == Main__H2){ //H1
			(out->rotoz_s).code = 162;
		}					
	}
	if(_res.c_img_act && _res.sl_act){
		if(_res.c_img_ver == Main__S && _res.sl_ver == Main__S){
			(out->contrast_img).code = 170;
		}else if(_res.c_img_ver == Main__H1 && _res.sl_ver == Main__H1){ //H1
			(out->contrast_img).code = 171;
		}else if(_res.c_img_ver == Main__H2 && _res.sl_ver == Main__H2){ //H1
			(out->contrast_img).code = 172;
		}					
	}
	if(_res.me_img_act && _res.tl_act){
		if(_res.me_img_ver == Main__S && _res.tl_ver == Main__S){
			(out->motion_estim_img).code = 180;
		}else if( _res.me_img_ver == Main__H1 && _res.tl_ver == Main__H1){ //H1
			(out->motion_estim_img).code = 181;
		}else if(_res.me_img_ver == Main__H2 && _res.tl_ver == Main__H2){ //H1
			(out->motion_estim_img).code = 182;
		}
	}
	if(_res.me_img_act && _res.rs_act){
		if(_res.me_img_ver == Main__S && _res.rs_ver == Main__S){
			(out->rotoz_s).code = 190;
		}else if(_res.me_img_ver == Main__H1 && _res.rs_ver == Main__H1){ //H1
			(out->rotoz_s).code = 191;
		}else if(_res.me_img_ver == Main__H2 && _res.rs_ver == Main__H2){ //H1
			(out->rotoz_s).code = 192;
		}		
	}
	if(_res.me_img_act && _res.sl_act){
		if(_res.me_img_ver == Main__S && _res.sl_ver == Main__S){
			(out->motion_estim_img).code = 200;
		}else if(_res.me_img_ver == Main__H1 && _res.sl_ver == Main__H1){ //H1
			(out->motion_estim_img).code = 201;
		}else if(_res.me_img_ver == Main__H2 && _res.sl_ver == Main__H2){ //H1
			(out->motion_estim_img).code = 202;
		}		
	}
	if(_res.rs_act  && _res.tl_act){
		if(_res.rs_ver == Main__S && _res.tl_ver == Main__S){
			(out->rotoz_s).code = 210;
		}else if(_res.rs_ver == Main__H1 &&  _res.tl_ver  == Main__H1){ //H1
			(out->rotoz_s).code = 211;
		}else if(_res.rs_ver == Main__H2 && _res.tl_ver == Main__H2){ //H1
			(out->rotoz_s).code = 212;
		}				
	}
	if(_res.tl_act && _res.sl_act ){
		if(_res.tl_ver == Main__S && _res.sl_ver == Main__S){
			(out->t_landing).code = 220;
		}else if(_res.tl_ver == Main__H1 && _res.sl_ver == Main__H1){ //H1
			(out->t_landing).code = 221;
		}else if(_res.tl_ver == Main__H2 && _res.sl_ver == Main__H2){ //H1
			(out->t_landing).code = 222;
		}					
	}
	if(_res.rs_act && _res.sl_act){
		if(_res.rs_ver == Main__S && _res.sl_ver == Main__S){
			(out->rotoz_s).code = 230;
		}else if(_res.rs_ver == Main__H1 && _res.sl_ver == Main__H1){ //H1
			(out->rotoz_s).code = 231;
		}else if(_res.rs_ver == Main__H2 && _res.sl_ver == Main__H2){ //H1
			(out->rotoz_s).code = 232;
		}					
	}

	/***********************************************************/


	/******/
	/******/



	    /* (out->contrast_img).size = _res.c_img_size; */
	    (out->contrast_img).act = _res.c_img_act;

	    /* (out->motion_estim_imu).size = _res.me_imu_size; */
	    (out->motion_estim_imu).act = _res.me_imu_act;

	    /* (out->motion_estim_img).size = _res.me_img_size; */
	    (out->motion_estim_img).act = _res.me_img_act;

	    /* (out->search_landing).size = _res.sl_size; */
	    (out->search_landing).act = _res.sl_act;

	    /* (out->obstacle_avoidance).size = _res.oa_size; */
	    (out->obstacle_avoidance).act = _res.oa_act;

	    /* (out->t_landing).size = _res.tl_size; */
	    (out->t_landing).act = _res.tl_act;

	    /* (out->rotoz_s).size = _res.rs_size; */
	    (out->rotoz_s).act= _res.rs_act;

	    /* (out->rotoz_b).size = _res.rb_size; */
	    (out->rotoz_b).act = _res.rb_act;

	    /* (out->replanning).size = _res.rpl_size; */
	    (out->replanning).act= _res.rpl_act;

	    /* (out->detection).size  = _res.dt_size; */
	    (out->detection).act= _res.dt_act;

	    /* (out->tracking).size = _res.trk_size; */
	    (out->tracking).act= _res.trk_act;

	/*****/
	/*****/
}






/*
gcc -c  -I/home/gueyes/.opam/4.04.0/lib/heptagon/c -Imain_c -I. -Imain_controller_c _main.c call.c main_c/main.c main_c/main_types.c main_controller_c/main_controller.c main_controller_c/main_controller_types.c



gcc -o sim  -I/home/gueyes/.opam/4.04.0/lib/heptagon/c -Imain_c -I. -Imain_controller_c _main.c call.c main_c/main.c main_c/main_types.c main_controller_c/main_controller.c main_controller_c/main_controller_types.c


    int ; // 1 : active, 0: stop or attente ressource
    int .code; // -1, (SW =>) 0,  (HW/tile =>) 1, 2, 3

    int achievable; // 0 (NO), 1 (YES)

    int up_pos;   //  0 (NO), 1 (YES)
    int down_pos; //  0 (NO), 1 (YES)
    int keep_pos; //  0 (NO), 1 (YES)

    int qos_pos;  //  0 (NO), 1 (YES)

*/


