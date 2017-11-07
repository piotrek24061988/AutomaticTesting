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

	MOCK_METHOD0(isInternetAvailable, bool());

};

#endif //_connectionChecker_mock_hpp
