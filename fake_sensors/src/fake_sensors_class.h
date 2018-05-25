#ifndef FAKE_SENSORS_CLASS_H
#define FAKE_SENSORS_CLASS_H

# include <ros/ros.h>
# include "communication/fake_sensors_msg.h"

class fake_sensors_class
{
	public:
		fake_sensors_class();
		~fake_sensors_class();
		
		void setIRS(float value,int sector);
		void setIRL(float value,int sector);
		void setUS(float value,int sector);
		
		bool hasChanged();
		
		communication::fake_sensors_msg toPublish();
		
	private:
	
		std::vector<float> my_US ;
		std::vector<float> my_IRS;
		std::vector<float> my_IRL;
		bool changed;
};
#endif