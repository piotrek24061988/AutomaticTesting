#ifndef _timeKeeper_mock_hpp
#define _timeKeeper_mock_hpp

#include "gmock/gmock.h"

#include "timeKeeper.hpp"

class timeKeeper_mock : public timeKeeper
{
public:
	MOCK_METHOD1(getTimeValid, bool(std::time_t curTime));
};

#endif //_timeKeeper_mock_hpp
