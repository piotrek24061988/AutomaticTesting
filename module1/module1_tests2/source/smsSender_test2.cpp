#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

#include "smsSender.hpp"

class smsSender_test2 : public CppUnit::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();
	
	void sendValidatedSms();

public:
	CPPUNIT_TEST_SUITE(smsSender_test2);
	CPPUNIT_TEST(sendValidatedSms);
	CPPUNIT_TEST_SUITE_END();

	smsSender * sender;
};

CPPUNIT_TEST_SUITE_REGISTRATION(smsSender_test2);

void smsSender_test2::setUp()
{
	sender = NULL;
	sender = new smsSender();
}

void smsSender_test2::tearDown()
{
	if(sender)
	{
		delete sender;
		sender = NULL;
	}
}

void smsSender_test2::sendValidatedSms()
{
	CPPUNIT_ASSERT(true == sender->send(std::string("537240688"), std::string("Hello")));
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
