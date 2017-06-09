#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

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

class smsPlanner_test2 : public CppUnit::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();

        void addDeliveryNoError();
	void addDeliveryRetDiffId();
	void cancelDeliveries();
	void validTimeTrue();
	void nonValidTimeFalse();
	void sendAllSuccess();
	void notSendTwoTimes();
	void notSendCanceled();
	void notCancelAfterSend();

public:
	CPPUNIT_TEST_SUITE(smsPlanner_test2);
	CPPUNIT_TEST(addDeliveryNoError);
	CPPUNIT_TEST(addDeliveryRetDiffId);
	CPPUNIT_TEST(cancelDeliveries);
	CPPUNIT_TEST(validTimeTrue);
	CPPUNIT_TEST(nonValidTimeFalse);
	CPPUNIT_TEST(sendAllSuccess);
	CPPUNIT_TEST(notSendTwoTimes);
	CPPUNIT_TEST(notSendCanceled);
	CPPUNIT_TEST(notCancelAfterSend);
	CPPUNIT_TEST_SUITE_END();

	smsPlanner * planner;
	smsPlanner * planner_mock;

	timeKeeper * keeper;
	timeKeeper_mock * keeper_mock;

	smsSender * sender;
	smsSender_mock * sender_mock;
};

CPPUNIT_TEST_SUITE_REGISTRATION(smsPlanner_test2);

void smsPlanner_test2::setUp()
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

void smsPlanner_test2::tearDown()
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

void smsPlanner_test2::addDeliveryNoError()
{
	CPPUNIT_ASSERT(-1 != planner->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60));
}

void smsPlanner_test2::addDeliveryRetDiffId()
{
	int id = planner->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);
	CPPUNIT_ASSERT(id != planner->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60));
}

void smsPlanner_test2::cancelDeliveries()
{
	int id1 = planner->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL) + 60);
	int id2 = planner->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL) + 60);
	CPPUNIT_ASSERT(true == planner->cancelDelivery(id1));
	CPPUNIT_ASSERT(false == planner->cancelDelivery(id1));	
	CPPUNIT_ASSERT(true == planner->cancelDelivery(id2));
	CPPUNIT_ASSERT(false == planner->cancelDelivery(id2));
}

void smsPlanner_test2::validTimeTrue()
{
	std::time_t sendTime = time(NULL) + 60;
	EXPECT_CALL(*keeper_mock, getTimeValid(sendTime)).WillOnce(Return(true));
	CPPUNIT_ASSERT(-1 != planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), sendTime));
}

void smsPlanner_test2::nonValidTimeFalse()
{
	std::time_t sendTime = time(NULL);
	EXPECT_CALL(*keeper_mock, getTimeValid(sendTime)).WillOnce(Return(false));

	CPPUNIT_ASSERT(-1 == planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), sendTime));
}

void smsPlanner_test2::sendAllSuccess()
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).WillRepeatedly(Return(true));

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL));
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL));
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL));

	EXPECT_CALL(*sender_mock, send(_, _)).WillRepeatedly(Return(true));

	CPPUNIT_ASSERT(planner_mock->sendAll());
}

void smsPlanner_test2::notSendTwoTimes()
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).WillRepeatedly(Return(true));

	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL));
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 2"), time(NULL));
	planner_mock->addDelivery(std::string("537240688"), std::string("Hello 3"), time(NULL));

	EXPECT_CALL(*sender_mock, send(_, _)).WillRepeatedly(Return(true));

	planner_mock->sendAll();

	EXPECT_CALL(*sender_mock, send(_, _)).Times(0);

	CPPUNIT_ASSERT(planner_mock->sendAll());
}

void smsPlanner_test2::notSendCanceled()
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).WillRepeatedly(Return(true));

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello 1"), time(NULL));
	planner->cancelDelivery(id);
	
	EXPECT_CALL(*sender_mock, send(_, _)).Times(0);

	CPPUNIT_ASSERT(planner_mock->sendAll());
}

void smsPlanner_test2::notCancelAfterSend()
{
	EXPECT_CALL(*keeper_mock, getTimeValid(_)).WillRepeatedly(Return(true));

	int id = planner_mock->addDelivery(std::string("537240688"), std::string("Hello"), time(NULL));
	
	EXPECT_CALL(*sender_mock, send(_, _)).WillRepeatedly(Return(true));

	planner_mock->sendAll();

	CPPUNIT_ASSERT(false == planner->cancelDelivery(id));
}

int main(int argc, char * argv[])
{
	// Get the top level suite from regitry
	CppUnit::Test * suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	// Adds thetest to the list of test to run
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(suite);

	//Run the tests
	return runner.run() ? 0 : -1;
}
