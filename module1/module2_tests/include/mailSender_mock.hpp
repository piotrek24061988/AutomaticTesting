#ifndef _smsSender_mock_hpp
#define _smsSender_mock_hpp

#include "gmock/gmock.h"

#include "mailSender.hpp"
#include "mailDevice_mock.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class mailSender_mock : public mailSender
{
public:
	mailSender_mock(mailDevice_mock * device) : mailSender(device)
	{
#ifdef IntegrationTests
		real_ = NULL;
		real_ = new mailSender(device);
		ON_CALL(*this, send(_, _, _, _)).WillByDefault(Invoke(real_, &mailSender::send));
#endif
	}

	~mailSender_mock()
	{
#ifdef IntegrationTests
		if(real_)
		{
			delete real_;
			real_ = NULL;
		}
#endif		
	}

	MOCK_METHOD4(send, int(std::string from, std::string to, std::string topic, std::string content));

private:
	mailSender * real_;
};

#endif //_mailSender_mock_hpp
