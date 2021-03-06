#ifndef _smsPlanner_hpp
#define _smsPlanner_hpp

#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <utility>

#include "timeKeeper.hpp"
#include "smsSender.hpp"

class smsPlanner
{
public:
	smsPlanner(timeKeeper * tk, smsSender * sd);
	~smsPlanner();

	//Add new delivery and return delivery ID or -1 if error.
	int addDelivery(std::string targetNumber, std::string message, std::time_t sendTime);
	//Cancel delivery and return true if ok. 
	bool cancelDelivery(int curId);
	//Send all added sms's.
	bool sendAll();

private:
	int id;
	std::vector<int> ids;
	std::map<int, std::pair<std::string, std::string>> contents;
	
	timeKeeper * tk;
	smsSender * sd;
};

#endif //_smsPlanner_hpp
