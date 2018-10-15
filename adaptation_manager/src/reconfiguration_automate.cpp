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
	M= initialise_sortie(5, 12, s); //conversion sortie step -> table 
    mapping(M); //chargement des bITSTREAMS DANS LA CARTE 
}

//********************BOUDABZA Ghizlane; la fonction qui permet de creer la table de sortie de l'automate..EXEMPLE: S_L ET DETECTION
std::vector < std::vector<std::string> > initialise_sortie(int n, int m, Step_out s){
    int i;
    std::vector< std::vector<std::string> > M(n); 

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

	M[1][1]=to_string(s.contrast_img.act);
	M[1][2]=to_string(s.motion_estim_imu.act);
	M[1][3]=to_string(s.motion_estim_img.act);
	M[1][4]=to_string(s.search_landing.act);
	M[1][5]=to_string(s.rotoz_b.act);
	M[1][6]=to_string(s.tracking.act);
	M[1][7]=to_string(s.rotoz_s.act);
	M[1][8]=to_string(s.detection.act);
	M[1][9]=to_string(s.obstacle_avoidance.act);
	M[1][10]=to_string(s.t_landing.act);
	M[1][11]=to_string(s.replanning.act);

	M[2][1]=to_string(s.contrast_img.code);
	M[2][2]=to_string(s.motion_estim_imu.code);
	M[2][3]=to_string(s.motion_estim_img.code);
	M[2][4]=to_string(s.search_landing.code);
	M[2][5]=to_string(s.rotoz_b.code);
	M[2][6]=to_string(s.tracking.code);
	M[2][7]=to_string(s.rotoz_s.code);
	M[2][8]=to_string(s.detection.code);
	M[2][9]=to_string(s.obstacle_avoidance.code);
	M[2][10]=to_string(s.t_landing.code);
	M[2][11]=to_string(s.replanning.code);

	M[3][1]=to_string((s.contrast_img.code)%10);
	M[3][2]=to_string((s.motion_estim_imu.code)%10);
	M[3][3]=to_string((s.motion_estim_img.code)%10);
	M[3][4]=to_string((s.search_landing.code)%10);
	M[3][5]=to_string((s.rotoz_b.code)%10);
	M[3][6]=to_string((s.tracking.code)%10);
	M[3][7]=to_string((s.rotoz_s.code)%10);
	M[3][8]=to_string((s.detection.code)%10);
	M[3][9]=to_string((s.obstacle_avoidance.code)%10);
	M[3][10]=to_string((s.t_landing.code)%10);
	M[3][11]=to_string((s.replanning.code)%10);

    int l,j;
	for(j=1;j < m;j++){
        if ((M[1][j] != "0") && (M[3][j] != "0")) // active : 1 (!="0")
        { 
            if (stoi(M[2][j]) < stoi("140")) // || M[2][j] < "99"
            { 
                for (l = j + 1; l < m + 1; l++)
                    if ((M[3][j] == M[3][l]) && (M[1][l] != "0") && (stoi(M[2][l]) < stoi("140"))) //meme region_id
                    { 
                        M[4][j] = "s";
                        M[4][l] = "s";
                    }
            }
            if (stoi(M[2][j]) >= stoi("140"))
            {
                M[4][j] = "f";
            }
            else
            {
                if (M[4][j] != "s" || M[4][j] == "")
                    M[4][j] = "-";
            }
        }
        else
        {
            M[4][j] = "-";
        }
    }

	for(int j=1; j<m; j++)
	  std::cout << M[4][j] << "          ";
	std::cout << std::endl; 

	return M;       
}
