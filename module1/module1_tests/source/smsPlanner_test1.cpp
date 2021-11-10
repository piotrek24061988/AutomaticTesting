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

#include <memory>

using ::testing::Ge;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::_;

class smsPlanner_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		if(!keeper)
		{
			keeper = std::make_shared<timeKeeper>();
		}
		if(!device)
		{
			device = std::make_shared<smsDevice>();
		}
		if(!sender)
		{
			sender = std::make_shared<smsSender>(device);
		}
		if(!planner)
		{
			planner = std::make_unique<smsPlanner>(keeper, sender);
		}

		if(!keeper_mock)
		{
			keeper_mock = std::make_shared<timeKeeper_mock>();
		}
		if(!device_mock)
		{
			device_mock = std::make_shared<smsDevice_mock>();
		}
		if(!sender_mock)
		{
			sender_mock = std::make_shared<smsSender_mock>(device_mock);
		}
		if(!planner_mock)
		{
			planner_mock = std::make_unique<smsPlanner>(keeper_mock, sender_mock);
		}
	} 

	virtual void TearDown()
	{
		//smart_ptr used so no need to clean anything
	}

	std::unique_ptr<smsPlanner> planner;
	std::unique_ptr<smsPlanner> planner_mock;

	std::shared_ptr<timeKeeper> keeper;
	std::shared_ptr<timeKeeper_mock> keeper_mock;

	std::shared_ptr<smsSender> sender;
	std::shared_ptr<smsSender_mock> sender_mock;

	std::shared_ptr<smsDevice> device;
	std::shared_ptr<smsDevice_mock> device_mock;
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
//sendAll function error case behaviour.
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

//Verify that sending smses from queue mechanism is working.
#ifdef IntegrationTests
TEST_F(smsPlanner_test1, sendAllSuccess2BlackBox)
{
	planner->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);

	EXPECT_TRUE(planner->sendAll());
}
#endif

//Verify that sending smses from queue mechanism is working.
#ifdef IntegrationTests
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
#endif
