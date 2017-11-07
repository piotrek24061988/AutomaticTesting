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
		checker = NULL;
		checker = new connectionChecker();

		device_mock = NULL;
		device_mock = new mailDevice_mock();
		sender_mock = NULL;
		sender_mock = new mailSender(device_mock);
		checker_mock = NULL;
		checker_mock = new connectionChecker_mock();
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
		if(checker)
		{
			delete checker;
			checker = NULL;
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
		if(checker_mock)
		{
			delete checker_mock;
			checker_mock = NULL;
		}
	}

	mailSender * sender;
	mailSender * sender_mock;

	mailDevice * device;
	mailDevice_mock * device_mock;

	connectionChecker * checker;
	connectionChecker_mock * checker_mock;
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

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendValidatedMail_ITBB)
{
	EXPECT_NE(-1, sender->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMail_ITBB", "sendValidatedMail_ITBB"));
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendValidatedMail_ITWBLogs)
{
	coutRedirect cR;

	EXPECT_NE(-1, sender->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMail_ITBBLogs", "sendValidatedMail_ITBBLogs"));

	std::string str = cR.getString();

	EXPECT_TRUE( str.find("bool connectionChecker::isInternetAvailable()") != std::string::npos);
	EXPECT_TRUE( str.find("FILE * mailDevice::openDriver()") != std::string::npos);
	EXPECT_TRUE( str.find("int mailDevice::sendMail(FILE *, std::string, std::string, std::string, std::string)") != std::string::npos);
	EXPECT_TRUE( str.find("int mailDevice::closeDriver(FILE *)") != std::string::npos);
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendValidatedMail_ITWBMocks)
{
	EXPECT_CALL(*device_mock, openDriver()).Times(1);
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(1);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(1);

	EXPECT_NE(-1, sender_mock->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMail_ITWBMocks", "sendValidatedMail_ITWBMocks"));
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendValidatedMail_ITWBMocksLogs)
{
	coutRedirect cR;

	EXPECT_CALL(*device_mock, openDriver()).Times(1);
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(1);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(1);

	EXPECT_NE(-1, sender_mock->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMail_ITWBMocksLogs", "sendValidatedMail_ITWBMocksLogs"));

	std::string str = cR.getString();

	EXPECT_TRUE( str.find("bool connectionChecker::isInternetAvailable()") != std::string::npos);
}
#endif
