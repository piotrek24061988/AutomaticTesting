#include <iostream>

#include "smsSender.hpp"

smsSender::smsSender(std::shared_ptr<smsDevice> sd): sd(sd)
{
}

smsSender::~smsSender()
{
}

//Send sms with the message to the tel number.
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
