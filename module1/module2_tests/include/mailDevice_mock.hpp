#ifndef _mailDevice_mock_hpp
#define _mailDevice_mock_hpp

#include "gmock/gmock.h"

#include "mailDevice.hpp"

#include <string>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class mailDevice_mock : public mailDevice
{
public:

	MOCK_METHOD0(openDriver, FILE *());
	MOCK_METHOD1(closeDriver, int(FILE *));
	MOCK_METHOD5(sendMail, int(FILE * id, std::string to, std::string from, std::string subject, std::string message));

};

#endif //_mailDevice_mock_hpp
