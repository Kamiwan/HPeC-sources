#include "fake_sensors_class.h"

#include<vector>
#include<ros/ros.h>

fake_sensors_class::fake_sensors_class()
{
	std::vector<float> temp0(6,0);
	my_US = temp0;
	my_IRS = temp0;
	my_IRL = temp0;
	changed = false;
}

fake_sensors_class::~fake_sensors_class()
{
}

void fake_sensors_class::setIRS(float value,int sector)
{
	my_IRS[sector-1]=value;
	changed = true;
	return;
}

void fake_sensors_class::setIRL(float value,int sector)
{
	my_IRL[sector-1]=value;
	changed = true;
	return;
}

void fake_sensors_class::setUS(float value,int sector)
{
	my_US[sector-1]=value;
	changed = true;
	return;
}		

bool fake_sensors_class::hasChanged()
{
	return changed;
}

communication::fake_sensors_msg fake_sensors_class::toPublish()
{
	communication::fake_sensors_msg my_msg;
	my_msg.US = my_US;
	my_msg.IRS = my_IRS;
	my_msg.IRL = my_IRL;
	changed = false;
	return my_msg;
}
