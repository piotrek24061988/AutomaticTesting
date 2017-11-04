#ifndef _mailPlanner_hpp
#define _mailPlanner_hpp

#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <utility>

#include "mailSender.hpp"

class mailPlanner
{
public:
	mailPlanner(mailSender * ms);
	~mailPlanner();

	//Add new delivery and return delivery ID or -1 if error.
	int addDelivery(std::string from, std::string to, std::string topic, std::string content);
	//Cancel delivery and return true if ok. 
	bool cancelDelivery(int curId);
	//Send all added mail's.
	bool sendAll();

private:
	int id;
	std::vector<int> ids;
	std::map<int, std::vector<std::string>> contents;
	
	mailSender * ms;
};

#endif //_mailPlanner_hpp
