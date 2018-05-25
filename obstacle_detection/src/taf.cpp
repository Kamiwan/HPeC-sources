#include "taf.h"

#include <cstdlib>
#include <cfloat>
taf::taf()
{
	w1=0;
	w2=0;
}

taf::~taf()
{
}

double taf::update( double ml, double nl, double m, double nr, double mr, double T)
{
	int w0;
	bool b1 = false;
	bool b2 = false;
	if (ml < T*nl)
	{
		b1=true;
	}
	if (mr < T*nr)
	{
		b2=true;
	}
	if (m < nl || b1)
	{
		if(nl<nr || b2)
		{
			w0=0;
		}
		else
		{
			w0= 1;
		}
	}
	else
	{
		if(nl<nr)
		{
			w0=-1;
		}
		else
		{
			w0= 1;
		}
	}
	int S = w0 + w1 + w2;
	w2 = w1;
	w1 = w0;
	double d = m;
	if (S <-1)
	{
		d = nl;
	}
	if (S >1)
	{
		d = nr;
	}
	return d;
}