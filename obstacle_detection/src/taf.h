#ifndef TAF_H
#define TAF_H

class taf
{
	public:
	
		taf();
		~taf();
		
		double update( double ml, double nl, double m, double nr, double mr, double T);
		
	private:
		
		int w1;
		int w2;
};
#endif