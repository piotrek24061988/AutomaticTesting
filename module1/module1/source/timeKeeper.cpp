/** \file timeKeeper.cpp
 * Contains timeKeeper class definition.
 * Class timeKeeper is used by smsPlanner class as dependency.
 * Class timeKeeper doesn't use any other classes dependency.
 */
#include <iostream>

#include "timeKeeper.hpp"

/**
 * Standard constructor.
 * Just print some debug data.
 */
timeKeeper::timeKeeper()
{
	std::cout << "timeKeeper::timeKeeper()" << std::endl;
}

/**
 * Standard destructor.
 * Just print some debug data.
 */
timeKeeper::~timeKeeper()
{
	std::cout << "timeKeeper::~timeKeeper()" << std::endl;
}

/**
 * Compare provided time with minimal expected time needed for device preparation.
 * @param curTime as std::time_t which is requested time to be check.
 * @return true if requested time is valid and far enough or otherwise return false.
 */
bool timeKeeper::getTimeValid(std::time_t curTime)
{
	std::cout << "bool timeKeeper::getTimeValid(std::time_t curTime)" << std::endl;

	if(curTime > time(NULL))
	{
		return true;
	}
	return false;
}
