#ifndef _smsPlanner_hpp
#define _smsPlanner_hpp

#include <string>
#include <ctime>
#include <vector>

class smsPlanner
{
public:
	smsPlanner();
	~smsPlanner();

	//Add new delivery and return delivery ID or -1 if error
	int addDelivery(std::string targetNumber, std::string message, std::time_t time);
	//Cancel delivery and return true if ok. 
	bool cancelDelivery(int curId);

private:
	int id;
	std::vector<int> ids;
};



#endif //_smsPlanner_hpp
