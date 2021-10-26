#include "smsPlanner.hpp"

#include <algorithm>

smsPlanner::smsPlanner(timeKeeper * tk, smsSender * sd): tk(tk), sd(sd)
{
	id = 0;
}

smsPlanner::~smsPlanner()
{
}

//Add new delivery and return delivery ID or -1 if error.
int smsPlanner::addDelivery(std::string targetNumber, std::string message, std::time_t sendTime)
{
	if(tk->getTimeValid(sendTime))
	{
		ids.push_back(++id);
		contents.insert(std::make_pair(id, std::make_pair(targetNumber, message)));
		return id; 
	}
	return -1;
}

//Cancel delivery and return true if ok. 
bool smsPlanner::cancelDelivery(int curId)
{
	auto it = std::find(ids.begin(), ids.end(), curId);
	if(it != ids.end())
	{
		ids.erase(it);	
		contents.erase(*it);
		return true;	
	}
	return false;
}

//Send all added sms's.
bool smsPlanner::sendAll()
{
	for(auto it = contents.begin(); it != contents.end(); it++)
	{
		if(!sd->send(it->second.first, it->second.second))
		{
			return false;
		}
		contents.erase(it);
                auto a = std::find(ids.begin(), ids.end(), it->first);//
                //ids.erase(std::remove(ids.begin(), ids.end(), it->first), ids.end());
                ids.erase(a);//
	}
	return true;
}
