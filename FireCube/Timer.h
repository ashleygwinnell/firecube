#ifndef __TIMER_H
#define __TIMER_H
class FIRECUBE_API Timer
{
public:
	void Init();
	void Update();
	double Passed();
	
	LARGE_INTEGER now;
	LARGE_INTEGER temp;
	double res;
};
#endif