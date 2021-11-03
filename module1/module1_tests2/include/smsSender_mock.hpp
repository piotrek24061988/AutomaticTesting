#ifndef _smsSender_mock_hpp
#define _smsSender_mock_hpp

#include "gmock/gmock.h"

#include "smsSender.hpp"
#include "smsDevice_mock.hpp"

#include <memory>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class smsSender_mock : public smsSender
{
public:
	smsSender_mock(std::shared_ptr<smsDevice_mock> device) : smsSender(device)
	{
#ifdef IntegrationTests
		real_ = std::make_unique<smsSender>(device);
		ON_CALL(*this, send(_, _)).WillByDefault(Invoke(real_.get(), &smsSender::send));
#endif
	}

	~smsSender_mock()
	{
		//smart_ptr used so no need to clean anything		
	}

	MOCK_METHOD2(send, bool(std::string number, std::string message));

private:
	std::unique_ptr<smsSender> real_;
};

#endif //_smsSender_mock_hpp
