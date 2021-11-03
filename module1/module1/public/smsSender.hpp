#ifndef _smsSender_hpp
#define _smsSender_hpp

#include <string>
#include <memory>

#include "smsDevice.hpp"

class smsSender
{
public:
	smsSender(std::shared_ptr<smsDevice> sd);
	~smsSender();

	//Send sms with the message to the tel number.
	virtual bool send(std::string number, std::string message);

private:
	std::shared_ptr<smsDevice> sd;
};

#endif //_smsSender_hpp
