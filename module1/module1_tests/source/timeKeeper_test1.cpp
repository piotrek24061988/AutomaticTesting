#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "timeKeeper.hpp"

#include <ctime>

class timeKeeper_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		keeper = NULL;
		keeper = new timeKeeper();
	}
	virtual void TearDown()
	{
		if(keeper)
		{
			delete keeper;
			keeper = NULL;
		}
	}

	timeKeeper * keeper;
};

#ifndef IntegrationTests
TEST_F(timeKeeper_test1, timeInvalid)
{
	EXPECT_FALSE( keeper->getTimeValid(time(NULL) - 1));
}
#endif

#ifndef IntegrationTests
TEST_F(timeKeeper_test1, timeVvalidated)
{
	EXPECT_TRUE( keeper->getTimeValid(time(NULL) + 1));
}
#endif
