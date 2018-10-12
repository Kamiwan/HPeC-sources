#include "reconfiguration_automate.h"

//*************** step()
Step_out doStep (Step_in e){  
    Step_out s; 
    std::cout << "[TRAITEMENT][ENTREES][AUTOMATE]...."<<std::endl;
    std::cout << "..................................."<<std::endl;
    std::cout << "[PRISE DES DECISIONS PAR AUTOMATE......]"<< std::endl;
    std::cout << "[SORTIE][AUTOMATE]..." <<std::endl;
    s= sortie();
    return s;
}

//*****************
void do1( Step_in e){	
    std::vector < std::vector<std::string> > M;
    Step_out s = doStep (e);
	// doStep ( &e, &s);   //l'automate de config auto-adaptatif
	int a = verify(s); //VERIFIER la variable Achievable des taches
	publish_to_MM(a,s);       //alerter le niveau mission si il existe des taches nn réalisables
	M= initialise_sortie(5, 12, s); //conversion sortie step -> table 
    mapping(M); //chargement des bITSTREAMS DANS LA CARTE 
}

//********************BOUDABZA Ghizlane; la fonction qui permet de creer la table de sortie de l'automate..EXEMPLE: S_L ET DETECTION
std::vector < std::vector<std::string> > initialise_sortie(int n, int m, Step_out s){
   int i;
   	std::vector< std::vector<std::string> > M(n); 
   for(i=0; i < n; ++i) {
     M[i] = std::vector<std::string>(m);
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
	std::cout << M[i][0] << " ";
    std::cout << M[i][1] << "   ";
	std::cout << M[i][2] << "   ";
	std::cout << M[i][3] << "   ";
	std::cout << M[i][4] << "   ";
	std::cout << M[i][5] << " ";
	std::cout << M[i][6] << " ";
	std::cout << M[i][7] << " ";
	std::cout << M[i][8] << " ";
	std::cout << M[i][9] << " ";
    std::cout << M[i][10] << " ";
	std::cout << M[i][11] << " ";
	std::cout << std::endl;
	i=1;                   
	std::cout << M[i][0] << "           ";
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
	  std::cout << "  "<<stoi(M[i][j]) << "       ";
          } 
	std::cout << std::endl; 
    i=2; 
	std::cout << M[i][0] << "      ";
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
	  std::cout << "  "<<stoi(M[i][j]) << "      ";
          } 
	std::cout << std::endl; 
	i=3; 
	std::cout << M[i][0] << "       ";
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
	  std::cout << "  "<<stoi(M[i][j]) << "        ";
          } 
    std::cout << std::endl; 
	i=4;  std::vector< std::string> test; int l,j,p,t=1;
	std::cout << M[i][0] << "   ";
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
	  std::cout << M[i][j] << "          ";
          }
	std::cout << std::endl; 
	  return M;       
}


//************initialiser sortie automate // appartient au step fantome //a effacer apres
 Step_out sortie(){
	 Step_out s;
	 
     s.contrast_img.act = 1;
	 s.contrast_img.code = 142; 
     s.contrast_img.achievable = 1;
	 
	 std::cout<<s.contrast_img.act<<std::endl;
	 std::cout<<s.contrast_img.code<<std::endl; 
     std::cout<<s.contrast_img.achievable<<std::endl;

     s.motion_estim_imu.act = 0;
	 s.motion_estim_imu.code = 122; 
     s.motion_estim_imu.achievable = 1;
    	
	 s.motion_estim_img.act = 1;
	 s.motion_estim_img.code = 182; 
     s.motion_estim_img.achievable = 1;

	 s.search_landing.act = 1;
	 s.search_landing.code = 13; 
     s.search_landing.achievable = 0;

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