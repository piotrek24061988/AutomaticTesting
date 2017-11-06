#ifndef _connectionChecker_mock_hpp
#define _connectionChecker_mock_hpp

#include "gmock/gmock.h"

#include "connectionChecker.hpp"

#include <string>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class connectionChecker_mock : public connectionChecker
{
public:
	connectionChecker_mock()
	{
#ifdef IntegrationTests
		ON_CALL(*this, isInternetAvailable()).WillByDefault(Invoke(&real_, &connectionChecker::isInternetAvailable));
#endif
	}

	MOCK_METHOD0(isInternetAvailable, bool());

private:
	connectionChecker real_;
};

#endif //_connectionChecker_mock_hpp
