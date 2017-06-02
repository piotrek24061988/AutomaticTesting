
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "smsPlanner.hpp"

using ::testing::Return;

class smsPlannerTest : public testing::Test
{
protected:
	smsPlanner *planner;

	virtual void SetUp()
	{
		planner = NULL;
		planner = new smsPlanner();
	} 

	virtual void TearDown()
	{
		delete planner;
		planner = NULL;
	}
};

TEST_F(smsPlannerTest, addDelivery)
{
	std::time_t time1;
	std::localtime(&time1);
	EXPECT_LT(-1, planner->addDelivery(std::string("537240688"), std::string("Hello"), time1));
}
