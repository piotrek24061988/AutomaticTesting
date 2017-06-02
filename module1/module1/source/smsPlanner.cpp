#include "smsPlanner.hpp"

#include <algorithm>

smsPlanner::smsPlanner() 
{
	id = 0;
}

smsPlanner::~smsPlanner()
{
}

int smsPlanner::addDelivery(std::string targetNumber, std::string message, std::time_t time)
{
	id++;
	ids.push_back(id);
	return id; 
}

bool smsPlanner::cancelDelivery(int curId)
{
	auto it = std::find(ids.begin(), ids.end(), curId);
	if(it != ids.end())
	{
		ids.erase(it);	
		return true;		
	}
	return false;
}
