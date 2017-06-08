#include "cppunit/CompilerOutputter.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

int main(int argc, char * argv[])
{
	// Get the top level suite from regitry
	CppUnit::Test * suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	// Adds thetest to the list of test to run
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(suite);

	//Change default outputter to compiler error format outputter
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));

	//Run the tests
	bool wasSuccesfull = runner.run();

	return wasSuccesfull ? 0 : -1;
}
