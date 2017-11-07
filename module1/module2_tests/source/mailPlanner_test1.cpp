#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mailPlanner.hpp"
#include "connectionChecker.hpp"
#include "mailSender.hpp"
#include "mailDevice.hpp"

#include "connectionChecker_mock.hpp"
#include "mailSender_mock.hpp"
#include "mailDevice_mock.hpp"

#include "coutRedirect.hpp"

using ::testing::Ge;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::_;

class mailPlanner_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		checker = NULL;
		checker = new connectionChecker();
		device = NULL;
		device = new mailDevice();
		sender = NULL;
		sender = new mailSender(device);
		planner = NULL;
		planner = new mailPlanner(sender);

		checker_mock = NULL;
		checker_mock = new connectionChecker_mock();
		device_mock = NULL;
		device_mock = new mailDevice_mock();
		sender_mock = NULL;
		sender_mock = new mailSender_mock(device_mock);
		planner_mock = NULL;
		planner_mock = new mailPlanner(sender_mock);
	} 

	virtual void TearDown()
	{
		if(planner)
		{
			delete planner;
			planner = NULL;
		}
		if(checker)
		{
			delete checker;
			checker = NULL;
		}
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

		if(planner_mock)
		{
			delete planner_mock;
			planner_mock = NULL;
		}
		if(checker_mock)
		{
			delete checker_mock;
			checker_mock = NULL;
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

	mailPlanner * planner;
	mailPlanner * planner_mock;

	connectionChecker * checker;
	connectionChecker_mock * checker_mock;

	mailSender * sender;
	mailSender_mock * sender_mock;

	mailDevice * device;
	mailDevice_mock * device_mock;
};

//Insert mail to queue and check if was accepted and has a proper id in queue.
#ifndef IntegrationTests
TEST_F(mailPlanner_test1, addDeliveryNoErrorUT)
{
	EXPECT_NE(-1, planner_mock->addDelivery(std::string("from"), std::string("to"), std::string("topic"), std::string("content")));
}
#endif

//Insert 2 mail to queue and check if accepted and have different ids in queue.
#ifndef IntegrationTests
TEST_F(mailPlanner_test1, addDeliveryRetDiffIdUT)
{
	int id = planner_mock->addDelivery(std::string("from"), std::string("to"), std::string("topic1"), std::string("content1"));
	EXPECT_NE(id, planner_mock->addDelivery(std::string("from"), std::string("to"), std::string("topic2"), std::string("content2")));
}
#endif

//Insert 2 mail to queue and remove them by cancel mechanism and check if it works.
#ifndef IntegrationTests 
TEST_F(mailPlanner_test1, cancelDeliveriesUT)
{
	int id1 = planner_mock->addDelivery(std::string("from"), std::string("to"), std::string("topic1"), std::string("content1"));
	int id2 = planner_mock->addDelivery(std::string("from"), std::string("to"), std::string("topic2"), std::string("content2"));
	EXPECT_TRUE(planner_mock->cancelDelivery(id1));
	EXPECT_FALSE(planner_mock->cancelDelivery(id1));	
	EXPECT_TRUE(planner_mock->cancelDelivery(id2));
	EXPECT_FALSE(planner_mock->cancelDelivery(id2));	
}
#endif

//Verify that sending mail from queue mechanism is working.
#ifndef IntegrationTests
TEST_F(mailPlanner_test1, sendAllSuccessUT)
{
	planner_mock->addDelivery(std::string("from"), std::string("to"), std::string("topic1"), std::string("content1"));
	planner_mock->addDelivery(std::string("from"), std::string("to"), std::string("topic2"), std::string("content2"));

        EXPECT_CALL(*sender_mock, send(_, _, _, _)).Times(2).WillRepeatedly(Return(true));

	EXPECT_TRUE(planner_mock->sendAll());
}
#endif

//Verify that sending mail from queue mechanism is working.
#ifdef IntegrationTests
TEST_F(mailPlanner_test1, sendAllSuccessITBB)
{
	planner->addDelivery(std::string("piotrek24061988@gmail.com"), std::string("piotrek24061988@gmail.com"), std::string("sendAllSuccessITBB1"), std::string("sendAllSuccessITBB1"));
	planner->addDelivery(std::string("piotrek24061988@gmail.com"), std::string("piotrek24061988@gmail.com"), std::string("sendAllSuccessITBB2"), std::string("sendAllSuccessITBB2"));

	EXPECT_TRUE(planner->sendAll());
}
#endif

//Verify that sending mail from queue mechanism is working.
#ifdef IntegrationTests
TEST_F(mailPlanner_test1, sendAllSuccessITWBLogs)
{
	coutRedirect cR;

	planner->addDelivery(std::string("piotrek24061988@gmail.com"), std::string("piotrek24061988@gmail.com"), std::string("sendAllSuccessITWBLogs1"), std::string("sendAllSuccessITWBLogs1"));
	planner->addDelivery(std::string("piotrek24061988@gmail.com"), std::string("piotrek24061988@gmail.com"), std::string("sendAllSuccessITWBLogs2"), std::string("sendAllSuccessITWBLogs2"));

	EXPECT_TRUE(planner->sendAll());

	std::string str = cR.getString();

	EXPECT_TRUE( str.find("int mailSender::send(std::string, std::string, std::string, std::string)") != std::string::npos);
	EXPECT_TRUE( str.find("bool connectionChecker::isInternetAvailable()") != std::string::npos);
	EXPECT_TRUE( str.find("FILE * mailDevice::openDriver()") != std::string::npos);
	EXPECT_TRUE( str.find("int mailDevice::sendMail(FILE *, std::string, std::string, std::string, std::string)") != std::string::npos);
	EXPECT_TRUE( str.find("int mailDevice::closeDriver(FILE *)") != std::string::npos);
}
#endif

//Verify that sending mail from queue mechanism is working.
#ifdef IntegrationTests
TEST_F(mailPlanner_test1, sendAllSuccessITWBMocks)
{
	planner_mock->addDelivery(std::string("piotrek24061988@gmail.com"), std::string("piotrek24061988@gmail.com"), std::string("sendAllSuccessITWBMocks1"), std::string("sendAllSuccessITWBMocks1"));
	planner_mock->addDelivery(std::string("piotrek24061988@gmail.com"), std::string("piotrek24061988@gmail.com"), std::string("sendAllSuccessITWBMocks2"), std::string("sendAllSuccessITWBMocks2"));

        EXPECT_CALL(*sender_mock, send(_, _, _, _)).Times(2);
	EXPECT_CALL(*device_mock, openDriver()).Times(2);
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(2);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(2);

	EXPECT_TRUE(planner_mock->sendAll());
}
#endif

//Verify that sending mail from queue mechanism is working.
#ifdef IntegrationTests
TEST_F(mailPlanner_test1, sendAllSuccessITWBMocksLogs)
{
	coutRedirect cR;

	planner_mock->addDelivery(std::string("piotrek24061988@gmail.com"), std::string("piotrek24061988@gmail.com"), std::string("sendAllSuccessITWBMocksLogs1"), std::string("sendAllSuccessITWBMocksLogs1"));
	planner_mock->addDelivery(std::string("piotrek24061988@gmail.com"), std::string("piotrek24061988@gmail.com"), std::string("sendAllSuccessITWBMocksLogs2"), std::string("sendAllSuccessITWBMocksLogs2"));

        EXPECT_CALL(*sender_mock, send(_, _, _, _)).Times(2);
	EXPECT_CALL(*device_mock, openDriver()).Times(2);
	EXPECT_CALL(*device_mock, sendMail(_, _, _, _, _)).Times(2);
	EXPECT_CALL(*device_mock, closeDriver(_)).Times(2);

	EXPECT_TRUE(planner_mock->sendAll());

	std::string str = cR.getString();

	EXPECT_TRUE( str.find("bool connectionChecker::isInternetAvailable()") != std::string::npos);
}
#endif
