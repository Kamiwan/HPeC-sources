#include "reconfiguration_automate.h"


//*****************
Step_out do1( Step_in e){	
    Step_out fake_results = fake_Step(e);
	//doStep ( &e, &s);   //l'automate de config auto-adaptatif
    return fake_results;
}

//*************** step()
Step_out fake_Step (Step_in e){  
    Step_out s; 
    std::cout << "[TRAITEMENT][ENTREES][AUTOMATE]...."<<std::endl;
    std::cout << "..................................."<<std::endl;
    std::cout << "[PRISE DES DECISIONS PAR AUTOMATE......]"<< std::endl;
    std::cout << "[SORTIE][AUTOMATE]..." <<std::endl;
    s= fake_output();
    return s;
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
	 s.search_landing.code = 41; 
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
	 s.rotoz_b.code = 81; 
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

     s.motion_estim_imu.act = 1;
	 s.motion_estim_imu.code = 22; 
     s.motion_estim_imu.achievable = 1;
    	
	 s.motion_estim_img.act = 1;
	 s.motion_estim_img.code = 182; 
     s.motion_estim_img.achievable = 1;

	 s.search_landing.act = 0;
	 s.search_landing.code = 41; 
     s.search_landing.achievable = 1;

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
	 s.rotoz_b.code = 81; 
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




    /* EM, Memo comment
    TODO DELETE
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
    M[4][0] = "fusion_sequence";*/

