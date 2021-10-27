#include "mailPlanner.hpp"

#include <algorithm>

mailPlanner::mailPlanner(mailSender * ms): ms(ms)
{
	id = 0;
}

mailPlanner::~mailPlanner()
{
}

//Add new delivery and return delivery ID or -1 if error.
int mailPlanner::addDelivery(std::string from, std::string to, std::string topic, std::string content)
{
	ids.push_back(++id);

	std::vector<std::string> vec;
	vec.push_back(from);
	vec.push_back(to);
	vec.push_back(topic);
	vec.push_back(content);
	contents.insert(std::make_pair(id, vec));

	return id; 
}

//Cancel delivery and return true if ok. 
bool mailPlanner::cancelDelivery(int curId)
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

//Send all added mail's.
//BUG to demonstrate testing beauty
bool mailPlanner::sendAll()
{
	for(auto it = contents.begin(); it != contents.end(); it++)
	{
		if(ms->send(it->second[0], it->second[1], it->second[2], it->second[3]) == -1)
		{
			return false;
		}
		contents.erase(it);
                ids.erase(std::remove(ids.begin(), ids.end(), it->first), ids.end());	
	}
	return true;
}
