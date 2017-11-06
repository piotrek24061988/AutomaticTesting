#ifndef _mailDevice_hpp
#define _mailDevice_hpp

#include <string>
#include <cstdio>

class mailDevice
{
public:
	mailDevice();
	~mailDevice();

	//Open driver before sending message.
        //Return NULL if failed.
	virtual FILE * openDriver();
	//Send message to email address from email account with following subject.
        //Return -1 if failed.
	virtual int sendMail(FILE * id, std::string to, std::string from, std::string subject, std::string message);
	//Close driver at the end of work.
        //Return -1 if failed.
	virtual int closeDriver(FILE * id);
};

#endif //_mailDevice_hpp
