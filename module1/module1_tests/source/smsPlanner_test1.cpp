#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smsPlanner.hpp"
#include "timeKeeper.hpp"
#include "smsSender.hpp"
#include "smsDevice.hpp"

#include "timeKeeper_mock.hpp"
#include "smsSender_mock.hpp"
#include "smsDevice_mock.hpp"

#include "coutRedirect.hpp"

using ::testing::Ge;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::_;

class smsPlanner_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		keeper = NULL;
		keeper = new timeKeeper();
		device = NULL;
		device = new smsDevice();
		sender = NULL;
		sender = new smsSender(device);
		planner = NULL;
		planner = new smsPlanner(keeper, sender);

		keeper_mock = NULL;
		keeper_mock = new timeKeeper_mock();
		device_mock = NULL;
		device_mock = new smsDevice_mock();
		sender_mock = NULL;
		sender_mock = new smsSender_mock(device_mock);
		planner_mock = NULL;
		planner_mock = new smsPlanner(keeper_mock, sender_mock);
	} 

	virtual void TearDown()
	{
		if(planner)
		{
			delete planner;
			planner = NULL;
		}
		if(keeper)
		{
			delete keeper;
			keeper = NULL;
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
		if(keeper_mock)
		{
			delete keeper_mock;
			keeper_mock = NULL;
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

	smsPlanner * planner;
	smsPlanner * planner_mock;

	timeKeeper * keeper;
	timeKeeper_mock * keeper_mock;

	smsSender * sender;
	smsSender_mock * sender_mock;

	smsDevice * device;
	smsDevice_mock * device_mock;
};

//Insert sms to queue and check if was accepted and has a proper id in queue. 
TEST_F(smsPlanner_test1, addDeliveryNoError)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1);
#else
        EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1).WillOnce(Return(true));
#endif

	EXPECT_NE(-1, planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60));
}

//Insert 2 smses to queue and check if accepted and have different ids in queue. 
TEST_F(smsPlanner_test1, addDeliveryRetDiffId)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2);
#else
        EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2).WillRepeatedly(Return(true));
#endif

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);
	EXPECT_NE(id, planner_mock->addDelivery(std::string("537240688"), std::string("Hello2"), time(NULL) + 60));
}

//Insert 2 smses to queue and remove them by cancel mechanism and check if it works. 
TEST_F(smsPlanner_test1, cancelDeliveries)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2);
#else
        EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2).WillRepeatedly(Return(true));
#endif

	int id1 = planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);
	int id2 = planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	EXPECT_TRUE(planner_mock->cancelDelivery(id1));
	EXPECT_FALSE(planner_mock->cancelDelivery(id1));	
	EXPECT_TRUE(planner_mock->cancelDelivery(id2));
	EXPECT_FALSE(planner_mock->cancelDelivery(id2));	
}

//Verify that there is no possibility to insert sms with wrong time to queue.
TEST_F(smsPlanner_test1, nonValidTimeFalse)
{
	std::time_t sendTime = time(NULL);
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1);
#else
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1).WillOnce(Return(false));
#endif

	EXPECT_EQ(-1, planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), sendTime));
}

//Verify that sending smses from queue mechanism is working.
TEST_F(smsPlanner_test1, sendAllSuccess)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(3);
#else
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(3).WillRepeatedly(Return(true));
#endif

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);

#ifdef IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(3);
	EXPECT_CALL(*device_mock, init()).Times(3);
	EXPECT_CALL(*device_mock, send(_, _)).Times(3);
	EXPECT_CALL(*device_mock, deInit()).Times(3);
#else
        EXPECT_CALL(*sender_mock, send(_, _)).Times(3).WillRepeatedly(Return(true));
#endif

	EXPECT_TRUE(planner_mock->sendAll());
}

//Verify that sending smses from queue mechanism is working.
//Function send from sender_mock forced to return false to check planner
//sendAll function error case behaviour. Only unit test.
#ifndef IntegrationTests
TEST_F(smsPlanner_test1, sendAllFail)
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2).WillRepeatedly(Return(true));

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);

        EXPECT_CALL(*sender_mock, send(_, _)).Times(2).WillOnce(Return(true)).WillOnce(Return(false));

	EXPECT_FALSE(planner_mock->sendAll());
}
#endif

//Verify that it is not possible to send the same sms two times.
TEST_F(smsPlanner_test1, notSendTwoTimes)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(3);
#else
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(3).WillRepeatedly(Return(true));
#endif

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);

#ifdef IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(3);
	EXPECT_CALL(*device_mock, init()).Times(3);
	EXPECT_CALL(*device_mock, send(_, _)).Times(3);
	EXPECT_CALL(*device_mock, deInit()).Times(3);
#else
	EXPECT_CALL(*sender_mock, send(_, _)).Times(3).WillRepeatedly(Return(true));
#endif

	planner_mock->sendAll();

	EXPECT_CALL(*sender_mock, send(_, _)).Times(0);
	EXPECT_CALL(*device_mock, init()).Times(0);
	EXPECT_CALL(*device_mock, send(_, _)).Times(0);
	EXPECT_CALL(*device_mock, deInit()).Times(0);

	EXPECT_TRUE(planner_mock->sendAll());
}

//Verify that smses removed from queue with cancel mechanism will not be send.
TEST_F(smsPlanner_test1, notSendCanceled)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1);
#else
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1).WillOnce(Return(true));
#endif

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->cancelDelivery(id);
	
	EXPECT_CALL(*sender_mock, send(_, _)).Times(0);
	EXPECT_CALL(*device_mock, init()).Times(0);
	EXPECT_CALL(*device_mock, send(_, _)).Times(0);
	EXPECT_CALL(*device_mock, deInit()).Times(0);

	EXPECT_TRUE(planner_mock->sendAll());
}

//Verify that it is not possible to cancel sms after send.
TEST_F(smsPlanner_test1, notCancelAfterSend)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1);
#else
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1).WillOnce(Return(true));
#endif

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);

#ifdef IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(1);
	EXPECT_CALL(*device_mock, init()).Times(1);
	EXPECT_CALL(*device_mock, send(_, _)).Times(1);
	EXPECT_CALL(*device_mock, deInit()).Times(1);
#else
	EXPECT_CALL(*sender_mock, send(_, _)).Times(1).WillOnce(Return(true));
#endif

	planner_mock->sendAll();

	EXPECT_FALSE(planner_mock->cancelDelivery(id));
}

TEST_F(smsPlanner_test1, sendAllSuccess2BlackBox)
{
	planner->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);

	EXPECT_TRUE(planner->sendAll());
}

TEST_F(smsPlanner_test1, sendAllSuccess2WhiteBox)
{
	coutRedirect cR;

	planner->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);

	EXPECT_TRUE(planner->sendAll());

	std::string str = cR.getString();

        EXPECT_TRUE( str.find("bool timeKeeper::getTimeValid(std::time_t curTime)") != std::string::npos);
        EXPECT_TRUE( str.find("bool smsSender::send(std::string number, std::string message)") != std::string::npos);
        EXPECT_TRUE( str.find("bool smsDevice::init()") != std::string::npos);
        EXPECT_TRUE( str.find("bool smsSender::send(std::string number, std::string message)") != std::string::npos);
        EXPECT_TRUE( str.find("bool smsDevice::deInit()") != std::string::npos);
}
