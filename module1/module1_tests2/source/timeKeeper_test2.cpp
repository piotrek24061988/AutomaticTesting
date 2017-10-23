#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

#include "timeKeeper.hpp"

#include <ctime>

class timeKeeper_test2 : public CppUnit::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();

#ifndef IntegrationTests	
	void timeInvalid();
#endif
#ifndef IntegrationTests
	void timeValidated();
#endif

private:
	CPPUNIT_TEST_SUITE(timeKeeper_test2);
#ifndef IntegrationTests
	CPPUNIT_TEST(timeInvalid);
#endif
#ifndef IntegrationTests
	CPPUNIT_TEST(timeValidated);
#endif
	CPPUNIT_TEST_SUITE_END();

	timeKeeper * keeper;
};

CPPUNIT_TEST_SUITE_REGISTRATION(timeKeeper_test2);

void timeKeeper_test2::setUp()
{
	keeper = NULL;
	keeper = new timeKeeper();
}

void timeKeeper_test2::tearDown()
{
	if(keeper)
	{
		delete keeper;
		keeper = NULL;
	}
}

#ifndef IntegrationTests
void timeKeeper_test2::timeInvalid()
{
	CPPUNIT_ASSERT(false == keeper->getTimeValid(time(NULL) - 1));
}
#endif

#ifndef IntegrationTests
void timeKeeper_test2::timeValidated()
{
	CPPUNIT_ASSERT(true == keeper->getTimeValid(time(NULL) + 1));
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
