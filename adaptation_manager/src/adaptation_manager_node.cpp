//EM Add header calls here
#include "adaptation_manager_node.h"

using namespace std; 
using namespace cv;

/****** GLOBAL VARIABLES ********/
vector<Map_app_out> prev_app_output_config;
vector<Map_app_out> app_output_config;
bool first_step = true;

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

boost::shared_ptr<ros::Publisher> achievable_pub=NULL;
/****** END GLOBAL VARIABLES ********/


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
bool verify(Step_out s){
    bool a = true; 
	a = a & (s.contrast_img.achievable) & (s.motion_estim_imu.achievable)
		  & (s.motion_estim_img.achievable)& (s.search_landing.achievable)
		  & (s.obstacle_avoidance.achievable)& (s.t_landing.achievable)
		  & (s.rotoz_s.achievable)& (s.rotoz_b.achievable)& (s.replanning.achievable)
		  & (s.detection.achievable)& (s.tracking.achievable);
	return a;
}

//********************* Publier l'alerte à Mission Manager
void publish_to_MM(bool a,Step_out s)
{   
	std_msgs::Int32 msg1;
	//remplissage de liste des taches nn realisables
   	achievable_tab(s);
   	if(a==1)
	{
        ROS_INFO("[LOADING][BITSTREAM][SCHEDULING] , Achievable : [%d]", a);
	}else{
        ROS_INFO("[ALERT][MISSION_MANAGER]");
        msg1.data =1;      
        achievable_pub->publish(msg1);   
		ROS_INFO("STATE OF Total_Achievable : [%d]", a);
		ROS_INFO("[LOADING][BITSTREAM][LAST AUTOMATE OUTPUT]");
	}
}


