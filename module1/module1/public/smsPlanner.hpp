#ifndef _smsPlanner_hpp
#define _smsPlanner_hpp

#include <string>
#include <ctime>

class smsPlanner
{
public:
	smsPlanner();
	~smsPlanner();

	int addDelivery(std::string targetNumber, std::string message, std::time_t time);
};



#endif //_smsPlanner_hpp
