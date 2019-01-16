/* 
 * This file is part of the HPeC distribution (https://github.com/Kamiwan/HPeC-sources).
 * Copyright (c) 2018 Lab-STICC Laboratory.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
/*************************************************************************************
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: September 17, 2018
 * 
 * Main file of the Adaptation Manager ROS node.
 *   This application controls the execution of other HPeC nodes and execute a 
 *   reconfiguration automaton to get the right schedule.
 *   The configuration is given according Mission Manager requests 
 *************************************************************************************/

#include "adaptation_manager_node.h"

using namespace std; 
using namespace cv;
namespace bip = boost::interprocess;

/****** GLOBAL VARIABLES ********/
vector<Map_app_out> prev_app_output_config;
vector<Map_app_out> app_output_config;
bool MM_request = false;
int verbose;
boost::shared_ptr<boost::thread> sequence_thread[TILE_NUMBER];
bool active_thread[TILE_NUMBER];
App_scheduler sequence_apps[2];
/****** GLOBAL VARIABLES ********/

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


//******************BOUDABZA Ghizlane, check if an app is not achieved
bool check_achievable(MemoryCoordinator & shared_memory, Step_out s){
    bool is_achievable = true; 
    is_achievable = is_achievable & (s.contrast_img.achievable) 
        & (s.motion_estim_imu.achievable)
        & (s.motion_estim_img.achievable)& (s.search_landing.achievable)
        & (s.obstacle_avoidance.achievable)& (s.t_landing.achievable)
        & (s.rotoz_s.achievable)& (s.rotoz_b.achievable)& (s.replanning.achievable)
        & (s.detection.achievable)& (s.tracking.achievable);
    
    sh_mem_write_achievable(shared_memory, s);

    return is_achievable;
}

void sh_mem_write_achievable(MemoryCoordinator & shared_memory, Step_out s)
{
    shared_memory.achievable_Write(s.contrast_img.achievable, CONTRAST_IMG);
    shared_memory.achievable_Write(s.motion_estim_imu.achievable, MOTION_ESTIM_IMU);
    shared_memory.achievable_Write(s.motion_estim_img.achievable, MOTION_ESTIM_IMG);
    shared_memory.achievable_Write(s.search_landing.achievable, SEARCH_LANDING);
    shared_memory.achievable_Write(s.obstacle_avoidance.achievable, OBSTACLE_AVOIDANCE);
    shared_memory.achievable_Write(s.t_landing.achievable, T_LANDING);
    shared_memory.achievable_Write(s.rotoz_s.achievable, ROTOZ_S);
    shared_memory.achievable_Write(s.rotoz_b.achievable, ROTOZ_B);
    shared_memory.achievable_Write(s.replanning.achievable, REPLANNING);
    shared_memory.achievable_Write(s.detection.achievable, DETECTION);
    shared_memory.achievable_Write(s.tracking.achievable, TRACKING);
}

//********************* Send Alert to Mission Manager if needed
void publish_to_MM(bool is_achievable,Step_out s)
{   
    std_msgs::Int32 msg1;
    if(is_achievable)
    {
        ROS_INFO("[ADAPTATION MANAGER]: Configuration achievable");
    }else
    {
        ROS_INFO("[ADAPTATION MANAGER]: Configuration NOT achievable!");
        ROS_INFO("[ADAPTATION MANAGER]: Send alert to Mission Manager");
        msg1.data =(int)is_achievable;      
        achievable_pub->publish(msg1);   
    }
}


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



bool compare(std::vector<App_timing_qos> time_qos, std::vector<Task_in> C3) 
{ 
    if(time_qos.size() != C3.size())
    {
        ROS_ERROR("COMPARE failed! C3 and time_qos are not of the same size! C3=%ld and time_qos=%ld"
                    ,time_qos.size(), C3.size());
        exit(1);
    }

    vector<bool> T; 
    bool res=true;

    for(int j = 0; j < C3.size(); j++)
    {
        if( (time_qos[j].texec <= C3[j].maxtexec)  //&& time_qos[j].texec >= C3[j].mintexec
            && (time_qos[j].qos <= C3[j].maxqos) ) //&& time_qos[j].qos >= C3[j].minqos
            T.push_back(true);
        else
            T.push_back(false);
    }

    if(verbose > VERBOSITY_OFF)
    {
        for(int i=0;i<T.size();i++){
            res= res & T[i];
            cout<<T[i]<<" ";
        }
        cout<<endl;
    }

    return res;
}

