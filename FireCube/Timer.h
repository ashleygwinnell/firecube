#ifndef __TIMER_H
#define __TIMER_H

/**
* A class for a timer.
*/
class FIRECUBE_API Timer
{
public:
	/**
	* Initializes the timer.
	*/
	void Init();
	/**
	* Resets the timer.
	*/
	void Update();
	/**
	* Returns the time that has passed since the last call to Update() in seconds.
	*/
	double Passed();
	
	LARGE_INTEGER now;
	LARGE_INTEGER temp;
	double res;
};
#endif