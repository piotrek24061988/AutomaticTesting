#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mailSender.hpp"
#include "mailDevice.hpp"
#include "connectionChecker.hpp"

#include "mailDevice_mock.hpp"
#include "connectionChecker_mock.hpp"

#include "coutRedirect.hpp"

using ::testing::Ge;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::_;

class mailSender_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		device = NULL;
		device = new mailDevice();
		sender = NULL;
		sender = new mailSender(device);

		device_mock = NULL;
		device_mock = new mailDevice_mock();
		sender_mock = NULL;
		sender_mock = new mailSender(device_mock);
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

	mailSender * sender;
	mailSender * sender_mock;

	mailDevice * device;
	mailDevice_mock * device_mock;
};

#ifndef IntegrationTests
TEST_F(mailSender_test1, sendValidatedMail_UT)
{
	FILE * id = new FILE();

	EXPECT_CALL(*device_mock, openDriver()).Times(1).WillOnce(Return(id));
	EXPECT_CALL(*device_mock, sendMail(id, _, _, _, _)).Times(1).WillOnce(Return(0));
	EXPECT_CALL(*device_mock, closeDriver(id)).Times(1).WillOnce(Return(0));

	EXPECT_NE(-1, sender_mock->send("from", "to", "sendValidatedMail_UT", "sendValidatedMail_UT"));
}
#endif