//***************************
void notify_Callback(const std_msgs::Int32::ConstPtr& msg)
{
    ROS_INFO("[FROM][MISSION_M]: New configuration request! \n");
    ROS_INFO("%d", msg->data);

    MM_request = true;
}



/************************************************************
*	EM 24/07/2018, MAIN
*	Adaptation manager main code
************************************************************/
int main (int argc, char ** argv)
{   
    ros::init(argc, argv, "adaptation_manager_node");
    ros::NodeHandle nh;     //EM non-private Node Handle to get topics notifications from other nodes
    ros::NodeHandle n("~"); //EM, use ~ for private parameters, here it's for verbose

    ROS_INFO("[ADAPTATION MANAGER] [RUNNING] \n");

    if (!n.hasParam("verbose"))
        ROS_INFO("No param named 'verbose'");
    
    if(n.getParam("verbose",  verbose))
        ROS_INFO("verbose level = %d", verbose);
    else
    {
        ROS_ERROR("FAILED TO GET verbose, default value %d",VERBOSITY_DEFAULT);
        verbose = VERBOSITY_DEFAULT;
    }

    contrast_img_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/contrast_img_mgt_topic", 1));
    motion_estim_imu_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/motion_estim_imu_mgt_topic", 1));
    motion_estim_img_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/motion_estim_img_mgt_topic", 1));
    search_land_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/search_landing_area_mgt_topic", 1));
    obstacle_avoidance_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/obstacle_avoidance_mgt_topic", 1));
    t_landing_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/t_landing_mgt_topic", 1));
    rotoz_s_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/rotoz_s_mgt_topic", 1));
    rotoz_b_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/rotoz_b_mgt_topic", 1));
    replanning_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/replanning_mgt_topic", 1));   
    detection_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/detection_mgt_topic", 1));
    tracking_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/tracking_mgt_topic", 1));
    achievable_pub = boost::make_shared<ros::Publisher>( 
                     nh.advertise<std_msgs::Int32>("/achievable_topic", 1000));
    ros::Subscriber notify_from_MM_sub;
        notify_from_MM_sub = nh.subscribe("/notify_from_MM_topic", 1000, notify_Callback);


    //EM, Data structure setup
    Step_in     automaton_in;
    Step_out    automaton_out;
    vector<Task_in>         C3_init = read_C3(PATH_TABLE_C3);
    vector<Bitstream_map>   bts_map = read_BTS_MAP(PATH_MAP_TAB);
    raz_timing_qos(C3_init);
    automaton_in.load_C3(C3_init);
    //EM, Shared Memory setup
    MemoryCoordinator sh_mem_access("Admin");
    sh_mem_setup(sh_mem_access, C3_init);
    
    for(int i = 0; i < TILE_NUMBER; i++)
        active_thread[i] = false;
    
    

    //EM, First Step use with start configuration
    automaton_out = do1(automaton_in); //Call reconfiguration automaton

    //EM, For the 1st Step, init each attributes of prev_config to 0 
    Map_app_out empty_map;
    empty_map.init();
    for(size_t i = 0; i < APPLICATION_NUMBER; i++)
        prev_app_output_config.push_back(empty_map);     
    app_output_config = init_output(automaton_out); //convert automaton_out to vector<App_map_out>

    task_mapping(app_output_config, prev_app_output_config, bts_map, sh_mem_access);
    
    // MAIN LOOP
    vector<App_timing_qos>  time_qos_data;
    vector<Task_in>         C3_current = sh_mem_read_C3(sh_mem_access);
    ros::Rate loop_rate(1); //10hz = 100ms, 0.1hz=10s

    int count_test = 0;

    while(ros::ok())
    { 
        ros::spinOnce();

        time_qos_data = sh_mem_read_time_qos(sh_mem_access);
        if(!compare(time_qos_data,C3_current) || MM_request || count_test == 10 || count_test == 30 )
        {
            if(MM_request) //EM, When the reconfiguration is asked from Mission Manager
            {
                C3_current    = sh_mem_read_C3(sh_mem_access);
                automaton_in.load_C3(C3_current);
                MM_request = false;
            }
            automaton_in.update_timing_qos(time_qos_data);
            if(count_test ==10)
                automaton_out = fake_output2();
            else
                automaton_out = do1(automaton_in); //Call reconfiguration automaton

            prev_app_output_config = app_output_config; 
            app_output_config = init_output(automaton_out);
            publish_to_MM(check_achievable(sh_mem_access, automaton_out),automaton_out);  

            task_mapping(app_output_config, prev_app_output_config, bts_map, sh_mem_access);
        }
        std::cout << "DONE app Check, Time to sleep " << count_test << std::endl;
        loop_rate.sleep();
        count_test++;
    }
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
   
    if(verbose > VERBOSITY_LOW)
    {
        cout << "There are "<< file_content.size() << " lines in the file" << endl;
        cout << "There are "<< res.size() << " tasks" << endl;
           for (size_t i = 0; i < res.size(); i++)
        {
            cout << "Task " << i << " : " <<  endl;
            res[i].print();
        }
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
   
    if(verbose > VERBOSITY_LOW)
    {
        cout << "There are "<< file_content.size() << " lines in the file" << endl;
        cout << "There are "<< res.size() << " tasks" << endl;
        for (size_t i = 0; i < res.size(); i++)
        {
            cout << "Task " << i << " : " <<  endl;
            res[i].print();
        }
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
        if( map_config_app[i].active == 1
            && map_config_app[i].version_code < MULTI_APP_THRESHOLD_CODE
            && map_config_app[i].region_id != 0)
            for(int j=i+1; j < APPLICATION_NUMBER; j++)
                if(map_config_app[i].region_id == map_config_app[j].region_id
                    && map_config_app[j].active == 1)
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
   
    if(verbose > VERBOSITY_LOW)
    {
        cout << "There are "<< file_content.size() << " lines in the file" << endl;
        cout << "There are "<< res.size() << " Bitstreams" << endl;
        for (size_t i = 0; i < res.size(); i++)
        {
            cout << "App MAP " << i << " : " <<  endl;
            res[i].print();
        }
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

void task_mapping(vector<Map_app_out> const& map_config_app, 
            vector<Map_app_out> const& prev_map_config_app, 
            vector<Bitstream_map> const& bitstream_map, 
            MemoryCoordinator & shared_memory)
{
    vector<App_scheduler> scheduler_array = create_scheduler_tab(map_config_app, prev_map_config_app,bitstream_map);
    std_msgs::Int32 msg;

    //EM, First loop: disable each Task, not only those that have to be stopped,
    //in order to free HW Tiles. This is not a problem since all tasks in 
    //scheduler change of state, involving a Stop at a time.
    cout << endl;
    for(size_t i = 0; i < scheduler_array.size(); i++)
    {
        msg.data = 0; 
        activate_desactivate_task(scheduler_array[i].app_index, msg);	
        cout << "\033[1;31m Disable Task no: " << scheduler_array[i].app_index << "\033[0m"  << endl;

        if(i == scheduler_array.size()-1 || i == scheduler_array.size()-2)
            scheduler_array[i].print();

        if(scheduler_array[i].region_id != 0 && scheduler_array[i].active == 0)
        { 
            //EM, write in shared memory the state of each Tile that are released thanks to HW tasks shutdown
            shared_memory.busy_tile_Write(0, scheduler_array[i].region_id-1); //EM, -1 because sh_vector index starts at 0

            //EM, Stop the ongoing sequence process
            if(active_thread[scheduler_array[i].region_id-1])
            {
                cout << " Disable thread of Tile no: " << scheduler_array[i].region_id << endl;
                stop_sequence(scheduler_array[i].region_id-1);
                active_thread[scheduler_array[i].region_id-1] = false;
            }
        }
    }

    //EM, Second loop: Ensure that all Tiles that are gonna be configured are freed.
    for(size_t i = 0; i < scheduler_array.size(); i++)
    {
        if(scheduler_array[i].region_id != 0 && scheduler_array[i].active ==1) 
        {
            wait_release(scheduler_array[i].app_index, scheduler_array[i].region_id, prev_map_config_app, shared_memory);
        }
    }

    //EM, Third loop: activation of each Task except those in sequence "s"
    for(size_t i = 0; i < scheduler_array.size(); i++)
    {
        //Enable SW tasks
        if(scheduler_array[i].region_id == 0 && scheduler_array[i].active == 1) 
        {
            msg.data = 1; 
            activate_desactivate_task(scheduler_array[i].app_index, msg);
            cout << "\033[1;32m Enable SW version of Task no: " << scheduler_array[i].app_index << "\033[0m" << endl;
        }

        //Enable classic HW tasks
        if(scheduler_array[i].region_id != 0 && scheduler_array[i].active == 1
            && scheduler_array[i].fusion_sequence != "f" && scheduler_array[i].fusion_sequence != "s") 
        {
            //EM, TODO: LOAD bitstream in FPGA!!! 
            msg.data = 2; 
            activate_desactivate_task(scheduler_array[i].app_index, msg);
            cout << "\033[1;36m Enable HW version of Task no: " << scheduler_array[i].app_index 
                    << " in Tile no: " << scheduler_array[i].region_id << "\033[0m" << endl;
            shared_memory.busy_tile_Write(1, scheduler_array[i].region_id-1); //EM, notify the Tile reservation in sh mem
            //EM, -1 because sh_vector index starts at 0
        }

        //Enable HW tasks in fusion "f"
        if(scheduler_array[i].region_id != 0 && scheduler_array[i].active == 1
            && scheduler_array[i].fusion_sequence == "f")
        {
            //EM, TODO: LOAD bitstream in FPGA!!! 
            msg.data = scheduler_array[i].version_code - scheduler_array[i].region_id; 
            //The 2 fusionned tasks will be activated, only one ROS node can understand the msg.data and launch the fusion
            activate_desactivate_task(scheduler_array[i].app_index, msg);
            cout << "\033[36m Enable HW FUSION version of Task no: " << scheduler_array[i].app_index 
                    << " in Tile no: " << scheduler_array[i].region_id << "\033[0m" << endl;
            shared_memory.busy_tile_Write(1, scheduler_array[i].region_id-1); //EM, notify the Tile reservation in sh mem
            //EM, -1 because sh_vector index starts at 0
        }
    }
    cout << endl;
    
    //EM, Forth loop: activation of routines for sequence "s"
    int tile_used[TILE_NUMBER] = {0,0,0};
    for(size_t i = 0; i < scheduler_array.size(); i++)
    {
        if(scheduler_array[i].region_id != 0 
            && scheduler_array[i].active == 1
            && scheduler_array[i].fusion_sequence == "s" 
            && tile_used[scheduler_array[i].region_id-1] == 0)
        {
            sequence_apps[0] = scheduler_array[i];
            for(int j = i+1; j < scheduler_array.size(); j++)
                if(scheduler_array[j].region_id == scheduler_array[i].region_id)
                {
                    sequence_apps[1] = scheduler_array[j];
                    tile_used[scheduler_array[j].region_id-1] = 1;
                }
            cout << "\033[1;34m Enable Sequence version of Tasks no: " << sequence_apps[0].app_index 
                    << " and no " << sequence_apps[1].app_index 
                    << " in Tile no: " << sequence_apps[0].region_id << "\033[0m" << endl;
            sequence_apps[0].print();
            sequence_apps[1].print();
            //LAUNCH SEQUENCE THREAD 
            active_thread[scheduler_array[i].region_id-1] = true;
            sequence_thread[scheduler_array[i].region_id-1] = 
                boost::make_shared<boost::thread>(&sequence_exec_routine, sequence_apps);
        }
    }
}


vector<App_scheduler>	create_scheduler_tab(vector<Map_app_out> const& map_config_app
                                            , vector<Map_app_out> const& prev_map_config_app
                                            , vector<Bitstream_map> const& bitstream_map)
{
    vector<App_scheduler> res;
    App_scheduler tmp;
    //EM, we add in the scheduler only applications with a different configuration from previous doStep()
    for(int i = 0; i < map_config_app.size(); i++)
        if( map_config_app[i].active != prev_map_config_app[i].active //EM, different state OR different version_code
          || map_config_app[i].version_code != prev_map_config_app[i].version_code)
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

    if(verbose > VERBOSITY_OFF)
    {
        cout << "Scheduler array size = " << res.size() << " : " <<  endl;
        for(int i = 0; i < res.size(); i++)
        {
            cout << "Scheduler array [" << i << "] : " <<  endl;
            res[i].print();
        }
    }
    return res;
}

void	wait_release(int app, int region_id, vector<Map_app_out> const& prev_map_config_app
                    , MemoryCoordinator & shared_memory)
{
    for(int app_index = 0; app_index < prev_map_config_app.size(); app_index++)
        if(prev_map_config_app[app_index].region_id == region_id 
            && prev_map_config_app[app_index].active == 1)
            {
                cout << "\033[0;33m Wait END for HW Task no: " << app << "\033[0m"  << endl;
                cout << "SIGNAL WAIT App : " << app_index << endl;
                while(!shared_memory.release_hw_Read(app_index));
                cout << "\033[1;33m Tile no: " << region_id  << " released !!! \033[0m"  << endl;
            }
}


void sequence_exec_routine(App_scheduler seq_app[2])
{
    int current_app = 0;
    std_msgs::Int32 msg;
    MemoryCoordinator shared_memory("User");

    seq_app[0].print();
    seq_app[1].print();

    //EM, notify the Tile reservation in sh mem
    shared_memory.busy_tile_Write(1, seq_app[0].region_id-1); //-1 because sh_vector index starts at 0

    std::chrono::seconds ms(5);
    std::chrono::time_point<std::chrono::system_clock> end;
   
    while(true)
    {
        secured_load_BTS();
        msg.data = 2; //Start app
        activate_desactivate_task(seq_app[current_app].app_index, msg);	
        ROS_INFO("Wait DONE, app_index = ", seq_app[current_app].app_index);
        end = std::chrono::system_clock::now() + ms;
        while(!shared_memory.done_Read(seq_app[current_app].app_index)
                && std::chrono::system_clock::now() < end);
        
        msg.data = 0; //Stop app
        ROS_INFO("Wait app END, app_index = ", seq_app[current_app].app_index);
        end = std::chrono::system_clock::now() + ms;
        activate_desactivate_task(seq_app[current_app].app_index, msg);	
        while(!shared_memory.release_hw_Read(seq_app[current_app].app_index)
                && std::chrono::system_clock::now() < end);

        current_app = 1 - current_app; //EM, to alternate between 1 and 0
    }
}

void secured_load_BTS()
{
    ROS_INFO("Try to load bts");
    /*bip::named_mutex bts_load_mutex{ //EM, add mutex for secured bitsream load
                    bip::open_or_create
                    , MUTEX_NAME_BTS_LOAD};
    //EM, lock access for BTS load in FPGA
    bip::scoped_lock<bip::named_mutex> lock(bts_load_mutex); */
    {
        boost::this_thread::disable_interruption di; //EM, disable interrupt for this thread

        std::this_thread::sleep_for(std::chrono::seconds(3));
        //EM, TODO: PUT here bitstream loading


        ROS_INFO("Bitstream loaded in FPGA!");
    }
}


void stop_sequence(int tile_index)
{
    ROS_INFO("Try to stop sequence in tile %d!", tile_index+1);
	if (sequence_thread[tile_index] != NULL)
	{
		//TODO SOMETHING
        sequence_thread[tile_index]->interrupt();
        sequence_thread[tile_index]->timed_join(boost::posix_time::milliseconds(500));
        //sequence_thread[tile_index]->join();
        pthread_cancel(sequence_thread[tile_index]->native_handle());
        ROS_INFO("Thread Stopped");
	} 
    else
        ROS_INFO("No thread to stop");
}


void compare_data_access_speed(MemoryCoordinator & shared_memory)
{
    clock_t start, ends;
    start = clock();
        read_value_file(PATH_RELEASE_HW,3);
    ends = clock();
    double res = double(ends - start) * 1000 / CLOCKS_PER_SEC;
    std::cout 	<< "READ DATA IN A FILE : " 
                << res << std::endl;

    start = clock();
        int foo = shared_memory.release_hw_Read(2);
    ends = clock();
    res =  double(ends - start) * 1000 / CLOCKS_PER_SEC;
    std::cout 	<< "READ DATA IN A SHARED MEMORY : " 
                << res << std::endl;

    start = clock();
        shared_memory.release_hw_Write(0,2);
    ends = clock();
    res =  double(ends - start) * 1000 / CLOCKS_PER_SEC;
    std::cout 	<< "WRITE DATA IN A SHARED MEMORY : " 
                << res << std::endl;
}

void sh_mem_setup(MemoryCoordinator & shared_memory, vector<Task_in> C3)
{
    std::vector<int> achievable_init;
    std::vector<int> release_hw_init;
    std::vector<int> done_init;
    std::vector<int> busy_tile_init;
    for(int i = 0; i < APPLICATION_NUMBER; i++)
    {
        achievable_init.push_back(1);   //EM, everything achievable by default
        release_hw_init.push_back(1);   //  everything released by default
        done_init.push_back(0);         //  nothing done by default

        if(i < TILE_NUMBER)
            busy_tile_init.push_back(0); //EM, no tile used by default
    }

    std::vector<int> Sh_C3_init;
    for(int i = 0; i < C3.size(); i++)
    {
        Sh_C3_init.push_back(C3[i].req);
        Sh_C3_init.push_back(C3[i].texec);
        Sh_C3_init.push_back(C3[i].mintexec);
        Sh_C3_init.push_back(C3[i].maxtexec);
        Sh_C3_init.push_back(C3[i].qos);
        Sh_C3_init.push_back(C3[i].minqos);
        Sh_C3_init.push_back(C3[i].maxqos);
        Sh_C3_init.push_back(C3[i].priority);
    }
    shared_memory.Fill_ShMem_C3_table(Sh_C3_init);
    shared_memory.Fill_ShMem_achievable(achievable_init);
    shared_memory.Fill_ShMem_release_hw(release_hw_init);
    shared_memory.Fill_ShMem_done(done_init);
    shared_memory.Fill_ShMem_busy_tile(busy_tile_init);
    std::cout << "Fill shared memories, done! " << std::endl;
}

vector<Task_in>	sh_mem_read_C3(MemoryCoordinator & shared_memory)
{
    vector<Task_in> res;
    Task_in tmp;
    vector<int> shared_data;
    for(int i=0; i<APPLICATION_NUMBER; i++)
    {
        shared_data = shared_memory.C3_table_Read(i);
        tmp.req			= shared_data[C3_ACTIVE_REQUEST];
        tmp.texec	    = shared_data[C3_CURRENT_TEXEC];
        tmp.mintexec	= shared_data[C3_MIN_TEXEC];
        tmp.maxtexec	= shared_data[C3_MAX_TEXEC];
        tmp.qos 		= shared_data[C3_CURRENT_QOS];
        tmp.minqos		= shared_data[C3_MIN_QOS];
        tmp.maxqos 		= shared_data[C3_MAX_QOS];
        tmp.priority	= shared_data[C3_PRIORITY];

        res.push_back(tmp);
    }
    return res;
}



vector<App_timing_qos> 	sh_mem_read_time_qos(MemoryCoordinator & shared_memory)
{
    vector<App_timing_qos> res;
    App_timing_qos tmp;
    for(int i=0; i<APPLICATION_NUMBER; i++)
    {
        tmp.texec		= shared_memory.Read_ExecTime(i);
        tmp.qos			= shared_memory.Read_QoS(i);
        res.push_back(tmp);
    }
    return res;
}


void	raz_timing_qos(vector<Task_in> & C3)
{
    for(size_t i = 0; i < C3.size(); i++)
        C3[i].raz_timing_qos();
}

    /*############################## TEST CODE ##############################*/
    /*std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();
    /*############################## TEST CODE ##############################*/
