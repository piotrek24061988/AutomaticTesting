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
#include <chrono>

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

#ifdef SoftwareTests
	constexpr int SWREQ_ADD_DELIVERY_TIMEOUT = 10000; //10000us = 10 ms
	constexpr int SWREQ_CANCEL_DELIVERY_TIMEOUT = 10000; //10000us = 10 ms
	constexpr int SWREQ_SEND_3SMS_TIMEOUT = 100000; //100000us = 100 ms
	constexpr int SWREQ_SEND_5SMS_TIMEOUT = 200000; //200000us = 200 ms
	constexpr int SWREQ_SEND_10SMS_TIMEOUT = 300000; //300000us = 300 ms
#endif //SoftwareTests

//Insert sms to queue and check if was accepted and has a proper id in queue. 
TEST_F(smsPlanner_test1, addDeliveryNoError)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1);
#else //IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1).WillOnce(Return(true));
#endif //IntegrationTests

#ifdef SoftwareTests
	auto start = std::chrono::steady_clock::now();
#endif //SoftwareTests

	EXPECT_NE(-1, planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60));

#ifdef SoftwareTests
	auto end = std::chrono::steady_clock::now();

        EXPECT_TRUE(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < SWREQ_ADD_DELIVERY_TIMEOUT);
	std::cout << "SWREQ_ADD_DELIVERY_TIMEOUT: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;
#endif //SoftwareTests
}

//Insert 2 smses to queue and check if accepted and have different ids in queue. 
#ifndef SoftwareTests
TEST_F(smsPlanner_test1, addDeliveryRetDiffId)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2);
#else //IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2).WillRepeatedly(Return(true));
#endif //IntegrationTests

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);
	EXPECT_NE(id, planner_mock->addDelivery(std::string("537240688"), std::string("Hello2"), time(NULL) + 60));
}
#endif //SoftwareTests

//Insert 2 smses to queue and remove them by cancel mechanism and check if it works. 
TEST_F(smsPlanner_test1, cancelDeliveries)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2);
#else //IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2).WillRepeatedly(Return(true));
#endif //IntegrationTests

	int id1 = planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);
	int id2 = planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);

#ifdef SoftwareTests
	auto start = std::chrono::steady_clock::now();
#endif //SoftwareTests

	EXPECT_TRUE(planner_mock->cancelDelivery(id1));
	EXPECT_FALSE(planner_mock->cancelDelivery(id1));	
	EXPECT_TRUE(planner_mock->cancelDelivery(id2));
	EXPECT_FALSE(planner_mock->cancelDelivery(id2));

#ifdef SoftwareTests
	auto end = std::chrono::steady_clock::now();

        EXPECT_TRUE(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < SWREQ_CANCEL_DELIVERY_TIMEOUT);
	std::cout << "SWREQ_CANCEL_DELIVERY_TIMEOUT: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;
#endif //SoftwareTests	
}

//Verify that there is no possibility to insert sms with wrong time to queue.
#ifndef SoftwareTests
TEST_F(smsPlanner_test1, nonValidTimeFalse)
{
	std::time_t sendTime = time(NULL);
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1);
#else //IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1).WillOnce(Return(false));
#endif //IntegrationTests

	EXPECT_EQ(-1, planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), sendTime));
}
#endif //SoftwareTests

//Verify that sending smses from queue mechanism is working.
TEST_F(smsPlanner_test1, sendAllSuccess)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(3);
#else //IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(3).WillRepeatedly(Return(true));
#endif //IntegrationTests

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);

#ifdef IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(3);
	EXPECT_CALL(*device_mock, init()).Times(3);
	EXPECT_CALL(*device_mock, send(_, _)).Times(3);
	EXPECT_CALL(*device_mock, deInit()).Times(3);
#else //IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(3).WillRepeatedly(Return(true));
#endif //IntegrationTests

#ifdef SoftwareTests
	auto start = std::chrono::steady_clock::now();
#endif //SoftwareTests

	EXPECT_TRUE(planner_mock->sendAll());

#ifdef SoftwareTests
	auto end = std::chrono::steady_clock::now();

        EXPECT_TRUE(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < SWREQ_SEND_3SMS_TIMEOUT);
	std::cout << "SWREQ_SEND_3SMS_TIMEOUT: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()  << "us" << std::endl;
#endif //SoftwareTests
}

//Verify timing for sending 5 smses from queue.
#ifdef SoftwareTests
#ifdef IntegrationTests
TEST_F(smsPlanner_test1, sendAll5Success)
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(5);

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 4"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 5"), time(NULL) + 60);

	EXPECT_CALL(*sender_mock, send(_, _)).Times(5);
	EXPECT_CALL(*device_mock, init()).Times(5);
	EXPECT_CALL(*device_mock, send(_, _)).Times(5);
	EXPECT_CALL(*device_mock, deInit()).Times(5);

	auto start = std::chrono::steady_clock::now();

	EXPECT_TRUE(planner_mock->sendAll());

	auto end = std::chrono::steady_clock::now();

        EXPECT_TRUE(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < SWREQ_SEND_5SMS_TIMEOUT);
	std::cout << "SWREQ_SEND_5SMS_TIMEOUT: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()  << "us" << std::endl;
}
#endif //IntegrationTests
#endif //SoftwareTests

