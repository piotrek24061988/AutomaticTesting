#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

#include "gmock/gmock.h"

#include "smsSender.hpp"
#include "smsDevice.hpp"

#include "smsDevice_mock.hpp"

using ::testing::Ge;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::_;

class smsSender_test2 : public CppUnit::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();
	
	void sendValidatedSms();
#ifndef IntegrationTests
	void sendValidatedSmsInitError();
#endif
#ifndef IntegrationTests
	void sendValidatedSmsSendError();
#endif
#ifndef IntegrationTests
	void sendValidatedSmsDeinitError();
#endif

public:
	CPPUNIT_TEST_SUITE(smsSender_test2);
	CPPUNIT_TEST(sendValidatedSms);
#ifndef IntegrationTests
	CPPUNIT_TEST(sendValidatedSmsInitError);
#endif
#ifndef IntegrationTests
	CPPUNIT_TEST(sendValidatedSmsSendError);
#endif
#ifndef IntegrationTests
	CPPUNIT_TEST(sendValidatedSmsDeinitError);
#endif
	CPPUNIT_TEST_SUITE_END();

	smsSender * sender;
	smsSender * sender_mock;

	smsDevice * device;
	smsDevice_mock * device_mock;
};

CPPUNIT_TEST_SUITE_REGISTRATION(smsSender_test2);

void smsSender_test2::setUp()
{
	device = NULL;
	device = new smsDevice();
	sender = NULL;
	sender = new smsSender(device);

	device_mock = NULL;
	device_mock = new smsDevice_mock();
	sender_mock = NULL;
	sender_mock = new smsSender(device_mock);
}

void smsSender_test2::tearDown()
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

void smsSender_test2::sendValidatedSms()
{
#ifdef IntegrationTests
	EXPECT_CALL(*device_mock, init()).Times(1);
	EXPECT_CALL(*device_mock, send(std::string("537240688"), std::string("Hello"))).Times(1);
	EXPECT_CALL(*device_mock, deInit()).Times(1);
#else
        EXPECT_CALL(*device_mock, init()).Times(1).WillOnce(Return(true));
        EXPECT_CALL(*device_mock, send(std::string("537240688"), std::string("Hello"))).Times(1).WillOnce(Return(true));
	EXPECT_CALL(*device_mock, deInit()).Times(1).WillOnce(Return(true));
#endif

	CPPUNIT_ASSERT(sender_mock->send(std::string("537240688"), std::string("Hello")));
}

//smsDevice initialization error simulation by mock. Only unit test.
#ifndef IntegrationTests
void smsSender_test2::sendValidatedSmsInitError()
{
        EXPECT_CALL(*device_mock, init()).Times(1).WillOnce(Return(false));

	CPPUNIT_ASSERT(!sender_mock->send(std::string("537240688"), std::string("Hello")));
}
#endif

//smsDevice send error simulation by mock. Only unit test.
#ifndef IntegrationTests
void smsSender_test2::sendValidatedSmsSendError()
{
        EXPECT_CALL(*device_mock, init()).Times(1).WillOnce(Return(true));
        EXPECT_CALL(*device_mock, send(std::string("537240688"), std::string("Hello"))).Times(1).WillOnce(Return(false));

	CPPUNIT_ASSERT(!sender_mock->send(std::string("537240688"), std::string("Hello")));
}
#endif

//smsDevice deinitialization error simulation by mock. Only unit test.
#ifndef IntegrationTests
void smsSender_test2::sendValidatedSmsDeinitError()
{
        EXPECT_CALL(*device_mock, init()).Times(1).WillOnce(Return(true));
        EXPECT_CALL(*device_mock, send(std::string("537240688"), std::string("Hello"))).Times(1).WillOnce(Return(true));
	EXPECT_CALL(*device_mock, deInit()).Times(1).WillOnce(Return(false));

	CPPUNIT_ASSERT(!sender_mock->send(std::string("537240688"), std::string("Hello")));
}
#endif

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
