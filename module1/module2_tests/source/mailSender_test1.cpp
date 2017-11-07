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
TEST_F(mailSender_test1, sendValidatedMai_ITBB)
{
	EXPECT_NE(-1, sender->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMai_ITBB", "sendValidatedMai_ITBB"));
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendValidatedMai_ITWBLogs)
{
	coutRedirect cR;

	EXPECT_NE(-1, sender->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMai_ITBB", "sendValidatedMai_ITBBLogs"));

	std::string str = cR.getString();

	EXPECT_TRUE( str.find("bool connectionChecker::isInternetAvailable()") != std::string::npos);
	EXPECT_TRUE( str.find("FILE * mailDevice::openDriver()") != std::string::npos);
	EXPECT_TRUE( str.find("int mailDevice::sendMail(FILE *, std::string, std::string, std::string, std::string)") != std::string::npos);
	EXPECT_TRUE( str.find("int mailDevice::closeDriver(FILE *)") != std::string::npos);
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendValidatedMai_ITWBMock)
{
	EXPECT_CALL(*device_mock, openDriver()).Times(1);
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(1);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(1);

	EXPECT_NE(-1, sender_mock->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMai_ITWBMock", "sendValidatedMai_ITWBMock"));
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendValidatedMai_ITWBMockLogs)
{
	coutRedirect cR;

	EXPECT_CALL(*device_mock, openDriver()).Times(1);
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(1);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(1);

	EXPECT_NE(-1, sender_mock->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMai_ITWBMockLogs", "sendValidatedMai_ITWBMockLogs"));

	std::string str = cR.getString();

	EXPECT_TRUE( str.find("bool connectionChecker::isInternetAvailable()") != std::string::npos);
}
#endif

#ifndef IntegrationTests
TEST_F(mailSender_test1, sendMailOpenDriverError_UT)
{
	FILE * id = NULL;
	EXPECT_CALL(*device_mock, openDriver()).Times(1).WillOnce(Return(id));
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(0);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(0);

	EXPECT_EQ(-1, sender_mock->send("from", "to", "sendMailOpenDriverError_UT", "sendMailOpenDriverError_UT"));
}
#endif

#ifndef IntegrationTests
TEST_F(mailSender_test1, sendMailSendMailError_UT)
{
	FILE * id = new FILE();

	EXPECT_CALL(*device_mock, openDriver()).Times(1).WillOnce(Return(id));
	EXPECT_CALL(*device_mock, sendMail(id, _, _, _, _)).Times(1).WillOnce(Return(-1));
	EXPECT_CALL(*device_mock, closeDriver(id)).Times(0);

	EXPECT_EQ(-1, sender_mock->send("from", "to", "sendMailSendMailError_UT", "sendMailSendMailError_UT"));
}
#endif

#ifndef IntegrationTests
TEST_F(mailSender_test1, sendMailCloseDriverError_UT)
{
	FILE * id = new FILE();

	EXPECT_CALL(*device_mock, openDriver()).Times(1).WillOnce(Return(id));
	EXPECT_CALL(*device_mock, sendMail(id, _, _, _, _)).Times(1).WillOnce(Return(0));
	EXPECT_CALL(*device_mock, closeDriver(id)).Times(1).WillOnce(Return(-1));

	EXPECT_EQ(-1, sender_mock->send("from", "to", "sendMailCloseDriverError_UT", "sendMailCloseDriverError_UT"));
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendMailError_ITBB)
{
	system("sudo ifconfig eth0 down");
	system("sudo ifconfig wlan0 down");

	EXPECT_EQ(-1, sender->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMai_ITBB", "sendValidatedMai_ITBB"));

	system("sudo ifconfig eth0 up");
	system("sudo ifconfig wlan0 up");
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendMailError_ITWBLogs)
{
	system("sudo ifconfig eth0 down");
	system("sudo ifconfig wlan0 down");

	coutRedirect cR;

	EXPECT_EQ(-1, sender->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendValidatedMai_ITBB", "sendValidatedMai_ITBB"));

	std::string str = cR.getString();

	EXPECT_TRUE( str.find("bool connectionChecker::isInternetAvailable()") != std::string::npos);
	EXPECT_FALSE( str.find("FILE * mailDevice::openDriver()") != std::string::npos);
	EXPECT_FALSE( str.find("int mailDevice::sendMail(FILE *, std::string, std::string, std::string, std::string)") != std::string::npos);
	EXPECT_FALSE( str.find("int mailDevice::closeDriver(FILE *)") != std::string::npos);

	system("sudo ifconfig eth0 up");
	system("sudo ifconfig wlan0 up");
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendMailError_ITWBMock)
{
	system("sudo ifconfig eth0 down");
	system("sudo ifconfig wlan0 down");

	EXPECT_CALL(*device_mock, openDriver()).Times(0);
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(0);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(0);

	EXPECT_EQ(-1, sender_mock->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendMailError_ITWBMock", "sendMailError_ITWBMock"));

	system("sudo ifconfig eth0 up");
	system("sudo ifconfig wlan0 up");
}
#endif

#ifdef IntegrationTests
TEST_F(mailSender_test1, sendMailError_ITWBMockLogs)
{
	system("sudo ifconfig eth0 down");
	system("sudo ifconfig wlan0 down");

	coutRedirect cR;

	EXPECT_CALL(*device_mock, openDriver()).Times(0);
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(0);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(0);

	EXPECT_EQ(-1, sender_mock->send("piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "sendMailError_ITWBMockLogs", "sendMailError_ITWBMockLogs"));

	std::string str = cR.getString();

	EXPECT_TRUE( str.find("bool connectionChecker::isInternetAvailable()") != std::string::npos);

	system("sudo ifconfig eth0 up");
	system("sudo ifconfig wlan0 up");
}
#endif
