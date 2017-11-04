#ifndef _mailSender_hpp
#define _mailSender_hpp

#include <string>

#include "mailDevice.hpp"

class mailSender
{
public:
	mailSender(mailDevice * sd);
	~mailSender();

	//Send message to email address from email account with following subject.
        //Return -1 if failed.
	virtual int send(std::string from, std::string to, std::string topic, std::string content);

private:
	mailDevice * md;
};

#endif //_mailSender_hpp
