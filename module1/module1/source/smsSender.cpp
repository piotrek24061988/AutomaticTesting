#include "smsSender.hpp"
#include <iostream>

smsSender::smsSender()
{
}

smsSender::~smsSender()
{
}

//Send sms with the message to the tel number.
bool smsSender::send(std::string number, std::string message)
{
	std::cout << "bool smsSender::send(std::string number, std::string message)" << std::endl;

	return true;
}
