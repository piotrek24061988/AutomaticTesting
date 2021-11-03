#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "timeKeeper.hpp"

#include <ctime>
#include <memory>

class timeKeeper_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		if(!keeper)
		{
			keeper = std::make_unique<timeKeeper>();
		}
	}
	virtual void TearDown()
	{
		//smart_ptr used so no need to clean anything
	}

	std::unique_ptr<timeKeeper> keeper;
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
