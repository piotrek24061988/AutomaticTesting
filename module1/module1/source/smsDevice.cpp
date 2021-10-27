#include <iostream>

#include "smsDevice.hpp"

smsDevice::smsDevice()
{
	initialized = false;
}

smsDevice::~smsDevice()
{
}

//Init device before sending message.
bool smsDevice::init()
{
	std::cout << "bool smsDevice::init()" << std::endl;
	initialized = true;
	return true;
}

//Send sms with the message to the tel number.
bool smsDevice::send(std::string number, std::string message)
{
	std::cout << "bool smsDevice::send(std::string number, std::string message)" << std::endl;
	if(initialized)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Deinit device before sending message.
bool smsDevice::deInit()
{
	std::cout << "bool smsDevice::deInit()" << std::endl;
	initialized = false;
	return true;
}