//******************fonction d'activation des taches en sw ou hw: en (-), ou en (s,f,-)
void activate_desactivate_task(int app_index, std_msgs::Int32 msg){

	switch (app_index)
	{
	case 0:
		contrast_img_pub->publish(msg);
		break;
	case 1:
		motion_estim_imu_pub->publish(msg);
		break;
	case 2:
		motion_estim_img_pub->publish(msg);
		break;
	case 3:
		search_land_pub->publish(msg);
		break;
	case 4:
		obstacle_avoidance_pub->publish(msg);
		break;
	case 5:
		t_landing_pub->publish(msg);
		break;
	case 6:
		rotoz_s_pub->publish(msg);
		break;
	case 7:
		rotoz_b_pub->publish(msg);
		break;
	case 8:
		replanning_pub->publish(msg);
		break;
	case 9:
		detection_pub->publish(msg);
		break;
	case 10:
		tracking_pub->publish(msg);
		break;
	default:
		break;
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

 	//contrast_img_pub;
	contrast_img_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/contrast_img_area_mgt_topic", 1));
	//motion_estim_imu_pub;
	motion_estim_imu_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/motion_estim_imu_area_mgt_topic", 1));
 	//motion_estim_img_pub;
	motion_estim_img_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/motion_estim_img_area_mgt_topic", 1));
	//search_land_pub;
	search_land_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/search_landing_area_mgt_topic", 1));
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
	//replanning_pub;
	replanning_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/replanning_area_mgt_topic", 1));   
 	//detection_pub;
	detection_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/detection_area_mgt_topic", 1));
 	//tracking_pub;
	tracking_pub= boost::make_shared<ros::Publisher>( 
			         nh.advertise<std_msgs::Int32>("/tracking_area_mgt_topic", 1));

	//***************************envoie de liste des taches nn realisables
	achievable_pub= boost::make_shared<ros::Publisher>( 
						nh.advertise<std_msgs::Int32>("/achievable_topic", 1000));

	ros::Subscriber notify_from_MM_sub;
		notify_from_MM_sub = nh.subscribe("/notify_from_MM_topic", 1000, notify_Callback);
					
	ros::Publisher cpu_pub;
		cpu_pub = nh.advertise<std_msgs::Float32>("/cpu_load_topic", 1);				    
	//*********************				  

	
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
	vector<Bitstream_map> bts_map = read_BTS_MAP(PATH_MAP_TAB);

	/*############################## TEST CODE ##############################*/
	s = do1(e);
	publish_to_MM(verify(s),s);  //alerter le niveau mission si il existe des taches nn réalisables
	if(!first_step)
		prev_app_output_config = app_output_config;
	app_output_config = init_output(s); //conversion sortie step -> table 

	mapping(app_output_config, bts_map);
	
	s = fake_output2();
	if(!first_step)
		prev_app_output_config = app_output_config;
	app_output_config = init_output(s); //conversion sortie step -> table 

    mapping(app_output_config, bts_map);

	write_value_file(PATH_RELEASE_HW,"detection",0);
	write_value_file(PATH_RELEASE_HW,"search_landing",0);

	/*############################## TEST CODE ##############################*/

	/*
	ros::Rate loop_rate(10); //10hz = 100ms, 0.1hz=10s
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

vector<Map_app_out>	init_output(Step_out const& step_output)
{
	//EM, instantiation of the returned structure
	vector<Map_app_out> res;
	Map_app_out tmp;
	tmp.init();
	for(int i=0; i < APPLICATION_NUMBER; i++)
		res.push_back(tmp);

	if(first_step)
	{
		prev_app_output_config = res;
		first_step = false;
	}

	//EM, I know it's dirty, it would have been better with an array of attributes...
	res[0] 	= step_output.contrast_img;
	res[1] 	= step_output.motion_estim_imu;
	res[2] 	= step_output.motion_estim_img;
	res[3] 	= step_output.search_landing;
	res[4] 	= step_output.obstacle_avoidance;
	res[5] 	= step_output.t_landing;
	res[6] 	= step_output.rotoz_s;
	res[7] 	= step_output.rotoz_b;
	res[8] 	= step_output.replanning;
	res[9] 	= step_output.detection;
	res[10] = step_output.tracking;

	check_sequence(res);
	return res;
}

void check_sequence(vector<Map_app_out> & map_config_app)
{
	if(map_config_app.size() < APPLICATION_NUMBER)
	{
		std::cout << "The map_config_app table provided is too small! map_config_app size=" << map_config_app.size() << std::endl;
		return; //EM, to leave a void function
	}

	//EM, check if 2 active apps have the same HW Tile location 
	//	  => meaning sequence execution
	for(int i=0; i < (APPLICATION_NUMBER - 1); i++)
		if(map_config_app[i].active != 0 
			&& map_config_app[i].version_code < MULTI_APP_THRESHOLD_CODE
			&& map_config_app[i].region_id != 0)
			for(int j=i+1; j < APPLICATION_NUMBER; j++)
				if(map_config_app[i].region_id == map_config_app[j].region_id
					&& map_config_app[j].active != 0)
				{
					map_config_app[i].fusion_sequence = "s";
					map_config_app[j].fusion_sequence = "s";
				}
}

vector<Bitstream_map> read_BTS_MAP(const char* path)
{
	vector<Bitstream_map> res;
	vector<string> file_content = readfile(path);

	Bitstream_map tmp;
	for(int i=0; i<file_content.size(); i+=2)
	{
		tmp.version_code		= stoi(file_content[i]);
		tmp.bitstream_addr		= stoi(file_content[i+1]);
		res.push_back(tmp);
	}
   
   	cout << "There are "<< file_content.size() << " lines in the file" << endl;
	cout << "There are "<< res.size() << " Bitstreams" << endl;
   	for (size_t i = 0; i < res.size(); i++)
	{
       	cout << "App MAP " << i << " : " <<  endl;
		res[i].print();
	}
	return res;
}

int find_BTS_addr(vector<Bitstream_map> bts_map, int version_code)
{
	for (int i = 0; i < bts_map.size(); i++)
		if(bts_map[i].version_code == version_code)
			return bts_map[i].bitstream_addr;

	return -1;
}


void mapping(vector<Map_app_out> const& map_config_app, vector<Bitstream_map> const& bitstream_map)
{
	vector<App_scheduler> scheduler_array = create_scheduler_tab(map_config_app, bitstream_map);
	
	std_msgs::Int32 msg;
	//EM, First loop: disable each Task, must be done first to free HW Tiles
	//Taskes that are going in fusion are disabled to allow the fusion activation 
	cout << endl;
	for(size_t i = 0; i < scheduler_array.size(); i++)
	{
		if(scheduler_array[i].active == 0 || scheduler_array[i].fusion_sequence == "f") 
		{
			msg.data = 0; 
			activate_desactivate_task(scheduler_array[i].app_index, msg);	
			cout << "\033[1;31m Disable Task no: " << scheduler_array[i].app_index << "\033[0m"  << endl;
		}
	}

	//EM, Second loop: activation of every Task except those in sequence "s"
	for(size_t i = 0; i < scheduler_array.size(); i++)
	{
		//Enable SW taskes
		if(scheduler_array[i].region_id == 0 && scheduler_array[i].active == 1) 
		{
			msg.data = 1; 
			activate_desactivate_task(scheduler_array[i].app_index, msg);
			cout << "\033[1;32m Enable SW version of Task no: " << scheduler_array[i].app_index << "\033[0m" << endl;
		}

		//Enable classic HW taskes
		if(scheduler_array[i].region_id != 0 && scheduler_array[i].active == 1
			&& scheduler_array[i].fusion_sequence != "f" && scheduler_array[i].fusion_sequence != "s") 
		{
			//EM, TODO: CHECK if the previous app on the Tile is done! 
			//EM, TODO: LOAD bitstream in FPGA!!! 
			msg.data = 2; 
			activate_desactivate_task(scheduler_array[i].app_index, msg);
			cout << "\033[1;36m Enable HW version of Task no: " << scheduler_array[i].app_index 
					<< " in Tile no: " << scheduler_array[i].region_id << "\033[0m" << endl;
		}

		//Enable HW taskes in fusion "f"
		if(scheduler_array[i].region_id != 0 && scheduler_array[i].active == 1
			&& scheduler_array[i].fusion_sequence == "f")
		{
			//EM, TODO: CHECK if the previous app on the Tile is done
			//EM, TODO: LOAD bitstream in FPGA!!! 
			msg.data = scheduler_array[i].version_code - scheduler_array[i].region_id; 
			//The 2 fusionned taskes will be activated, only one ROS node can understand the msg.data and launch the fusion
			activate_desactivate_task(scheduler_array[i].app_index, msg);
			cout << "\033[36m Enable HW FUSION version of Task no: " << scheduler_array[i].app_index 
					<< " in Tile no: " << scheduler_array[i].region_id << "\033[0m" << endl;
		}
	}
	cout << endl;
	//EM, TODO: work on sequence HW Taskes "s"
}


vector<App_scheduler>	create_scheduler_tab(vector<Map_app_out> const& map_config_app, vector<Bitstream_map> const& bitstream_map)
{
	vector<App_scheduler> res;
	App_scheduler tmp;
	//EM, we add in the scheduler only applications with a different configuration from previous doStep()
	for(int i = 0; i < map_config_app.size(); i++)
		if( map_config_app[i].active != prev_app_output_config[i].active //EM, different state OR different version_code
		  || map_config_app[i].version_code != prev_app_output_config[i].version_code)
		{
			tmp.app_index = i;
			tmp = map_config_app[i];
			if(map_config_app[i].region_id != 0) //EM, if HW version -> need bitstream addr
				tmp.bitstream_addr = find_BTS_addr(bitstream_map, map_config_app[i].version_code);
			else
				tmp.bitstream_addr = 0;
			tmp.loaded	= 0;
			tmp.done 	= 0;

			res.push_back(tmp);
		}

	cout << "Scheduler array size = " << res.size() << " : " <<  endl;
	for(int i = 0; i < res.size(); i++)
	{
		cout << "Scheduler array [" << i << "] : " <<  endl;
		res[i].print();
	}
	return res;
}







