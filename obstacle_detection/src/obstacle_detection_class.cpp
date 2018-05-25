#include "obstacle_detection_class.h"
#include "taf.h"

#include <cstdlib>
#include<vector>
#include<math.h>
#include<ros/ros.h>
# include "communication/obstacle_detection_msg.h"
obstacle_detection_class::obstacle_detection_class()
{
	a=1;
	b=1;
	c=1;
	d=1;
	e=0.5;
	q=1;
	t=1;
	x = 0;
	y = 0;
	alpha = 0;
	beta = 0;
	std::vector<float> temp0(6,0);
	irs = temp0;
	irl = temp0;
	us = temp0;
	old_L = temp0;
	old_S = temp0;
	old_D = temp0;
	old_U = temp0;
	old_I = temp0;
	std::vector<float> temp1(6,1);
	old_WL = temp1;
	old_WS = temp1;
	old_WI = temp1;
	old_WU = temp1;
	
}

obstacle_detection_class::~obstacle_detection_class()
{
}

void obstacle_detection_class::set_parameters(float in_a,float in_b,float in_c,float in_d,float in_e,float in_q,float in_t)
{
	a=in_a;
	b=in_b;
	c=in_c;
	d=in_d;
	e=in_e;
	q=in_q;
	t=in_t;
}

void obstacle_detection_class::setIRS(std::vector<float> in_irs)
{
	irs = in_irs;
}

void obstacle_detection_class::setIRL(std::vector<float> in_irl)
{
	irl = in_irl;
}

void obstacle_detection_class::setUS(std::vector<float> in_us)
{
	us = in_us;
}

void obstacle_detection_class::setOF(float in_x, float in_y)
{
	x= in_x;
	y = in_y;
}

void obstacle_detection_class::setIMU(float in_alpha, float in_beta)
{
	alpha= in_alpha;
	beta = in_beta;
}

void obstacle_detection_class::setSeuil(float in_seuil)
{
	seuil = in_seuil;
}

void obstacle_detection_class::setMaxs(float max_irs, float max_irl, float min_irl)
{
	Smax = max_irs;
	Lmax = max_irl;
	Lmin = min_irl;
}

communication::obstacle_detection_msg obstacle_detection_class::publish()
{
	std::vector<float> new_O(6,0);
	std::vector<float> new_E(6,0);
	for (int i=0; i<6; i++)
	{
		new_O[i]=x*cos(-i*M_PI/3) + y*sin(-i*M_PI/3);
		new_E[i]= 9.81 * 0.01 * (sin(alpha)*sin(-i*M_PI/3) + sin(beta)*cos(i*M_PI/3)); 
	}
	std::vector<float> new_D = compute(irl, irs, us, new_E, new_O);
	
	old_L = irl;
	old_S = irs;
	old_U = us;
	old_D = new_D;
	for (int i = 0; i<6; i++)
	{
		new_D[i]=round(100*new_D[i])/100;
	}
	communication::obstacle_detection_msg my_msg;
	my_msg.distances = new_D;
	if (new_D[0]<seuil)
	{
		if (new_D[1]<seuil)
		{
			my_msg.direction=2;
		}
		else
		{
			my_msg.direction=1;
		}
	}
	else
	{
		my_msg.direction=0;
	}
	return my_msg;
}

std::vector<float> obstacle_detection_class::compute(std::vector<float> L, std::vector<float> S, std::vector<float> U, std::vector<float> E, std::vector<float> O)
{
	std::vector<float> new_WL(6,0);
	std::vector<float> new_WS(6,0);
	std::vector<float> new_I(6,0);
	std::vector<float> new_WI(6,0);
	std::vector<float> new_WU(6,0);
	std::vector<float> output(6,0);
	for (int i=0; i<6; i++)
	{
		if ( (S[i]>=Smax)&&(L[i]>=S[i])&&(L[i]>=Lmax)&&(U[i]>=L[i]) )
		{
			output[i]=Lmax;
			new_I[i] = L[i];
		}
		else
		{
			new_WL[i] = irwf(old_L[i], E[i], L[i], O[i],old_D[i], U[i], old_WL[i]);
			new_WS[i] = irwf(old_S[i], E[i], S[i], O[i],old_D[i], U[i], old_WS[i]);
			if ((S[i]>=Smax)&&(L[i]>=S[i]))
			{
				new_I[i] = L[i];
				old_WL[i]=new_WL[i];
			}
			else if ((L[i]<=Lmin)&&(S[i]<=L[i]))
			{
				new_I[i] = S[i];
				old_WS[i]=new_WS[i];
			}			
			else
			{
				if ((abs(new_WS[i] - new_WL[i])) / (std::max(new_WS[i], new_WL[i]))<0.1)
				{
					new_I[i] = (new_WS[i] * L[i] + new_WL[i] * S[i]) / (new_WS[i] + new_WL[i]);
				}
				else
				{
					if (new_WL[i]< new_WS[i])
					{
						new_I[i] = L[i];
					}
					else
					{
						new_I[i] = S[i];
					}
				}
				old_WL[i]=new_WL[i];
				old_WS[i]=new_WS[i];
			}
			
			new_WI[i] = wf(old_I[i], E[i], new_I[i], O[i],old_D[i], old_WI[i]);
			new_WU[i] = wf(old_U[i], E[i], U[i], O[i],old_D[i], old_WU[i]);
			if ((abs(new_WU[i] - new_WI[i])) / (std::max(new_WU[i], new_WI[i]))<0.1)
			{
				output[i] = (new_WU[i] * new_I[i] + new_WI[i] * U[i]) / (new_WU[i] + new_WI[i]);
			}
			else
			{
				if (new_WU[i]< new_WI[i])
				{
					output[i] = U[i];
				}
				else
				{	
					output[i] = new_I[i];
				}
			}
			old_WI[i]=new_WI[i];
			old_WU[i]=new_WU[i];
		}
		old_I[i]=new_I[i];
	}
	
	return output;
}
//old_L[i], E[i], L[i], O[i],old_D[i], U[i], old_WL[i]
float obstacle_detection_class::irwf( float v1, float v2, float v3, float v4, float v5, float v6, float v7)
{
	float N1 = abs(v1 + v2 - v3);
	float N2 = abs(v1 + v4 - v3);
	float N3 = abs(v5 + v2 - v3);
	float N4 = abs(v5 + v4 - v3);
	float N5 = abs(v6 - v3);
	return ( e * ( a * ( N1 + b * q * N2) + c * ( N3 + b * q * N4) + d * N5 ) + ( 1 - e) * v7 );
}

float obstacle_detection_class::wf( float v1, float v2, float v3, float v4, float v5, float v6)
{
	float N1 = abs(v1 + v2 - v3);
	float N2 = abs(v1 + v4 - v3);
	float N3 = abs(v5 + v2 - v3);
	float N4 = abs(v5 + v4 - v3);
	return ( e * ( a * ( N1 + b * q * N2) + c * ( N3 + b * q * N4) ) + ( 1 - e) * v6 );
}
