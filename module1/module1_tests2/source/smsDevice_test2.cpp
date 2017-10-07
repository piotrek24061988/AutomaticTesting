#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

#include "smsSender.hpp"

class smsDevice_test2 : public CppUnit::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();
	
	void initialize();
	void deInitialize();
	void sendSmsWithoutInit();
	void sendSmsWithInit();
	void sendSmsWithDeinit();

public:
	CPPUNIT_TEST_SUITE(smsDevice_test2);
	CPPUNIT_TEST(initialize);
	CPPUNIT_TEST(deInitialize);
	CPPUNIT_TEST(sendSmsWithoutInit);
	CPPUNIT_TEST(sendSmsWithInit);
	CPPUNIT_TEST(sendSmsWithDeinit);
	CPPUNIT_TEST_SUITE_END();

	smsDevice * device;
};

CPPUNIT_TEST_SUITE_REGISTRATION(smsDevice_test2);

void smsDevice_test2::setUp()
{
	device = NULL;
	device = new smsDevice();
}

void smsDevice_test2::tearDown()
{
	if(device)
	{
		delete device;
		device = NULL;
	}
}

void smsDevice_test2::initialize()
{
	CPPUNIT_ASSERT(device->init());
}

void smsDevice_test2::deInitialize()
{
	CPPUNIT_ASSERT(device->deInit());
}

void smsDevice_test2::sendSmsWithoutInit()
{
	CPPUNIT_ASSERT(!device->send(std::string("537240688"), std::string("Hello")));
}

void smsDevice_test2::sendSmsWithInit()
{
	CPPUNIT_ASSERT(device->init());
	CPPUNIT_ASSERT(device->send(std::string("537240688"), std::string("Hello")));
}

void smsDevice_test2::sendSmsWithDeinit()
{
	CPPUNIT_ASSERT(device->init());
	CPPUNIT_ASSERT(device->deInit());
	CPPUNIT_ASSERT(!device->send(std::string("537240688"), std::string("Hello")));
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
