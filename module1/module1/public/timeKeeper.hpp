/** \file timeKeeper.hpp
 * Contains timeKeeper class declaration.
 * Class timeKeeper is used by smsPlanner class as dependency.
 * Class timeKeeper doesn't use any other classes dependency. 
 */
#ifndef _timeKeeper_hpp
#define _timeKeeper_hpp

#include <ctime>

/**
 *  A timeKeeper class.
 *  Provides functionality to check if requested time is valid and far enough for device
 *  to initialize and send a massege. 
 */
class timeKeeper
{
public:
	/**
	 * Standard constructor.
	 * Just print some debug data.
	 */
	timeKeeper();

	/**
	 * Standard destructor.
	 * Just print some debug data.
	 */
	~timeKeeper();

	/**
	 * Compare provided time with minimal expected time needed for device preparation.
	 * @param curTime as std::time_t which is requested time to be check.
	 * @return true if requested time is valid and far enough or otherwise return false.
	 */
	virtual bool getTimeValid(std::time_t curTime);
};

#endif //_timeKeeper_hpp
