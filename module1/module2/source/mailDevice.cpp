#include "mailDevice.hpp"
#include <iostream>

mailDevice::mailDevice()
{
}

mailDevice::~mailDevice()
{
}

//Open driver before sending message.
//Return NULL if failed.
FILE * mailDevice::openDriver()
{
	std::cout << "FILE * mailDevice::openDriver()" << std::endl;

	return popen("/usr/lib/sendmail -t", "w");
}

//Send message to email address from email account with following subject.
//Return -1 if failed.
int mailDevice::sendMail(FILE * id, std::string to, std::string from, std::string subject, std::string message)
{
	std::cout << "int mailDevice::sendMail(FILE *, std::string, std::string, std::string, std::string)" << std::endl;

        int retval = -1;
        if (id != NULL) 
        {
            fprintf(id, "To: %s\n", to.c_str());
            fprintf(id, "From: %s\n", from.c_str());
            fprintf(id, "Subject: %s\n\n", subject.c_str());
            fwrite(message.c_str(), 1, message.size(), id);
            fwrite("\n", 1, 2, id);
            retval = 0;
        }
        return retval;
}

//Close driver at the end of work.
//Return -1 if failed.
int mailDevice::closeDriver(FILE * id)
{
	std::cout << "int mailDevice::closeDriver(FILE *)" << std::endl;

	if(id != NULL)
	{
		return pclose(id);
	}
	else
	{
		return -1;
	}
}
