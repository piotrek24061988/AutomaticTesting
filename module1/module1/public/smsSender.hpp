#ifndef _smsSender_hpp
#define _smsSender_hpp

#include <string>

class smsSender
{
public:
	smsSender();
	~smsSender();

	//Send sms with the message to the tel number.
	virtual bool send(std::string number, std::string message);
};

#endif //_smsSender_hpp
