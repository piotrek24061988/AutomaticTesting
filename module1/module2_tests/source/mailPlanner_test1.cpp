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
		device = NULL;
		device = new mailDevice();
		sender = NULL;
		sender = new mailSender(device);
		planner = NULL;
		planner = new mailPlanner(sender);

		device_mock = NULL;
		device_mock = new mailDevice_mock();
		sender_mock = NULL;
		sender_mock = new mailSender_mock();
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
