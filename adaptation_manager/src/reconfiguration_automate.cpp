#include "reconfiguration_automate.h"

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

//*****************
void do1( Step_in e){	
    std::vector < std::vector<std::string> > M;
    Step_out s = fake_Step(e);
	//doStep ( &e, &s);   //l'automate de config auto-adaptatif
	int a = verify(s); //VERIFIER la variable Achievable des taches
	publish_to_MM(a,s);       //alerter le niveau mission si il existe des taches nn réalisables
	//M= daube(5, 12, s); //conversion sortie step -> table 
    mapping(M); //chargement des bITSTREAMS DANS LA CARTE 
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

    