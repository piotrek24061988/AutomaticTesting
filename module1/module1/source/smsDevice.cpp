/** \file smsDevice.cpp
 * Contains smsDevice class definition.
 * Class smsDevice is used by smsSender class as dependency.
 * Class smsDevice doesn't use any other classes dependency. 
 */
#include <iostream>

#include "smsDevice.hpp"

/**
* Standard constructor.
* Initialize private members.
*/
smsDevice::smsDevice()
{
	std::cout << "smsDevice::smsDevice()" << std::endl;

	initialized = false;
}

/**
 * Standard destructor.
 * Just print some debug data.
 */
smsDevice::~smsDevice()
{
	std::cout << "smsDevice::~smsDevice()" << std::endl;
}

/**
 * Init device before sending a message.
 * @see send()
 * @return true if initialization was successful or otherwise return false.
 */
bool smsDevice::init()
{
	std::cout << "bool smsDevice::init()" << std::endl;
	initialized = true;
	return true;
}

/**
 * Send sms with the message to provided number.
 * @param number as std::string which is requested number to receive message.
 * @param message as std::string which is requested message to be send.
 * @return true if sending was successful or otherwise return false.
 */
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

/**
 * Deinit device at the end of work.
 * @return true if deinitialization was successful or otherwise return false.
 */
bool smsDevice::deInit()
{
	std::cout << "bool smsDevice::deInit()" << std::endl;
	initialized = false;
	return true;
}
