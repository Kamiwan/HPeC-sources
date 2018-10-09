#include "reconfiguration_automate.h"

//*************** step()
Step_out doStep (Step_in e){  
    Step_out s; 
    std::cout << "[TRAITEMENT][ENTREES][AUTOMATE]...."<<std::endl;
    std::cout << "..................................."<<std::endl;
    std::cout<< "[PRISE DES DECISIONS PAR AUTOMATE......]"<< std::endl;
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

//********** initialiser l'entree du step
Step_in entree0(std::vector< std::string> C3){
    Step_in e;  //std::string::size_type sz;
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
 Step_in entree1(std::vector<string> C3){
     Step_in e;  //std::string::size_type sz;
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

 Step_in entree(std::vector<std::string> C3, std::vector<std::string> lignes){
     Step_in e;  //std::string::size_type sz;
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

	std::cout<<"s_l :"<<e.search_landing.texec<<std::endl;

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
	std::cout<<"track :"<<e.tracking.texec<<std::endl;
	std::cout<<"track :"<<e.tracking.qos<<std::endl;
   return e;
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