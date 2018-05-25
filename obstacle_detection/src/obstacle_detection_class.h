#ifndef OBSTACLE_DETECTION_CLASS_H
#define OBSTACLE_DETECTION_CLASS_H

# include <ros/ros.h>
#include "taf.h"
# include "communication/obstacle_detection_msg.h"

class obstacle_detection_class
{
	public:
		obstacle_detection_class();
		~obstacle_detection_class();
		
		//fonctions

		void set_parameters(float in_a,float in_b,float in_c,float in_d,float in_e,float in_q,float in_t);
		
		communication::obstacle_detection_msg publish();
		
		void setIRS(std::vector<float> in_irs);
		
		void setIRL(std::vector<float> in_irl);
		
		void setUS(std::vector<float> in_us);
		
		void setOF(float in_x, float in_y);
		
		void setIMU(float in_alpha, float in_beta);
		
		void setSeuil(float in_seuil);
		
		void setMaxs(float max_irs, float max_irl,float min_irl);
		//variables
		
	private:
		
		//fonctions
		
		std::vector<float> compute(std::vector<float> L, std::vector<float> S, std::vector<float> U, std::vector<float> E, std::vector<float> O);
		
		float irwf( float v1, float v2, float v3, float v4, float v5, float v6, float v7);
		
		float wf( float v1, float v2, float v3, float v4, float v5, float v6);
		
		//variables
		
		float a;
		float b;
		float c;
		float d;
		float e;
		float q;
		float t;
		
		float x;
		float y;
		float alpha;
		float beta;
		
		float seuil;
		float Smax;
		float Lmax;
		float Lmin;
		
		std::vector<float> us;
		std::vector<float> irl;
		std::vector<float> irs;
		
		std::vector<float> old_L ;
		std::vector<float>  old_S;
		std::vector<float> old_D;
		std::vector<float> old_U;
		std::vector<float> old_I;
		std::vector<float> old_WL;
		std::vector<float> old_WS;
		std::vector<float> old_WI;
		std::vector<float> old_WU;
};
#endif
