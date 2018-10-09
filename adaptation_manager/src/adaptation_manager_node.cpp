//EM Add header calls here
#include "adaptation_manager_node.h"

#include "utils.h"
#include "handle_applications.h"
#include "reconfiguration_automate.h"
#include "reconfig.h"

using namespace std; 
using namespace cv;

/****** GLOBAL VARIABLES ********/
struct timeval  beginning, current1,  current2, current3, current4, current5;
int time_tk = 0;
int time_notif = 0;

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

boost::shared_ptr<ros::Publisher> achievable_pub=NULL;


string verify1[3][7]={{"0","0","0"},{"1","2","3"}};
/****** END GLOBAL VARIABLES ********/



void Task_in::raz_timing_qos()
{
	texec	= 0;
    qos		= 0;
}

void Task_in::raz_all()
{
 req		= 0; // 1 : activation, 0 : arrêt

 texec		= 0; // texec , [mintexec, maxtexec]
 mintexec	= 0;
 maxtexec	= 0;

 qos		= 0; //qos , [minqos, maxqos]
 minqos 	= 0;
 maxqos 	= 0;

 priority	= 0;
}


void Step_in::init()
{
	contrast_img.raz_all();
	motion_estim_imu.raz_all();
	motion_estim_img.raz_all();
	search_landing.raz_all();
	obstacle_avoidance.raz_all();
	t_landing.raz_all();
	rotoz_s.raz_all();
	rotoz_b.raz_all();
	replanning.raz_all();
	detection.raz_all();
	tracking.raz_all();

	h1.av =	1;	//EM, Everything is available at the beginning
	h2.av =	1;
	h3.av =	1;
}

//***************
void achievable_tab(Step_out s){
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

	ofstream file(PATH_ACHIEVABLE_TAB, ios::out); 
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
int verify(Step_out s){
    int a =1; 
	a = a & (s.contrast_img.achievable) & (s.motion_estim_imu.achievable)
		  & (s.motion_estim_img.achievable)& (s.search_landing.achievable)
		  & (s.obstacle_avoidance.achievable)& (s.t_landing.achievable)
		  & (s.rotoz_s.achievable)& (s.rotoz_b.achievable)& (s.replanning.achievable)
		  & (s.detection.achievable)& (s.tracking.achievable);
	return a;
}

//********************* Publier l'alerte à Mission Manager
void publish_to_MM(int a,Step_out s)
{   
	ros::Rate loop_rate(0.5); 
	std_msgs::Int32 msg1;
	//remplissage de liste des taches nn realisables
   	achievable_tab(s);
   	if(a==1)
	{
        ROS_INFO("[CHARGEMENT][BITSTREAM][SCHEDULING] , Achievable : [%d]", a);
	}else{
        ROS_ERROR("[ALTERTE][MISSION_MANAGER]");
        msg1.data =1;      
        achievable_pub->publish(msg1);   
        loop_rate.sleep();   	
		ROS_ERROR("LISTE DES TACHES NN REALISABLES ENVOYEE, Total_Achievable : [%d]", a);
		ROS_INFO("[CHARGEMENT][BITSTREAM][DERNIERE_SORTIE_AUTOMATE]");
	}
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
   vector <string> map =readfile1(PATH_MAP_TAB);
 
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
                cout << as[i][j] << "        "; 
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
					vector<string> done = readfile1(PATH_DONE);
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
					vector<string> done = readfile1(PATH_DONE);
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
					vector<string> done = readfile1(PATH_DONE);
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


//**************************BOUDABZA Ghizlane; 
//la fonction qui verifie est ce que texe appartient à l'intervalle [Tmin, Tmax]...pour chaque fonction;
void comparer(vector<string> timing_data, vector<string> C3, Step_in e) 
{ 
	//EM, TODO change 2 following variabls in boolean
	vector<int> T; int a=1;

	for(int j = 0; j < C3.size(); j++)
	{
		for(int m=0;m<timing_data.size();m=m+3)
		{
			if(C3[j]==timing_data[m])
			{
				if((timing_data[m+1] >= C3[j+3] && timing_data[m+1] <= C3[j+4]) 
				&& (timing_data[m+2] > C3[j+6] && timing_data[m+2] <= C3[j+7]))
				{
					T.push_back(1);
				}else{
					T.push_back(0);
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
  	C3 = readfile1(PATH_TABLE_C3);
  	Step_in e= entree1(C3); 
  	do1(e);
}



/************************************************************
*	EM 24/07/2018, MAIN
*	Adaptation manager main code
************************************************************/
int main (int argc, char ** argv)
{   
	ros::init(argc, argv, "adaptation_manager_node");
	ros::NodeHandle nh;
	
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

	//EM, Global variables moved to the Main
	int min_thres = 0;
	int max_thres = 2000;
	int time_notif = 0 ;
	/*int r = 0;
	int e = 0;*/

	//current1 = clock();  
	gettimeofday (&current1 , NULL);
	printf("adaptation_manager_req %.0f 0\n", ((double) time_micros(&current1, &beginning)));	
	printf("adaptation_manager_max %.0f %d\n",((double) time_micros(&current1, &beginning)), max_thres);
	printf("adaptation_manager_min %.0f %d\n",((double) time_micros(&current1, &beginning)),min_thres);
	//printf("adaptation_manager_ver %.0f %d\n",((double) time_micros(&current1, &beginning)), _res.res);
	
	//*****************Ghizlane BOUDABZA


	std_msgs::Float32 load;
	vector<string> time_qos_data, C3;

	Step_in e0,e;
	Step_out s;
	Step_in* in;

	ROS_ERROR("[ADAPTATION MANAGER] [RUNNING] \n");
	C3 = readfile1(PATH_TABLE_C3);
	e0 = entree0(C3); // texe,qos observé =0; 1ere utilisation du step()

	do1(e0); 

	ros::Rate loop_rate(10); //10hz = 100ms, 0.1hz=10s
	while(ros::ok())
	{ 
		ros::spinOnce();

		load.data = cpuload ( ) ;
		cpu_pub.publish( load );

		time_qos_data = readfile1(PATH_TIME_QOS); 
		if(time_qos_data.size()!=0)
		{
			e = entree(C3,time_qos_data);
			comparer(time_qos_data,C3, e);	
		}
		loop_rate.sleep();
	}

}

