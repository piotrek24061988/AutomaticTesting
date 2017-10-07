#ifndef _smsSender_mock_hpp
#define _smsSender_mock_hpp

#include "gmock/gmock.h"

#include "smsSender.hpp"
#include "smsDevice_mock.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class smsSender_mock : public smsSender
{
public:
	smsSender_mock(smsDevice_mock * device) : smsSender(device)
	{
#ifdef IntegrationTests
		real_ = NULL;
		real_ = new smsSender(device);
		ON_CALL(*this, send(_, _)).WillByDefault(Invoke(real_, &smsSender::send));
#endif
	}

	~smsSender_mock()
	{
#ifdef IntegrationTests
		if(real_)
		{
			delete real_;
			real_ = NULL;
		}
#endif		
	}

	MOCK_METHOD2(send, bool(std::string number, std::string message));

private:
	smsSender * real_;
};

#endif //_smsSender_mock_hpp
