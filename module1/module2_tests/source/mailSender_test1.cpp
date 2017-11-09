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
}
#endif