//Verify timing for sending 5 smses from queue.
#ifdef SoftwareTests
#ifdef IntegrationTests
TEST_F(smsPlanner_test1, sendAll10Success)
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(10);

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 4"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 5"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 6"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 7"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 8"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 9"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 10"), time(NULL) + 60);

	EXPECT_CALL(*sender_mock, send(_, _)).Times(10);
	EXPECT_CALL(*device_mock, init()).Times(10);
	EXPECT_CALL(*device_mock, send(_, _)).Times(10);
	EXPECT_CALL(*device_mock, deInit()).Times(10);

	auto start = std::chrono::steady_clock::now();

	EXPECT_TRUE(planner_mock->sendAll());

	auto end = std::chrono::steady_clock::now();

        EXPECT_TRUE(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < SWREQ_SEND_10SMS_TIMEOUT);
	std::cout << "SWREQ_SEND_10SMS_TIMEOUT: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()  << "us" << std::endl;
}
#endif //IntegrationTests
#endif //SoftwareTests

//Verify that sending smses from queue mechanism is working.
//Function send from sender_mock forced to return false to check planner
//sendAll function error case behaviour.
#ifndef SoftwareTests
#ifndef IntegrationTests
TEST_F(smsPlanner_test1, sendAllFail)
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(2).WillRepeatedly(Return(true));

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);

	EXPECT_CALL(*sender_mock, send(_, _)).Times(2).WillOnce(Return(true)).WillOnce(Return(false));

	EXPECT_FALSE(planner_mock->sendAll());
}
#endif //IntegrationTests
#endif //SoftwareTests

//Verify that it is not possible to send the same sms two times.
#ifndef SoftwareTests
TEST_F(smsPlanner_test1, notSendTwoTimes)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(3);
#else //IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(3).WillRepeatedly(Return(true));
#endif //IntegrationTests

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);

#ifdef IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(3);
	EXPECT_CALL(*device_mock, init()).Times(3);
	EXPECT_CALL(*device_mock, send(_, _)).Times(3);
	EXPECT_CALL(*device_mock, deInit()).Times(3);
#else //IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(3).WillRepeatedly(Return(true));
#endif //IntegrationTests

	planner_mock->sendAll();

	EXPECT_CALL(*sender_mock, send(_, _)).Times(0);
	EXPECT_CALL(*device_mock, init()).Times(0);
	EXPECT_CALL(*device_mock, send(_, _)).Times(0);
	EXPECT_CALL(*device_mock, deInit()).Times(0);

	EXPECT_TRUE(planner_mock->sendAll());
}
#endif //SoftwareTests

//Verify that smses removed from queue with cancel mechanism will not be send.
#ifndef SoftwareTests
TEST_F(smsPlanner_test1, notSendCanceled)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1);
#else //IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1).WillOnce(Return(true));
#endif //IntegrationTests

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner_mock->cancelDelivery(id);
	
	EXPECT_CALL(*sender_mock, send(_, _)).Times(0);
	EXPECT_CALL(*device_mock, init()).Times(0);
	EXPECT_CALL(*device_mock, send(_, _)).Times(0);
	EXPECT_CALL(*device_mock, deInit()).Times(0);

	EXPECT_TRUE(planner_mock->sendAll());
}
#endif //SoftwareTests

//Verify that it is not possible to cancel sms after send.
#ifndef SoftwareTests
TEST_F(smsPlanner_test1, notCancelAfterSend)
{
#ifdef IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1);
#else //IntegrationTests
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).Times(1).WillOnce(Return(true));
#endif //IntegrationTests

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);

#ifdef IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(1);
	EXPECT_CALL(*device_mock, init()).Times(1);
	EXPECT_CALL(*device_mock, send(_, _)).Times(1);
	EXPECT_CALL(*device_mock, deInit()).Times(1);
#else //IntegrationTests
	EXPECT_CALL(*sender_mock, send(_, _)).Times(1).WillOnce(Return(true));
#endif //IntegrationTests

	planner_mock->sendAll();

	EXPECT_FALSE(planner_mock->cancelDelivery(id));
}
#endif //SoftwareTests

//Verify that sending smses from queue mechanism is working.
#ifndef SoftwareTests
#ifdef IntegrationTests
TEST_F(smsPlanner_test1, sendAllSuccess2BlackBox)
{
	planner->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL) + 60);
	planner->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	planner->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL) + 60);

	EXPECT_TRUE(planner->sendAll());
}
#endif //IntegrationTests
#endif //SoftwareTests

//Verify that sending smses from queue mechanism is working.
#ifndef SoftwareTests
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
#endif //IntegrationTests
#endif //SoftwareTests
