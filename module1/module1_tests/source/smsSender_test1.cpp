#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smsSender.hpp"
#include "smsDevice.hpp"

#include "smsDevice_mock.hpp"

using ::testing::Ge;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::_;

class smsSender_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		device = NULL;
		device = new smsDevice();
		sender = NULL;
		sender = new smsSender(device);

		device_mock = NULL;
		device_mock = new smsDevice_mock();
		sender_mock = NULL;
		sender_mock = new smsSender(device_mock);
	}
	virtual void TearDown()
	{
		if(sender)
		{
			delete sender;
			sender = NULL;
		}
		if(device)
		{
			delete device;
			device = NULL;
		}

		if(sender_mock)
		{
			delete sender_mock;
			sender_mock = NULL;		
		}	
		if(device_mock)
		{
			delete device_mock;
			device_mock = NULL;
		}	
	}

	smsSender * sender;
	smsSender * sender_mock;

	smsDevice * device;
	smsDevice_mock * device_mock;
};

TEST_F(smsSender_test1, sendValidatedSms)
{
#ifdef IntegrationTests
	EXPECT_CALL(*device_mock, init()).Times(1);
	EXPECT_CALL(*device_mock, send(std::string("537240688"), std::string("Hello"))).Times(1);
	EXPECT_CALL(*device_mock, deInit()).Times(1);
#else
        EXPECT_CALL(*device_mock, init()).Times(1).WillOnce(Return(true));
        EXPECT_CALL(*device_mock, send(std::string("537240688"), std::string("Hello"))).Times(1).WillOnce(Return(true));
	EXPECT_CALL(*device_mock, deInit()).Times(1).WillOnce(Return(true));
#endif

	EXPECT_TRUE(sender_mock->send(std::string("537240688"), std::string("Hello")));
}

//smsDevice initialization error simulation by mock. Only unit test.
#ifndef IntegrationTests
TEST_F(smsSender_test1, sendValidatedSmsInitError)
{
        EXPECT_CALL(*device_mock, init()).Times(1).WillOnce(Return(false));

	EXPECT_FALSE(sender_mock->send(std::string("537240688"), std::string("Hello")));
}
#endif

//smsDevice send error simulation by mock. Only unit test.
#ifndef IntegrationTests
TEST_F(smsSender_test1, sendValidatedSmsSendError)
{
        EXPECT_CALL(*device_mock, init()).Times(1).WillOnce(Return(true));
        EXPECT_CALL(*device_mock, send(std::string("537240688"), std::string("Hello"))).Times(1).WillOnce(Return(false));

	EXPECT_FALSE(sender_mock->send(std::string("537240688"), std::string("Hello")));
}
#endif

//smsDevice deinitialization error simulation by mock. Only unit test.
#ifndef IntegrationTests
TEST_F(smsSender_test1, sendValidatedSmsDeinitError)
{
        EXPECT_CALL(*device_mock, init()).Times(1).WillOnce(Return(true));
        EXPECT_CALL(*device_mock, send(std::string("537240688"), std::string("Hello"))).Times(1).WillOnce(Return(true));
	EXPECT_CALL(*device_mock, deInit()).Times(1).WillOnce(Return(false));

	EXPECT_FALSE(sender_mock->send(std::string("537240688"), std::string("Hello")));
}
#endif
