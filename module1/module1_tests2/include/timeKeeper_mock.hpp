#ifndef _timeKeeper_mock_hpp
#define _timeKeeper_mock_hpp

#include "gmock/gmock.h"

#include "timeKeeper.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class timeKeeper_mock : public timeKeeper
{
public:
	timeKeeper_mock()
	{
#ifdef IntegrationTests
		ON_CALL(*this, getTimeValid(_)).WillByDefault(Invoke(&real_, &timeKeeper::getTimeValid));
#endif
	}

	MOCK_METHOD1(getTimeValid, bool(std::time_t curTime));

private:
	timeKeeper real_;
};

#endif //_timeKeeper_mock_hpp
