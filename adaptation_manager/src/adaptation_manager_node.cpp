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
        ROS_INFO("[ALTERTE][MISSION_MANAGER]");
        msg1.data =1;      
        achievable_pub->publish(msg1);   
        loop_rate.sleep();   	
		ROS_INFO("LISTE DES TACHES NN REALISABLES ENVOYEE, Total_Achievable : [%d]", a);
		ROS_INFO("[CHARGEMENT][BITSTREAM][DERNIERE_SORTIE_AUTOMATE]");
	}
}



//************************ Creation des tables de scheduling
vector<vector<string>> scheduler_tab(int n, int m)
{
	vector<vector<string>> A(n);
	for (int i = 0; i < n; ++i)
		A[i] = vector<string>(m);

	A[0][0] = "func";
	A[0][1] = "Bitstream@";
	A[0][2] = "version_code";
	A[0][3] = "sequence";
	A[0][4] = "loaded";
	A[0][5] = "region_id";
	A[0][6] = "Done";
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
void mapping(vector<vector<string>> M)
{
	int nb = 6;
	int t1 = 0, t2 = 0;
	string static_tab[nb][7];
	string as[4][7];
	string as2[4][7];
	string as3[4][7];
	std_msgs::Int32 msg;
	vector<string> map = readfile(PATH_MAP_TAB);

	int i = 0;
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
	int j, k, l, m, m1, d, b, b1 = 0, indice = 1, p, t = 1;
	int l2, l3;
	string n, n1, n2, c1, push, push2, push3;
	vector<string> test;
	vector<vector<string>> a, af;
	//std::vector<std::vector<string>> static_tab=scheduler_tab(9,6);
	for (j = 1; j < 12; j++)
	{
		//*********TEST pour eviter la redondance des traitement des taches
		for (p = 0; p < test.size(); p++)
		{
			if (M[0][j] != test[p])
			{
				t = 1;
			}
			else
			{
				t = 0;
				break;
			}
		}

		if ((t == 1) && (M[1][j] != "0"))
		{
			if (M[4][j] == "-")
			{
				a = scheduler_tab(2, 7);
				a[1][0] = M[0][j];
				a[1][2] = M[2][j];
				a[1][3] = M[4][j];
				a[1][4] = "0"; // a remplir
				a[1][5] = M[3][j];
				a[1][6] = "0"; //a remplir

				for (k = 0; k < map.size(); k++)
				{
					if (a[1][2] == map[k])
					{
						a[1][1] = map[k + 1];
					}
				}
				std::vector<string> temp;
				for (int i = 1; i < 2; ++i)
				{
					for (int j(0); j < 7; ++j)
					{
						temp.push_back(a[i][j]);
					}
				}
				//remplissage de static_tab...
				if (indice == j)
				{
					for (b1 = 0, b = 0; b1 < 7, b < temp.size(); ++b1, ++b)
					{
						static_tab[indice][b1] = temp[b];
					}
					indice++;
				}
				else
				{
					for (b1 = 0, b = 0; b1 < 7, b < temp.size(); ++b1, ++b)
					{
						static_tab[indice][b1] = temp[b];
					}
					indice++;
				}
			}
			else if (M[4][j] == "f")
			{
				af = scheduler_tab(2, 7);
				af[1][2] = M[2][j];
				af[1][3] = M[4][j];
				af[1][4] = "0"; // a remplir
				af[1][6] = "0"; //a remplir
				af[1][5] = M[3][j];
				n1 = M[0][j];
				for (l = j + 1; l < M[i].size(); l++)
				{
					if ((M[3][l] == M[3][j]) && (M[4][j] == M[4][l]))
					{ //meme tuile , meme char f
						n2 = M[0][l];
						test.push_back(n2);
					}
				}
				n = n1 + '&' + n2;
				af[1][0] = n;
				for (k = 0; k < map.size(); k++)
				{
					if (af[1][2] == map[k])
					{
						af[1][1] = map[k + 1];
					}
				}
				for (int i = 1; i < 2; ++i)
				{
					std::vector<string> temp;
					for (int j(0); j < 7; ++j)
					{
						temp.push_back(af[i][j]);
					}
					//remplissae de static_tab...
					if (indice == j)
					{
						for (b1 = 0, b = 0; b1 < 7, b < temp.size(); ++b1, ++b)
						{
							static_tab[indice][b1] = temp[b];
						}
						indice++;
					}
					else
					{
						for (b1 = 0, b = 0; b1 < 7, b < temp.size(); ++b1, ++b)
						{
							static_tab[indice][b1] = temp[b];
						}
						indice++;
					}
				}
			}
			else
			{
				if (M[4][j] == "s")
				{
					//cas detect et track en seq : 2 bts dans meme tuile ; notion de "Done"
					as[1][0] = M[0][j];
					as[1][2] = M[2][j];
					as[1][3] = M[4][j];
					as[1][4] = "0"; // a remplir
					as[1][6] = "0"; //a remplir
					as[1][5] = M[3][j];
					for (k = 0; k < map.size(); k++)
					{
						if (as[1][2] == map[k])
						{
							as[1][1] = map[k + 1];
						}
					}
					d = M[i].size();
					int indice = j;
					int l = 2;
					for (m1 = indice + 1; m1 < (d + 1); m1++)
					{
						if ((M[3][m1] == M[3][j]) && (M[4][j] == M[4][m1]))
						{ //meme tuile, meme char s
							c1 = M[0][m1];
							as[l][0] = c1;
							as[l][2] = M[2][m1];
							as[l][3] = M[4][j];
							as[l][4] = "0"; // a remplir
							as[l][6] = "0"; //a remplir
							as[l][5] = M[3][j];
							indice = m1;
							for (k = 0; k < map.size(); k++)
							{
								if (as[l][2] == map[k])
								{
									as[l][1] = map[k + 1];
									cout << as[l][2] << endl;
								}
							}
							push = as[l][0];
							test.push_back(push);
							l++;
						}
						//test supp
						else if ((M[3][m1] != M[3][j]) && (M[4][j] == M[4][m1]))
						{ //#tuile, meme char s
							l2 = 1;
							if (as2[l2][0] == "")
							{
								string c2 = M[0][m1];
								as2[l2][0] = c2;
								as2[l2][2] = M[2][m1];
								as2[l2][3] = M[4][j];
								as2[l2][4] = "0"; // a remplir
								as2[l2][6] = "0"; //a remplir
								as2[l2][5] = M[3][m1];
								indice = m1;
								for (k = 0; k < map.size(); k++)
								{
									if (as2[l2][2] == map[k])
									{
										as2[l2][1] = map[k + 1];
									}
								}
								push2 = as2[l2][0];
								test.push_back(push2);
							}
							else
							{
								if (as2[1][5] == M[3][m1] && as2[1][3] == M[4][m1])
								{
									l2 = 2;
									string c2 = M[0][m1];
									as2[l2][0] = c2;
									as2[l2][2] = M[2][m1];
									as2[l2][3] = M[4][j];
									as2[l2][4] = "0"; // a remplir
									as2[l2][6] = "0"; //a remplir
									as2[l2][5] = M[3][m1];
									indice = m1;
									for (k = 0; k < map.size(); k++)
									{
										if (as2[l2][2] == map[k])
										{
											as2[l2][1] = map[k + 1];
										}
									}
									push2 = as2[l2][0];
									test.push_back(push2);
									l2++;
								}
								//test inf
								else
								{
									l3 = 1;
									if (as3[l3][0] == "")
									{
										string c3 = M[0][m1];
										as3[l3][0] = c3;
										as3[l3][2] = M[2][m1];
										as3[l3][3] = M[4][j];
										as3[l3][4] = "0"; // a remplir
										as3[l3][6] = "0"; //a remplir
										as3[l3][5] = M[3][m1];
										indice = m1;
										for (int k = 0; k < map.size(); k++)
										{
											if (as3[l3][2] == map[k])
											{
												as3[l3][1] = map[k + 1];
											}
										}
										push3 = as3[l3][0];
										test.push_back(push3);
									}
									else
									{
										l3 = 2;
										string c3 = M[0][m1];
										as3[l3][0] = c3;
										as3[l3][2] = M[2][m1];
										as3[l3][3] = M[4][j];
										as3[l3][4] = "0"; // a remplir
										as3[l3][6] = "0"; //a remplir
										as3[l3][5] = M[3][m1];
										indice = m1;
										for (int k = 0; k < map.size(); k++)
										{
											if (as3[l3][2] == map[k])
											{
												as3[l3][1] = map[k + 1];
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

					//********** EM, FUCKING DISPLAYS
					/*for (int i(0); i < 4; ++i)
					{
						for (int j(0); j < 7; ++j)
						{
							cout << as[i][j] << "        ";
						}
						cout << endl;
					}
					if (as2[1][0] != "")
					{
						for (int i(0); i < 4; ++i)
						{
							for (int j(0); j < 7; ++j)
							{
								cout << as2[i][j] << "        ";
							}
							cout << endl;
						}
					}
					if (as3[1][0] != "")
					{
						for (int i(0); i < 4; ++i)
						{
							for (int j(0); j < 7; ++j)
							{
								cout << as3[i][j] << "        ";
							}
							cout << endl;
						}
					}*/
				}
			}
			//**********
		}
	}
	//********** EM, Worst Loop EVER



	ROS_INFO("STATIC_TAB_AFFICHAGE...");
	for (int i(0); i < nb; ++i)
	{
		for (int j(0); j < 6; ++j)
		{
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
	for (int i = 1; i < nb; i++)
	{
		if (static_tab[i][5] == "1" || static_tab[i][5] == "2" || static_tab[i][5] == "3")
		{ //test pour eviter de recharger le meme bts ou charger un nv bts alors que la tuile contient une tache en cours d'exec.
			for (int i1 = 0; i1 < 3; i1++)
			{
				if (static_tab[i][0] == verify1[i1][0] && static_tab[i][5] == verify1[i1][1])
				{			//dernier etat (Bts) dans les 3 regions reconfigurables(nom,region_id)
					t1 = 1; //bts est deja configuré
					break;
				}
				else
				{
					t1 = 0;
					msg.data = 0;
					activate_desactivate_task(verify1, i1, msg); //desactiver la tache en cours d'exec dans la tuile demandé pour la nv config
				}
			}
			if (t1 == 0)
			{
				msg.data = 2;
				int bitstream_address = std::stoi(static_tab[i][1]);
				int loaded = 0;
				int region_id = std::stoi(static_tab[i][5]);
				/* EM 24/07/2018, comment code calling bitstreams
					start_reconfiguration(fd_mem, bitstream_address, region_id);
					do{
					loaded = reconfiguration_done(fd_mem);
					}while(loaded==1);
					*/
				static_tab[i][4] = loaded;
				v(static_tab, i);							   //sauvegarder le dernier etat configuré dans la region reconfigurable (non_tache, region_id)
				activate_desactivate_task(static_tab, i, msg); //activer la tache
			}
		}
	}
	vector<int> B;
	b = 1;
	//charger bts de la 1er tache en s et l'activer,de meme pour taches suivantes apres Done=1 de la tache precedante
	//TODO : normaliser les seuils du for
	for (int i = 1, i3 = 1, i4 = 1; i < 4, i3 < 4, i4 < 4; i++, i3++, i4++)
	{
		if (as[i][0] != "" || as2[i3][0] != "" || as3[i4][0] != "")
		{
			for (int i2 = 0; i2 < 3; i2++)
			{
				if (as[i][0] == verify1[i2][0] && as[i][5] == verify1[i2][1] || as2[i3][0] == verify1[i2][0] && as2[i3][5] == verify1[i2][1] || as3[i4][0] == verify1[i2][0] && as3[i4][5] == verify1[i2][1])
				{ //dernier etat (Bts) dans les 3 regions reconfigurables
					t2 = 1;
					B.push_back(t2); //bts est deja configuré
					break;
				}
				else
				{
					t2 = 0;
					B.push_back(t2);
					msg.data = 0;
					activate_desactivate_task(verify1, i2, msg); //desactiver la tache en cours d'exec dans la tuile demandé pour la nv config au cas d'une nouvelle seq s
				}
			}
			for (int i = 0; i < B.size(); i++)
			{
				b = b & B[i];
			}
			if (b == 0)
			{
				//test as
				if (as[i][0] != "" && as[i][4] != "1")
				{
					msg.data = 2; //si on a une exec en s dans notre sequence produite par M_M
					int bitstream_address = stoi(as[i][1]);
					int loaded = 0;
					int region_id = stoi(as[i][5]);
					/* EM 24/07/2018, comment code calling bitstreams
					start_reconfiguration(fd_mem, bitstream_address, region_id);
					do{ 
					loaded = reconfiguration_done(fd_mem);
					}while(loaded==1);
					*/
					as[i][4] = loaded;
					v(as, i);
					activate_desactivate_task(as, i, msg); //activer la tache
				}
				//fin test as

				//test as2
				if (as2[i3][0] != "" && as2[i3][4] != "1")
				{
					msg.data = 2; //si on a une exec en s dans notre sequence produite par M_M
					int bitstream_address = stoi(as2[i3][1]);
					int loaded = 0;
					int region_id = stoi(as2[i3][5]);
					/* EM 24/07/2018, comment code calling bitstreams
					start_reconfiguration(fd_mem, bitstream_address, region_id);
					do{ 
					loaded = reconfiguration_done(fd_mem);
					}while(loaded==1);
					*/
					as2[i3][4] = loaded;
					v(as2, i3);
					activate_desactivate_task(as2, i3, msg); //activer la tache
				}
				//fin test as2

				//test as3
				if (as3[i4][0] != "" && as3[i4][4] != "1")
				{
					msg.data = 2; //si on a une exec en s dans notre sequence produite par M_M
					int bitstream_address = stoi(as3[i4][1]);
					int loaded = 0;
					int region_id = stoi(as3[i4][5]);
					/* EM 24/07/2018, comment code calling bitstreams
					start_reconfiguration(fd_mem, bitstream_address, region_id);
					do{ 
					loaded = reconfiguration_done(fd_mem);
					}while(loaded==1);
					*/
					as3[i4][4] = loaded;
					v(as3, i4);
					activate_desactivate_task(as3, i4, msg); //activer la tache
				}
				//fin test as3

				//attente des Done des taches actives en s
				i = 1;
				int i3 = 1, i4 = 1;
				bool done1 = false;
				bool done2 = false;
				bool done3 = false;
				if (as[i][0] != "")
				{
					do
					{
						vector<string> done = readfile(PATH_DONE);
						for (int l = 0; l < done.size(); l++)
						{
							if (done[l] == as[i][0])
							{
								as[i][6] = done[l + 1];
								cout << "as" << endl;
								cout << as[i][6] << endl;
							}
						}
						if (as[i][6] == "1")
							done1 = true;
					} while (!done1);
				}
				if (as2[i3][0] != "")
				{
					do
					{
						vector<string> done = readfile(PATH_DONE);
						for (int l = 0; l < done.size(); l++)
						{
							if (done[l] == as2[i3][0])
							{
								as2[i3][6] = done[l + 1];
								cout << "as2" << endl;
								cout << as2[i3][6] << endl;
							}
						}
						if (as2[i3][6] == "1")
							done2 = true;
					} while (!done2);
				}
				if (as3[i4][0] != "")
				{
					do
					{
						vector<string> done = readfile(PATH_DONE);
						for (int l = 0; l < done.size(); l++)
						{
							if (done[l] == as3[i4][0])
							{
								as3[i4][6] = done[l + 1];
								cout << "as" << endl;
								cout << as3[i4][6] << endl;
							}
						}
						if (as3[i4][6] == "1")
							done3 = true;
					} while (!done3);
				}
			}
		}
	}
	//activer les taches en sw
	for (int i = 1; i < nb; i++)
	{
		if (static_tab[i][5] == "0")
		{
			msg.data = 1;
			activate_desactivate_task(static_tab, i, msg); //activer en sw
		}
	}
	//affichage des taches en s
	if (as[1][0] != "")
	{
		for (int i(0); i < 4; ++i)
		{
			for (int j(0); j < 7; ++j)
			{
				cout << as[i][j] << "        ";
			}
			cout << endl;
		}
	}
	if (as2[1][0] != "")
	{
		for (int i(0); i < 4; ++i)
		{
			for (int j(0); j < 7; ++j)
			{
				cout << as2[i][j] << "        ";
			}
			cout << endl;
		}
	}
	if (as3[1][0] != "")
	{
		for (int i(0); i < 4; ++i)
		{
			for (int j(0); j < 7; ++j)
			{
				cout << as3[i][j] << "        ";
			}
			cout << endl;
		}
	}
	ROS_INFO("STATIC_TAB_COMPLETE..");
	for (int i(0); i < nb; ++i)
	{
		for (int j(0); j < 6; ++j)
		{
			cout << static_tab[i][j] << "       ";
		}
		cout << endl;
	}
}

//**************************BOUDABZA Ghizlane; 
//la fonction qui verifie est ce que texe appartient à l'intervalle [Tmin, Tmax]...pour chaque fonction;
bool compare(std::vector<App_timing_qos> time_qos, std::vector<Task_in> C3) 
{ 
	if(time_qos.size() != C3.size())
	{
		ROS_ERROR("COMPARE function failed! C3 and time_qos are not of the same size! C3=%ld and time_qos=%ld"
					,time_qos.size(), C3.size());
		exit(1);
	}

	vector<bool> T; 
	bool res=true;

	for(int j = 0; j < C3.size(); j++)
	{
		if((time_qos[j].texec >= C3[j].mintexec && time_qos[j].qos <= C3[j].mintexec) 
			&& (time_qos[j].qos >= C3[j].minqos && time_qos[j].qos <= C3[j].maxqos))
			T.push_back(true);
		else
			T.push_back(false);
	}

	for(int i=0;i<T.size();i++){
		res= res & T[i];
		cout<<T[i]<<" ";
	}
	cout<<endl;

	return res;
}

//***************************
void notify_Callback(const std_msgs::Int32::ConstPtr& msg){
  	ROS_INFO("[RECU][MISSION_M][CHANGEMENT D'INTERVALLES DANS TABLE C3..] \n");
  	ROS_INFO("%d", msg->data);
  	float texe; 

	vector<Task_in> C3 = read_C3(PATH_TABLE_C3);
  	Step_in e;
	e.init();
	e.load_C3(C3); 
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

	Step_in e0,e;
	Step_out s;
	Step_in* in;

	ROS_INFO("[ADAPTATION MANAGER] [RUNNING] \n");

	vector<Task_in> C3 = read_C3(PATH_TABLE_C3);
	e.init();
	e.load_C3(C3); 
	e.raz_timing_qos();

	vector<App_timing_qos> time_qos_data;

	
	do1(e0);
	/*ros::Rate loop_rate(10); //10hz = 100ms, 0.1hz=10s
	while(ros::ok())
	{ 
		ros::spinOnce();

		load.data = cpuload ( ) ;
		cpu_pub.publish( load );

		time_qos_data = read_time_qos(PATH_TIME_QOS);
		if(time_qos_data.size()!=0)
		{
			e.update_timing_qos(time_qos_data);	
			if(!compare(time_qos_data,C3))
				do1(e); 
		}
		loop_rate.sleep();
	}
	*/
}





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

void Task_in::print(){
	std::cout << "req = " << req << std::endl;
	std::cout << "texec = " << texec << std::endl;
	std::cout << "mintexec = " << mintexec << std::endl;
	std::cout << "maxtexec = " << maxtexec << std::endl;
	std::cout << "qos = " << qos << std::endl;
	std::cout << "minqos = " << minqos << std::endl;
	std::cout << "maxqos = " << maxqos << std::endl;
	std::cout << "priority = " << priority << std::endl;
}


Task_in& Task_in::operator=( Task_in const& rhs )
{
	// Check for self-assignment!
    if (this == &rhs)
    	return *this;        

	req			= rhs.req; 		// 1 : activation, 0 : stop

	texec		= rhs.texec; 	// texec , [mintexec, maxtexec]
	mintexec	= rhs.mintexec;
	maxtexec	= rhs.maxtexec;

	qos			= rhs.qos; 		//qos , [minqos, maxqos]
	minqos 		= rhs.minqos;
	maxqos 		= rhs.maxqos;

	priority	= rhs.priority;
	return *this;
}


Task_in& Task_in::operator=(App_timing_qos const& rhs)
{
	texec		= rhs.texec; 	// texec , [mintexec, maxtexec]
	qos			= rhs.qos; 		//qos , [minqos, maxqos]
	return *this;
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

void Step_in::raz_timing_qos()
{
	contrast_img.raz_timing_qos();
	motion_estim_imu.raz_timing_qos();
	motion_estim_img.raz_timing_qos();
	search_landing.raz_timing_qos();
	obstacle_avoidance.raz_timing_qos();
	t_landing.raz_timing_qos();
	rotoz_s.raz_timing_qos();
	rotoz_b.raz_timing_qos();
	replanning.raz_timing_qos();
	detection.raz_timing_qos();
	tracking.raz_timing_qos();
}

void Step_in::load_C3(const std::vector<Task_in> C3)
{
	if(C3.size() < APPLICATION_NUMBER)
	{
		std::cout << "The C3 table provided is too small! C3 size=" << C3.size() << std::endl;
		return; //EM, to leave a void function
	}
	contrast_img 		= C3[0];
	motion_estim_imu 	= C3[1];
	motion_estim_img 	= C3[2];
	search_landing 		= C3[3];
	obstacle_avoidance 	= C3[4];
	t_landing 			= C3[5];
	rotoz_s 			= C3[6];
	rotoz_b 			= C3[7];
	replanning 			= C3[8];
	detection 			= C3[9];
	tracking 			= C3[10];
}

void Step_in::update_timing_qos(std::vector<App_timing_qos> time_qos)
{
	if(time_qos.size() < APPLICATION_NUMBER)
	{
		std::cout << "The time_qos table provided is too small! time_qos size=" << time_qos.size() << std::endl;
		return; //EM, to leave a void function
	}
	contrast_img 		= time_qos[0];
	motion_estim_imu 	= time_qos[1];
	motion_estim_img 	= time_qos[2];
	search_landing 		= time_qos[3];
	obstacle_avoidance 	= time_qos[4];
	t_landing 			= time_qos[5];
	rotoz_s 			= time_qos[6];
	rotoz_b 			= time_qos[7];
	replanning 			= time_qos[8];
	detection 			= time_qos[9];
	tracking 			= time_qos[10];
}


vector<Task_in> read_C3(const char* path)
{
	vector<Task_in> res;
	vector<string> file_content = readfile(path);

	Task_in tmp;
	for(int i=0; i<file_content.size(); i+=9)
	{
		//EM, The first string every 9 rows is the Task name, ex: [0],[9]...
		tmp.req			= stoi(file_content[i+1]);
		tmp.texec		= stoi(file_content[i+2]);
		tmp.mintexec	= stoi(file_content[i+3]);
		tmp.maxtexec	= stoi(file_content[i+4]);
		tmp.qos			= stoi(file_content[i+5]);
		tmp.minqos 		= stoi(file_content[i+6]);
		tmp.maxqos 		= stoi(file_content[i+7]);
		tmp.priority	= stoi(file_content[i+8]);

		res.push_back(tmp);
	}
   
   	cout << "There are "<< file_content.size() << " lines in the file" << endl;
	cout << "There are "<< res.size() << " Taskes" << endl;
   	for (size_t i = 0; i < res.size(); i++)
	{
       	cout << "Task " << i << " : " <<  endl;
		res[i].print();
	}
	return res;
}


void App_timing_qos::print(){
	std::cout << "texec = " << texec << std::endl;
	std::cout << "qos = " << qos << std::endl;
}


vector<App_timing_qos> read_time_qos(const char* path)
{
	vector<App_timing_qos> res;
	vector<string> file_content = readfile(path);

	App_timing_qos tmp;
	for(int i=0; i<file_content.size(); i+=3)
	{
		//EM, The first string every 3 rows is the Task name, ex: [0],[3]...
		tmp.texec		= stoi(file_content[i+1]);
		tmp.qos			= stoi(file_content[i+2]);
		res.push_back(tmp);
	}
   
   	cout << "There are "<< file_content.size() << " lines in the file" << endl;
	cout << "There are "<< res.size() << " Taskes" << endl;
   	for (size_t i = 0; i < res.size(); i++)
	{
       	cout << "Task " << i << " : " <<  endl;
		res[i].print();
	}
	return res;
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
	 s.search_landing.code = 43; 
     s.search_landing.achievable = 0;

	 s.obstacle_avoidance.act = 0;
	 s.obstacle_avoidance.code = 50; 
     s.obstacle_avoidance.achievable = 1;

	 s.t_landing.act = 0;
	 s.t_landing.code = 182; 
     s.t_landing.achievable = 1;

	 s.rotoz_s.act = 0;
	 s.rotoz_s.code = 70; 
     s.rotoz_s.achievable = 1;

	 s.rotoz_b.act = 0;
	 s.rotoz_b.code = 81; 
     s.rotoz_b.achievable = 1;

	 s.replanning.act = 0;
	 s.replanning.code = 90; 
     s.replanning.achievable = 1;

	 s.detection.act = 0;
	 s.detection.code = 121; 
     s.detection.achievable = 1;

	 s.tracking.act = 0;
	 s.tracking.code = 131; 
     s.tracking.achievable = 1;

	return s;
}