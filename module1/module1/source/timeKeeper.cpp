#include <iostream>

#include "timeKeeper.hpp"

timeKeeper::timeKeeper()
{
}

timeKeeper::~timeKeeper()
{
}

//Compare provided time with minimal expected time needed for device preparation.
bool timeKeeper::getTimeValid(std::time_t curTime)
{
	std::cout << "bool timeKeeper::getTimeValid(std::time_t curTime)" << std::endl;

	if(curTime > time(NULL))
	{
		return true;
	}
	return false;
}
