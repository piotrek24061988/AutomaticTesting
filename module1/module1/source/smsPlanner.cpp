/** \file smsPlanner.cpp
 * Contains smsPlanner class definition.
 * Class smsPlanner isn't used by any other class as dependency so far as it is top level interface.
 * Class smsPlanner uses timeKeeper and smsSender classes as dependency. 
 */
#include <iostream>
#include <algorithm>

#include "smsPlanner.hpp"

/**
 * Constructor taking over timeKeeper which will be used to verify send time
 * and smsSender which will be used to send sms messages to dedicated numer.
 * @param tk as std::shared_ptr<timeKeeper> which is used to verify send time.
 * @param sd as std::shared_ptr<smsSender> which is used to send sms messages 
 * to dedicated numer.
 * @see addDelivery()
 * @see sendAll()
 */
smsPlanner::smsPlanner(std::shared_ptr<timeKeeper> tk, std::shared_ptr<smsSender> sd): tk(tk), sd(sd)
{
	std::cout << "smsPlanner::smsPlanner(std::shared_ptr<timeKeeper> tk, std::shared_ptr<smsSender> sd)" << std::endl;

	id = 0;
}

/**
 * Standard destructor.
 * Just print some debug data.
 */
smsPlanner::~smsPlanner()
{
	std::cout << "smsPlanner::~smsPlanner()" << std::endl;
}

/**
 * Add sms message to backlog (create delivery) to be send to dedicated number.
 * @param targetNumber as std::string which is requested number to receive message.
 * @param message as std::string which is requested message to be send.
 * @param sendTime as std::time_t which is requested send time to be check.
 * @see sendAll()
 * @return id as int of prepared delivery if preparation was successful or -1 in not.
 */
int smsPlanner::addDelivery(std::string targetNumber, std::string message, std::time_t sendTime)
{
	std::cout << "int smsPlanner::addDelivery(std::string targetNumber, std::string message, std::time_t sendTime)" << std::endl;

	if(tk->getTimeValid(sendTime))
	{
		ids.push_back(++id);
		contents.insert(std::make_pair(id, std::make_pair(targetNumber, message)));
		return id; 
	}
	return -1;
}

/**
 * Remove sms message from backlog (cancel created delivery) to be send to dedicated number.
 * @param curId as int which is id of prepared delivery.
 * @see addDelivery()
 * @return true if sms message was successfully removed from backlog or otherwise return false.
 */
bool smsPlanner::cancelDelivery(int curId)
{
	std::cout << "bool smsPlanner::cancelDelivery(int curId)" << std::endl;

	auto it = std::find(ids.begin(), ids.end(), curId);
	if(it != ids.end())
	{
		ids.erase(it);	
		contents.erase(*it);
		return true;	
	}
	return false;
}

/**
 * Send backlog content (sms messages) to dedicated receivers (numbers) and clean backlog
 * from each delivery which is succesfully send.
 * @see addDelivery()
 * @return true if whole backlog was succesfully send or otherwise return false.
 */
bool smsPlanner::sendAll()
{
	std::cout << "bool smsPlanner::sendAll()" << std::endl;

	for(auto it = contents.begin(); it != contents.end(); )
	{
		if(!sd->send(it->second.first, it->second.second))
		{
			return false;
		}
		ids.erase(std::remove(ids.begin(), ids.end(), it->first), ids.end());
		auto itErase = it++;
		contents.erase(itErase);
	}
	id = 0;
	return true;
}
