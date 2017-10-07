#ifndef _smsDevice_mock_hpp
#define _smsDevice_mock_hpp

#include "gmock/gmock.h"

#include "smsDevice.hpp"

#include <string>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class smsDevice_mock : public smsDevice
{
public:
	smsDevice_mock()
	{
#ifdef IntegrationTests
		ON_CALL(*this, send(_, _)).WillByDefault(Invoke(&real_, &smsDevice::send));
		ON_CALL(*this, init()).WillByDefault(Invoke(&real_, &smsDevice::init));
		ON_CALL(*this, deInit()).WillByDefault(Invoke(&real_, &smsDevice::deInit));
#endif
	}

	MOCK_METHOD2(send, bool(std::string number, std::string message));
	MOCK_METHOD0(init, bool());
	MOCK_METHOD0(deInit, bool());

private:
	smsDevice real_;
};

#endif //_smsDevice_mock_hpp
