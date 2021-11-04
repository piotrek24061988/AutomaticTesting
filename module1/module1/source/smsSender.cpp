/** \file smsSender.cpp
 * Contains smsSender class definition.
 * Class smsSender is used by smsPlanner class as dependency.
 * Class smsSender uses smsDevice class as dependency. 
 */
#include <iostream>

#include "smsSender.hpp"

/**
 * Constructor taking over smsDevice which will be used to send sms messages.
 * @param sd as std::shared_ptr<smsDevice> which is used to send sms messages.
 * @see send()
 */
smsSender::smsSender(std::shared_ptr<smsDevice> sd): sd(sd)
{
	std::cout << "smsSender::smsSender(std::shared_ptr<smsDevice> sd)" << std::endl;
}

/**
 * Standard destructor.
 * Just print some debug data.
 */
smsSender::~smsSender()
{
	std::cout << "smsSender::~smsSender()" << std::endl;
}

/**
 * Send sms with the message to provided number.
 * @param number as std::string which is requested number to receive message.
 * @param message as std::string which is requested message to be send.
 * @return true if sending was successful or otherwise return false.
 */
bool smsSender::send(std::string number, std::string message)
{
	std::cout << "bool smsSender::send(std::string number, std::string message)" << std::endl;

	if(!sd->init())
	{
		return false;
	}

	if(!sd->send(number, message))
	{
		return false;
	}

	if(!sd->deInit())
	{
		return false;
	}

	return true;
}
