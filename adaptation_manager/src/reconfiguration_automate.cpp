#include "reconfiguration_automate.h"


//*****************
Step_out do1( Step_in e){	
    Step_out fake_results = fake_output();
	//doStep ( &e, &s);   //l'automate de config auto-adaptatif
    return fake_results;
}


Step_out fake_output(){
	 Step_out s;
	 
     s.contrast_img.act = 1;
	 s.contrast_img.code = 10; 
     s.contrast_img.achievable = 1;

     s.motion_estim_imu.act = 0;
	 s.motion_estim_imu.code = 22; 
     s.motion_estim_imu.achievable = 1;
    	
	 s.motion_estim_img.act = 1;
	 s.motion_estim_img.code = 182; 
     s.motion_estim_img.achievable = 1;

	 s.search_landing.act = 1;
	 s.search_landing.code = 40; 
     s.search_landing.achievable = 0;

	 s.obstacle_avoidance.act = 0;
	 s.obstacle_avoidance.code = 50; 
     s.obstacle_avoidance.achievable = 1;

	 s.t_landing.act = 1;
	 s.t_landing.code = 182; 
     s.t_landing.achievable = 1;

	 s.rotoz_s.act = 0;
	 s.rotoz_s.code = 70; 
     s.rotoz_s.achievable = 1;

	 s.rotoz_b.act = 0;
	 s.rotoz_b.code = 80; 
     s.rotoz_b.achievable = 1;

	 s.replanning.act = 1;
	 s.replanning.code = 93; 
     s.replanning.achievable = 1;

	 s.detection.act = 0;
	 s.detection.code = 121; 
     s.detection.achievable = 1;

	 s.tracking.act = 0;
	 s.tracking.code = 131; 
     s.tracking.achievable = 1;

	return s;
}

Step_out fake_output2(){
	 Step_out s;
	 
     s.contrast_img.act = 0;
	 s.contrast_img.code = 10; 
     s.contrast_img.achievable = 1;

     s.motion_estim_imu.act = 0;
	 s.motion_estim_imu.code = 22; 
     s.motion_estim_imu.achievable = 1;
    	
	 s.motion_estim_img.act = 0;
	 s.motion_estim_img.code = 30; 
     s.motion_estim_img.achievable = 1;

	 s.search_landing.act = 0;
	 s.search_landing.code = 41; 
     s.search_landing.achievable = 1;

	 s.obstacle_avoidance.act = 0;
	 s.obstacle_avoidance.code = 50; 
     s.obstacle_avoidance.achievable = 1;

	 s.t_landing.act = 0;
	 s.t_landing.code = 60; 
     s.t_landing.achievable = 1;

	 s.rotoz_s.act = 1;
	 s.rotoz_s.code = 70; 
     s.rotoz_s.achievable = 1;

	 s.rotoz_b.act = 1;
	 s.rotoz_b.code = 82; 
     s.rotoz_b.achievable = 1;

	 s.replanning.act = 1;
	 s.replanning.code = 93; 
     s.replanning.achievable = 1;

	 s.detection.act = 1;
	 s.detection.code = 121; 
     s.detection.achievable = 1;

	 s.tracking.act = 1;
	 s.tracking.code = 131; 
     s.tracking.achievable = 1;

	return s;
}

Step_out fake_output3(){
	 Step_out s;
	 
     s.contrast_img.act = 0;
	 s.contrast_img.code = 10; 
     s.contrast_img.achievable = 1;

     s.motion_estim_imu.act = 1;
	 s.motion_estim_imu.code = 22; 
     s.motion_estim_imu.achievable = 1;
    	
	 s.motion_estim_img.act = 1;
	 s.motion_estim_img.code = 30; 
     s.motion_estim_img.achievable = 1;

	 s.search_landing.act = 0;
	 s.search_landing.code = 41; 
     s.search_landing.achievable = 1;

	 s.obstacle_avoidance.act = 1;
	 s.obstacle_avoidance.code = 53; 
     s.obstacle_avoidance.achievable = 1;

	 s.t_landing.act = 0;
	 s.t_landing.code = 60; 
     s.t_landing.achievable = 1;

	 s.rotoz_s.act = 1;
	 s.rotoz_s.code = 70; 
     s.rotoz_s.achievable = 1;

	 s.rotoz_b.act = 0;
	 s.rotoz_b.code = 82; 
     s.rotoz_b.achievable = 1;

	 s.replanning.act = 0;
	 s.replanning.code = 93; 
     s.replanning.achievable = 1;

	 s.detection.act = 1;
	 s.detection.code = 121; 
     s.detection.achievable = 1;

	 s.tracking.act = 1;
	 s.tracking.code = 131; 
     s.tracking.achievable = 1;

	return s;
}
