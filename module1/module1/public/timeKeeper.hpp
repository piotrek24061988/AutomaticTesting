#ifndef _timeKeeper_hpp
#define _timeKeeper_hpp

#include <ctime>

class timeKeeper
{
public:
	timeKeeper();
	~timeKeeper();

	//Compare provided time with minimal expected time needed for device preparation.
	virtual bool getTimeValid(std::time_t curTime);
};

#endif //_timeKeeper_hpp
