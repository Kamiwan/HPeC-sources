//TODO Insert header comment

#include "adaptation_manager_struct.h"

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
	std::cout 	<< "req = " << req << std::endl
				<< "texec = " << texec << std::endl
				<< "mintexec = " << mintexec << std::endl
				<< "maxtexec = " << maxtexec << std::endl
				<< "qos = " << qos << std::endl
				<< "minqos = " << minqos << std::endl
				<< "maxqos = " << maxqos << std::endl
				<< "priority = " << priority << std::endl;
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
	//EM, I know it's dirty, it would have been better with an array of attributes...
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
	//EM, I know it's dirty, it would have been better with an array of attributes...
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


void App_timing_qos::print(){
	std::cout 	<< "texec = " << texec << std::endl
				<< "qos = " << qos << std::endl;
}


std::string Map_app_out::set_fusion(Map_app_out const& config_app)
{
	if(config_app.active != 0 && config_app.region_id != 0)
	{
		if(config_app.version_code >= MULTI_APP_THRESHOLD_CODE)
			return "f";
		else 
			return "-";
	}
	else
		return "-";
}

Map_app_out& Map_app_out::operator=(Task_out const& rhs)
{
	Map_app_out curr_app_out; //EM, intermediate variable to set fusion_sequence
	curr_app_out.active			= rhs.act;
	curr_app_out.version_code	= rhs.code;
	curr_app_out.region_id		= rhs.code %10;

	active			= rhs.act;
	version_code	= rhs.code;
	region_id		= curr_app_out.region_id;
	fusion_sequence	= set_fusion(curr_app_out); //EM, function needed for this attribute
	return *this;
}

void Map_app_out::init()
{
	active			= 0;
	version_code	= 0;
	region_id		= 0;
	fusion_sequence	= "";
}



void Bitstream_map::print()
{
	std::cout 	<< "version_code = " << version_code << std::endl
				<< "bitstream_addr = " << bitstream_addr << std::endl;
}

void App_scheduler::print()
{
	std::cout 	<< "app_index = " << app_index << std::endl
				<< "active = " << active << std::endl
				<< "version_code = " << version_code << std::endl
				<< "region_id = " << region_id << std::endl
				<< "fusion_sequence = " << fusion_sequence << std::endl
				<< "bitstream_addr = " << bitstream_addr << std::endl
				<< "loaded = " << loaded << std::endl
				<< "done = " << done << std::endl;
}


App_scheduler& App_scheduler::operator=(Map_app_out const& rhs)
{
	active			= rhs.active;
	version_code 	= rhs.version_code;
	region_id 		= rhs.region_id;
	fusion_sequence = rhs.fusion_sequence;
	return *this;
}

