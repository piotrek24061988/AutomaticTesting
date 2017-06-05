#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smsPlanner.hpp"
#include "timeKeeper.hpp"
#include "smsSender.hpp"

#include "timeKeeper_mock.hpp"
#include "smsSender_mock.hpp"

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
		sender = NULL;
		sender = new smsSender();
		planner = NULL;
		planner = new smsPlanner(keeper, sender);

		keeper_mock = NULL;
		keeper_mock = new timeKeeper_mock();
		sender_mock = NULL;
		sender_mock = new smsSender_mock();
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
	}

	smsPlanner * planner;
	smsPlanner * planner_mock;

	timeKeeper * keeper;
	timeKeeper_mock * keeper_mock;

	smsSender * sender;
	smsSender_mock * sender_mock;
};

TEST_F(smsPlanner_test1, addDeliveryNoError)
{
	EXPECT_NE(-1, planner->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60));
}

TEST_F(smsPlanner_test1, addDeliveryRetDiffId)
{
	int id = planner->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);
	EXPECT_NE(id, planner->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60));
}

TEST_F(smsPlanner_test1, cancelDeliveries)
{
	int id1 = planner->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);
	int id2 = planner->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	EXPECT_TRUE(planner->cancelDelivery(id1));
	EXPECT_FALSE(planner->cancelDelivery(id1));	
	EXPECT_TRUE(planner->cancelDelivery(id2));
	EXPECT_FALSE(planner->cancelDelivery(id2));	
}

TEST_F(smsPlanner_test1, validTimeTrue)
{
	std::time_t sendTime = time(NULL) + 60;
	EXPECT_CALL(*keeper_mock, getTimeValid(sendTime)).WillOnce(Return(true));

	EXPECT_NE(-1, planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), sendTime));
}

TEST_F(smsPlanner_test1, nonValidTimeFalse)
{
	std::time_t sendTime = time(NULL);
	EXPECT_CALL(*keeper_mock, getTimeValid(sendTime)).WillOnce(Return(false));

	EXPECT_EQ(-1, planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), sendTime));
}

TEST_F(smsPlanner_test1, sendAllSuccess)
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).WillRepeatedly(Return(true));

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL));
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL));
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL));

	EXPECT_CALL(*sender_mock, send(_, _)).WillRepeatedly(Return(true));

	EXPECT_TRUE(planner_mock->sendAll());
}

TEST_F(smsPlanner_test1, notSendTwoTimes)
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).WillRepeatedly(Return(true));

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL));
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL));
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL));

	EXPECT_CALL(*sender_mock, send(_, _)).WillRepeatedly(Return(true));

	planner_mock->sendAll();

	EXPECT_CALL(*sender_mock, send(_, _)).Times(0);

	EXPECT_TRUE(planner_mock->sendAll());
}

TEST_F(smsPlanner_test1, notSendCanceled)
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).WillRepeatedly(Return(true));

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL));
	planner->cancelDelivery(id);
	
	EXPECT_CALL(*sender_mock, send(_, _)).Times(0);

	EXPECT_TRUE(planner_mock->sendAll());
}

TEST_F(smsPlanner_test1, notCancelAfterSend)
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).WillRepeatedly(Return(true));

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL));
	
	EXPECT_CALL(*sender_mock, send(_, _)).WillRepeatedly(Return(true));

	planner_mock->sendAll();

	EXPECT_FALSE(planner->cancelDelivery(id));
}
