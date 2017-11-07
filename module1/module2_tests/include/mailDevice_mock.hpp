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
	mailDevice_mock()
	{
#ifdef IntegrationTests
		ON_CALL(*this, openDriver()).WillByDefault(Invoke(&real_, &mailDevice::openDriver));
		ON_CALL(*this, closeDriver(_)).WillByDefault(Invoke(&real_, &mailDevice::closeDriver));
		ON_CALL(*this, sendMail(_, _, _, _, _)).WillByDefault(Invoke(&real_, &mailDevice::sendMail));
#endif
	}

	MOCK_METHOD0(openDriver, FILE *());
	MOCK_METHOD1(closeDriver, int(FILE *));
	MOCK_METHOD5(sendMail, int(FILE * id, std::string to, std::string from, std::string subject, std::string message));

private:
	mailDevice real_;
};

#endif //_mailDevice_mock_hpp
