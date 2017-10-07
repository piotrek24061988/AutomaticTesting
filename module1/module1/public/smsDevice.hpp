#ifndef _smsDevice_hpp
#define _smsDevice_hpp

#include <string>

class smsDevice
{
public:
	smsDevice();
	~smsDevice();

	//Init device before sending message.
	virtual bool init();
	//Send sms with the message to the tel number.
	virtual bool send(std::string number, std::string message);
	//Deinit device when at the end of work.
	virtual bool deInit();

private:
	bool initialized;
};

#endif //_smsDevice_hpp
