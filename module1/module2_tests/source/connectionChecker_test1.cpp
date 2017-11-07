#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "connectionChecker.hpp"
#include <cstdio>

class connectionChecker_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		checker = NULL;
		checker = new connectionChecker();
	}
	virtual void TearDown()
	{
		if(checker)
		{
			delete checker;
			checker = NULL;
		}
	}

	connectionChecker * checker;
};

#ifndef IntegrationTests
TEST_F(connectionChecker_test1, internetAccessible)
{
	EXPECT_TRUE(checker->isInternetAvailable());
}
#endif
